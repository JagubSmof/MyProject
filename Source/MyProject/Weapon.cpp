// Fill out your copyright notice in the Description page of Project Settings.

#include "Weapon.h"

AWeapon::AWeapon()
{
	fireRate = 2;
	coolDown = 0;
	ammoCount = -1;
	// Cache our sound effect
	static ConstructorHelpers::FObjectFinder<USoundBase> FireAudio(TEXT("/Game/TwinStick/Audio/TwinStickFire.TwinStickFire"));
	FireSound = FireAudio.Object;
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

FLinearColor AWeapon::GetColour()
{
	if (rainbowGun)
	{
		FRandomStream RandomStream;
		RandomStream.GenerateNewSeed();
		int random_number = RandomStream.RandRange(1, 5);
		//std::random_device rd;
		//std::default_random_engine generator{ rd() };
		//std::uniform_int_distribution<int> dist{1, 4};
		//int random_number{ dist(generator) };
		if (random_number >= 4)
			return FLinearColor::Red;
		if (random_number >= 3)
			return FLinearColor::Blue;
		if (random_number >= 2)
			return FLinearColor::Green;
		else//if (random_number == 1)
			return FLinearColor::Yellow;
		//if (GEngine)
		//	GEngine->AddOnScreenDebugMessage(-1, 1.5, FColor::Green, TEXT("RNG is shit"));
	}
	else return projectileColour;
}

void AWeapon::Tick(float deltaTime)
{
	if (!deltaTime)
		return;
	if (!coolDown)
		return;
	if (coolDown > 0)//&& ammoCount == 1
		coolDown -= deltaTime;
}

void AWeapon::LaunchProjectile(FVector SpawnLocation, FRotator FireRotation)
{
	UWorld* const World = GetWorld();
	if (World != NULL)
	{
		AProjectile* projectile = World->SpawnActor<AProjectile>(SpawnLocation, FireRotation);
		if (projectile)
			projectile->initialize(damage, speed, projectileColour);
	}
}