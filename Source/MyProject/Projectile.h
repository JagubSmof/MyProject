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

	int speed;

public:
	AProjectile();
	void setDamage(int);
	virtual void initialize(int, int, FLinearColor);
	UPROPERTY(EditAnywhere)
		int damage;
};
