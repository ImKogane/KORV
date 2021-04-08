// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Ennemiess.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "AmyZombie.generated.h"

/**
 *
 */
UCLASS()
class FPS_API AAmyZombie : public AEnnemiess
{
	GENERATED_BODY()

public:
	AAmyZombie();

	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditDefaultsOnly)
		class USkeletalMeshComponent* AmyZombie;

private:
	float movement;

	class AAIController* control;
};
