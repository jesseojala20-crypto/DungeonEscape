// Fill out your copyright notice in the Description page of Project Settings.


#include "Mover.h"


#include "Math/UnrealMathUtility.h" //you might need this for interpolation functions

// Sets default values for this component's properties
UMover::UMover()
{
    // Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
    // off to improve performance if you don't need them.
    PrimaryComponentTick.bCanEverTick = true;

    // ...
}


// Called when the game starts
void UMover::BeginPlay()
{
    Super::BeginPlay();

    AActor* MyOwner = GetOwner();

    StartLocation = MyOwner->GetActorLocation();
    

}

// Called every frame
void UMover::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
    if (bShouldMove)
    {
        TargetLocation = StartLocation + MovementOffset;  // Target location is the start location + the movement offset, when the boolean is true.
    }
    else
    {
        TargetLocation = StartLocation;
    }
    FVector Currentlocation = GetOwner()->GetActorLocation();
   
    ReachedTarget = Currentlocation.Equals(TargetLocation);

    if (ReachedTarget == false)
    {
        float Speed = MovementOffset.Length() / MovementTime;
        FVector NewLocation = FMath::VInterpConstantTo(Currentlocation, TargetLocation, DeltaTime, Speed);

        GetOwner()->SetActorLocation(NewLocation);

       
    }
    
    
};

