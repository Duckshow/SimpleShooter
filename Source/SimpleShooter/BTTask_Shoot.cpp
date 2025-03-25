// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_Shoot.h"
#include "AIController.h"
#include "ShooterCharacter.h"

UBTTask_Shoot::UBTTask_Shoot() 
{
	NodeName = TEXT("Shoot");
}

EBTNodeResult::Type UBTTask_Shoot::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	AAIController* Owner = OwnerComp.GetAIOwner();
	if (!Owner)
	{
		return EBTNodeResult::Failed;
	}

	ACharacter* Character = Owner->GetCharacter();
	if (!Character)
	{
		return EBTNodeResult::Failed;
	}

	AShooterCharacter* ShooterCharacter = Cast<AShooterCharacter>(Character);
	if (!ShooterCharacter)
	{
		return EBTNodeResult::Failed;
	}

	ShooterCharacter->PullTriggerAI();
	return EBTNodeResult::Succeeded;
}