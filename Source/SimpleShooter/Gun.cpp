// Fill out your copyright notice in the Description page of Project Settings.


#include "Gun.h"
#include "Components/SkeletalMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"
#include "Engine/DamageEvents.h"

// Sets default values
AGun::AGun()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = Root;

	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(RootComponent);

	MuzzleFlash = nullptr;
	HitEffect = nullptr;
}

void AGun::PullTrigger() 
{
	if (MuzzleFlash)
	{
		FName SocketName = TEXT("MuzzleFlashSocket");
		UGameplayStatics::SpawnEmitterAttached(MuzzleFlash, Mesh, SocketName);
		UGameplayStatics::SpawnSoundAttached(MuzzleSound, Mesh, SocketName);
	}

	FHitResult Hit;
	FVector ShotDirection;

	if (GunTrace(Hit, ShotDirection))
	{	
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), HitEffect, Hit.Location, ShotDirection.Rotation());
		UGameplayStatics::SpawnSoundAtLocation(GetWorld(), HitSound, Hit.Location);

		if (Hit.GetActor())
		{
			FPointDamageEvent DamageEvent(Damage, Hit, ShotDirection, nullptr);
			Hit.GetActor()->TakeDamage(Damage, DamageEvent, GetOwnerController(), this);
		}
	}
}

AController* AGun::GetOwnerController() const 
{
	APawn* OwnerPawn = Cast<APawn>(GetOwner());

	if (!OwnerPawn)
	{
		return nullptr;
	}

	return OwnerPawn->GetController();
}

bool AGun::GunTrace(FHitResult& Hit, FVector& ShotDirection) const 
{
	AController* OwnerController = GetOwnerController();

	if (!OwnerController)
	{
		return false;
	}

	FVector PlayerViewLocation;
	FRotator PlayerViewRotation;
	OwnerController->GetPlayerViewPoint(PlayerViewLocation, PlayerViewRotation);

	ShotDirection = -PlayerViewRotation.Vector();
	FVector Target = PlayerViewLocation + PlayerViewRotation.Vector() * MaxRange;

	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);
	Params.AddIgnoredActor(GetOwner());

	return GetWorld()->LineTraceSingleByChannel(Hit, PlayerViewLocation, Target, ECollisionChannel::ECC_GameTraceChannel1, Params);
}

// Called when the game starts or when spawned
void AGun::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AGun::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

