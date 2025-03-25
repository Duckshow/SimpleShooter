// Fill out your copyright notice in the Description page of Project Settings.


#include "KillEmAllGameMode.h"
#include "EngineUtils.h"
#include "GameFramework//Controller.h"
#include "ShooterAIController.h"

void AKillEmAllGameMode::PawnKilled(APawn* PawnKilled) 
{
	Super::PawnKilled(PawnKilled);

	if (!PawnKilled)
	{
		return;
	}

	APlayerController* PlayerController = Cast<APlayerController>(PawnKilled->GetController());
	
	if (PlayerController)
	{
		EndGame(false);
	}

	for (AShooterAIController* AIController : TActorRange<AShooterAIController>(GetWorld())) 
	{
		if (!AIController->IsDead())
		{
			return;
		}
	}

	EndGame(true);
}

void AKillEmAllGameMode::EndGame(bool bIsPlayerWinner) 
{
	for (AController* Controller : TActorRange<AController>(GetWorld())) 
	{
		bool isWinner = Controller->IsPlayerController() == bIsPlayerWinner;

		UE_LOG(LogTemp, Warning, TEXT("%s is winner: %s"), *Controller->GetName(), isWinner ? TEXT("true") : TEXT("false"))

		Controller->GameHasEnded(Controller->GetPawn(), isWinner);
	}
}