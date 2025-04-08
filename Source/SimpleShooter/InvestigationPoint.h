// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "InvestigationPoint.generated.h"

UCLASS()
class SIMPLESHOOTER_API AInvestigationPoint : public AActor
{
	GENERATED_BODY()
	
public:	
	AInvestigationPoint();

	virtual void OnConstruction(const FTransform& Transform) override;
	virtual void Tick(float DeltaTime) override;

	bool HasInvestigatedRecently(const ACharacter*& Character, float& LastTimeInvestigated) const 
	{ 
		if (RecentInvestigations.Contains(Character))
		{
			LastTimeInvestigated = RecentInvestigations[Character];
			return true;
		}

		return false;
	}

	TArray<TObjectPtr<AInvestigationPoint>> GetNeighbors() const 
	{
		return Neighbors;
	}

private:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class USphereComponent> TriggerVolume;

	UPROPERTY(EditAnywhere, Category = "Investigation Point")
	TArray<TObjectPtr<AInvestigationPoint>> Neighbors;

	UPROPERTY(EditAnywhere, Category = "Investigation Point")
	float TimeBeforeBecomeUnInvestigated = 90.f;

	TSet<TObjectPtr<ACharacter>> OverlappingCharacters;
	TMap<TObjectPtr<ACharacter>, float> RecentInvestigations;
	
	UFUNCTION(CallInEditor, Category = "Investigation Point")
	void VisualizeNeighbors() const;

	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	
	void RemoveOldInvestigations();
};
