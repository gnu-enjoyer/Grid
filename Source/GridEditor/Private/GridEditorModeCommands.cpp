// copyright 2022 gnu-enjoyer / mike

#include "GridEditorModeCommands.h"
#include "GridEditorMode.h"
#include "EditorStyleSet.h"

#define LOCTEXT_NAMESPACE "GridEditorModeCommands"

FGridEditorModeCommands::FGridEditorModeCommands()
	: TCommands<FGridEditorModeCommands>("GridEditorMode",
	                                     NSLOCTEXT("GridEditorMode", "GridEditorModeCommands", "Grid Editor Mode"),
	                                     NAME_None,
	                                     FAppStyle::GetAppStyleSetName())
{
}

void FGridEditorModeCommands::RegisterCommands()
{
	TArray<TSharedPtr<FUICommandInfo>>& ToolCommands = Commands.FindOrAdd(NAME_Default);

	UI_COMMAND(InteractiveTool, "Grid", "Grid Plugin", EUserInterfaceActionType::ToggleButton, FInputChord());
	ToolCommands.Add(InteractiveTool);
}

TMap<FName, TArray<TSharedPtr<FUICommandInfo>>> FGridEditorModeCommands::GetCommands()
{
	return FGridEditorModeCommands::Get().Commands;
}

#undef LOCTEXT_NAMESPACE
