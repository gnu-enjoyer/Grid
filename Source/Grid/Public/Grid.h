// copyright 2022 gnu-enjoyer / mike

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Modules/ModuleManager.h"

class FGridModule : public IModuleInterface
{
public:
	virtual void StartupModule() override;

	void RegisterNativeTags();

	/* Native Tags */
	FGameplayTag Grid;
	FGameplayTag Passable;
	FGameplayTag Interactable;

	virtual void ShutdownModule() override;
};
