// Fill out your copyright notice in the Description page of Project Settings.

#include "Weapon.h"

AWeapon::AWeapon()
{
	fireRate = 2;
	coolDown = 0;
	ammoCount = -1;
	// Cache our sound effect
	static ConstructorHelpers::FObjectFinder<USoundBase> FireAudio(TEXT("/Game/TwinStick/Audio/TwinStickFire.TwinStickFire"));
	FireSound = FireAudio.Object;//TODO: Get this working.
}

AWeapon::~AWeapon()
{
}

bool AWeapon::FireWeapon(FVector SpawnLocation, FRotator FireRotation)
{
	if (coolDown <= 0)
	{
		if (fireRate != 0)
			coolDown = 1 / fireRate;
		else
			coolDown = 1;

		if (FireSound != nullptr)
		{
			UGameplayStatics::PlaySoundAtLocation(this, FireSound, GetActorLocation());
		}

		LaunchProjectile(SpawnLocation, FireRotation);

		if (ammoCount > 0)
			ammoCount--;

		if (ammoCount == 0)
			return false;
	}
	return true;
}

void AWeapon::Tick(float deltaTime)
{
	if (coolDown > 0)//&& ammoCount == 1
		coolDown -= deltaTime;
}

void AWeapon::LaunchProjectile(FVector SpawnLocation, FRotator FireRotation)
{
	UWorld* const World = GetWorld();
	if (World != NULL)
	{
		AProjectile* projectile = World->SpawnActor<AProjectile>(SpawnLocation, FireRotation);
		projectile->initialize(damage, speed, projectileColour);
	}
}