// copyright 2022 gnu-enjoyer / mike

#include "GridEditorTool.h"
#include "InteractiveToolManager.h"
#include "ToolBuilderUtil.h"
#include "BaseBehaviors/ClickDragBehavior.h"
#include "GameplayTags.h"
#include "CollisionQueryParams.h"
#include "DesktopPlatformModule.h"
#include "Grid.h"
#include "GridDataAsset.h"
#include "GridEditorModeToolkit.h"
#include "JsonObjectConverter.h"
#include "Engine/World.h"

#include "Misc/FileHelper.h"

// localization namespace
#define LOCTEXT_NAMESPACE "UGridEditorInteractiveTool"

/*
 * ToolBuilder
 */

UInteractiveTool* UGridEditorToolBuilder::BuildTool(const FToolBuilderState& SceneState) const
{
	UGridEditorTool* NewTool = NewObject<UGridEditorTool>(SceneState.ToolManager);
	NewTool->SetWorld(SceneState.World);
	return NewTool;
}

UGridEditorToolProperties::UGridEditorToolProperties()
{
	//
	GridData = nullptr;
	ShowGrid = false;
	//
	CurrentCoords = {0, 0, 0};
	CurrentGridTags = {};
}

void UGridEditorToolProperties::SaveDataAsset()
{
	if (!GridData) return;
	GridData->Modify(true);
}

void UGridEditorToolProperties::ExportJSON()
{
	if (!GridData) return;

	TArray<FString> StringBuffer{"["};
	StringBuffer.Reserve(GridData->Grid.Num());
	FString Out;

	for (auto& a : GridData->Grid)
		if (FJsonObjectConverter::UStructToJsonObjectString(
			FGridJSON{a.Key, a.Value}, Out, 0, 0, 0, nullptr, false))
			StringBuffer.Add(Out + TEXT(","));

	if (StringBuffer.IsEmpty()) return;

	StringBuffer.Last().LeftInline(StringBuffer.Last().Len() - 1);

	StringBuffer.Add("]");

	const auto Save = file + FDateTime::UtcNow().ToString() + TEXT(".json");

	if (!IFileManager::Get().DirectoryExists(*Save))
		FFileHelper::SaveStringArrayToFile(StringBuffer, *Save, FFileHelper::EEncodingOptions::ForceUTF8);
}

void UGridEditorToolProperties::ImportJSON()
{
	if (!GridData) return;

	IDesktopPlatform* DesktopPlatform = FDesktopPlatformModule::Get();

	const FText Title = LOCTEXT("Import JSON", "Import JSON from file");
	const FString FileTypes = TEXT("JSON (*.json)|*.json|Text (*.txt)|*.txt");

	TArray<FString> Files;

	if (auto Fs = FSlateApplication::Get().GetActiveTopLevelWindow()->GetNativeWindow()->GetOSWindowHandle())
		DesktopPlatform->OpenFileDialog(Fs,
		                                Title.ToString(),
		                                *file,
		                                *file,
		                                FileTypes,
		                                0,
		                                Files
		);

	if (Files.IsEmpty()) return;

	auto Fd = Files.Last();

	if (!FFileHelper::LoadFileToStringArray(Files, *Fd)) return;

	FString Buffer;

	for (auto& a : Files)
		Buffer.Append(a);

	TArray<FGridJSON> Out;

	if (!FJsonObjectConverter::JsonArrayStringToUStruct<FGridJSON>(Buffer, &Out, 0, 0)) return;

	GridData->Grid.Empty();

	for (auto& json : Out)
		GridData->Grid.Add(json.a, json.b);

	JSONDelegate.Broadcast();
}

void UGridEditorTool::SetWorld(UWorld* World)
{
	check(World);
	this->TargetWorld = World;
}


void UGridEditorTool::Visualise(const bool show, const FIntVector& in)
{
	if (!show)
	{
		FlushPersistentDebugLines(TargetWorld);
		return;
	}
	const FVector Extent{0.5, 0.5, 0.5};

	if (in.IsZero())
		for (auto& a : Properties->GridData->Grid)
		{
			auto Location = Properties->GridData->GridToWorld(a.Key);
			DrawDebugSolidBox(TargetWorld,
			                  Location,
			                  (Extent * Properties->GridData->GridSize),
			                  FQuat(0),
			                  FColor::Orange.WithAlpha(25),
			                  true,
			                  -1,
			                  0);
		}
	else
	{
		DrawDebugSolidBox(TargetWorld,
		                  Properties->GridData->GridToWorld(in),
		                  (Extent * Properties->GridData->GridSize),
		                  FQuat(0),
		                  FColor::Orange.WithAlpha(25),
		                  true,
		                  -1,
		                  0);
	}
}

void UGridEditorTool::Primary()
{
	DrawDebugSolidBox(TargetWorld,
	                  Properties->GridData->GridToWorld(Properties->CurrentCoords),
	                  FVector(0.5, 0.5, 0.5) * (Properties->GridData->GridSize),
	                  FQuat(0),
	                  FColor::Green.WithAlpha(60),
	                  false,
	                  2,
	                  0);
}

void UGridEditorTool::Primary(int i)
{
	DrawDebugSolidBox(TargetWorld,
	                  Properties->GridData->GridToWorld(Properties->CurrentCoords),
	                  FVector(0.5, 0.5, 0.5) * (Properties->GridData->GridSize),
	                  FQuat(0),
	                  FColor::Red.WithAlpha(60),
	                  false,
	                  2,
	                  0);
}

void UGridEditorTool::Shutdown(EToolShutdownType ShutdownType)
{
	Super::Shutdown(ShutdownType);
	FlushPersistentDebugLines(TargetWorld);
}

void UGridEditorTool::ImportedJSON()
{
	FlushPersistentDebugLines(TargetWorld);
	Visualise(Properties->ShowGrid);
}

void UGridEditorTool::Setup()
{
	UInteractiveTool::Setup();

	FPlatformFileManager::Get().GetPlatformFile().CreateDirectoryTree(*Dir);

	UClickDragInputBehavior* MouseBehavior = NewObject<UClickDragInputBehavior>();
	MouseBehavior->Modifiers.RegisterModifier(MoveSecondPointModifierID, FInputDeviceState::IsShiftKeyDown);

	MouseBehavior->Initialize(this);
	AddInputBehavior(MouseBehavior);

	Properties = NewObject<UGridEditorToolProperties>(this, "Grid");
	AddToolPropertySource(Properties);

	Properties->JSONDelegate.AddUObject(
		this, &UGridEditorTool::ImportedJSON);

	GridModule = FModuleManager::Get().GetModulePtr<FGridModule>("Grid");

	bSecondPointModifierDown = false;
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
	FVector Temp;
	FInputRayHit Result = FindRayHit(PressPos.WorldRay, Temp);
	return Result;
}

void UGridEditorTool::OnClickRelease(const FInputDeviceRay& ReleasePos)
{
	FlushPersistentDebugLines(TargetWorld);


	const auto Data = Properties->GridData;

	if (!Data) return;

	if (bSecondPointModifierDown)
	{
		Data->Grid.Remove(Properties->CurrentCoords);
		Visualise();
		return;
	}

	if (const auto Found = Data->Grid.Find(Properties->CurrentCoords))
		Properties->CurrentGridTags.Emplace(
			Properties->CurrentCoords,
			*Found);
	else
	{
		auto tag = Properties->GridData->Grid.Emplace(
			Properties->CurrentCoords, FGameplayTagContainer{
				GridModule->Grid
			});

		Properties->CurrentGridTags.Emplace(Properties->CurrentCoords, tag);
	}


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
	FCollisionObjectQueryParams QueryParams(FCollisionObjectQueryParams::AllObjects);
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
	if (!Properties->GridData) return;

	FlushPersistentDebugLines(TargetWorld);

	if (!Property) return;

	if (Property->GetName() == "CurrentGridTags")
	{
		Properties->GridData->Grid.Append(Properties->CurrentGridTags);
		Properties->SaveDataAsset();
	}

	Visualise(Properties->ShowGrid);
}


void UGridEditorTool::Render(IToolsContextRenderAPI* RenderAPI)
{
}


#undef LOCTEXT_NAMESPACE
