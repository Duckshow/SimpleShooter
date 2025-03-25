// Fill out your copyright notice in the Description page of Project Settings.


#include "ShooterPlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "Blueprint//UserWidget.h"

void AShooterPlayerController::BeginPlay()
{
    Super::BeginPlay();

    UEnhancedInputLocalPlayerSubsystem* InputSubsystem =
        ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());

    if (InputSubsystem && InputMappingContext)
    {
        InputSubsystem->ClearAllMappings();
        InputSubsystem->AddMappingContext(InputMappingContext, 0);
    }

    Hud = CreateWidget(this, HudClass);
    if (Hud)
    {
        Hud->AddToViewport();
    }
}

void AShooterPlayerController::GameHasEnded(AActor* EndGameFocus, bool bIsWinner) 
{
    Super::GameHasEnded(EndGameFocus, bIsWinner);

    if (Hud)
    {
        Hud->RemoveFromViewport();
    }

    UUserWidget* Screen = CreateWidget(this, bIsWinner ? WinScreenClass : LoseScreenClass);

    if (Screen)
    {
        Screen->AddToViewport();
    }

    GetWorldTimerManager().SetTimer(RestartTimer, this, &APlayerController::RestartLevel, RestartDelay);
}