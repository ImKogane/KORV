// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Weapon.generated.h"

class UInputComponent;
class USkeletalMeshComponent;
class USceneComponent;
class UCameraComponent;
class UMotionControllerComponent;
class UAnimMontage;
class USoundBase;

UCLASS()
class FPS_API AWeapon : public AActor
{
	GENERATED_BODY()


	/** Gun mesh: 1st person view (seen only by self) */
	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
	USkeletalMeshComponent* FP_Gun;

	/** Location on gun mesh where projectiles should spawn. */
	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
		USceneComponent* FP_MuzzleLocation;


public:
	AWeapon();


public:



	/** Gun muzzle's offset from the characters location */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Mesh)
		FVector GunOffset;

	/** Weapon range */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stats)
		int range;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stats)
		int damage;



	UPROPERTY(EditDefaultsOnly, Category = Particle)
		class UParticleSystem* ShotParticle;

	UPROPERTY(EditDefaultsOnly, Category = Particle)
		class UParticleSystem* ImpactParticle;


	/// <summary>
	/// Ammo system
	/// </summary>
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ClipSystem)
		int AmmoInClip;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ClipSystem)
		int MaxAmmoInClip;

	/** Sound to play each time we shoot with empty clip */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ClipSystem)
		USoundBase* ReloadSound;

	/** Sound to play each time we shoot with empty clip */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ClipSystem)
		USoundBase* EmptyClipSound;

	/** End of ammo system */



	/** Sound to play each time we fire */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	USoundBase* FireSound;

	void HideMesh();
	void ShowMesh();

	/** Fires a projectile. */
	void Fire();

	void Reload();

	
public:

	int getDamage();

	int getRange();

	int getAmmoInClip();
	int getMaxAmmoInClip();

	void EmptyClip();
	

};
