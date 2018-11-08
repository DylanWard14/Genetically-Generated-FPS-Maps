// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DNA.h"
#include "Wall.h"
#include <vector>
#include "EnemyAICharacter.h"
#include "GeneticMap.generated.h"


UCLASS()
class GENETICFPS_API AGeneticMap : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGeneticMap();

	UPROPERTY(EditAnywhere)
		int sizeX = 50;
	UPROPERTY(EditAnywhere)
		int sizeY = 50;
	UPROPERTY(EditAnywhere)
		int genomeLength = 10;
	UPROPERTY(EditAnywhere, blueprintReadWrite)
		bool crossOver = false;
	UPROPERTY(EditAnywhere)
		AGeneticMap* p1;
	UPROPERTY(EditAnywhere)
		AGeneticMap* p2;

	UPROPERTY(EditAnywhere)
		int NumberOfBots = 4;
	UPROPERTY(EditAnywhere)
		int NumberOfTeams = 2;
	UPROPERTY(EditAnywhere)
		int GameplaySpeed = 1;

	UPROPERTY(EditAnywhere, blueprintReadWrite)
		TSubclassOf<class AEnemyAICharacter> bot;

	UPROPERTY(EditAnywhere)
		float Gametime;
	UPROPERTY(EditAnywhere)
		float MaxPlaytime;

	UPROPERTY(EditAnywhere)
		float MapFitness;



	DNA myDNA = DNA();
	TArray<AEnemyAICharacter*> bots;
	std::vector<AActor*> actors;



	//int grid[50][50];

	std::vector<std::vector<int>> myGrid;

	void InitGrid();

	void CreateArenas();
	void CreateCorridors();
	void CreateCover();

	void GenerateGrid();

	void SpawnBots();

	void CalculateFitness();

	int GetLevelSize();

	bool calculatedFitness = false;

	UFUNCTION(BlueprintCallable, Category="Genetics")
	void StartCrossOver();

	AActor objectToSpawn();

	UPROPERTY(EditAnywhere)
	TSubclassOf<class AWall> wallActor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<class AWall> LeftWallActor;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<class AWall> RightWallActor;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<class AWall> TopWallActor;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<class AWall> BottomWallActor;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<class AWall> FloorActor;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<class AWall> CeilingActor;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<class AWall> CoverActor;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<class AWall> CoverActorRotated;

	bool CheckLeft(int x, int y);
	bool CheckRight(int x, int y);
	bool CheckTop(int x, int y);
	bool CheckBottom(int x, int y);

	void DestoryLevel();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	
};
