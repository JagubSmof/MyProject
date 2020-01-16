// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapon.h"
#include "NormalProjectile.h"
#include "Shotgun.generated.h"

/**
 * 
 */
UCLASS()
class MYPROJECT_API AShotgun : public AWeapon
{
	GENERATED_BODY()
	void spawnTing(UWorld*, FVector, FRotator);
public:
	AShotgun();

	void LaunchProjectile(FVector, FRotator);
};
