// Fill out your copyright notice in the Description page of Project Settings.


#include "Shotgun.h"

AShotgun::AShotgun()
{
	//ANormalProjectile newProjectile = ANormalProjectile();
	weaponClass = FString("Shotgun");
	damage = 1;
	speed = 2500;
	projectileColour = FLinearColor::Green;
	ammoCount = 10;
}

void spawnTing(UWorld* World)
{

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
		ANormalProjectile* projectile1 = World->SpawnActor<ANormalProjectile>(SpawnLocation, bullet1);
		projectile1->setLight(projectileColour);
		//if (!projectile1)
		//	if (GEngine)
		//		GEngine->AddOnScreenDebugMessage(-1, 1.5, FColor::Red, TEXT("WTF"));
		ANormalProjectile* projectile2 = World->SpawnActor<ANormalProjectile>(SpawnLocation, bullet2);
		projectile2->setLight(projectileColour);
		//ANormalProjectile* projectile3 = World->SpawnActor<ANormalProjectile>(SpawnLocation, FireRotation);
		//projectile3->setLight(projectileColour);
		//ANormalProjectile* projectile4 = World->SpawnActor<ANormalProjectile>(SpawnLocation, FireRotation);
		//projectile4->setLight(projectileColour);
		//ANormalProjectile* projectile5 = World->SpawnActor<ANormalProjectile>(SpawnLocation, FireRotation);
		//projectile5->setLight(projectileColour);
		//ANormalProjectile* projectile6 = World->SpawnActor<ANormalProjectile>(SpawnLocation, FireRotation);
		//projectile6->setLight(projectileColour);
		if (GEngine)
			GEngine->AddOnScreenDebugMessage(-1, 1.5, FColor::Cyan, TEXT("Firing! Allegedly..."));
	}
}