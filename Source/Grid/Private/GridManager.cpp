// copyright 2022 gnu-enjoyer / mike

#include "GridManager.h"

AGridManager::AGridManager()
{
	PrimaryActorTick.bCanEverTick = false;
}

bool AGridManager::StartGridManager(UGridDataAsset* Data)
{
	if (GetOuter()->GetWorld() && Data)
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

	FHitResult HitResult;

	const FCollisionObjectQueryParams ObjectParams(Channel);
	const FCollisionQueryParams Params;

	UWorld* World = GetOuter()->GetWorld();

	auto Loc = GridData->GridToWorld(Centre);

	if (auto bHit = World
		                ? World->SweepSingleByObjectType(HitResult,
		                                                 Loc, Loc, FQuat(), ObjectParams,
		                                                 FCollisionShape::MakeBox(
			                                                 Extent * GridData->GetGridSize()), Params)
		                : false)
		OutActor.Add(HitResult.GetActor());

	return OutActor;
}
