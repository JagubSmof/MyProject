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

void AMyProjectPawn::equipShotgun(AShotgun* shotgun)
{
	currentShotgun = shotgun;
	equippedWeaponClass = FString("Shotgun");
}

void AMyProjectPawn::equipAssaultRifle(AAssaultRifle* assaultRifle)
{
	currentAssaultRifle = assaultRifle;
	equippedWeaponClass = "Assault Rifle";
}

void AMyProjectPawn::equipMarksmanRifle(AMarksmanRifle* marksmanRifle)
{
	currentMarksmanRifle = marksmanRifle;
	equippedWeaponClass = "Marksman Rifle";
}

void AMyProjectPawn::equipShotgun()
{ equipShotgun(CreateDefaultSubobject<AShotgun>(TEXT("Testing Shotgun"))); }

void AMyProjectPawn::equipAssaultRifle()
{ equipAssaultRifle(CreateDefaultSubobject<AAssaultRifle>(TEXT("Testing AR"))); }

void AMyProjectPawn::equipMarksmanRifle()
{ equipMarksmanRifle(CreateDefaultSubobject<AMarksmanRifle>(TEXT("Testing MR"))); }

void AMyProjectPawn::SetWeaponsNull()
{
	defaultWeapon = 0;
	currentShotgun = 0;
	currentAssaultRifle = 0;
	currentMarksmanRifle = 0;
}

void AMyProjectPawn::TickWeapons(float DeltaSeconds)
{
	if (defaultWeapon != 0)
		defaultWeapon->Tick(DeltaSeconds);
	else
		if (GEngine)
			GEngine->AddOnScreenDebugMessage(-1, 1.5, FColor::Blue, TEXT("There is no weapon."));
	if (currentShotgun != 0)
		currentShotgun->Tick(DeltaSeconds);
	if (currentAssaultRifle != 0)
		currentAssaultRifle->Tick(DeltaSeconds);
	if (currentMarksmanRifle != 0)
		currentMarksmanRifle->Tick(DeltaSeconds);
}

AMyProjectPawn::AMyProjectPawn()
{	
	floatAmount = 3;

	//for (TActorIterator<APistol> ActorItr(GetWorld()); ActorItr; ++ActorItr) {
	//	if (ActorItr->GetName() == "yourName")

	//APistol newPistol = APistol;
	//defaultWeapon = &newPistol;
	//equippedWeaponClass = FString("Pistol");

	//defaultWeapon = FindObject<APistol>(GetLevel(), TEXT("Pistol1"));
	CreateDefaultPistol();

	//equipShotgun(CreateDefaultSubobject<AShotgun>(TEXT("Testing Shotgun")));
	//equipAssaultRifle(CreateDefaultSubobject<AAssaultRifle>(TEXT("Testing AR")));
	//equipMarksmanRifle(CreateDefaultSubobject<AMarksmanRifle>(TEXT("Testing MR")));

	static ConstructorHelpers::FObjectFinder<UStaticMesh> ShipMesh(TEXT("/Game/TwinStick/Meshes/TwinStickUFO.TwinStickUFO"));
	//static ConstructorHelpers::FObjectFinder<UStaticMesh> ShipMesh(TEXT("/Game/TwinStick/Meshes/AAAAAAAAAAAAAA"));
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
	static ConstructorHelpers::FObjectFinder<UMaterial> DecalMaterialAsset(TEXT("Material'/Game/Decal/M_Cursor_Decal.M_Cursor_Decal'"));
	if (DecalMaterialAsset.Succeeded())
	{
		CursorToWorld->SetDecalMaterial(DecalMaterialAsset.Object);
	}
	CursorToWorld->DecalSize = FVector(16.0f, 32.0f, 32.0f);
	CursorToWorld->SetRelativeRotation(FRotator(90.0f, 0.0f, 0.0f).Quaternion());
	originZ = GetActorLocation().Z;
	relativeZ = originZ;
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

	checkGravity();
	TickWeapons(DeltaSeconds);

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
	else if (equippedWeaponClass == "Shotgun")
	{
		if (!currentShotgun->FireWeapon(SpawnLocation, FireRotation))
			equippedWeaponClass = "Pistol";
	}
	else if (equippedWeaponClass == "Marksman Rifle")
	{
		if (!currentMarksmanRifle->FireWeapon(SpawnLocation, FireRotation))
			equippedWeaponClass = "Pistol";
	}
	else if (equippedWeaponClass == "Assault Rifle")
	{
		if (!currentAssaultRifle->FireWeapon(SpawnLocation, FireRotation))
			equippedWeaponClass = "Pistol";
	}
	else
	{
		if (defaultWeapon != nullptr)
			defaultWeapon->FireWeapon(SpawnLocation, FireRotation);
		else
		{
			if (GEngine)
				GEngine->AddOnScreenDebugMessage(-1, 1.5, FColor::Blue, TEXT("There is no weapon."));
		}
	}
}

//void AMyProjectPawn::ShotTimerExpired()
//{
//	bCanFire = true;
//}

void AMyProjectPawn::checkGravity()
{
	//FVector offset = FVector(0, 0, 50);
	FHitResult HitResult;
	FVector StartTrace = GetActorLocation();// +offset;
	FVector DownVector = FVector(0, 0, -1);
	FVector EndVector = ((DownVector * 5000.0f) + StartTrace);
	//FVector moveUpLimit;
	//GetWorld()->LineTraceSingleByChannel(HitResult, StartTrace, EndVector, ECollisionChannel TraceChannel, const FCollisionQueryParams& Params, /* = FCollisionQueryParams::DefaultQueryParam */, const FCollisionResponseParams& ResponseParam /* = FCollisionResponseParams::DefaultResponseParam */);
	GetWorld()->LineTraceSingleByChannel(HitResult, StartTrace, EndVector, ECollisionChannel::ECC_WorldStatic, FCollisionQueryParams::DefaultQueryParam, FCollisionResponseParams::DefaultResponseParam);
	float ZDiff = HitResult.Location.Z - relativeZ;
	FHitResult Hit(1.f);
	relativeZ += ZDiff;
	RootComponent->MoveComponent(FVector(0, 0, ZDiff), GetActorRotation(), true, &Hit);
	//RootComponent->SetWorldLocation(FVector(GetActorLocation().X, GetActorLocation().Y, HitResult.Location.Z + floatAmount));
	//if (GEngine)
	//	GEngine->AddOnScreenDebugMessage(-1, 1.5, FColor::Blue, FString::Printf(TEXT("%f"), ZDiff));
}