// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_SetMoveSpeed.h"
#include "AIController.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

UBTTask_SetMoveSpeed::UBTTask_SetMoveSpeed()
{
	NodeName = TEXT("Set Move Speed");
}

EBTNodeResult::Type UBTTask_SetMoveSpeed::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	if (TObjectPtr<UCharacterMovementComponent> MovementComp = OwnerComp.GetAIOwner()->GetCharacter()->GetCharacterMovement())
	{
		MovementComp->MaxWalkSpeed = Value;
	}

	return EBTNodeResult::Succeeded;
}