// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "InteractiveTool.h"
#include "GridEditorToolProperties.generated.h"

class UGridDataAsset;

UCLASS(MinimalAPI)
class UGridEditorToolProperties : public UInteractiveToolPropertySet
{
	GENERATED_BODY()

public:
	/** */
	TMulticastDelegate<void()> JSONDelegate;

	/** Currently Selected Grid Coords */
	FIntVector CurrentCoords{};

	/* Data Asset currently being edited */
	UPROPERTY(EditAnywhere, meta = (DisplayName = "Grid Data Asset"), Category="Grid")
	UGridDataAsset* GridData = nullptr;

	/** If enabled: grids that contain tags will be drawn in UEd */
	UPROPERTY(EditAnywhere, meta = (DisplayName = "Visualise Grid"), Category="Grid")
	bool bShowGrid = true;

	/** Currently Selected Grid Tags (hack workaround for details customisation of GTag container ) */
	UPROPERTY(EditAnywhere, Category="Grid",
		meta=(ForceInlineRow, ReadOnlyKeys, NoResetToDefault, DisplayName = "Selected Grid", Categories="Grid"))
	TMap<FIntVector, FGameplayTagContainer> CurrentGridTags{};

	/** Save to Data Asset */
	UFUNCTION(CallInEditor, Category="Grid")
	void SaveDataAsset();

	/** Export to JSON */
	UFUNCTION(CallInEditor, Category="Grid")
	void ExportJSON();

	/** Import from JSON */
	UFUNCTION(CallInEditor, Category="Grid")
	void ImportJSON();
};
