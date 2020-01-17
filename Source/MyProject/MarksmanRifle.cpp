// Fill out your copyright notice in the Description page of Project Settings.


#include "MarksmanRifle.h"

AMarksmanRifle::AMarksmanRifle()
{
	//ANormalProjectile newProjectile = ANormalProjectile();
	weaponClass = FString("marksman Rifle");
	fireRate = 1.1f;
	coolDown = 0;
	damage = 5;
	speed = 3500;
	projectileColour = FLinearColor::Yellow;
	numberOfShots = 2;
	ammoCount = 10;
	static ConstructorHelpers::FObjectFinder<USoundBase> FireAudio(TEXT("/Game/Audio/Heavy_Shot"));
	FireSound = FireAudio.Object;
	rainbowGun = false;
	FRandomStream RandomStream;
	RandomStream.GenerateNewSeed();
	int random_number = RandomStream.RandRange(1, 4);
	if (random_number >= 3.2f)
		rainbowGun = true;
}

void AMarksmanRifle::LaunchProjectile(FVector SpawnLocation, FRotator FireRotation)
{
	//if (GEngine)
	//	GEngine->AddOnScreenDebugMessage(-1, 1.5, FColor::White, TEXT("Firing! Allegedly..."));

	//UWorld* const World = GetWorld();

	shotsToFire = 2;
	NextSpawnLocation = SpawnLocation;
	NextFireRotation = FireRotation;

	//if (World != NULL)
	//{
	//	ANormalProjectile* projectile = World->SpawnActor<ANormalProjectile>(SpawnLocation, FireRotation);
	//	//projectile->initialize(damage, speed, projectileColour);
	//	if (projectile)
	//	{
	//		projectile->setLight(GetColour());
	//		projectile->setDamage(damage);
	//	}
	//	//projectile->printFunction();

	//	ANormalProjectile* projectile2 = World->SpawnActor<ANormalProjectile>(SpawnLocation, FireRotation);
	//	if (projectile2)
	//	{
	//		projectile2->setLight(GetColour());
	//		projectile2->setDamage(damage);
	//	}
	//}
	//else
	//	if (GEngine)
	//		GEngine->AddOnScreenDebugMessage(-1, 1.5, FColor::White, TEXT("There is no world apparently xD"));
}