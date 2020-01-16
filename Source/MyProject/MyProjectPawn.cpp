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

void AMyProjectPawn::CreateMovementComponent()
{
	//MovementComponent = CreateDefaultSubobject<UPawnMovementComponent>(TEXT("CharacterMovement"));
	//MovementComponent->SetupAttachment(RootComponent, TEXT("CharacterMovement"));
	//MovementComponent = CreateDefaultSubobject<UCharacterMovementComponent>(TEXT("CharacterMovement"));
	//MovementComponent.defa
}

void AMyProjectPawn::SetWeaponsNull()
{
	defaultWeapon = 0;
	currentShotgun = 0;
	currentAssaultRifle = 0;
	currentMarksmanRifle = 0;
}

AMyProjectPawn::AMyProjectPawn()
{
	//CreateMovementComponent();
	CreateDefaultPistol();

	//equipShotgun(CreateDefaultSubobject<AShotgun>(TEXT("Testing Shotgun")));
	equipAssaultRifle(CreateDefaultSubobject<AAssaultRifle>(TEXT("Testing AR")));
	//equipMarksmanRifle(CreateDefaultSubobject<AMarksmanRifle>(TEXT("Testing MR")));

	static ConstructorHelpers::FObjectFinder < UStaticMesh > ShipMesh(TEXT("/Game/TwinStick/Meshes/TwinStickUFO.TwinStickUFO"));
	//static ConstructorHelpers::FObjectFinder < USkeletalMesh > ShipMesh(TEXT("/Game/Mannequin/Character/Mesh/SK_Mannequin.SK_Mannequin"));
	// Create the mesh component
	//ShipMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("ShipMesh"));
	ShipMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ShipMesh"));
	RootComponent = ShipMeshComponent;
	ShipMeshComponent->SetCollisionProfileName(UCollisionProfile::Pawn_ProfileName);
	//ShipMeshComponent->SetSkeletalMesh(ShipMesh.Object, true);
	ShipMeshComponent->SetStaticMesh(ShipMesh.Object);
	CreateMovementComponent();

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
	//FireRate = 0.1f;
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

void AMyProjectPawn::Tick(float DeltaSeconds)
{
	FHitResult* HitResult = new FHitResult();
	FVector ForwardVector = GetActorLocation();
	FVector EndTrace;

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
		//AddMovementInput(FVector(1, 0, 0), Movement.Y);
		//MoveForward(Movement.Y);
		//AddMovementInput(FVector(0, 1, 0), Movement.X);
		//MoveRight(Movement.X);
		//FString movementString = (Movement.Y, ", ", Movement.X);
		//if(GEngine && Movement.X > 0.0f)
			//GEngine->AddOnScreenDebugMessage(-1, 1.5, FColor::Yellow, TEXT("XMovement is 1"));
		if (Hit.IsValidBlockingHit())
		{
			const FVector Normal2D = Hit.Normal.GetSafeNormal2D();
			const FVector Deflection = FVector::VectorPlaneProject(Movement, Normal2D) * (1.f - Hit.Time);
			RootComponent->MoveComponent(Deflection, currentRotation, true);
		}
	}
	
	checkGravity();
	// Create fire direction vector
	TickWeapons(DeltaSeconds);

	const float FireForwardValue = GetInputAxisValue(FireForwardBinding);
	const float FireRightValue = GetInputAxisValue(FireRightBinding);
	const FVector FireDirection = FVector(FireForwardValue, FireRightValue, 0.f);

	//Next bit's scary.

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
		else
			if (GEngine)
				GEngine->AddOnScreenDebugMessage(-1, 1.5, FColor::Blue, TEXT("Cursor not found"));
	}
	else
	{
		if (GEngine)
			GEngine->AddOnScreenDebugMessage(-1, 1.5, FColor::Red, TEXT("Cursor not found"));
	}

	if (fireKeyDown)
		whichWeapon();
}

//void AMyProjectPawn::FireShot(FVector FireDirection)
//{
//	// If it's ok to fire again
//	if (true)
//	{
//		// If we are pressing fire stick in a direction
//		if (FireDirection.SizeSquared() > 0.0f)
//		{
//			const FRotator FireRotation = FireDirection.Rotation();
//			// Spawn projectile at an offset from this pawn
//			const FVector SpawnLocation = GetActorLocation() + FireRotation.RotateVector(GunOffset);
//
//			UWorld* const World = GetWorld();
//			if (World != NULL)
//			{
//				// spawn the projectile
//				whichWeapon();
//				//World->SpawnActor<AMyProjectProjectile>(SpawnLocation, FireRotation);
//				if (defaultWeapon != nullptr)
//					defaultWeapon->FireWeapon(SpawnLocation, FireRotation);
//				//World->SpawnActor<AMyProjectile>(SpawnLocation, FireRotation);
//			}
//
//			//bCanFire = false;
//			//World->GetTimerManager().SetTimer(TimerHandle_ShotTimerExpired, this, &AMyProjectPawn::ShotTimerExpired, FireRate);
//
//			// try and play the sound if specified
//			//if (FireSound != nullptr)
//			//{
//			//	UGameplayStatics::PlaySoundAtLocation(this, FireSound, GetActorLocation());
//			//}
//
//			//bCanFire = false;
//		}
//	}
//}

void AMyProjectPawn::checkGravity()
{
	FHitResult HitResult;
	FVector StartTrace = GetActorLocation();
	FVector DownVector = FVector(0, 0, -1);
	FVector EndVector = ((DownVector * 5000.0f) + StartTrace);
	//GetWorld()->LineTraceSingleByChannel(HitResult, StartTrace, EndVector, ECollisionChannel TraceChannel, const FCollisionQueryParams& Params, /* = FCollisionQueryParams::DefaultQueryParam */, const FCollisionResponseParams& ResponseParam /* = FCollisionResponseParams::DefaultResponseParam */);
	GetWorld()->LineTraceSingleByChannel(HitResult, StartTrace, EndVector, ECollisionChannel::ECC_WorldStatic, FCollisionQueryParams::DefaultQueryParam, FCollisionResponseParams::DefaultResponseParam);
	float ZDiff = HitResult.Location.Z - GetActorLocation().Z;
	FHitResult Hit(1.f);
	RootComponent->MoveComponent(FVector(0, 0, ZDiff), FRotator(0, 0, 0), true, &Hit);
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