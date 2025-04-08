// Fill out your copyright notice in the Description page of Project Settings.


#include "InvestigationPointManager.h"
#include "GameFramework/Character.h"
#include "InvestigationPoint.h"
#include "EngineUtils.h"
#include "NavigationSystem.h"
#include "Algo/RandomShuffle.h"

// Sets default values
AInvestigationPointManager::AInvestigationPointManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AInvestigationPointManager::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AInvestigationPointManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

FVector AInvestigationPointManager::FindClosestPointNotInvestigated(const ACharacter* Character, const FVector& StartingPoint) const
{
	FVector OutPoint;
	if (FindUnInvestigatedNeighborOfCurrentInvestigationPoint(Character, OutPoint))
	{
		return OutPoint;
	}

	if (FindClosestUnInvestigatedPoint(Character, OutPoint))
	{
		return OutPoint;
	}

	UNavigationSystemV1* Nav = UNavigationSystemV1::GetCurrent(GetWorld());

	FNavLocation NavLocation;
	if (Nav && Nav->GetRandomReachablePointInRadius(StartingPoint, RandomInvestigationRange, NavLocation))
	{
		return NavLocation.Location;
	}

	return StartingPoint;
}

bool AInvestigationPointManager::FindUnInvestigatedNeighborOfCurrentInvestigationPoint(const ACharacter* Character, FVector& OutPoint) const
{
	float CurrentTimeSeconds = GetWorld()->GetRealTimeSeconds();

	for (TObjectPtr<AInvestigationPoint> Point : TActorRange<AInvestigationPoint>(GetWorld()))
	{
		float LastTimeInvestigated;
		if (!Point->HasInvestigatedRecently(Character, LastTimeInvestigated))
		{
			continue;
		}

		if (CurrentTimeSeconds - LastTimeInvestigated > 1.f)
		{
			continue;
		}

		TArray<int32> Indices;
		for (size_t i = 0; i < Point->GetNeighbors().Num(); i++)
		{
			Indices.Add(i);
		}

		Algo::RandomShuffle(Indices);

		for (int32 Index : Indices)
		{
			TObjectPtr<AInvestigationPoint> PointNeighbor = Point->GetNeighbors()[Index];
			if (PointNeighbor && !PointNeighbor->HasInvestigatedRecently(Character, LastTimeInvestigated))
			{
				OutPoint = PointNeighbor->GetActorLocation();
				return true;
			}
		}
	}

	return false;
}

bool AInvestigationPointManager::FindClosestUnInvestigatedPoint(const ACharacter* Character, FVector& OutPoint) const
{
	UNavigationSystemV1* Nav = UNavigationSystemV1::GetCurrent(GetWorld());

	TObjectPtr<AInvestigationPoint> BestPoint = nullptr;
	double BestPathLength = INFINITY;
	double BestPathCost = INFINITY;

	for (TObjectPtr<AInvestigationPoint> Point : TActorRange<AInvestigationPoint>(GetWorld()))
	{
		float LastTimeInvestigated;
		if (Point->HasInvestigatedRecently(Character, LastTimeInvestigated))
		{
			continue;
		}

		double PathLength, PathCost;
		ENavigationQueryResult::Type PathResult = Nav->GetPathLengthAndCost(Character->GetActorLocation(), Point->GetActorLocation(), PathLength, PathCost);

		if (PathResult != ENavigationQueryResult::Success)
		{
			continue;
		}

		if (PathLength > BestPathLength)
		{
			continue;
		}

		if (PathCost > BestPathCost)
		{
			continue;
		}

		BestPathLength = PathLength;
		BestPathCost = PathCost;
		BestPoint = Point;
	}

	if (BestPoint)
	{
		OutPoint = BestPoint->GetActorLocation();
		return true;
	}

	return false;
}