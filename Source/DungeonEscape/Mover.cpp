// Fill out your copyright notice in the Description page of Project Settings.


#include "Mover.h"

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
	UE_LOG(LogTemp, Display, TEXT("%s is in location: %s"), *MyOwner->GetActorNameOrLabel(),
		*StartLocation.ToCompactString());

}

// Called every frame
void UMover::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	FVector Currentlocation = GetOwner() ->GetActorLocation();

	Currentlocation.Z = Currentlocation.Z + 100.0f * DeltaTime;

	GetOwner()->SetActorLocation(Currentlocation);
}

