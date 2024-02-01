// copyright 2022 gnu-enjoyer / mike

#include "GridEditorModeToolkit.h"
#include "EditorModeManager.h"

#define LOCTEXT_NAMESPACE "GridEditorModeToolkit"

void FGridEditorModeToolkit::OnToolStarted(UInteractiveToolManager* Manager, UInteractiveTool* Tool)
{
	FModeToolkit::OnToolStarted(Manager, Tool);
}

void FGridEditorModeToolkit::Init(const TSharedPtr<IToolkitHost>& InitToolkitHost, TWeakObjectPtr<UEdMode> InOwningMode)
{
	FModeToolkit::Init(InitToolkitHost, InOwningMode);
}

void FGridEditorModeToolkit::GetToolPaletteNames(TArray<FName>& PaletteNames) const
{
	PaletteNames.Add(NAME_Default);
}

FName FGridEditorModeToolkit::GetToolkitFName() const
{
	return FName("GridEditorMode");
}

FText FGridEditorModeToolkit::GetBaseToolkitName() const
{
	return LOCTEXT("DisplayName", "GridEditorMode Toolkit");
}

#undef LOCTEXT_NAMESPACE
