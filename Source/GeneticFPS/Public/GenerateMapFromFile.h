// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include <vector>
#include "GenerateMapFromFile.generated.h"

UCLASS()
class GENETICFPS_API AGenerateMapFromFile : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AGenerateMapFromFile();

	UFUNCTION(BlueprintCallable, Category = "Custom", meta = (Keywords = "SaveTxt"))
		static bool SaveTxt(FString SaveTextB, FString FileNameB);

	UFUNCTION(BlueprintPure, Category = "Custom", meta = (Keywords = "LoadTxt"))
		static bool LoadTxt(FString FileNameA, FString& SaveTextA);

	std::vector<std::vector<TCHAR>> myGrid;

	void CreateGridFromFile(FString Level);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;


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

	void GenerateGrid();

	int sizeX = 50;
	int sizeY = 50;
};
