// copyright 2022 gnu-enjoyer / mike

#include "GridEditorModule.h"
#include "GridEditorModeCommands.h"

#define LOCTEXT_NAMESPACE "GridEditorModule"

void FGridEditorModule::StartupModule()
{
	FGridEditorModeCommands::Register();
}

void FGridEditorModule::ShutdownModule()
{
	FGridEditorModeCommands::Unregister();
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FGridEditorModule, GridEditorMode)
