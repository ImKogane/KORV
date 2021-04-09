// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Math/UnrealMathUtility.h"
#include "EnnemiController.generated.h"

UCLASS()
class FPS_API AEnnemiController : public AActor
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = Ennemies)
		TSubclassOf<class AAmy> MyAmy;

	UPROPERTY(EditAnywhere, Category = Ennemies)
		TSubclassOf<class AAmyZombie> MyAmyZombie;
	
public:	
	// Sets default values for this actor's properties
	AEnnemiController();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void resetVar();

	TArray<AActor*> lesPoints;

	int nbEzEnnemis = 2;
	float nbNormalEnnemis = 0;
	float nbHardEnnemis = 0;

	int compteurEz = nbEzEnnemis;
	int compteurNormal = nbNormalEnnemis;
	int compteurHard = nbHardEnnemis;

	int nbWave = 1;

	float spawnDelay = 0.f;

	float waveDelay = 0.f;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
};
