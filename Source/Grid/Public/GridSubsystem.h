// copyright 2022 gnu-enjoyer / mike

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "GridSubsystem.generated.h"

UCLASS()
class GRID_API UGridSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()

public:
	UGridSubsystem();
	
	//If multiple Grid Managers are desired then you will have to handle your own storage 
	UFUNCTION(BlueprintCallable, BlueprintPure, Category="Grid")
	FORCEINLINE class AGridManager* GetGridManager() const { return GridManager; }
	
private:
	//Handles conversion of world space to grid space
	UPROPERTY()
	class AGridManager* GridManager = nullptr;	
};
