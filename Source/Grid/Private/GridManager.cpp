// copyright 2022 gnu-enjoyer / mike

#include "GridManager.h"

bool AGridManager::StartGridManager(UGridDataAsset* Data)
{
	if (WorldPtr = GetOuter()->GetWorld(); Data != nullptr)
		GridData = Data;

	return GridData == Data;
}

FIntVector AGridManager::GetGridUnderCursor(APlayerController* PC, ECollisionChannel Channel) const
{
	if (PC && GridData)
	{
		FHitResult HitResult;

		if (PC->GetHitResultUnderCursor(Channel, false, HitResult))
			return GridData->WorldToGrid(HitResult.Location);
	}

	return {0, 0, 0};
}

FIntVector AGridManager::GetActorGrid(AActor* inActor) const
{
	if (inActor && GridData)
		return GridData->WorldToGrid(inActor->GetActorLocation());

	return {0, 0, 0};
}

TArray<AActor*> AGridManager::GetGridArea(const FIntVector& Centre, ECollisionChannel Channel,
                                          const FVector& Extent) const
{

	TArray<AActor*> OutActor;

	if (!GridData) return OutActor;
	
	TArray<FOverlapResult> Overlaps;

	if(WorldPtr->OverlapMultiByChannel(Overlaps, GridData->GridToWorld(Centre), FQuat::Identity, Channel, FCollisionShape::MakeBox(GridData->GridSize*(Extent/2))))
	{

		OutActor.Reserve(Overlaps.Num());

		for(auto &i : Overlaps)
			OutActor.Emplace(i.GetActor());			
	}
	
	return OutActor;
}
