// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NormalProjectile.h"
#include "Weapon.h"
#include "MarksmanRifle.generated.h"

/**
 * 
 */
UCLASS()
class MYPROJECT_API AMarksmanRifle : public AWeapon
{
	GENERATED_BODY()
public:
	AMarksmanRifle();
	void LaunchProjectile(FVector, FRotator);
};
