// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectile.h"

AProjectile::AProjectile()
{
	//class MYPROJECT_API AProjectile : public AMyProjectProjectile
	setDefaultProjectileMesh();
	setDefaultProjectileMovement();
	setDefaultLight();
	//setLight(colour);
}
void AProjectile::initialize(int damage, int speed, FLinearColor colour)
{
	setLight(colour);
	this->damage = damage;
	this->speed = speed;
	setSpeed(speed);
}