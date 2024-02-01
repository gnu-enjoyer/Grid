// copyright 2022 gnu-enjoyer / mike

#pragma once

#include "CoreMinimal.h"
#include "InteractiveToolBuilder.h"
#include "BaseTools/ClickDragTool.h"
#include "GridEditorTool.generated.h"

class UGridEditorToolProperties;
class UGridDataAsset;

UCLASS(MinimalAPI)
class UGridEditorToolBuilder final : public UInteractiveToolBuilder
{
	GENERATED_BODY()

public:
	virtual bool CanBuildTool(const FToolBuilderState& SceneState) const override { return true; }
	virtual UInteractiveTool* BuildTool(const FToolBuilderState& SceneState) const override;
};

UCLASS(MinimalAPI)
class UGridEditorTool : public UInteractiveTool, public IClickDragBehaviorTarget
{
	GENERATED_BODY()

	void HandleImportedJSON();

	FInputRayHit FindRayHit(const FRay& WorldRay, FVector& HitPos);

public:
	void Visualise(bool bShow = true, const FIntVector& InCentre = {0, 0, 0});

	/** LMB is pressed */
	void Primary();

	/** LMB with modifiers */
	void Primary(int32 i);

	/** Clean up before PIE, etc. */
	virtual void Shutdown(EToolShutdownType ShutdownType) override;

	void DrawEditorBox(const FVector& Loc, FColor Colour, bool Persistent = false,
	                   const FVector& Extent = {0.5, 0.5, 0.5}) const;

	void SetWorld(UWorld* InWorld)
	{
		check(InWorld)
		TargetWorld = InWorld;
	}

	/** UInteractiveTool */
	virtual void Setup() override;
	virtual void OnPropertyModified(UObject* PropertySet, FProperty* Property) override;

	/** IClickDragBehaviorTarget */
	virtual FInputRayHit CanBeginClickDragSequence(const FInputDeviceRay& PressPos) override;
	virtual void OnClickPress(const FInputDeviceRay& PressPos) override;
	virtual void OnClickRelease(const FInputDeviceRay& ReleasePos) override;

	virtual void OnClickDrag(const FInputDeviceRay& DragPos) override
	{
	}

	virtual void OnTerminateDragSequence() override
	{
	}

	/** IModifierToggleBehaviorTarget */
	virtual void OnUpdateModifierState(int ModifierID, bool bIsOn) override;

protected:
	/** */
	FVector HitPosVector{};

	/** */
	UWorld* TargetWorld = nullptr;

	/** identifier we associate with the shift key */
	static constexpr int MoveSecondPointModifierID = 1;

	/** flag we use to keep track of modifier state */
	bool bSecondPointModifierDown = false;

	UPROPERTY()
	TObjectPtr<UGridEditorToolProperties> Properties;
};
