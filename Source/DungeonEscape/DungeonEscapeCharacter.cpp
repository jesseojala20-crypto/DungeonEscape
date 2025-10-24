// Copyright Epic Games, Inc. All Rights Reserved.

#include "DungeonEscapeCharacter.h"
#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "EnhancedInputComponent.h"
#include "InputActionValue.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "CollectableItem.h"
#include "Lock.h"
#include "DungeonEscape.h"

ADungeonEscapeCharacter::ADungeonEscapeCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);
	
	// Create the first person mesh that will be viewed only by this character's owner
	FirstPersonMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("First Person Mesh"));

	FirstPersonMesh->SetupAttachment(GetMesh());
	FirstPersonMesh->SetOnlyOwnerSee(true);
	FirstPersonMesh->FirstPersonPrimitiveType = EFirstPersonPrimitiveType::FirstPerson;
	FirstPersonMesh->SetCollisionProfileName(FName("NoCollision"));

	// Create the Camera Component	
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("First Person Camera"));
	FirstPersonCameraComponent->SetupAttachment(FirstPersonMesh, FName("head"));
	FirstPersonCameraComponent->SetRelativeLocationAndRotation(FVector(-2.8f, 5.89f, 0.0f), FRotator(0.0f, 90.0f, -90.0f));
	FirstPersonCameraComponent->bUsePawnControlRotation = true;
	FirstPersonCameraComponent->bEnableFirstPersonFieldOfView = true;
	FirstPersonCameraComponent->bEnableFirstPersonScale = true;
	FirstPersonCameraComponent->FirstPersonFieldOfView = 70.0f;
	FirstPersonCameraComponent->FirstPersonScale = 0.6f;

	// configure the character comps
	GetMesh()->SetOwnerNoSee(true);
	GetMesh()->FirstPersonPrimitiveType = EFirstPersonPrimitiveType::WorldSpaceRepresentation;

	GetCapsuleComponent()->SetCapsuleSize(34.0f, 96.0f);

	// Configure character movement
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;
	GetCharacterMovement()->AirControl = 0.5f;
}

void ADungeonEscapeCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{	
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		// Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ADungeonEscapeCharacter::DoJumpStart);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ADungeonEscapeCharacter::DoJumpEnd);

		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ADungeonEscapeCharacter::MoveInput);

		// Looking/Aiming
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ADungeonEscapeCharacter::LookInput);
		EnhancedInputComponent->BindAction(MouseLookAction, ETriggerEvent::Triggered, this, &ADungeonEscapeCharacter::LookInput);
        // Interacting
		EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Started, this, &ADungeonEscapeCharacter::Interact);
	}
	else
	{
		UE_LOG(LogDungeonEscape, Error, TEXT("'%s' Failed to find an Enhanced Input Component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}



void ADungeonEscapeCharacter::Interact()
{

	FVector StartPoint = FirstPersonCameraComponent->GetComponentLocation();
	FVector EndPoint = StartPoint + (FirstPersonCameraComponent->GetForwardVector() * MaxInteractionDistance);
	DrawDebugLine(GetWorld(), StartPoint, EndPoint, FColor::Cyan, false, 10.0f);

	FCollisionShape InteractionSphere = FCollisionShape::MakeSphere(InteractionSphereRadius);
	DrawDebugSphere(GetWorld(), StartPoint, InteractionSphereRadius, 20, FColor::Green, false, 10.0f);
	DrawDebugSphere(GetWorld(), EndPoint, InteractionSphereRadius, 20, FColor::Red, false, 10.0f);

	FHitResult HitResult;
	bool HasHit = GetWorld()->SweepSingleByChannel(HitResult, StartPoint, EndPoint, FQuat::Identity, ECC_GameTraceChannel2, InteractionSphere);

	if (HasHit)
	{
		AActor* HitActor = HitResult.GetActor();
		
		if (HitActor->ActorHasTag("CollectableItem"))
		{
			UE_LOG(LogTemp, Display, TEXT("Hit actor is a collectable"));
			ACollectableItem* CollectableItem = Cast<ACollectableItem>(HitActor);
			if (CollectableItem)
			{
				ItemList.Add(CollectableItem->ItemName);

				CollectableItem->Destroy(true);
			}
		}

		else if (HitActor->ActorHasTag("Lock"))
		{
			UE_LOG(LogTemp, Display, TEXT("Hit actor is a lock"))
				ALock* LockActor = Cast<ALock>(HitActor); // now we can acces the functions of this class like item name ect.
			if (LockActor)
			{
				
				if (!LockActor->GetIsKeyInLock()) // checks if the key is not in the lock
				{
					int32 ItemsRemoved = ItemList.RemoveSingle(LockActor->KeyItemName);
					if (ItemsRemoved)
					{
						LockActor->SetIsKeyInLock(true); // activates the lock
					}
					else
					{
						UE_LOG(LogTemp, Display, TEXT("Player doesn't have the KeyItem"));
					}
				}
				else
				{
					int32 ItemAdded = ItemList.Add(LockActor->KeyItemName);
					if (ItemAdded)
					{
						LockActor->SetIsKeyInLock(false);
					}
				}
			}
		}
	}
	else
	{
		UE_LOG(LogTemp, Display, TEXT("ShapeTrace has not hit an actor"));
	}
}


void ADungeonEscapeCharacter::MoveInput(const FInputActionValue& Value)
{
	// get the Vector2D move axis
	FVector2D MovementVector = Value.Get<FVector2D>();

	// pass the axis values to the move input
	DoMove(MovementVector.X, MovementVector.Y);

}

void ADungeonEscapeCharacter::LookInput(const FInputActionValue& Value)
{
	// get the Vector2D look axis
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	// pass the axis values to the aim input
	DoAim(LookAxisVector.X, LookAxisVector.Y);

}

void ADungeonEscapeCharacter::DoAim(float Yaw, float Pitch)
{
	if (GetController())
	{
		// pass the rotation inputs
		AddControllerYawInput(Yaw);
		AddControllerPitchInput(Pitch);
	}
}

void ADungeonEscapeCharacter::DoMove(float Right, float Forward)
{
	if (GetController())
	{
		// pass the move inputs
		AddMovementInput(GetActorRightVector(), Right);
		AddMovementInput(GetActorForwardVector(), Forward);
	}
}

void ADungeonEscapeCharacter::DoJumpStart()
{
	// pass Jump to the character
	Jump();
}

void ADungeonEscapeCharacter::DoJumpEnd()
{
	// pass StopJumping to the character
	StopJumping();
}
