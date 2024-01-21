// copyright 2022 gnu-enjoyer / mike

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "GridSubsystem.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FGridLoaded);

class UGridDataAsset;

UCLASS()
class GRID_API UGridSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()

public:
	//Loads a grid from DA
	UFUNCTION(BlueprintCallable, Category="Grid")
	void LoadGrid(UGridDataAsset* InDataAsset);

	//Returns the grid coordinate under the cursor
	UFUNCTION(BlueprintCallable, BlueprintPure, Category="Grid")
	FIntVector GetGridUnderCursor(APlayerController* InPC, ECollisionChannel InChannel = ECC_Visibility) const;

	//Returns an actor's grid coordinate
	UFUNCTION(BlueprintCallable, BlueprintPure, Category="Grid")
	FIntVector GetActorGrid(AActor* InActor) const;

	//Returns the actor(s) in a grid box: 1*(1x1x1) but shape can be customised
	UFUNCTION(BlueprintCallable, BlueprintPure, Category="Grid")
	TArray<AActor*> GetGridArea(const FIntVector& InCentre, ECollisionChannel InChannel = ECC_Visibility,
	                            const FVector& InExtent = FVector(1, 1, 1)) const;

	//Callback for when a (new) grid is loaded
	UPROPERTY(BlueprintAssignable, Category="Grid")
	FGridLoaded OnGridLoaded;

protected:
	UPROPERTY(Transient)
	TObjectPtr<UGridDataAsset> CurrentGrid;
};
