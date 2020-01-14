// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MyProjectProjectile.h"
#include "Projectile.generated.h"

/**
 * 
 */
UCLASS()
class MYPROJECT_API AProjectile : public AMyProjectProjectile
{
	GENERATED_BODY()

	int damage;
	int speed;

public:
	AProjectile();
	virtual void initialize(int, int, FLinearColor);

};
