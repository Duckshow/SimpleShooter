// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "ShooterCharacter.generated.h"

class AGun;

UCLASS()
class SIMPLESHOOTER_API AShooterCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AShooterCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	UFUNCTION(BlueprintPure)
	bool IsDead() const;

	UFUNCTION(BlueprintPure)
	float GetHealthPercent() const;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
	
	void PullTriggerAI();

private:	
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputAction* InputMove = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputAction* InputLook = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputAction* InputJump = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputAction* InputPullTrigger = nullptr;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AGun> GunClass;

	UPROPERTY()
	AGun* GunInstance = nullptr;

	UPROPERTY(EditDefaultsOnly)
	float MaxHealth = 100.f;

	UPROPERTY(VisibleAnywhere)
	float Health = 100.f;

	void Move(const struct FInputActionInstance& Instance);
	void Look(const struct FInputActionInstance& Instance);
	void DoJump(const struct FInputActionInstance& Instance);
	void PullTrigger(const struct FInputActionInstance& Instance);
};
