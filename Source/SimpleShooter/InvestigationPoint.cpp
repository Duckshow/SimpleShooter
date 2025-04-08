// Fill out your copyright notice in the Description page of Project Settings.


#include "InvestigationPoint.h"
#include "Components/SphereComponent.h"
#include "GameFramework/Character.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"

AInvestigationPoint::AInvestigationPoint()
{
	PrimaryActorTick.bCanEverTick = true;

	TriggerVolume = CreateDefaultSubobject<USphereComponent>(TEXT("TriggerVolume"));
	RootComponent = TriggerVolume;
	TriggerVolume->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	TriggerVolume->SetCollisionResponseToAllChannels(ECR_Ignore);
	TriggerVolume->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);

	TriggerVolume->OnComponentBeginOverlap.AddDynamic(this, &AInvestigationPoint::OnOverlapBegin);
	TriggerVolume->OnComponentEndOverlap.AddDynamic(this, &AInvestigationPoint::OnOverlapEnd);
}

void AInvestigationPoint::OnConstruction(const FTransform& Transform) 
{
	for (TObjectPtr<AInvestigationPoint> Neighbor : Neighbors)
	{
		if (!Neighbor)
		{
			continue;
		}

		if (!Neighbor->Neighbors.Contains(this))
		{
			Neighbor->Neighbors.Add(this);
		}
	}
}

void AInvestigationPoint::VisualizeNeighbors() const
{ 
	for (TObjectPtr<AInvestigationPoint> Neighbor : Neighbors)
	{
		if (Neighbor)
		{
			DrawDebugLine(GetWorld(), GetActorLocation(), Neighbor->GetActorLocation(), FColor::Yellow, false, 5.f);
		}
	}
}

void AInvestigationPoint::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	RemoveOldInvestigations();

	for (TObjectPtr<ACharacter> Character : OverlappingCharacters) 
	{
		if (!Character || !Character->Controller)
		{
			return;
		}

		if (Character->Controller->IsPlayerController() || !Character->Controller->LineOfSightTo(this))
		{
			return;
		}

		if (!RecentInvestigations.Contains(Character))
		{
			RecentInvestigations.Add(Character, 0.f);
		}

		RecentInvestigations[Character] = GetWorld()->GetTimeSeconds();
	}
}

void AInvestigationPoint::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	TObjectPtr<ACharacter> Character = Cast<ACharacter>(OtherActor);
	
	if (!Character || !Character->Controller || Character->Controller->IsPlayerController())
	{
		return;
	}

	OverlappingCharacters.Add(Character);
}

void AInvestigationPoint::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	TObjectPtr<ACharacter> Character = Cast<ACharacter>(OtherActor);

	if (!Character || !Character->Controller || Character->Controller->IsPlayerController())
	{
		return;
	}

	OverlappingCharacters.Remove(Character);
}

void AInvestigationPoint::RemoveOldInvestigations() 
{
	float CurrentTime = GetWorld()->GetTimeSeconds();

	for (auto It = RecentInvestigations.CreateIterator(); It; ++It) 
	{
		if (!It->Key || !It->Key->Controller)
		{
			It.RemoveCurrent();
		}

		if (CurrentTime - It->Value > TimeBeforeBecomeUnInvestigated)
		{
			It.RemoveCurrent();
		}
	}
}