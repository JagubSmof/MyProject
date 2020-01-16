// Fill out your copyright notice in the Description page of Project Settings.

#include "Projectile.h"

AProjectile::AProjectile()
{
	//class MYPROJECT_API AProjectile : public AMyProjectProjectile
	setDefaultProjectileMesh();
	setDefaultProjectileMovement();
	setDefaultLight();
	damage = 1;
	//setLight(colour);
}
void AProjectile::setDamage(int damage)
{
	this->damage = damage;
}
void AProjectile::initialize(int damage, int speed, FLinearColor colour)
{
	setLight(colour);
	this->damage = damage;
	this->speed = speed;
	setSpeed(speed);
}