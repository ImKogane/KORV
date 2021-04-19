// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Ennemiess.generated.h"

class UBoxComponent;

UCLASS()
class FPS_API AEnnemiess : public ACharacter
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = Box)
		UBoxComponent* BoxCollider;
	
public:	
	// Sets default values for this actor's properties
	AEnnemiess();

	//void OnBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	int getLife();

	void setLife(int l);

	void loseLife(int l);

	int getDamage();

	void setDamage(int d);

	float getSpeed();

	void setSpeed(float s);

	FVector getDir();

	void setDir(ACharacter* target);

	bool getIsDead();

	void setIsDead(bool b);

	int getScore();

	void setScore(int s);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stats)
	int life;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stats)
	int dmg;

	int score;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stats)
	int speed;

	FVector dir;

	bool isDead;

	class AAIController* control;
};
