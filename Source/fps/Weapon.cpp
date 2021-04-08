// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon.h"
#include "fpsCharacter.h"
#include <iostream>
#include "Engine/EngineTypes.h"
#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/InputSettings.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "MotionControllerComponent.h"
#include "XRMotionControllerBase.h" // for FXRMotionControllerBase::RightHandSourceId
#include "DrawDebugHelpers.h"
DEFINE_LOG_CATEGORY_STATIC(LogFPChar, Warning, All);

// Sets default values
AWeapon::AWeapon()
{

	// Create a gun mesh component
	FP_Gun = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("FP_Gun"));
	FP_Gun->SetOnlyOwnerSee(false);			// otherwise won't be visible in the multiplayer
	FP_Gun->bCastDynamicShadow = false;
	FP_Gun->CastShadow = false;
	// FP_Gun->SetupAttachment(Mesh1P, TEXT("GripPoint"));
	FP_Gun->SetupAttachment(RootComponent);

	FP_MuzzleLocation = CreateDefaultSubobject<USceneComponent>(TEXT("MuzzleLocation"));
	FP_MuzzleLocation->SetupAttachment(FP_Gun);
	FP_MuzzleLocation->SetRelativeLocation(FVector(0.2f, 48.4f, -10.6f));

	// Default offset from the character location for projectiles to spawn
	GunOffset = FVector(100.0f, 0.0f, 10.0f);

}


void AWeapon::Fire(FVector Loc, FRotator Rot)
{
	UWorld* const World = GetWorld();
	if (World != nullptr)
	{
		if (AmmoInClip > 0)
		{

			FRotator SpawnRotation;
			// MuzzleOffset is in camera space, so transform it to world space before offsetting from the character location to find the final muzzle position
			const FVector SpawnLocation = ((FP_MuzzleLocation != nullptr) ? FP_MuzzleLocation->GetComponentLocation() : GetActorLocation()) + SpawnRotation.RotateVector(GunOffset);

			//Set Spawn Collision Handling Override
			FActorSpawnParameters ActorSpawnParams;
			ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;


			//Spawn Particle
			UGameplayStatics::SpawnEmitterAtLocation(World, ShotParticle, SpawnLocation, SpawnRotation);
			AmmoInClip = AmmoInClip - 1;
			//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, "Shoot");



			//Line trace
			Loc = SpawnLocation;
			Rot = SpawnRotation;

			//GetController()->GetPlayerViewPoint(Loc, Rot);

			FVector Start = Loc;
			FVector End = Start + (Rot.Vector() * range);

			FHitResult Hit;
			FCollisionQueryParams test;
			if (World->LineTraceSingleByChannel(Hit, Start, End, ECC_Visibility, test))
			{
				//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Cyan, "Impact");
				UGameplayStatics::SpawnEmitterAtLocation(World, ImpactParticle, Hit.ImpactPoint);


				//DrawDebugLine(World, Start, Hit.ImpactPoint, FColor::Cyan, false, 10.0f);
			}


			// try and play the sound if specified
			if (FireSound != nullptr)
			{
				UGameplayStatics::PlaySoundAtLocation(this, FireSound, GetActorLocation());
			}

		}
		else
		{
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Cyan, "No ammo in clip");

			// try and play the EmptyClip sound if specified
			if (EmptyClipSound != nullptr)
			{
				UGameplayStatics::PlaySoundAtLocation(this, EmptyClipSound, GetActorLocation());
			}
		}

	}

}

void AWeapon::Reload()
{
	if (AmmoInClip < MaxAmmoInClip)
	{
		AmmoInClip = MaxAmmoInClip;
		if (ReloadSound != nullptr)
		{
			AmmoInClip = MaxAmmoInClip;
			UGameplayStatics::PlaySoundAtLocation(this, ReloadSound, GetActorLocation());
		}
	}
}

void AWeapon::HideMesh()
{
	FP_Gun->SetHiddenInGame(true, true);
}

void AWeapon::ShowMesh()
{
	FP_Gun->SetHiddenInGame(false, true);
}