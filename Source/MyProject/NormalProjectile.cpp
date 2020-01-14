// Fill out your copyright notice in the Description page of Project Settings.


#include "NormalProjectile.h"

ANormalProjectile::ANormalProjectile()
{
	//setDefaultProjectileMesh();
	//setDefaultProjectileMovement();
	//damage = 1;
	//speed = 2500;
}

void ANormalProjectile::setLight(FLinearColor colour)
{
	//light = CreateDefaultSubobject<UPointLightComponent>(TEXT("Light"));
	light->SetLightColor(colour, true);
	//light->SetupAttachment(RootComponent);
}

void ANormalProjectile::setSpeed(int speed)
{
	//this->speed = speed;
}

void ANormalProjectile::setDamage(int damage)
{
	//this->damage = damage;
}

void ANormalProjectile::printFunction()
{
	if (GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 1.5, FColor::Red, TEXT("AAAAAAAAAA"));
}