// copyright 2022 gnu-enjoyer / mike

#pragma once

#include "CoreMinimal.h"
#include "Tools/UEdMode.h"
#include "GridEditorMode.generated.h"

UCLASS(MinimalAPI)
class UGridEditorMode : public UEdMode
{
	GENERATED_BODY()

public:
	const static FEditorModeID EM_GridEditorModeId;

	static FString SimpleToolName;
	static FString InteractiveToolName;

	UGridEditorMode();
	virtual ~UGridEditorMode() override = default;

	/** UEdMode */
	virtual void Enter() override;
	virtual void CreateToolkit() override;
	virtual TMap<FName, TArray<TSharedPtr<FUICommandInfo>>> GetModeCommands() const override;
};
