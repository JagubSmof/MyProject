// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Projectile.h"
#include "CoreMinimal.h"
#include "ConstructorHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundBase.h"
#include "Weapon.generated.h"

/**
 * 
 */
UCLASS(config = Game)
class AWeapon : public AActor
{
	GENERATED_BODY()

public:

	/** Sound to play each time we fire */
	UPROPERTY(Category = Audio, EditAnywhere, BlueprintReadWrite)
		class USoundBase* FireSound;

	UPROPERTY(EditAnywhere)
		bool rainbowGun;

	UPROPERTY(EditAnywhere)
		FLinearColor projectileColour;

	//UPROPERTY(EditAnywhere)
		//AProjectile* projectile;

	/** This is how many shots are fired per second*/
	UPROPERTY(EditAnywhere)
		float fireRate;

	UPROPERTY(EditAnywhere)
		float coolDown;

	UPROPERTY(EditAnywhere)
		int ammoCount;

	UPROPERTY(EditAnywhere)
		int damage;

	UPROPERTY(EditAnywhere)
		int speed;

	UPROPERTY(EditAnywhere)
		FString weaponClass;

	//virtual void pickRandomColour();
	virtual bool FireWeapon(FVector, FRotator);
	virtual void LaunchProjectile(FVector, FRotator);

	void Tick(float);

	AWeapon();
	~AWeapon();
};
