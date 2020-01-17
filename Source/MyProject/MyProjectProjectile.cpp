// Copyright 1998-2019 Epic Games, Inc. All Rights Reserve

#include "MyProjectProjectile.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Engine/StaticMesh.h"

AMyProjectProjectile::AMyProjectProjectile()
{
	//red = FLinearColor::Red;
	//AMyProjectProjectile(red);
}

AMyProjectProjectile::AMyProjectProjectile(FLinearColor colour) 
{
	setDefaultProjectileMesh();
	setDefaultProjectileMovement();
	setLight(colour);
	// Die after 3 seconds by default
	InitialLifeSpan = 3.0f;
}

void AMyProjectProjectile::setDefaultProjectileMovement()
{
	// Use a ProjectileMovementComponent to govern this projectile's movement
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement0"));
	ProjectileMovement->UpdatedComponent = ProjectileMesh;
	ProjectileMovement->InitialSpeed = 3000.f;
	ProjectileMovement->MaxSpeed = 3000.f;
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->bShouldBounce = false;
	ProjectileMovement->ProjectileGravityScale = 0.f; // No gravity
}

void AMyProjectProjectile::setSpeed(int newSpeed)
{
	ProjectileMovement->InitialSpeed = newSpeed;
	ProjectileMovement->MaxSpeed = newSpeed;
}

void AMyProjectProjectile::setDefaultProjectileMesh()
{
	// Static reference to the mesh to use for the projectile
	static ConstructorHelpers::FObjectFinder<UStaticMesh> ProjectileMeshAsset(TEXT("/Game/TwinStick/Meshes/TwinStickProjectile.TwinStickProjectile"));

	// Create mesh component for the projectile sphere
	//if (!ProjectileMesh)
	ProjectileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ProjectileMesh0"));
	ProjectileMesh->SetStaticMesh(ProjectileMeshAsset.Object);
	ProjectileMesh->SetupAttachment(RootComponent);
	ProjectileMesh->BodyInstance.SetCollisionProfileName("Projectile");
	ProjectileMesh->OnComponentHit.AddDynamic(this, &AMyProjectProjectile::OnHit);		// set up a notification for when this component hits something
	ProjectileMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel1, ECollisionResponse::ECR_Ignore);
	RootComponent = ProjectileMesh;
	InitialLifeSpan = 3.0f;
}

void AMyProjectProjectile::setDefaultLight()
{
	light = CreateDefaultSubobject<UPointLightComponent>(TEXT("Light"));
	light->SetLightColor(FLinearColor::White, true);
	light->SetupAttachment(RootComponent);
}

void AMyProjectProjectile::setLight(FLinearColor colour)
{
	light = CreateDefaultSubobject<UPointLightComponent>(TEXT("Light"));
	light->SetLightColor(colour, true);
	light->SetupAttachment(RootComponent);
}

void AMyProjectProjectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	// Only add impulse and destroy projectile if we hit a physics
	if ((OtherActor != NULL) && (OtherActor != this) && (OtherComp != NULL) && OtherComp->IsSimulatingPhysics())
	{
		OtherComp->AddImpulseAtLocation(GetVelocity() * 20.0f, GetActorLocation());
	}

	Destroy();
}