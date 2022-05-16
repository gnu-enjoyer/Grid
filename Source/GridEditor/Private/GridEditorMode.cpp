// copyright 2022 gnu-enjoyer / mike

#include "GridEditorMode.h"
#include "GridEditorModeToolkit.h"
#include "EdModeInteractiveToolsContext.h"
#include "InteractiveToolManager.h"
#include "GridEditorModeCommands.h"


//////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////// 
// AddYourTool Step 1 - include the header file for your Tools here
//////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////// 
//#include "Tools/GridEditorSimpleTool.h"
#include "GridEditorModule.h"
#include "Tools/GridEditorTool.h"

// step 2: register a ToolBuilder in FGridEditorMode::Enter() below


#define LOCTEXT_NAMESPACE "GridEditorMode"

const FEditorModeID UGridEditorMode::EM_GridEditorModeId = TEXT("EM_GridEditorMode");

//FString UGridEditorMode::SimpleToolName = TEXT("GridEditor_ActorInfoTool");
FString UGridEditorMode::InteractiveToolName = TEXT("GridEditor_MeasureDistanceTool");

UGridEditorMode::UGridEditorMode()
{
	FModuleManager::Get().LoadModule("EditorStyle");

	// appearance and icon in the editing mode ribbon can be customized here
	Info = FEditorModeInfo(EM_GridEditorModeId,
	                       LOCTEXT("ModeName", "Grid"),
	                       FSlateIcon(),
	                       true);
}


UGridEditorMode::~UGridEditorMode()
{
}


void UGridEditorMode::ActorSelectionChangeNotify()
{
}

void UGridEditorMode::Enter()
{
	UEdMode::Enter();

	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
	// AddYourTool Step 2 - register the ToolBuilders for your Tools here.
	// The string name you pass to the ToolManager is used to select/activate your ToolBuilder later.
	//////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////// 
	const FGridEditorModeCommands& SampleToolCommands = FGridEditorModeCommands::Get();

	//RegisterTool(SampleToolCommands.SimpleTool, SimpleToolName, NewObject<UGridEditorSimpleToolBuilder>(this));
	RegisterTool(SampleToolCommands.InteractiveTool, InteractiveToolName, NewObject<UGridEditorToolBuilder>(this));

	// active tool type is not relevant here, we just set to default
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
