// Fill out your copyright notice in the Description page of Project Settings.


// Fill out your copyright notice in the Description page of Project Settings.


#include "TriggerComponent.h"

UTriggerComponent::UTriggerComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

}

void UTriggerComponent::BeginPlay()
{
	Super::BeginPlay();

	if (MoverActor)   //If Moveractor is not nullpointer
	{
		Mover = MoverActor->FindComponentByClass<UMover>();
		if (Mover) // is not equal to nullpointer.
		{
			UE_LOG(LogTemp, Display, TEXT("Found the component"));
		}
		else
		{
			UE_LOG(LogTemp, Display, TEXT("Failed to find component!"));
		}
	}
	else //MoverActor is nullptr
	{
		UE_LOG(LogTemp, Display, TEXT("MoverActor is nullptr"));
	}
	if (IsPressurePlate)
	{
		OnComponentBeginOverlap.AddDynamic(this, &UTriggerComponent::OnOverlapBegin); //this links the delegate to the function call.
		OnComponentEndOverlap.AddDynamic(this, &UTriggerComponent::OnOverlapEnd); //this links the delegate to the function call.
	}
}

void UTriggerComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);


}

void UTriggerComponent::Trigger(bool NewTriggervalue)
{
	IsTriggered = NewTriggervalue;
	if (Mover)
	{
		Mover->SetShouldMove(IsTriggered);
	}
	else
	{
		UE_LOG(LogTemp, Display, TEXT("%s doesn't have a mover to trigger"), *GetOwner()->GetActorNameOrLabel());
	}
}

void UTriggerComponent::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && OtherActor->ActorHasTag("PressurePlateActivator")) //This forces the game to check if the colliding actor is the player and has the right name
	{
		ActivatorCount++;
		if (!IsTriggered)
		{
			Trigger(true); //this mechanism will make sure that if the collider is triggered it doesn't do so again
		}

	}
}


void UTriggerComponent::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor && OtherActor->ActorHasTag("PressurePlateActivator"))
	{
		ActivatorCount--;
			if (IsTriggered)
			{
				Trigger(false); //this mechanism will make sure that if the collider is triggered it doesn't do so again
			}
	}
}
