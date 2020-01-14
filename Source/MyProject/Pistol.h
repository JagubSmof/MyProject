// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapon.h"
#include "NormalProjectile.h"
#include "Pistol.generated.h"

/**
 * 
 */
UCLASS()
class MYPROJECT_API APistol : public AWeapon
{
	GENERATED_BODY()

public:
	APistol();

public:
	void LaunchProjectile(FVector SpawnLocation, FRotator FireRotation) override;

};
