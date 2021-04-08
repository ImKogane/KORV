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

void AWeapon::Fire()
{
	AmmoInClip = AmmoInClip - 1;
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

int AWeapon::getDamage()
{
	return damage;
}

int AWeapon::getRange()
{
	return range;
}

int AWeapon::getAmmoInClip()
{
	return AmmoInClip;
}

int AWeapon::getMaxAmmoInClip()
{
	return MaxAmmoInClip;
}

void AWeapon::EmptyClip()
{
	UGameplayStatics::PlaySoundAtLocation(this, EmptyClipSound, GetActorLocation());
}

int AWeapon::getAim()
{
	return Aim;
}

USoundBase* AWeapon::getFireSound()
{
	return FireSound;
}
