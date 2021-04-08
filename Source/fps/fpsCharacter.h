// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Weapon.h"
#include "GameFramework/Character.h"
#include "fpsCharacter.generated.h"

class UInputComponent;
class USkeletalMeshComponent;
class USceneComponent;
class UCameraComponent;
class UMotionControllerComponent;
class UAnimMontage;
class USoundBase;

UCLASS(config = Game)
class AfpsCharacter : public ACharacter
{
	GENERATED_BODY()

		/** Pawn mesh: 1st person view (arms; seen only by self) */
		UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
		USkeletalMeshComponent* Mesh1P;


	/** Gun mesh: 1st person view (seen only by self) */
	//UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
		//USkeletalMeshComponent* FP_Gun;

	/** Location on gun mesh where projectiles should spawn. */
	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
		USceneComponent* FP_MuzzleLocation;

	




	/** First person camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		UCameraComponent* FirstPersonCameraComponent;


public:
	AfpsCharacter();


protected:
	virtual void BeginPlay();

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	TArray<TSubclassOf<AWeapon>> Weapons;

	AWeapon* actualWeapon;

	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	//TSubclassOf<AWeapon> Weapon2;

	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
		float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
		float BaseLookUpRate;


	/** Gun muzzle's offset from the characters location */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Gameplay)
	FVector GunOffset;


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

	

	/// <summary>
	/// Gun stats 
	/// </summary>
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gun)
	int range;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gun)
	int damage;

	UPROPERTY(EditDefaultsOnly, Category = Particle)
	class UParticleSystem* ShotParticle;


	/** End of gun stats */


	UPROPERTY(VisibleDefaultsOnly, Category = Gameplay)
	int score;

	/** Player life point */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		int PlayerLife;



	UPROPERTY(EditDefaultsOnly, Category = Particle)
		class UParticleSystem* ImpactParticle;

	/** End of gun stats */


	/** Projectile class to spawn */
	UPROPERTY(EditDefaultsOnly, Category = Projectile)
		TSubclassOf<class AfpsProjectile> ProjectileClass;

	/** Sound to play each time we fire */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		USoundBase* FireSound;

	/** AnimMontage to play each time we fire */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		UAnimMontage* FireAnimation;


protected:

	/** Fires a projectile. */
	void OnFire();


	void Reload();

	/** Handles moving forward/backward */
	void MoveForward(float Val);

	/** Handles stafing movement, left and right */
	void MoveRight(float Val);

	/**
	 * Called via input to turn at a given rate.
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void TurnAtRate(float Rate);

	/**
	 * Called via input to turn look up/down at a given rate.
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void LookUpAtRate(float Rate);

	void Sprint();
	void Walk();

	void Aim();
	void UnAim();

	void SwitchWeapon();

	TArray< AWeapon* > listWeapon;
	int weaponIndex;


protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(UInputComponent* InputComponent) override;
	// End of APawn interface


public:
	/** Returns Mesh1P subobject **/
	USkeletalMeshComponent* GetMesh1P() const { return Mesh1P; }
	/** Returns FirstPersonCameraComponent subobject **/
	UCameraComponent* GetFirstPersonCameraComponent() const { return FirstPersonCameraComponent; }

};

