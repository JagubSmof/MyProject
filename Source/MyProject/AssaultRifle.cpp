// Fill out your copyright notice in the Description page of Project Settings.


#include "AssaultRifle.h"

AAssaultRifle::AAssaultRifle()
{
	//ANormalProjectile newProjectile = ANormalProjectile();
	weaponClass = FString("Assault Rifle");
	fireRate = 5;
	coolDown = 0;
	damage = 1;
	speed = 3500;
	projectileColour = FLinearColor::Blue;
	ammoCount = 30;
	rainbowGun = false;
	FRandomStream RandomStream;
	RandomStream.GenerateNewSeed();
	int random_number = RandomStream.RandRange(1, 4);
	if (random_number >= 3.2f)
		rainbowGun = true;
	static ConstructorHelpers::FObjectFinder<USoundBase> FireAudio(TEXT("/Game/Audio/Normal_Shot"));
	FireSound = FireAudio.Object;
}

void AAssaultRifle::LaunchProjectile(FVector SpawnLocation, FRotator FireRotation)
{
	//if (GEngine)
	//	GEngine->AddOnScreenDebugMessage(-1, 1.5, FColor::White, TEXT("Firing! Allegedly..."));

	//UWorld* const World = GetWorld();
	if (World != NULL)
	{
		ANormalProjectile* projectile = World->SpawnActor<ANormalProjectile>(SpawnLocation, FireRotation);
		//projectile->initialize(damage, speed, projectileColour);
		if (projectile)
		{
			projectile->setLight(GetColour());
			projectile->setDamage(damage);
		}
		//projectile->printFunction();
	}
	//if (GEngine)
	//	GEngine->AddOnScreenDebugMessage(-1, 1.5, FColor::White, TEXT("There is no world apparently xD"));
}