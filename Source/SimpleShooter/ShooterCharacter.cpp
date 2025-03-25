// Fill out your copyright notice in the Description page of Project Settings.


#include "ShooterCharacter.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Gun.h"
#include "Components/CapsuleComponent.h"
#include "ShooterGameModeBase.h"

// Sets default values
AShooterCharacter::AShooterCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

void AShooterCharacter::BeginPlay()
{
    Super::BeginPlay();

	Health = MaxHealth;

	GunInstance = GetWorld()->SpawnActor<AGun>(GunClass);
	GetMesh()->HideBoneByName(TEXT("weapon_r"), EPhysBodyOp::PBO_None);
	GunInstance->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("WeaponSocket"));
	GunInstance->SetOwner(this);
}

bool AShooterCharacter::IsDead() const 
{
	return !(Health > 0.f);
}

float AShooterCharacter::GetHealthPercent() const
{
	return Health / MaxHealth;
}

// Called every frame
void AShooterCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AShooterCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	auto Input = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	
	if (!Input)
	{
		return;
	}

	Input->BindAction(InputMove, ETriggerEvent::Triggered, this, &AShooterCharacter::Move);
	Input->BindAction(InputLook, ETriggerEvent::Triggered, this, &AShooterCharacter::Look);
	Input->BindAction(InputJump, ETriggerEvent::Triggered, this, &AShooterCharacter::DoJump);
	Input->BindAction(InputPullTrigger, ETriggerEvent::Triggered, this, &AShooterCharacter::PullTrigger);
}

void AShooterCharacter::Move(const FInputActionInstance& Instance) 
{
	FVector2D Value = Instance.GetValue().Get<FVector2D>();

	AddMovementInput(GetActorForwardVector() * Value.X + GetActorRightVector() * Value.Y);
}

void AShooterCharacter::Look(const FInputActionInstance& Instance)
{
	FVector2D Value = Instance.GetValue().Get<FVector2D>();

	AddControllerPitchInput(Value.Y);
	AddControllerYawInput(Value.X);
}

void AShooterCharacter::DoJump(const FInputActionInstance& Instance)
{
	Super::Jump();
}

void AShooterCharacter::PullTrigger(const FInputActionInstance& Instance) 
{
	PullTriggerAI();
}

void AShooterCharacter::PullTriggerAI()
{
	if (!GunInstance)
	{
		return;
	}

	GunInstance->PullTrigger();
}

float AShooterCharacter::TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float DamageApplied = Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);
	float NewHealth = FMath::Max(Health - DamageApplied, 0.f);
	float Diff = Health - NewHealth;

	Health = NewHealth;

	if (IsDead())
	{
		if (AShooterGameModeBase* GameMode = GetWorld()->GetAuthGameMode<AShooterGameModeBase>())
		{
			GameMode->PawnKilled(this);
		}
		
		DetachFromControllerPendingDestroy();
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}

	return Diff;
}