// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Runtime/Engine/Classes/Components/DecalComponent.h"
#include "Pistol.h"
#include "Shotgun.h"
#include "AssaultRifle.h"
#include "MarksmanRifle.h"
#include "MyProjectPawn.generated.h"

UCLASS(Blueprintable)
class AMyProjectPawn : public APawn
{
	GENERATED_BODY()

	/** The mesh component */
	UPROPERTY(Category = Mesh, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* ShipMeshComponent;

	/** The camera */
	UPROPERTY(Category = Camera, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* CameraComponent;

	/** Camera boom positioning the camera above the character */
	UPROPERTY(Category = Camera, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	void SetWeaponsNull();
	//void CreateMovementComponent();
	void TickWeapons(float);

	float originZ;
	float floatAmount;
	float relativeZ;

	void checkGravity();
	void equipShotgun(AShotgun*);
	void equipAssaultRifle(AAssaultRifle*);
	void equipMarksmanRifle(AMarksmanRifle*);

public:
	AMyProjectPawn();
	void CreateDefaultPistol();

	//UPROPERTY(Category = Gameplay, BlueprintReadWrite)
		void equipShotgun();
	//UPROPERTY(Category = Gameplay, BlueprintReadWrite)
		void equipAssaultRifle();
	//UPROPERTY(Category = Gameplay, BlueprintReadWrite)
		void equipMarksmanRifle();

	UPROPERTY(Category = Gameplay, EditAnywhere, BlueprintReadWrite)
		APistol* defaultWeapon;
	UPROPERTY(Category = Gameplay, EditAnywhere, BlueprintReadWrite)
		AShotgun* currentShotgun;
	UPROPERTY(Category = Gameplay, EditAnywhere, BlueprintReadWrite)
		AAssaultRifle* currentAssaultRifle;
	UPROPERTY(Category = Gameplay, EditAnywhere, BlueprintReadWrite)
		AMarksmanRifle* currentMarksmanRifle;

	/** Offset from the ships location to spawn projectiles */
	UPROPERTY(Category = Gameplay, EditAnywhere, BlueprintReadWrite )
		FVector GunOffset;

	UPROPERTY(Category = Gameplay, EditAnywhere, BlueprintReadWrite)
		FString equippedWeaponClass;

	class UDecalComponent* CursorToWorld;
	
	/* How fast the weapon will fire */
	UPROPERTY(Category = Gameplay, EditAnywhere, BlueprintReadWrite)
	float FireRate;

	/* The speed our ship moves around the level */
	UPROPERTY(Category = Gameplay, EditAnywhere, BlueprintReadWrite)
	float MoveSpeed;

	/** Sound to play each time we fire */
	UPROPERTY(Category = Audio, EditAnywhere, BlueprintReadWrite)
	class USoundBase* FireSound;

	// Begin Actor Interface
	virtual void Tick(float DeltaSeconds) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;
	// End Actor Interface

	/* Fire a shot in the specified direction */
	void FireShot(FVector FireDirection);

	void whichWeapon();

	// Static names for axis bindings
	static const FName MoveForwardBinding;
	static const FName MoveRightBinding;
	static const FName FireForwardBinding;
	static const FName FireRightBinding;
	static const FName FireBinding;

private:

	/* Flag to control firing  */
	uint32 bCanFire : 1;

	/** Handle for efficient management of ShotTimerExpired timer */
	FTimerHandle TimerHandle_ShotTimerExpired;

public:
	/** Returns ShipMeshComponent subobject **/
	FORCEINLINE class UStaticMeshComponent* GetShipMeshComponent() const { return ShipMeshComponent; }
	/** Returns CameraComponent subobject **/
	FORCEINLINE class UCameraComponent* GetCameraComponent() const { return CameraComponent; }
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
};

