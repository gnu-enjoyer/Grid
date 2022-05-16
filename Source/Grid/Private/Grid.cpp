// copyright 2022 gnu-enjoyer / mike

#include "Grid.h"
#include "GameplayTagsManager.h"

#define LOCTEXT_NAMESPACE "FGridModule"

void FGridModule::StartupModule()
{
	UGameplayTagsManager::Get().OnLastChanceToAddNativeTags().AddRaw(this, &FGridModule::RegisterNativeTags);
}

void FGridModule::RegisterNativeTags()
{
	UGameplayTagsManager& Manager = UGameplayTagsManager::Get();

	//base grid tag, if a grid does not have this tag you cannot enter, or interact with it
	Grid = Manager.AddNativeGameplayTag("Grid", TEXT("Grid"));

	//base move type for grid, can be walked walked in and out of
	Passable = Manager.AddNativeGameplayTag("Grid.Passable", TEXT("You can enter or exit this grid"));

	//base type for interactable but not necessarily enterable grid 
	Interactable = Manager.AddNativeGameplayTag("Grid.Interactable", TEXT("You can interact with this grid"));

	Manager.OnLastChanceToAddNativeTags().RemoveAll(this);
}

void FGridModule::ShutdownModule()
{
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FGridModule, Grid)
