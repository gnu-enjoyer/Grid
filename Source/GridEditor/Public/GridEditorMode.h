// copyright 2022 gnu-enjoyer / mike

#pragma once

#include "CoreMinimal.h"
#include "EdMode.h"
#include "Tools/UEdMode.h"
#include "GridEditorMode.generated.h"

UCLASS()
class UGridEditorMode : public UEdMode
{
	GENERATED_BODY()

public:
	const static FEditorModeID EM_GridEditorModeId;

	static FString SimpleToolName;
	static FString InteractiveToolName;

	UGridEditorMode();
	virtual ~UGridEditorMode();


	/** UEdMode interface */
	virtual void Enter() override;
	virtual void ActorSelectionChangeNotify() override;
	virtual void CreateToolkit() override;
	virtual TMap<FName, TArray<TSharedPtr<FUICommandInfo>>> GetModeCommands() const override;
};
