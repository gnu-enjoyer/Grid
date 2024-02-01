// copyright 2022 gnu-enjoyer / mike

#pragma once

#include "CoreMinimal.h"
#include "Toolkits/BaseToolkit.h"
#include "GridEditorMode.h"

class UGridEditorTool;

class FGridEditorModeToolkit final : public FModeToolkit
{
public:
	virtual void OnToolStarted(UInteractiveToolManager* Manager, UInteractiveTool* Tool) override;

	/** FModeToolkit */
	virtual void Init(const TSharedPtr<IToolkitHost>& InitToolkitHost, TWeakObjectPtr<UEdMode> InOwningMode) override;
	virtual void GetToolPaletteNames(TArray<FName>& PaletteNames) const override;

	/** IToolkit */
	virtual FName GetToolkitFName() const override;
	virtual FText GetBaseToolkitName() const override;

protected:
	UGridEditorTool* ToolPtr = nullptr;
};
