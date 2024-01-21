// copyright 2022 gnu-enjoyer / mike

#include "GridSubsystem.h"
#include "GridDataAsset.h"

void UGridSubsystem::LoadGrid(UGridDataAsset* InDataAsset)
{
	if (ensureAlwaysMsgf(InDataAsset, TEXT("Grid DA was null")))
	{
		CurrentGrid = InDataAsset;
		OnGridLoaded.Broadcast();
	}
}

FIntVector UGridSubsystem::GetGridUnderCursor(APlayerController* InPC, ECollisionChannel InChannel) const
{
	if (InPC && CurrentGrid)
	{
		FHitResult HitResult;
		if (InPC->GetHitResultUnderCursor(InChannel, false, HitResult))
		{
			return CurrentGrid->WorldToGrid(HitResult.Location);
		}
	}
	return {0, 0, 0};
}

FIntVector UGridSubsystem::GetActorGrid(AActor* InActor) const
{
	return (InActor && CurrentGrid) ? CurrentGrid->WorldToGrid(InActor->GetActorLocation()) : FIntVector{0, 0, 0};
}

TArray<AActor*> UGridSubsystem::GetGridArea(const FIntVector& InCentre, ECollisionChannel InChannel,
                                            const FVector& InExtent) const
{
	TArray<AActor*> OutActors{};
	if (!CurrentGrid)
	{
		return OutActors;
	}

	TArray<FOverlapResult> Overlaps;
	if (GetWorld()->OverlapMultiByChannel(Overlaps, CurrentGrid->GridToWorld(InCentre), FQuat::Identity, InChannel,
	                                      FCollisionShape::MakeBox(CurrentGrid->Grid.Size * (InExtent / 2))))
	{
		OutActors.Reserve(Overlaps.Num());
		for (auto& i : Overlaps)
		{
			OutActors.Emplace(i.GetActor());
		}
	}
	return OutActors;
}
