// copyright 2022 gnu-enjoyer / mike

#include "GridEditorTool.h"
#include "InteractiveToolManager.h"
#include "BaseBehaviors/ClickDragBehavior.h"
#include "CollisionQueryParams.h"
#include "GridDataAsset.h"
#include "GridEditorToolProperties.h"
#include "Engine/World.h"
#include "Misc/FileHelper.h"

#define LOCTEXT_NAMESPACE "UGridEditorInteractiveTool"

namespace
{
	const auto SelectedColour = FColor::Green.WithAlpha(60);
	const auto ShiftSelectedColour = FColor::Red.WithAlpha(60);
	const auto VisualiseColour = FColor::Orange.WithAlpha(25);

	constexpr auto Grid = TEXT("Grid");
}

UInteractiveTool* UGridEditorToolBuilder::BuildTool(const FToolBuilderState& SceneState) const
{
	UGridEditorTool* NewTool = NewObject<UGridEditorTool>(SceneState.ToolManager);
	NewTool->SetWorld(SceneState.World);
	return NewTool;
}

void UGridEditorTool::DrawEditorBox(const FVector& Loc, FColor Colour, bool Persistent, const FVector& Extent) const
{
	DrawDebugSolidBox(TargetWorld,
	                  Loc,
	                  (Extent * Properties->GridData->Grid.Size),
	                  FQuat(0),
	                  Colour,
	                  Persistent,
	                  2,
	                  0);
}

void UGridEditorTool::Visualise(bool bShow, const FIntVector& InCentre)
{
	if (!bShow)
	{
		FlushPersistentDebugLines(TargetWorld);
		return;
	}

	if (!InCentre.IsZero())
	{
		DrawEditorBox(Properties->GridData->GridToWorld(InCentre), VisualiseColour, true);
		return;
	}

	for (auto& [vec, tags] : Properties->GridData->Grid.Contents)
	{
		DrawEditorBox(Properties->GridData->GridToWorld(vec), VisualiseColour, true);
	}
}

void UGridEditorTool::Primary()
{
	DrawEditorBox(Properties->GridData->GridToWorld(Properties->CurrentCoords), SelectedColour);
}

void UGridEditorTool::Primary(int32 i)
{
	DrawEditorBox(Properties->GridData->GridToWorld(Properties->CurrentCoords), ShiftSelectedColour);
}

void UGridEditorTool::Shutdown(EToolShutdownType ShutdownType)
{
	Super::Shutdown(ShutdownType);
	FlushPersistentDebugLines(TargetWorld);
}

void UGridEditorTool::HandleImportedJSON()
{
	FlushPersistentDebugLines(TargetWorld);
	Visualise(Properties->bShowGrid);
}

void UGridEditorTool::Setup()
{
	UInteractiveTool::Setup();

	FPlatformFileManager::Get().GetPlatformFile().CreateDirectoryTree(Grid);
	UClickDragInputBehavior* MouseBehavior = NewObject<UClickDragInputBehavior>();
	MouseBehavior->Modifiers.RegisterModifier(MoveSecondPointModifierID, FInputDeviceState::IsShiftKeyDown);
	MouseBehavior->Initialize(this);
	AddInputBehavior(MouseBehavior);

	Properties = NewObject<UGridEditorToolProperties>(this, "Grid");
	AddToolPropertySource(Properties);
	Properties->JSONDelegate.AddUObject(this, &UGridEditorTool::HandleImportedJSON);
}

void UGridEditorTool::OnUpdateModifierState(int ModifierID, bool bIsOn)
{
	if (ModifierID == MoveSecondPointModifierID)
	{
		bSecondPointModifierDown = bIsOn;
	}
}

FInputRayHit UGridEditorTool::CanBeginClickDragSequence(const FInputDeviceRay& PressPos)
{
	FVector Temp{};
	FInputRayHit Result = FindRayHit(PressPos.WorldRay, Temp);
	return Result;
}

void UGridEditorTool::OnClickRelease(const FInputDeviceRay& ReleasePos)
{
	FlushPersistentDebugLines(TargetWorld);
	if (!Properties->GridData)
	{
		return;
	}

	auto& GridData = Properties->GridData->Grid.Contents;
	if (bSecondPointModifierDown)
	{
		GridData.Remove(Properties->CurrentCoords);
		Visualise();
		return;
	}

	auto& Found = GridData.FindOrAdd(Properties->CurrentCoords);
	Properties->CurrentGridTags.Emplace(Properties->CurrentCoords, Found);
	Visualise();
}

void UGridEditorTool::OnClickPress(const FInputDeviceRay& PressPos)
{
	FInputRayHit HitResult = FindRayHit(PressPos.WorldRay, HitPosVector);
	if (const auto Data = Properties->GridData)
	{
		Properties->CurrentCoords = Data->WorldToGrid(HitPosVector);
		Properties->CurrentGridTags.Empty();
		bSecondPointModifierDown ? Primary(1) : Primary();
	}
}

FInputRayHit UGridEditorTool::FindRayHit(const FRay& WorldRay, FVector& HitPos)
{
	const FCollisionObjectQueryParams QueryParams(FCollisionObjectQueryParams::AllObjects);
	FHitResult Result;
	bool bHitWorld = TargetWorld->LineTraceSingleByObjectType(Result, WorldRay.Origin, WorldRay.PointAt(999999),
	                                                          QueryParams);
	if (bHitWorld)
	{
		HitPos = Result.ImpactPoint;
		return FInputRayHit(Result.Distance);
	}
	return FInputRayHit();
}

void UGridEditorTool::OnPropertyModified(UObject* PropertySet, FProperty* Property)
{
	FlushPersistentDebugLines(TargetWorld);
	if (!Properties->GridData || !Property)
	{
		return;
	}

	if (Property->GetName() == "GridData" || Property->GetName() == "CurrentGridTags")
	{
		Properties->GridData->Grid.Contents.Append(Properties->CurrentGridTags);
		Properties->SaveDataAsset();
	}
	Visualise(Properties->bShowGrid);
}

#undef LOCTEXT_NAMESPACE
