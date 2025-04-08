// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_Patrol.h"
#include "AIController.h"
#include "GameFramework/Character.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "PatrolPath.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "Navigation/PathFollowingComponent.h"

UBTTask_Patrol::UBTTask_Patrol()
{
	NodeName = TEXT("Patrol");
}

EBTNodeResult::Type UBTTask_Patrol::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	TObjectPtr<AAIController> AI = OwnerComp.GetAIOwner();

	if (!AI)
	{
		return EBTNodeResult::Failed;
	}

	TObjectPtr<ACharacter> Character = AI->GetCharacter();
	TObjectPtr<UCharacterMovementComponent> MovementComp = Character->GetCharacterMovement();

	TObjectPtr<UObject> PatrolPathObject = OwnerComp.GetBlackboardComponent()->GetValueAsObject(PatrolPathKey.SelectedKeyName);
	TObjectPtr<APatrolPath> PatrolPath = Cast<APatrolPath>(PatrolPathObject);
	
	if (!PatrolPath)
	{
		return EBTNodeResult::Failed;
	}

	TTuple<FVector, float> StartingPointAndKey= PatrolPath->FindStartingPoint(Character->GetActorLocation());

	CurrentKey = StartingPointAndKey.Get<float>();
	DirectionSign = CurrentKey - (FMath::RoundToInt(CurrentKey) + 0.5f) < 0.f ? -1 : 1;

	UAIBlueprintHelperLibrary::SimpleMoveToLocation(AI, StartingPointAndKey.Get<FVector>());
	
	bNotifyTick = true;
	return EBTNodeResult::InProgress;
}

void UBTTask_Patrol::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	TObjectPtr<AAIController> AI = OwnerComp.GetAIOwner();

	if (!AI)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
		return;
	}

	TObjectPtr<ACharacter> Character = AI->GetCharacter();
	TObjectPtr<UCharacterMovementComponent> MovementComp = Character->GetCharacterMovement();
	TObjectPtr<UObject> PatrolPathObject = OwnerComp.GetBlackboardComponent()->GetValueAsObject(PatrolPathKey.SelectedKeyName);
	TObjectPtr<APatrolPath> PatrolPath = Cast<APatrolPath>(PatrolPathObject);

	EPathFollowingStatus::Type Status = AI->GetPathFollowingComponent()->GetStatus();

	if (Status == EPathFollowingStatus::Idle || Status == EPathFollowingStatus::Paused)
	{
		int NewDirectionSign = DirectionSign;
		FVector NextLocation = PatrolPath->GetNextLocation(CurrentKey, NewDirectionSign);
		
		if (NewDirectionSign != DirectionSign)
		{
			FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
			return;
		}

		UAIBlueprintHelperLibrary::SimpleMoveToLocation(AI, NextLocation);
		DrawDebugLine(GetWorld(), Character->GetActorLocation(), NextLocation, FColor::Red, false, 5.f);
	}
}