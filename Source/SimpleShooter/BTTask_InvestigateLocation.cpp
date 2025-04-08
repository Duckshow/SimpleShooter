// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_InvestigateLocation.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "EngineUtils.h"
#include "InvestigationPointManager.h"
#include "AIController.h"
#include "DrawDebugHelpers.h"
#include "GameFramework/Character.h"

UBTTask_InvestigateLocation::UBTTask_InvestigateLocation()
{
	NodeName = TEXT("Set Investigation Location");
}

void UBTTask_InvestigateLocation::InitializeFromAsset(UBehaviorTree& Asset) 
{
	Super::InitializeFromAsset(Asset);

	if (UBlackboardData* BBAsset = GetBlackboardAsset())
	{
		LastKnownPlayerLocationKey.ResolveSelectedKey(*BBAsset);
		InvestigationLocationKey.ResolveSelectedKey(*BBAsset);
	}
}

EBTNodeResult::Type UBTTask_InvestigateLocation::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	TObjectPtr<ACharacter> Character = OwnerComp.GetAIOwner()->GetCharacter();

	if (!Character)
	{
		UE_LOG(LogTemp, Error, TEXT("Investigation failed: no character!"));
		return EBTNodeResult::Failed;
	}

	for (AInvestigationPointManager* Manager : TActorRange<AInvestigationPointManager>(GetWorld())) 
	{
		FVector LastKnownPlayerLocation = OwnerComp.GetBlackboardComponent()->GetValueAsVector(LastKnownPlayerLocationKey.SelectedKeyName);
		FVector InvestigationLocation = Manager->FindClosestPointNotInvestigated(Character, LastKnownPlayerLocation);
		OwnerComp.GetBlackboardComponent()->SetValueAsVector(InvestigationLocationKey.SelectedKeyName, InvestigationLocation);

		UE_LOG(LogTemp, Warning, TEXT("Investigation Location: %s"), *InvestigationLocation.ToString());
		DrawDebugLine(GetWorld(), Character->GetActorLocation(), LastKnownPlayerLocation, FColor::Red, false, 5.f);
		DrawDebugLine(GetWorld(), LastKnownPlayerLocation, InvestigationLocation, FColor::Yellow, false, 5.f);

		DrawDebugPoint(GetWorld(), LastKnownPlayerLocation, 50.f, FColor::Red, false, 5.f);
		DrawDebugPoint(GetWorld(), InvestigationLocation, 50.f, FColor::Yellow, false, 5.f);
		
		return EBTNodeResult::Succeeded;
	}

	UE_LOG(LogTemp, Error, TEXT("Investigation failed: no manager!"));
	return EBTNodeResult::Failed;
}
