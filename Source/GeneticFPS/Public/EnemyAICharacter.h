// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "EnemyAICharacter.generated.h"

UCLASS()
class GENETICFPS_API AEnemyAICharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEnemyAICharacter();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float tension;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int TeamID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int GameplaySpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<float> fightingTimes;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintImplementableEvent)
		void IAmBeingDestroyed();

	
	
};
