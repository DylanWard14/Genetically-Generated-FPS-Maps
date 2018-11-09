// Fill out your copyright notice in the Description page of Project Settings.


#include "GenerateMapFromFile.h"
#include "FileHelper.h"
#include "Paths.h"
#include "Wall.h"

// Sets default values
AGenerateMapFromFile::AGenerateMapFromFile()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AGenerateMapFromFile::BeginPlay()
{
	Super::BeginPlay();

	FString levelString;
	LoadTxt("Maps.txt", levelString);
	UE_LOG(LogTemp, Warning, TEXT("%s"), *levelString);
	myGrid.resize(50);
	for (int x = 0; x < 50; x++)
	{
		myGrid[x].resize(50);
	}

	CreateGridFromFile(levelString);

	GenerateGrid();
}

bool AGenerateMapFromFile::SaveTxt(FString SaveTextB, FString FileNameB)
{
	return FFileHelper::SaveStringToFile(SaveTextB, *(FPaths::GameDir() + FileNameB));
}

bool AGenerateMapFromFile::LoadTxt(FString FileNameA, FString& SaveTextA)
{
	return FFileHelper::LoadFileToString(SaveTextA, *(FPaths::GameDir() + FileNameA));
}

void AGenerateMapFromFile::CreateGridFromFile(FString Level)
{
	int x = 0;
	int y = 0;
	for (int i = 0; i < Level.Len(); i++)
	{
		if (Level[i] == '0' || Level[i] == '1')
		{
			myGrid[x][y] = Level[i];
			y++;
			if (y > 49)
			{
				y = 0;
				x++;
				if (x > 49)
				{
					break;
				}
			}
		}
	}
}

// Called every frame
void AGenerateMapFromFile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AGenerateMapFromFile::GenerateGrid()
{
	for (int i = 0; i < sizeX; i++)
	{
		FVector thisLocation = GetActorLocation();
		FVector spawnLocation = FVector(i * 150 + thisLocation.X, thisLocation.Y, thisLocation.Z);
		FRotator rotator = FRotator(0, 0, 0);
		GetWorld()->SpawnActor<AWall>(LeftWallActor, spawnLocation, rotator);

		spawnLocation = FVector(i * 150 + thisLocation.X, (sizeY - 1) * 150 + thisLocation.Y, thisLocation.Z);
		rotator = FRotator(0, 0, 0);
		GetWorld()->SpawnActor<AWall>(RightWallActor, spawnLocation, rotator);

		spawnLocation = FVector(thisLocation.X, i * 150 + thisLocation.Y, thisLocation.Z);
		rotator = FRotator(0, 0, 0);
		GetWorld()->SpawnActor<AWall>(BottomWallActor, spawnLocation, rotator);

		spawnLocation = FVector((sizeX - 1) * 150 + thisLocation.X, i * 150 + thisLocation.Y, thisLocation.Z);
		rotator = FRotator(0, 0, 0);
		GetWorld()->SpawnActor<AWall>(TopWallActor, spawnLocation, rotator);
	}
	for (int x = 1; x < sizeX - 1; x++)
	{
		for (int y = 1; y < sizeY - 1; y++)
		{
			UWorld* world = GetWorld();
			if (myGrid[x][y] == '1')
			{
				if (world)
				{
					CheckLeft(x, y);
					CheckRight(x, y);
					CheckTop(x, y);
					CheckBottom(x, y);
				}

				/*FVector thisLocation = GetActorLocation();
				FVector spawnLocation = FVector(x * 300 + thisLocation.X, y * 300 + thisLocation.Y, thisLocation.Z);
				FRotator rotator = FRotator(0, 0, 0);
				GetWorld()->SpawnActor<AWall>(CeilingActor, spawnLocation, rotator);
				*/
			}

			if (myGrid[x][y] == '0' || myGrid[x][y] == '2' || myGrid[x][y] == '3')
			{
				// spawn floor
				FVector thisLocation = GetActorLocation();
				FVector spawnLocation = FVector(x * 150 + thisLocation.X, y * 150 + thisLocation.Y, thisLocation.Z);
				FRotator rotator = FRotator(0, 0, 0);
				GetWorld()->SpawnActor<AWall>(FloorActor, spawnLocation, rotator);
			}

			if (myGrid[x][y] == '2')
			{
				// spawn floor
				FVector thisLocation = GetActorLocation();
				FVector spawnLocation = FVector(x * 150 + thisLocation.X, y * 150 + thisLocation.Y, thisLocation.Z);
				FRotator rotator = FRotator(0, 0, 0);
				GetWorld()->SpawnActor<AWall>(CoverActor, spawnLocation, rotator);
			}
			if (myGrid[x][y] == '3')
			{
				// spawn floor
				FVector thisLocation = GetActorLocation();
				FVector spawnLocation = FVector(x * 150 + thisLocation.X, y * 150 + thisLocation.Y, thisLocation.Z);
				FRotator rotator = FRotator(0, 0, 0);
				GetWorld()->SpawnActor<AWall>(CoverActorRotated, spawnLocation, rotator);
			}
		}
	}
}

bool AGenerateMapFromFile::CheckLeft(int x, int y)
{
	UWorld* world = GetWorld();
	if (myGrid[x][y - 1] == '0' || myGrid[x][y - 1] == '2' || myGrid[x][y - 1] == '3')
	{
		FVector thisLocation = GetActorLocation();
		FVector spawnLocation = FVector(x * 150 + thisLocation.X, y * 150 + thisLocation.Y, thisLocation.Z);
		FRotator rotator = FRotator(0, 0, 0);
		AWall* spawnedWall = world->SpawnActor<AWall>(RightWallActor, spawnLocation, rotator);
		return true;
	}
	else
		return false;
}

bool AGenerateMapFromFile::CheckRight(int x, int y)
{
	UWorld* world = GetWorld();
	if (myGrid[x][y + 1] == '0' || myGrid[x][y + 1] == '2' || myGrid[x][y + 1] == '3')
	{
		FVector thisLocation = GetActorLocation();
		FVector spawnLocation = FVector(x * 150 + thisLocation.X, y * 150 + thisLocation.Y, thisLocation.Z);
		FRotator rotator = FRotator(0, 0, 0);
		AWall* spawnedWall = world->SpawnActor<AWall>(LeftWallActor, spawnLocation, rotator);
		return true;
	}
	else
		return false;
}

bool AGenerateMapFromFile::CheckTop(int x, int y)
{
	UWorld* world = GetWorld();
	if (myGrid[x + 1][y] == '0' || myGrid[x + 1][y] == '2' || myGrid[x + 1][y] == '3')
	{
		FVector thisLocation = GetActorLocation();
		FVector spawnLocation = FVector(x * 150 + thisLocation.X, y * 150 + thisLocation.Y, thisLocation.Z);
		FRotator rotator = FRotator(0, 0, 0);
		AWall* spawnedWall = world->SpawnActor<AWall>(BottomWallActor, spawnLocation, rotator);
		return true;
	}
	else
		return false;
}

bool AGenerateMapFromFile::CheckBottom(int x, int y)
{
	UWorld* world = GetWorld();
	if (myGrid[x - 1][y] == '0' || myGrid[x - 1][y] == '2' || myGrid[x - 1][y] == '3')
	{
		FVector thisLocation = GetActorLocation();
		FVector spawnLocation = FVector(x * 150 + thisLocation.X, y * 150 + thisLocation.Y, thisLocation.Z);
		FRotator rotator = FRotator(0, 0, 0);
		AWall* spawnedWall = world->SpawnActor<AWall>(TopWallActor, spawnLocation, rotator);
		return true;
	}
	else
		return false;
}


