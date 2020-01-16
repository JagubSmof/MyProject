// Fill out your copyright notice in the Description page of Project Settings.


#include "Shotgun.h"

AShotgun::AShotgun()
{
	//ANormalProjectile newProjectile = ANormalProjectile();
	weaponClass = FString("Shotgun");
	damage = 1;
	speed = 2500;
	projectileColour = FLinearColor::Green;
	ammoCount = 8;
	fireRate = 0.75;
}

void AShotgun::spawnTing(UWorld* World, FVector SpawnLocation, FRotator rotation)
{
	if (GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 1.5, FColor::Red, TEXT("Firing! Allegedly..."));
	ANormalProjectile* projectile = World->SpawnActor<ANormalProjectile>(SpawnLocation, rotation);
	if (projectile)
		projectile->setLight(GetColour());
	else
	{
		if (GEngine)
			GEngine->AddOnScreenDebugMessage(-1, 1.5, FColor::Green, TEXT("No projectile fired!"));
	}
}

void AShotgun::LaunchProjectile(FVector SpawnLocation, FRotator FireRotation)
{
	if (GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 1.5, FColor::White, TEXT("Firing! Allegedly..."));
	FRotator bullet1 = FireRotation;
	bullet1.Add(0, -15, 0);
	FRotator bullet2 = FireRotation;
	bullet2.Add(0, -9, 0);
	FRotator bullet3 = FireRotation;
	bullet3.Add(0, -3, 0);
	FRotator bullet4 = FireRotation;
	bullet4.Add(0, 3, 0);
	FRotator bullet5 = FireRotation;
	bullet5.Add(0, 9, 0);
	FRotator bullet6 = FireRotation;
	bullet6.Add(0, 15, 0);
	UWorld* const World = GetWorld();
	if (World != NULL)
	{
		spawnTing(World, SpawnLocation, bullet1);
		spawnTing(World, SpawnLocation, bullet2);
		spawnTing(World, SpawnLocation, bullet3);
		spawnTing(World, SpawnLocation, bullet4);
		spawnTing(World, SpawnLocation, bullet5);
		spawnTing(World, SpawnLocation, bullet6);
		if (GEngine)
			GEngine->AddOnScreenDebugMessage(-1, 1.5, FColor::Cyan, TEXT("Firing! Allegedly..."));
	}
}