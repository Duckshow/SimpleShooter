// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTService_UpdatePatrolPosition.generated.h"

/**
 * 
 */
UCLASS()
class SIMPLESHOOTER_API UBTService_UpdatePatrolPosition : public UBTService
{
	GENERATED_BODY()

public:
	UBTService_UpdatePatrolPosition();

protected:
	virtual void InitializeFromAsset(UBehaviorTree& Asset) override;
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

private:
	UPROPERTY(EditAnywhere, Category = Blackboard)
	struct FBlackboardKeySelector PatrolPathKey;

	UPROPERTY(EditAnywhere, Category = Blackboard)
	struct FBlackboardKeySelector PatrolTargetKey;

	UPROPERTY(EditAnywhere, Category = Blackboard)
	struct FBlackboardKeySelector PatrolStartPosKey;
};