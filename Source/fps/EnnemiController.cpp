// Fill out your copyright notice in the Description page of Project Settings.

#include "EnnemiController.h"
#include "Kismet/GameplayStatics.h"
#include "Math/UnrealMathUtility.h"
#include "Spawner.h"
#include "Amy.h"
#include "AmyZombie.h"

// Sets default values
AEnnemiController::AEnnemiController()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AEnnemiController::BeginPlay()
{
	Super::BeginPlay();

	//On met en place un timer qui agira toutes les 60 secondes.
	FTimerHandle MemberTimerHandle; 
	GetWorld()->GetTimerManager().SetTimer(MemberTimerHandle, this, &AEnnemiController::spawnEnnemies, 60.f, true, 0);
}

// Called every frame
void AEnnemiController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AEnnemiController::spawnEnnemies()
{
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASpawner::StaticClass(), lesPoints);

	int compteurEz = nbEzEnnemis;
	int compteurNormal = nbNormalEnnemis;
	int compteurHard = nbHardEnnemis;

	int nbTotal = nbEzEnnemis + FMath::Floor(nbNormalEnnemis) + FMath::Floor(nbHardEnnemis);

	//On spawn des ennemis à des endroits différents
	for (int i = 0; i < nbTotal; i++)
	{
		int rand = FMath::RandRange(0, lesPoints.Num() - 1);

		FVector Location = lesPoints[rand]->GetActorLocation();
		FRotator Rotation = lesPoints[rand]->GetActorRotation();
		FActorSpawnParameters SpawnInfo;

		if (compteurEz != 0)
		{
			GetWorld()->SpawnActor<AAmy>(MyAmy, Location, Rotation, SpawnInfo);
			compteurEz--;
		}
		else if (compteurNormal != 0)
		{
			GetWorld()->SpawnActor<AAmyZombie>(MyAmyZombie, Location, Rotation, SpawnInfo);
		}
		else if (compteurHard != 0)
		{
			//TODO spawner un ennemi hardcore quand on en aura un
		}
	}

	//Les ennemis faciles seront toujours 2 de plus
	nbEzEnnemis += 2;

	//Les ennemis normaux seront un de plus toutes les 4 vagues
	nbNormalEnnemis += 0.25f;

	//Et les ennemis durs seront 1 de plus toutes les dix vagues
	nbHardEnnemis += 0.1f;
}