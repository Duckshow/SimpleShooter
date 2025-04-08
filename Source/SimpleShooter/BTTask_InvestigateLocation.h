// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_InvestigateLocation.generated.h"

/**
 * 
 */
UCLASS()
class SIMPLESHOOTER_API UBTTask_InvestigateLocation : public UBTTaskNode
{
	GENERATED_BODY()

	virtual void InitializeFromAsset(UBehaviorTree& Asset) override;
	
public:
	UBTTask_InvestigateLocation();

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

private:
	UPROPERTY(EditAnywhere, Category = Blackboard)
	struct FBlackboardKeySelector LastKnownPlayerLocationKey;

	UPROPERTY(EditAnywhere, Category = Blackboard)
	struct FBlackboardKeySelector InvestigationLocationKey;
};
