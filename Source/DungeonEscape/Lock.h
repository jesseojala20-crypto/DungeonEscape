//Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TriggerComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Lock.generated.h"

UCLASS()
class DUNGEONESCAPE_API ALock : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ALock();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	UPROPERTY(VisibleAnywhere)
	USceneComponent* RootComp;         //This is the scene root that we will attach everything else
	
	UPROPERTY(VisibleAnywhere)
	UTriggerComponent* TriggerComp;

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* KeyItemMesh; //this will be the item that we will place into to the lock

	UPROPERTY(EditAnywhere)
	FString KeyItemName;

	void SetIsKeyInLock(bool NewIsKeyInLock); // Setter function to be able to acces the Private Boolean IsKeyInLock
	bool GetIsKeyInLock();                    // Getter function to be able to acces the Private Boolean IsKeyInLock.

private:
	   UPROPERTY(VisibleAnywhere)
	   bool IsKeyInLock = false;
};
