// Fill out your copyright notice in the Description page of Project Settings.


#include "MarksmanRifle.h"

AMarksmanRifle::AMarksmanRifle()
{
	//ANormalProjectile newProjectile = ANormalProjectile();
	weaponClass = FString("marksman Rifle");
	fireRate = 1;
	coolDown = 0;
	damage = 5;
	speed = 3500;
	projectileColour = FLinearColor::Blue;
	ammoCount = 30;
	rainbowGun = true;
}

void AMarksmanRifle::LaunchProjectile(FVector SpawnLocation, FRotator FireRotation)
{
	//if (GEngine)
	//	GEngine->AddOnScreenDebugMessage(-1, 1.5, FColor::White, TEXT("Firing! Allegedly..."));

	UWorld* const World = GetWorld();
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
}