// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Projectile.h"
#include "NormalProjectile.generated.h"

/**
 * 
 */
UCLASS()
class MYPROJECT_API ANormalProjectile : public AProjectile
{
	GENERATED_BODY()
public:
	ANormalProjectile();
	//ANormalProjectile();
	void setLight(FLinearColor);
	void setSpeed(int);
	void setDamage(int);
	void printFunction();
};
