// copyright 2022 gnu-enjoyer / mike

#include "GridEditorModeCommands.h"

#define LOCTEXT_NAMESPACE "GridEditorModeCommands"

FGridEditorModeCommands::FGridEditorModeCommands()
	: TCommands("GridEditorMode",
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
	return Get().Commands;
}

#undef LOCTEXT_NAMESPACE
