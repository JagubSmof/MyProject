// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "MyProjectPawn.h"
#include "MyProjectProjectile.h"
#include "TimerManager.h"
#include "UObject/ConstructorHelpers.h"
#include "Camera/CameraComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Engine/CollisionProfile.h"
#include "Engine/StaticMesh.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundBase.h"
#include "Pistol.h"

const FName AMyProjectPawn::MoveForwardBinding("MoveForward");
const FName AMyProjectPawn::MoveRightBinding("MoveRight");
const FName AMyProjectPawn::FireForwardBinding("FireForward");
const FName AMyProjectPawn::FireRightBinding("FireRight");
const FName AMyProjectPawn::FireBinding("FireBinding");

void AMyProjectPawn::CreateDefaultPistol()
{
	defaultWeapon = CreateDefaultSubobject<APistol>(TEXT("DefaultPistol"));
}

AMyProjectPawn::AMyProjectPawn()
{	
	//for (TActorIterator<APistol> ActorItr(GetWorld()); ActorItr; ++ActorItr) {
	//	if (ActorItr->GetName() == "yourName")

	//APistol newPistol = APistol;
	//defaultWeapon = &newPistol;
	//equippedWeaponClass = FString("Pistol");

	//defaultWeapon = FindObject<APistol>(GetLevel(), TEXT("Pistol1"));
	CreateDefaultPistol();

	static ConstructorHelpers::FObjectFinder<UStaticMesh> ShipMesh(TEXT("/Game/TwinStick/Meshes/TwinStickUFO.TwinStickUFO"));
	// Create the mesh component
	ShipMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ShipMesh"));
	RootComponent = ShipMeshComponent;
	ShipMeshComponent->SetCollisionProfileName(UCollisionProfile::Pawn_ProfileName);
	ShipMeshComponent->SetStaticMesh(ShipMesh.Object);
	
	// Cache our sound effect
	static ConstructorHelpers::FObjectFinder<USoundBase> FireAudio(TEXT("/Game/TwinStick/Audio/TwinStickFire.TwinStickFire"));
	FireSound = FireAudio.Object;//TODO: Get this from the weapon.

	// Create a camera boom...
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->bAbsoluteRotation = true; // Don't want arm to rotate when ship does
	CameraBoom->TargetArmLength = 1200.f;
	CameraBoom->RelativeRotation = FRotator(-80.f, 0.f, 0.f);
	CameraBoom->bDoCollisionTest = false; // Don't want to pull camera in when it collides with level

	// Create a camera...
	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("TopDownCamera"));
	CameraComponent->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	CameraComponent->bUsePawnControlRotation = false;	// Camera does not rotate relative to arm

	// Movement
	MoveSpeed = 1000.0f;
	// Weapon
	GunOffset = FVector(90.f, 0.f, 0.f);
	FireRate = 0.1f;
	//bCanFire = true;


	// Create a decal in the world to show the cursor's location
	CursorToWorld = CreateDefaultSubobject<UDecalComponent>("CursorToWorld");
	CursorToWorld->SetupAttachment(RootComponent);
	static ConstructorHelpers::FObjectFinder<UMaterial> DecalMaterialAsset(TEXT("Material'/Game/Decaul/M_Cursor_Decal.M_Cursor_Decal'"));
	if (DecalMaterialAsset.Succeeded())
	{
		CursorToWorld->SetDecalMaterial(DecalMaterialAsset.Object);
	}
	CursorToWorld->DecalSize = FVector(16.0f, 32.0f, 32.0f);
	CursorToWorld->SetRelativeRotation(FRotator(90.0f, 0.0f, 0.0f).Quaternion());
}

void AMyProjectPawn::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	check(PlayerInputComponent);

	// set up gameplay key bindings
	PlayerInputComponent->BindAxis(MoveForwardBinding);
	PlayerInputComponent->BindAxis(MoveRightBinding);
	PlayerInputComponent->BindAxis(FireForwardBinding);
	PlayerInputComponent->BindAxis(FireRightBinding);

	PlayerInputComponent->BindAxis(FireBinding);
}

void AMyProjectPawn::Tick(float DeltaSeconds)
{
	// Find movement direction
	const float ForwardValue = GetInputAxisValue(MoveForwardBinding);
	const float RightValue = GetInputAxisValue(MoveRightBinding);
	const bool fireKeyDown = ( GetInputAxisValue(FireBinding) > 0);

	// Clamp max size so that (X=1, Y=1) doesn't cause faster movement in diagonal directions
	const FVector MoveDirection = FVector(ForwardValue, RightValue, 0.f).GetClampedToMaxSize(1.0f);

	// Calculate  movement
	const FVector Movement = MoveDirection * MoveSpeed * DeltaSeconds;

	// If non-zero size, move this actor
	const FRotator currentRotation = GetActorRotation();

	if (Movement.SizeSquared() > 0.0f)
	{
		//const FRotator NewRotation = Movement.Rotation();
		FHitResult Hit(1.f);
		RootComponent->MoveComponent(Movement, currentRotation, true, &Hit);
		
		if (Hit.IsValidBlockingHit())
		{
			const FVector Normal2D = Hit.Normal.GetSafeNormal2D();
			const FVector Deflection = FVector::VectorPlaneProject(Movement, Normal2D) * (1.f - Hit.Time);
			RootComponent->MoveComponent(Deflection, currentRotation, true);
		}
	}
	
	// Create fire direction vector

	const float FireForwardValue = GetInputAxisValue(FireForwardBinding);
	const float FireRightValue = GetInputAxisValue(FireRightBinding);
	const FVector FireDirection = FVector(FireForwardValue, FireRightValue, 0.f);

	//Gonna do some fucky stuff here.


	if (CursorToWorld != nullptr)
	{
		if (APlayerController* PC = Cast<APlayerController>(GetController()))
		{
			FHitResult TraceHitResult;
			PC->GetHitResultUnderCursor(ECC_Visibility, true, TraceHitResult);
			FVector CursorFV = TraceHitResult.ImpactNormal;
			FRotator CursorR = CursorFV.Rotation();
			CursorToWorld->SetWorldLocation(TraceHitResult.Location);
			CursorToWorld->SetWorldRotation(CursorR);
		}
	}
	else
	{
		if (GEngine)
			GEngine->AddOnScreenDebugMessage(-1, 1.5, FColor::Red, TEXT("FUUUUUUUUUUUU. Classic Rage Comics xd"));
	}

	if (defaultWeapon != nullptr)
		defaultWeapon->Tick(DeltaSeconds);
	else
	{
		if (GEngine)
			GEngine->AddOnScreenDebugMessage(-1, 1.5, FColor::Green, TEXT("FUUUUUUUUUUUU. Classic Rage Comics xd"));
	}

	//const FRotator FireRotation = FRotator(1, 1, 1);

	if (fireKeyDown)
		whichWeapon();
	//else
	//{
	//	if (GEngine)
	//		GEngine->AddOnScreenDebugMessage(-1, 1.5, FColor::Yellow, TEXT("FUUUUUUUUUUUU. Classic Rage Comics xd"));
	//}

	// Try and fire a shot
	//FireShot(FireDirection);
}

void AMyProjectPawn::FireShot(FVector FireDirection)
{
	// If it's ok to fire again
	if (true)
	{
		// If we are pressing fire stick in a direction
		if (FireDirection.SizeSquared() > 0.0f)
		{
			const FRotator FireRotation = FireDirection.Rotation();
			// Spawn projectile at an offset from this pawn
			const FVector SpawnLocation = GetActorLocation() + FireRotation.RotateVector(GunOffset);

			UWorld* const World = GetWorld();
			if (World != NULL)
			{
				// spawn the projectile
				whichWeapon();
				//World->SpawnActor<AMyProjectProjectile>(SpawnLocation, FireRotation);
				if (defaultWeapon != nullptr)
					defaultWeapon->FireWeapon(SpawnLocation, FireRotation);
				//World->SpawnActor<AMyProjectile>(SpawnLocation, FireRotation);
			}

			//bCanFire = false;
			//World->GetTimerManager().SetTimer(TimerHandle_ShotTimerExpired, this, &AMyProjectPawn::ShotTimerExpired, FireRate);

			// try and play the sound if specified
			if (FireSound != nullptr)
			{
				UGameplayStatics::PlaySoundAtLocation(this, FireSound, GetActorLocation());
			}

			//bCanFire = false;
		}
	}
}

void AMyProjectPawn::whichWeapon()
{
	const FRotator FireRotation = GetActorRotation();
	const FVector SpawnLocation = GetActorLocation() + FireRotation.RotateVector(GunOffset);
	if (equippedWeaponClass == "Rocket Launcher")
	{
		if (GEngine)
			GEngine->AddOnScreenDebugMessage(-1, 1.5, FColor::Blue, TEXT("You don't have a rocket launcher"));
	}
	else
	{
		if (defaultWeapon != nullptr)
			defaultWeapon->FireWeapon(SpawnLocation, FireRotation);
		else
		{
			if (GEngine)
				GEngine->AddOnScreenDebugMessage(-1, 1.5, FColor::Blue, TEXT("FUUUUUUUUUUUU. Classic Rage Comics xd"));
		}
	}
}

//void AMyProjectPawn::ShotTimerExpired()
//{
//	bCanFire = true;
//}

