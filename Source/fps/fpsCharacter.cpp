// Copyright Epic Games, Inc. All Rights Reserved.

#include "fpsCharacter.h"
#include "fpsProjectile.h"
#include "Amy.h"
#include "Ennemiess.h"
#include "fpsGameMode.h"
#include "Engine/EngineTypes.h"
#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "Components/ActorComponent.h"
#include "GameFramework/InputSettings.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "MotionControllerComponent.h"
#include "XRMotionControllerBase.h" // for FXRMotionControllerBase::RightHandSourceId
#include "DrawDebugHelpers.h"
#include <iostream>
DEFINE_LOG_CATEGORY_STATIC(LogFPChar, Warning, All);


//////////////////////////////////////////////////////////////////////////
// AfpsCharacter

AfpsCharacter::AfpsCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// Create a CameraComponent	
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent->SetupAttachment(GetCapsuleComponent());
	FirstPersonCameraComponent->SetRelativeLocation(FVector(-39.56f, 1.75f, 64.f)); // Position the camera
	FirstPersonCameraComponent->bUsePawnControlRotation = true;

	// Create a mesh component that will be used when being viewed from a '1st person' view (when controlling this pawn)
	Mesh1P = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh1P"));
	Mesh1P->SetOnlyOwnerSee(true);
	Mesh1P->SetupAttachment(FirstPersonCameraComponent);
	Mesh1P->bCastDynamicShadow = false;
	Mesh1P->CastShadow = false;
	Mesh1P->SetRelativeRotation(FRotator(1.9f, -19.19f, 5.2f));
	Mesh1P->SetRelativeLocation(FVector(-0.5f, -4.4f, -155.7f));

	/*
	// Create a gun mesh component
	FP_Gun = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("FP_Gun"));
	FP_Gun->SetOnlyOwnerSee(false);			// otherwise won't be visible in the multiplayer
	FP_Gun->bCastDynamicShadow = false;
	FP_Gun->CastShadow = false;
	// FP_Gun->SetupAttachment(Mesh1P, TEXT("GripPoint"));
	FP_Gun->SetupAttachment(RootComponent);
	$*/

	//FP_MuzzleLocation = CreateDefaultSubobject<USceneComponent>(TEXT("MuzzleLocation"));
	//FP_MuzzleLocation->SetupAttachment(FP_Gun);
	//FP_MuzzleLocation->SetRelativeLocation(FVector(0.2f, 48.4f, -10.6f));

	// Default offset from the character location for projectiles to spawn
	GunOffset = FVector(100.0f, 0.0f, 10.0f);


	// Uncomment the following line to turn motion controllers on by default:
	//bUsingMotionControllers = true;
}

void AfpsCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();



	AWeapon* temp;
	FActorSpawnParameters SpawnInfo;

	for (TSubclassOf<AWeapon> weap : Weapons)
	{
		temp = GetWorld()->SpawnActor<AWeapon>(weap);
		temp->AttachToComponent(Mesh1P, FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), TEXT("GripPoint"));
		temp->HideMesh();
		listWeapon.Push(temp);
	}


	actualWeapon = listWeapon[weaponIndex];
	actualWeapon->ShowMesh();
	weaponIndex++;

	//Attach gun mesh component to Skeleton, doing it here because the skeleton is not yet created in the constructor
	//FP_Gun->AttachToComponent(Mesh1P, FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), TEXT("GripPoint"));



}

//////////////////////////////////////////////////////////////////////////
// Input

void AfpsCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// set up gameplay key bindings
	check(PlayerInputComponent);

	// Bind jum p events
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);
	
	// Bind fire event
	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &AfpsCharacter::OnFire);

	PlayerInputComponent->BindAction("Look", IE_Pressed, this, &AfpsCharacter::Aim);
	PlayerInputComponent->BindAction("Look", IE_Released, this, &AfpsCharacter::UnAim);

	PlayerInputComponent->BindAction("SwitchWeapon", IE_Pressed, this, &AfpsCharacter::SwitchWeapon);

	
	PlayerInputComponent->BindAction("Reload", IE_Pressed, this, &AfpsCharacter::Reload);

	// Bind movement events
	PlayerInputComponent->BindAxis("MoveForward", this, &AfpsCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AfpsCharacter::MoveRight);

	PlayerInputComponent->BindAction("Sprint", IE_Pressed, this, &AfpsCharacter::Sprint);
	PlayerInputComponent->BindAction("Sprint", IE_Released, this, &AfpsCharacter::Walk);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &AfpsCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &AfpsCharacter::LookUpAtRate);
}

void AfpsCharacter::OnFire()
{

		UWorld* const World = GetWorld();
		if (World != nullptr)
		{

			if (actualWeapon->getAmmoInClip() > 0)
			{

				const FRotator SpawnRotation = GetControlRotation();
				// MuzzleOffset is in camera space, so transform it to world space before offsetting from the character location to find the final muzzle position
				const FVector SpawnLocation = ((FP_MuzzleLocation != nullptr) ? FP_MuzzleLocation->GetComponentLocation() : GetActorLocation()) + SpawnRotation.RotateVector(GunOffset);

				//Set Spawn Collision Handling Override
				FActorSpawnParameters ActorSpawnParams;
				ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;

				// spawn the projectile at the muzzle
				//World->SpawnActor<AfpsProjectile>(ProjectileClass, SpawnLocation, SpawnRotation, ActorSpawnParams);

				//Spawn Particle
				UGameplayStatics::SpawnEmitterAtLocation(World, ShotParticle, SpawnLocation, SpawnRotation);
				actualWeapon->Fire();

				//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, "Shoot");

				//Line trace
				FVector Loc = SpawnLocation;
				FRotator Rot = SpawnRotation;

				GetController()->GetPlayerViewPoint(Loc, Rot);

				FVector Start = Loc;

				FVector End = Start + (Rot.Vector() * actualWeapon->getRange());


				FHitResult* Hit = new FHitResult;
				FCollisionQueryParams test;
				if (World->LineTraceSingleByChannel(*Hit, Start, End, ECC_Visibility, test))
				{
					//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Cyan, "Impact");
					UGameplayStatics::SpawnEmitterAtLocation(World, ImpactParticle, Hit->ImpactPoint);

					if (Hit->GetActor()->GetName().Contains("BP_Amy"))
					{
						
						AEnnemiess* enemyTouch = Cast<AEnnemiess>(Hit->GetActor());
						if (enemyTouch)
						{
							enemyTouch->loseLife(actualWeapon->getDamage()); //Deal damage to enemy
							if (enemyTouch->getIsDead() == true)
							{
								AfpsGameMode* const gamemode = GetWorld()->GetAuthGameMode<AfpsGameMode>();
								gamemode->score+= enemyTouch->getScore();
							}

						}

					}

				}

				// try and play a firing animation if specified
				if (FireAnimation != nullptr)
				{
					// Get the animation object for the arms mesh
					UAnimInstance* AnimInstance = Mesh1P->GetAnimInstance();
					if (AnimInstance != nullptr)
					{
						AnimInstance->Montage_Play(FireAnimation, 1.f);
					}

					if (FireSound != nullptr)
					{
						UGameplayStatics::PlaySoundAtLocation(this, actualWeapon->getFireSound(), GetActorLocation());
					}
				}

			}
			else
			{
				actualWeapon->EmptyClip();
			}
				
		}
	}


void AfpsCharacter::Reload()
{
	actualWeapon->Reload();
}

void AfpsCharacter::MoveForward(float Value)
{
	if (Value != 0.0f)
	{
		// add movement in that direction
		AddMovementInput(GetActorForwardVector(), Value);
	}
}

void AfpsCharacter::MoveRight(float Value)
{
	if (Value != 0.0f)
	{
		// add movement in that direction
		AddMovementInput(GetActorRightVector(), Value);
	}
}

void AfpsCharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void AfpsCharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void AfpsCharacter::Sprint()
{
	GetCharacterMovement()->MaxWalkSpeed = 900;
}

void AfpsCharacter::Walk()
{
	GetCharacterMovement()->MaxWalkSpeed = 600;
}

void AfpsCharacter::Aim()
{
	FirstPersonCameraComponent->FieldOfView = actualWeapon->getAim();
}

void AfpsCharacter::UnAim()
{
	FirstPersonCameraComponent->FieldOfView = 90;
}

void AfpsCharacter::SwitchWeapon()
{

	for (AWeapon* weapon : listWeapon)
	{
		weapon->HideMesh();
	}
	//actualWeapon->HideMesh();

	//FP_Gun->DestroyComponent();
	

	actualWeapon = listWeapon[weaponIndex];
	actualWeapon->ShowMesh();
	weaponIndex = weaponIndex + 1;

	if (weaponIndex >= listWeapon.Num())
	{
		weaponIndex = 0;
	}
}