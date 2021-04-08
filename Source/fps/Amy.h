// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Ennemiess.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Amy.generated.h"

/**
 * 
 */
UCLASS()
class FPS_API AAmy : public AEnnemiess
{
	GENERATED_BODY()

public:
	AAmy();

	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditDefaultsOnly)
	class USkeletalMeshComponent* Ennemiess;

private:
	class AAIController* control;
};
