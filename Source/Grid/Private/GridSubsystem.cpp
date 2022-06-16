// copyright 2022 gnu-enjoyer / mike

#include "GridSubsystem.h"
#include "GridManager.h"

UGridSubsystem::UGridSubsystem()
{
	GridManager = CreateDefaultSubobject<AGridManager>("Grid Manager");	
}
