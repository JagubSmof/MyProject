// Fill out your copyright notice in the Description page of Project Settings.


#include "Pistol.h"

APistol::APistol()
{
	//ANormalProjectile newProjectile = ANormalProjectile();
	weaponClass = FString("Pistol");
	damage = 1;
	speed = 2250;
	projectileColour = FLinearColor::Red;
}

void APistol::LaunchProjectile(FVector SpawnLocation, FRotator FireRotation)
{
	//if (GEngine)
	//	GEngine->AddOnScreenDebugMessage(-1, 1.5, FColor::White, TEXT("Firing! Allegedly..."));

	UWorld* const World = GetWorld();
	if (World != NULL)
	{
		ANormalProjectile* projectile = World->SpawnActor<ANormalProjectile>(SpawnLocation, FireRotation);
		//projectile->initialize(damage, speed, projectileColour);
		projectile->setLight(projectileColour);
		//projectile->printFunction();
	}
}
