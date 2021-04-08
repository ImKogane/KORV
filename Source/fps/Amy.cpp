// Fill out your copyright notice in the Description page of Project Settings.

#include "Amy.h"
#include "AIController.h"
#include "AITypes.h"
#include "Kismet/GameplayStatics.h"

AAmy::AAmy()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

void AAmy::BeginPlay()
{
	Super::BeginPlay();

	GetCharacterMovement()->MaxWalkSpeed = speed;

	AController* p = GetController();

	control = (AAIController*)(GetController());
}

void AAmy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	ACharacter* myCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	FAIMoveRequest movReq = myCharacter->GetActorLocation();
	control->MoveTo(movReq);
}