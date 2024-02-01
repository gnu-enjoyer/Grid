// copyright 2022 gnu-enjoyer / mike

#include "GridEditorMode.h"
#include "GridEditorModeToolkit.h"
#include "InteractiveToolManager.h"
#include "GridEditorModeCommands.h"
#include "Tools/GridEditorTool.h"

#define LOCTEXT_NAMESPACE "GridEditorMode"

const FEditorModeID UGridEditorMode::EM_GridEditorModeId = TEXT("EM_GridEditorMode");
FString UGridEditorMode::InteractiveToolName = TEXT("GridEditor_MeasureDistanceTool");

UGridEditorMode::UGridEditorMode()
{
	FModuleManager::Get().LoadModule("EditorStyle");
	Info = FEditorModeInfo(EM_GridEditorModeId,
	                       LOCTEXT("ModeName", "Grid"),
	                       FSlateIcon(),
	                       true);
}

void UGridEditorMode::Enter()
{
	UEdMode::Enter();

	RegisterTool(FGridEditorModeCommands::Get().InteractiveTool, InteractiveToolName,
	             NewObject<UGridEditorToolBuilder>(this));
	GetToolManager()->SelectActiveToolType(EToolSide::Left, InteractiveToolName);
}

void UGridEditorMode::CreateToolkit()
{
	Toolkit = MakeShareable(new FGridEditorModeToolkit);
}

TMap<FName, TArray<TSharedPtr<FUICommandInfo>>> UGridEditorMode::GetModeCommands() const
{
	return FGridEditorModeCommands::Get().GetCommands();
}

#undef LOCTEXT_NAMESPACE
