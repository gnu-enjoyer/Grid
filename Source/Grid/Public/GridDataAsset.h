// copyright 2022 gnu-enjoyer / mike

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/DataAsset.h"
#include "Kismet/KismetMathLibrary.h"
#include "GridDataAsset.generated.h"

USTRUCT(BlueprintType)
struct GRID_API FGridData final
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category="Grid")
	FName Name = "Example";

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category="Grid")
	float Size = 200.0f;

	//A grid can hold Gameplay Tags that define how it is interacted with
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category="Grid",
		meta=(ForceInlineRow, ReadOnlyKeys, NoResetToDefault, Categories="Grid"))
	TMap<FIntVector, FGameplayTagContainer> Contents{};
};


UCLASS()
class GRID_API UGridDataAsset : public UDataAsset
{
	GENERATED_BODY()

#if WITH_EDITOR
	friend class UGridEditorTool;
	friend class UGridEditorToolProperties;
#endif
	friend class UGridSubsystem;

public:
	//Converts a world vector into X,Y,Z coordinates
	UFUNCTION(BlueprintCallable, BlueprintPure, Category="Grid")
	FORCEINLINE FIntVector WorldToGrid(const FVector& inVec) const
	{
		return (UKismetMathLibrary::FTruncVector(
			UKismetMathLibrary::Vector_SnappedToGrid(inVec, Grid.Size) / Grid.Size));
	}

	//Converts a 3D (X,Y,Z) grid coordinate back into world space
	UFUNCTION(BlueprintCallable, BlueprintPure, Category="Grid")
	FORCEINLINE FVector GridToWorld(const FIntVector& inGrid) const
	{
		return (UKismetMathLibrary::Conv_IntVectorToVector(inGrid) * Grid.Size);
	}

	UFUNCTION(BlueprintCallable, BlueprintPure, Category="Grid")
	FORCEINLINE float GetGridSize() const
	{
		return Grid.Size;
	}

protected:
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category="Grid", meta=(ShowOnlyInnerProperties))
	FGridData Grid{};
};
