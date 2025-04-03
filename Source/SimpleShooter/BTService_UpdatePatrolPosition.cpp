// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_UpdatePatrolPosition.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"
#include "PatrolPath.h"
#include "AIController.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

UBTService_UpdatePatrolPosition::UBTService_UpdatePatrolPosition()
{
	NodeName = TEXT("Update Patrol Position");
}

void UBTService_UpdatePatrolPosition::InitializeFromAsset(UBehaviorTree& Asset)
{
	Super::InitializeFromAsset(Asset);
	UBlackboardData* BBAsset = GetBlackboardAsset();
	if (ensure(BBAsset))
	{
		PatrolPathKey.ResolveSelectedKey(*BBAsset);
		PatrolTargetKey.ResolveSelectedKey(*BBAsset);
		PatrolStartPosKey.ResolveSelectedKey(*BBAsset);
	}
}

void UBTService_UpdatePatrolPosition::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	TObjectPtr<AAIController> AIController = OwnerComp.GetAIOwner();

	if (!AIController)
	{
		return;
	}

	TObjectPtr<UBlackboardComponent> Blackboard = OwnerComp.GetBlackboardComponent();
	
	TObjectPtr<UObject> PatrolPathObject = Blackboard->GetValueAsObject(PatrolPathKey.SelectedKeyName);

	if (!PatrolPathObject)
	{
		return;
	}

	TObjectPtr<APatrolPath> PatrolPath = Cast<APatrolPath>(PatrolPathObject);

	if (!PatrolPath)
	{
		return;
	}

	TObjectPtr<ACharacter> Character = AIController->GetCharacter();
	FVector ActorLocation = Character->GetActorLocation();

	//FVector ClosestPathLocation;
	//if (!PatrolPath->IsOnPath(ActorLocation, ClosestPathLocation))
	//{
	//	//UE_LOG(LogTemp, Warning, TEXT("Set start pos: %s"), *ClosestPathLocation.ToString());
	//	Blackboard->SetValueAsVector(PatrolStartPosKey.SelectedKeyName, ClosestPathLocation);
	//	return;
	//}

	//float ActorMoveSpeed = Character->GetCharacterMovement()->MaxWalkSpeed;
	//PatrolPath->UpdatePatrolTarget(ActorLocation, ActorMoveSpeed, DeltaSeconds);
	//
	////UE_LOG(LogTemp, Warning, TEXT("Is on path, moving: %s"), *PatrolPath->GetPatrolTarget()->GetActorLocation().ToString());

	//Blackboard->SetValueAsObject(PatrolTargetKey.SelectedKeyName, PatrolPath->GetPatrolTarget());
	//Blackboard->ClearValue(PatrolStartPosKey.SelectedKeyName);
}