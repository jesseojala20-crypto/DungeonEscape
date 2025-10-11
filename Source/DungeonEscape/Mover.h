#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Mover.generated.h"


UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class DUNGEONESCAPE_API UMover : public UActorComponent
{
    GENERATED_BODY()

public:
    // Sets default values for this component's properties
    UMover();

protected:
    // Called when the game starts
    virtual void BeginPlay() override;

public:
    // Called every frame
    virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

    UPROPERTY(EditAnywhere)
    FVector MovementOffset;

    UPROPERTY(EditAnywhere)
    float MovementTime = 4.0f;

    UPROPERTY(EditAnywhere)
    bool bShouldMove = false;
    UPROPERTY(VisibleAnywhere)
    bool ReachedTarget = false;

    FVector StartLocation;

    FVector TargetLocation;
    
};