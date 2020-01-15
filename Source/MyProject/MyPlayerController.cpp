// Fill out your copyright notice in the Description page of Project Settings.


#include "MyPlayerController.h"
#include "Kismet/KismetMathLibrary.h"
#include "Engine/World.h"

AMyPlayerController::AMyPlayerController()
{
	DefaultMouseCursor = EMouseCursor::Crosshairs;
}

void AMyPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);

	// keep updating the destination every tick
	MoveToMouseCursor();
}

void AMyPlayerController::MoveToMouseCursor()
{
	// Trace to see what is under the mouse cursor
	FHitResult Hit;
	GetHitResultUnderCursor(ECC_Visibility, false, Hit);

	// Face where we're clicking
	SetNewShootLocation(Hit.ImpactPoint);
}

void AMyPlayerController::SetNewShootLocation(const FVector DestLocation)
{
	APawn* const MyPawn = GetPawn();
	if (MyPawn)
	{
		FRotator fullRotation = UKismetMathLibrary::FindLookAtRotation(MyPawn->GetActorLocation(), DestLocation);
		FRotator zOnlyRotation = FRotator(0, fullRotation.Yaw, 0);
		MyPawn->SetActorRotation(zOnlyRotation);
	}
	else
	{
		if (GEngine)
			GEngine->AddOnScreenDebugMessage(-1, 1.5, FColor::White, TEXT("FUUUUUUUUUUUU. Classic Rage Comics xd"));
	}
}

void AMyPlayerController::SetupInputComponent()
{
	// set up gameplay key bindings
	Super::SetupInputComponent();

	InputComponent->BindAction("Fire", IE_Pressed, this, &AMyPlayerController::FireWeapon);

	//InputComponent->BindAction("Fire", IE_, this, &AMyPlayerController::FireWeapon);
	//InputComponent->BindAction("SetFacing", IE_Released, this, &AMyPlayerController::OnSetDestinationReleased);
}

void AMyPlayerController::FireWeapon()
{
	//MyPawn->FireShot();
}