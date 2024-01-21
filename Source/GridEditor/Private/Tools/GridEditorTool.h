// copyright 2022 gnu-enjoyer / mike

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "InteractiveToolBuilder.h"
#include "BaseTools/ClickDragTool.h"
#include "GridEditorTool.generated.h"

class UGridDataAsset;

UCLASS()
class GRIDEDITOR_API UGridEditorToolBuilder : public UInteractiveToolBuilder
{
	GENERATED_BODY()

public:
	virtual bool CanBuildTool(const FToolBuilderState& SceneState) const override { return true; }
	virtual UInteractiveTool* BuildTool(const FToolBuilderState& SceneState) const override;
};

UCLASS(Transient)
class GRIDEDITOR_API UGridEditorToolProperties : public UInteractiveToolPropertySet
{
	GENERATED_BODY()

public:
	/** */
	DECLARE_MULTICAST_DELEGATE(FOnImportedJSON);
	FOnImportedJSON JSONDelegate;

	/** */
	FString file = FPaths::ProjectPluginsDir() + TEXT("Grid/");

	/** Currently Selected Grid Coords */
	FIntVector CurrentCoords{};

	/* Ptr to Data Asset currently being edited */
	UPROPERTY(EditAnywhere, meta = (DisplayName = "Grid Data Asset"))
	UGridDataAsset* GridData = nullptr;

	/** If enabled: grids that contain tags will be drawn in UEd */
	UPROPERTY(EditAnywhere, meta = (DisplayName = "Visualise Grid"))
	bool bShowGrid = true;

	//hacky to get the GTag Container updating :/
	/** Currently Selected Grid Tags */
	UPROPERTY(EditAnywhere,
		meta=(ForceInlineRow, ReadOnlyKeys, NoResetToDefault, DisplayName = "Selected Grid", Categories="Grid"))
	TMap<FIntVector, FGameplayTagContainer> CurrentGridTags{};

	//Save to Data Asset (Recommended)
	UFUNCTION(CallInEditor, Category="Grid")
	void SaveDataAsset();

	//Export to JSON (Experimental)
	UFUNCTION(CallInEditor, Category="Grid")
	void ExportJSON();

	//Import from JSON (Experimental)
	UFUNCTION(CallInEditor, Category="Grid")
	void ImportJSON();
};

UCLASS()
class GRIDEDITOR_API UGridEditorTool : public UInteractiveTool, public IClickDragBehaviorTarget
{
	GENERATED_BODY()

public:
	DECLARE_MULTICAST_DELEGATE(FOnTagChanged);
	FOnTagChanged Del;

	void Visualise(bool bShow = true, const FIntVector& InCentre = {0, 0, 0});

	//Base LMB
	void Primary();

	//LMB with modifiers
	void Primary(int i);

	// Clean up before PIE, etc.
	virtual void Shutdown(EToolShutdownType ShutdownType) override;

	/* Config for JSON */
	const FString Dir = "Grid";

	void ImportedJSON();

	void SetWorld(UWorld* InWorld)
	{
		check(InWorld)
		TargetWorld = InWorld;
	}

	void DrawEditorBox(const FVector& Loc, FColor Colour, bool Persistent = false,
	                   const FVector& Extent = {0.5, 0.5, 0.5}) const;

	/** UInteractiveTool overrides */
	virtual void Setup() override;

	virtual void OnPropertyModified(UObject* PropertySet, FProperty* Property) override;

	/** IClickDragBehaviorTarget implementation */
	virtual FInputRayHit CanBeginClickDragSequence(const FInputDeviceRay& PressPos) override;

	virtual void OnClickPress(const FInputDeviceRay& PressPos) override;

	virtual void OnClickRelease(const FInputDeviceRay& ReleasePos) override;

	virtual void OnClickDrag(const FInputDeviceRay& DragPos) override
	{
	}

	virtual void OnTerminateDragSequence() override
	{
	}

	/** IModifierToggleBehaviorTarget implementation (inherited via IClickDragBehaviorTarget) */
	virtual void OnUpdateModifierState(int ModifierID, bool bIsOn) override;

protected:
	UPROPERTY()
	TObjectPtr<UGridEditorToolProperties> Properties;

	FVector HitPosVector;

	UWorld* TargetWorld = nullptr; // target World we will raycast into

	static const int Index;

	static constexpr int MoveSecondPointModifierID = 1; // identifier we associate with the shift key

	bool bSecondPointModifierDown = false; // flag we use to keep track of modifier state

	FInputRayHit FindRayHit(const FRay& WorldRay, FVector& HitPos); // raycasts into World
};
