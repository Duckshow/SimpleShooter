// Fill out your copyright notice in the Description page of Project Settings.


#include "PatrolPath.h"
#include "Components/SplineComponent.h"

APatrolPath::APatrolPath()
{
	Spline = CreateDefaultSubobject<USplineComponent>(TEXT("Path Spline"));
	RootComponent = Spline;
}

TTuple<FVector, float> APatrolPath::FindStartingPoint(const FVector& ActorWorldLocation) const 
{
	Spline->Duration = Spline->GetSplineLength();
	ESplineCoordinateSpace::Type Space = ESplineCoordinateSpace::World;

	float Key = Spline->FindInputKeyClosestToWorldLocation(ActorWorldLocation);
	FVector ClosestLocationOnSpline = Spline->GetLocationAtSplineInputKey(Key, Space);

	return TTuple<FVector, float>(ClosestLocationOnSpline, Key);
}

FVector APatrolPath::GetNextLocation(float& KeyRef, int& DirectionSignRef) const 
{
	int RoundedKey = FMath::RoundToInt(KeyRef);
	int KeyCount = Spline->SplineCurves.Position.Points.Num();
	
	if (RoundedKey == 0)
	{
		DirectionSignRef = 1;
	}
	else if (RoundedKey == KeyCount - 1)
	{
		DirectionSignRef = -1;
	}

	KeyRef = RoundedKey + DirectionSignRef;

	return Spline->GetLocationAtSplineInputKey(KeyRef, ESplineCoordinateSpace::World);
}
