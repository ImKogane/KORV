// Fill out your copyright notice in the Description page of Project Settings.

#include "Ennemiess.h"
#include "AIController.h"
#include "AITypes.h"
#include "Kismet/GameplayStatics.h"


// Sets default values
AEnnemiess::AEnnemiess()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AEnnemiess::BeginPlay()
{
	Super::BeginPlay();

	score = life;

	GetCharacterMovement()->MaxWalkSpeed = speed;

	control = (AAIController*)(GetController());
}

// Called every frame
void AEnnemiess::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	ACharacter* myCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	FAIMoveRequest movReq = myCharacter->GetActorLocation();
	control->MoveTo(movReq);
}

int AEnnemiess::getLife()
{
	return life;
}

void AEnnemiess::setLife(int l)
{
	life = l;
}

void AEnnemiess::loseLife(int l)
{
	life = life - l;
	if (life <= 0)
	{
		isDead = true;
		Destroy();
	}	
}

int AEnnemiess::getDamage()
{
	return dmg;
}

void AEnnemiess::setDamage(int d)
{
	dmg = d;
}

float AEnnemiess::getSpeed()
{
	return speed;
}

void AEnnemiess::setSpeed(float s)
{
	speed = s;
}

FVector AEnnemiess::getDir()
{
	return dir;
}

void AEnnemiess::setDir(ACharacter* t)
{
	dir = t->GetActorLocation();
}

bool AEnnemiess::getIsDead()
{
	return isDead;
}

void AEnnemiess::setIsDead(bool b)
{
	isDead = b;
}

int AEnnemiess::getScore()
{
	return score;
}

void AEnnemiess::setScore(int s)
{
	score = s;
}