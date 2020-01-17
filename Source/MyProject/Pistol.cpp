// Fill out your copyright notice in the Description page of Project Settings.


#include "Pistol.h"

APistol::APistol()
{
	//ANormalProjectile newProjectile = ANormalProjectile();
	fireRate = 1.4f;
	weaponClass = FString("Pistol");
	damage = 1;
	speed = 3250;
	projectileColour = FLinearColor::Red;
	static ConstructorHelpers::FObjectFinder<USoundBase> FireAudio(TEXT("/Game/Audio/Normal_Shot"));
	FireSound = FireAudio.Object;
}

void APistol::LaunchProjectile(FVector SpawnLocation, FRotator FireRotation)
{
	//if (GEngine)
	//	GEngine->AddOnScreenDebugMessage(-1, 1.5, FColor::White, TEXT("Firing! Allegedly..."));

	World = GetWorld();
	if (World != NULL)
	{
		ANormalProjectile* projectile = World->SpawnActor<ANormalProjectile>(SpawnLocation, FireRotation);
		//projectile->initialize(damage, speed, projectileColour);
		if (projectile)
<<<<<<< Updated upstream
=======
			//projectile->setLight(projectileColour);
>>>>>>> Stashed changes
		{
			projectile->setLight(GetColour());
			projectile->setDamage(damage);
		}
		//projectile->printFunction();
	}
	else
		if (GEngine)
			GEngine->AddOnScreenDebugMessage(-1, 1.5, FColor::White, TEXT("There is no world apparently xD"));
}

