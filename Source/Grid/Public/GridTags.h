// copyright 2022 gnu-enjoyer / mike

#pragma once

#include "NativeGameplayTags.h"

//base grid tag, used for UPROP detail customisation
GRID_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(GRID);

//move tag for grid, can be walked walked in and out of
GRID_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(GRID_PASSABLE);

//tag for interactable grids e.g. LMB
GRID_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(GRID_INTERACTABLE);
