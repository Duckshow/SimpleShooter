// Fill out your copyright notice in the Description page of Project Settings.


#include "ShooterAIController.h"
#include "Kismet/GameplayStatics.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "ShooterCharacter.h"
#include "PatrolPath.h"

void AShooterAIController::BeginPlay() 
{
	Super::BeginPlay();

	if (AIBehavior)
	{
		RunBehaviorTree(AIBehavior);

		if (Blackboard)
		{
			Blackboard->SetValueAsVector(StartLocationBlackboardkey, GetPawn()->GetActorLocation());

			if (AShooterCharacter* ControlledCharacter = Cast<AShooterCharacter>(GetPawn()))
			{
				Blackboard->SetValueAsObject(PatrolPathBlackboardkey, Cast<UObject>(ControlledCharacter->GetPatrolPath()));
			}
		}
	}
}

void AShooterAIController::Tick(float DeltaSeconds) 
{
	Super::Tick(DeltaSeconds);
}

bool AShooterAIController::IsDead() const
{
	if (AShooterCharacter* ControlledCharacter = Cast<AShooterCharacter>(GetPawn()))
	{
		return ControlledCharacter->IsDead();
	}

	return true;
}