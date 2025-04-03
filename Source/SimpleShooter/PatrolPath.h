// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PatrolPath.generated.h"

UCLASS()
class SIMPLESHOOTER_API APatrolPath : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APatrolPath();

public:	
	TTuple<FVector, float> FindStartingPoint(const FVector& ActorWorldLocation) const;
	FVector GetNextLocation(float& Key, int& DirectionSign) const;

private:
	UPROPERTY()
	TObjectPtr<class USplineComponent> Spline;

	UPROPERTY(EditAnywhere, Category = "Path")
	float WaitTimeAtSplineEnd = 3.f;
};
