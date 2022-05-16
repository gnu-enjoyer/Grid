// copyright 2022 gnu-enjoyer / mike

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Info.h"
#include "GridDataAsset.h"
#include "Kismet/KismetMathLibrary.h"
#include "GridManager.generated.h"

UCLASS()
class GRID_API AGridManager : public AInfo
{
	GENERATED_BODY()

public:
	AGridManager();

	//Starts the Grid Manager: required for all other functionality
	UFUNCTION(BlueprintCallable, Category="Grid|Grid Manager")
	bool StartGridManager(class UGridDataAsset* Data);

	//Returns the grid coordinate under the cursor (clientside only)
	UFUNCTION(BlueprintCallable, BlueprintPure, Category="Grid|Grid Manager")
	FIntVector GetGridUnderCursor(APlayerController* PC, ECollisionChannel Channel = ECC_Pawn) const;

	//Returns an actor's grid coordinate
	UFUNCTION(BlueprintCallable, BlueprintPure, Category="Grid|Grid Manager")
	FIntVector GetActorGrid(AActor* inActor) const;

	//Returns the actor(s) in a grid box: 1*(1x1x1) but shape can be customised
	UFUNCTION(BlueprintCallable, BlueprintPure, Category="Grid|Grid Manager")
	TArray<AActor*> GetGridArea(const FIntVector& Centre, ECollisionChannel Channel = ECC_Pawn,
	                            const FVector& Extent = FVector(0.5, 0.5, 0.5)) const;

protected:
	UFUNCTION(BlueprintCallable, BlueprintPure, Category="Grid|Grid Manager")
	FORCEINLINE class UGridDataAsset* GetGridData() const { return GridData; }

	//Returns a grid's tag container ref
	UFUNCTION(BlueprintCallable, BlueprintPure, Category="Grid|Grid Manager")
	FORCEINLINE struct FGameplayTagContainer GetTagContainer(const FIntVector& in) const
	{
		return GridData->Grid.FindRef(in);
	}

private:
	//Load an associated Grid Data asset to populate the grid with the right tags
	UPROPERTY(EditDefaultsOnly, Category="Grid|Grid Manager", meta = (AllowPrivateAccess = "true"))
	class UGridDataAsset* GridData = nullptr;

	//Pointer to the current world
	UPROPERTY(Transient)
	UWorld* WorldPtr;
};
