// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EnemyAICharacter.h"
#include <vector>
#include <iostream>
#include <fstream>
#include "GeneticMap.h"
#include "DNA.h"
#include "GeneticAlgorithm.generated.h"

UCLASS()
class GENETICFPS_API AGeneticAlgorithm : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGeneticAlgorithm();

	void InitPopulation();

	UPROPERTY(EditAnywhere)
		int GameSpeed = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int populationSize = 10;


	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool SimulationComplete = false;

	UPROPERTY(EditAnywhere, blueprintReadWrite)
		TSubclassOf<class AGeneticMap> map;

	UPROPERTY(EditAnywhere)
		int sizeX = 50;
	UPROPERTY(EditAnywhere)
		int sizeY = 50;
	UPROPERTY(EditAnywhere)
		int genomeLength = 10;

	std::vector<AGeneticMap*> population;

	bool CheckSimulationCompleted();

	DNA TournamentSelection(AGeneticMap* p1, AGeneticMap* p2);

	void NextGeneration();

	bool processingGeneration = false;

	std::vector<DNA> survivors;

	std::vector<DNA> SortSurvivors(std::vector<DNA> Survivors);

	FString convertLevelToText(DNA Level);

	UFUNCTION(BlueprintCallable, Category = "Custom", meta = (Keywords = "SaveTxt"))
		static bool SaveTxt(FString SaveTextB, FString FileNameB);

	UFUNCTION(BlueprintPure, Category = "Custom", meta = (Keywords = "LoadTxt"))
		static bool LoadTxt(FString FileNameA, FString& SaveTextA);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	
	
};
