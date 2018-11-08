// Fill out your copyright notice in the Description page of Project Settings.

#include "GeneticMap.h"
#include <cmath>

// Sets default values
AGeneticMap::AGeneticMap()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}


// Called when the game starts or when spawned
void AGeneticMap::BeginPlay()
{
	Super::BeginPlay();
	MaxPlaytime = MaxPlaytime * 60;
	if (!crossOver)
	{
		//myDNA = DNA(genomeLength, sizeX, sizeY);
		//InitGrid();
		//CreateArenas();
		//CreateCorridors();
		//GenerateGrid();
		//SpawnBots();
	}

}

// Called every frame
void AGeneticMap::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//Gametime += (DeltaTime * GameplaySpeed) / 60;
	Gametime += (DeltaTime * GameplaySpeed);

	if (Gametime >= MaxPlaytime && !calculatedFitness)
	{
		CalculateFitness();
		calculatedFitness = true;
	}

}

void AGeneticMap::InitGrid()
{
	myGrid.resize(sizeX);
	for (int x = 0; x < sizeX; x++)
	{
		myGrid[x].resize(sizeX);
	}

	for (int i = 0; i < sizeX; i++)
	{
		for (int j = 0; j < sizeY; j++)
		{
			myGrid[i][j] = 1;
		}
	}
}

void AGeneticMap::CreateArenas()
{
	for (int i = 0; i < myDNA.arenas.size(); i++)
	{
		for (int x = myDNA.arenas[i].xPosition; x < (myDNA.arenas[i].size + myDNA.arenas[i].xPosition); x++)
		{
			for (int y = myDNA.arenas[i].yPosition; y < (myDNA.arenas[i].size + myDNA.arenas[i].yPosition); y++)
			{
				if (x > 1 && x < (sizeX - 1) && y > 1 && y < (sizeY - 1))
				{
					myGrid[x][y] = 0;
				}
			}
		}
	}
	CreateCover();
}

void AGeneticMap::CreateCover()
{
	//UE_LOG(LogTemp, Warning, TEXT("Creating Cover"));
	for (int i = 0; i < myDNA.cover.size(); i++)
	{
		int xPos = myDNA.cover[i].xPosition;
		int yPos = myDNA.cover[i].yPosition;
		if (myDNA.cover[i].rotation == 1 && myDNA.cover[i].xPosition < sizeX - 1 && myDNA.cover[i].xPosition > 0 && myDNA.cover[i].yPosition < sizeY - 1 && myDNA.cover[i].yPosition
			 > 0)
		{
			myGrid[xPos][yPos] = 3;

			if (myDNA.cover[i].length == 2 && myGrid[xPos + 1][yPos] == 0)
			{
				myGrid[xPos + 1][yPos] = 3;
			}
			if (myDNA.cover[i].length == 3 && myGrid[xPos + 2][yPos] == 0)
			{
				myGrid[xPos + 2][yPos] = 3;
			}
		}
		else if (myDNA.cover[i].xPosition < sizeX - 1 && myDNA.cover[i].xPosition > 0 && myDNA.cover[i].yPosition < sizeY - 1 && myDNA.cover[i].yPosition
		> 0)
		{
			myGrid[xPos][yPos] = 2;

			if (myDNA.cover[i].length == 2 && myGrid[xPos][yPos + 1] == 0)
			{
				myGrid[xPos][yPos + 1] = 2;
			}
			if (myDNA.cover[i].length == 3 && myGrid[xPos][yPos + 2] == 0)
			{
				myGrid[xPos][yPos + 2] = 2;
			}
		}
	}

	//UE_LOG(LogTemp, Warning, TEXT("Cover created"));
}

void AGeneticMap::CreateCorridors()
{
	for (int i = 0; i < myDNA.corridors.size(); i++)
	{
		if (myDNA.corridors[i].length >= 0)
		{
			for (int x = myDNA.corridors[i].xPosition; x < (myDNA.corridors[i].length + myDNA.corridors[i].xPosition); x++)
			{
				for (int y = myDNA.corridors[i].yPosition; y < (myDNA.corridors[i].yPosition + myDNA.corridors[i].width); y++)
				{
					if (x > 1 && x < (sizeX - 1) && y > 1 && y < (sizeY - 1))
					{
						myGrid[x][y] = 0; // length along the x axis
						//UE_LOG(LogTemp, Warning, TEXT("Going this way"));
					}
				}
			}
		}

		else if (myDNA.corridors[i].length < 0)
		{
			for (int x = std::abs(myDNA.corridors[i].xPosition); x < std::abs(myDNA.corridors[i].xPosition + myDNA.corridors[i].width); x++)
			{
				for (int y = std::abs(myDNA.corridors[i].yPosition); y < std::abs(myDNA.corridors[i].yPosition + myDNA.corridors[i].length); y++)
				{
					if (x > 1 && x < (sizeX - 1) && y > 1 && y < (sizeY - 1))
					{
						myGrid[x][y] = 0; // length along the y axis
						
					}
				}
			}
		}
	}
}

void AGeneticMap::GenerateGrid()
{
	for (int i = 0; i < sizeX; i++)
	{
		FVector thisLocation = GetActorLocation();
		FVector spawnLocation = FVector(i * 150 + thisLocation.X, thisLocation.Y, thisLocation.Z);
		FRotator rotator = FRotator(0, 0, 0);
		actors.push_back(GetWorld()->SpawnActor<AWall>(LeftWallActor, spawnLocation, rotator));

		spawnLocation = FVector(i * 150 + thisLocation.X, (sizeY - 1) * 150 + thisLocation.Y, thisLocation.Z);
		rotator = FRotator(0, 0, 0);
		actors.push_back(GetWorld()->SpawnActor<AWall>(RightWallActor, spawnLocation, rotator));

		spawnLocation = FVector(thisLocation.X, i * 150 + thisLocation.Y, thisLocation.Z);
		rotator = FRotator(0, 0, 0);
		actors.push_back(GetWorld()->SpawnActor<AWall>(BottomWallActor, spawnLocation, rotator));

		spawnLocation = FVector((sizeX - 1) * 150 + thisLocation.X, i * 150 + thisLocation.Y, thisLocation.Z);
		rotator = FRotator(0, 0, 0);
		actors.push_back(GetWorld()->SpawnActor<AWall>(TopWallActor, spawnLocation, rotator));
	}
	for (int x = 1; x < sizeX - 1; x++)
	{
		for (int y = 1; y < sizeY - 1; y++)
		{
			UWorld* world = GetWorld();
			if (myGrid[x][y] == 1)
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

			if (myGrid[x][y] == 0 || myGrid[x][y] == 2 || myGrid[x][y] == 3)
			{
				// spawn floor
				FVector thisLocation = GetActorLocation();
				FVector spawnLocation = FVector(x * 150 + thisLocation.X, y * 150 + thisLocation.Y, thisLocation.Z);
				FRotator rotator = FRotator(0, 0, 0);
				actors.push_back(GetWorld()->SpawnActor<AWall>(FloorActor, spawnLocation, rotator));
			}

			if (myGrid[x][y] == 2)
			{
				// spawn floor
				FVector thisLocation = GetActorLocation();
				FVector spawnLocation = FVector(x * 150 + thisLocation.X, y * 150 + thisLocation.Y, thisLocation.Z);
				FRotator rotator = FRotator(0, 0, 0);
				actors.push_back(GetWorld()->SpawnActor<AWall>(CoverActor, spawnLocation, rotator));
			}
			if (myGrid[x][y] == 3)
			{
				// spawn floor
				FVector thisLocation = GetActorLocation();
				FVector spawnLocation = FVector(x * 150 + thisLocation.X, y * 150 + thisLocation.Y, thisLocation.Z);
				FRotator rotator = FRotator(0, 0, 0);
				actors.push_back(GetWorld()->SpawnActor<AWall>(CoverActorRotated, spawnLocation, rotator));
			}
		}
	}
}

void AGeneticMap::SpawnBots()
{
	int botsPerTeam = NumberOfBots / NumberOfTeams;
	UWorld* world = GetWorld();
	int botsSpawned = 0;
	if (world)
	{
		for (int x = 0; x < sizeX-1; x++)
		{
			for (int y = 0; y < sizeY-1; y++)
			{
				if (myGrid[x][y] == 0 && botsSpawned < botsPerTeam)
				{
					FVector thisLocation = GetActorLocation();
					FVector spawnLocation = FVector(x * 150 + thisLocation.X, y * 150 + thisLocation.Y, thisLocation.Z);
					FRotator rotator = FRotator(0, 0, 0);
					AEnemyAICharacter* spawnedAI = world->SpawnActor<AEnemyAICharacter>(bot, spawnLocation, rotator);
					bots.Add(spawnedAI);
					spawnedAI->TeamID = 1;
					spawnedAI->GameplaySpeed = GameplaySpeed;
					actors.push_back(spawnedAI);
					botsSpawned++;
				}

				if (botsSpawned >= botsPerTeam)
				{
					break;
				}
			}
		}
		botsSpawned = 0;
		
		for (int x = sizeX - 1; x > 0; x--)
		{
			for (int y = sizeY - 1; y > 0; y--)
			{
				if (myGrid[x][y] == 0 && botsSpawned < botsPerTeam)
				{
					FVector thisLocation = GetActorLocation();
					FVector spawnLocation = FVector(x * 150 + thisLocation.X, y * 150 + thisLocation.Y, thisLocation.Z);
					FRotator rotator = FRotator(0, 0, 0);
					AEnemyAICharacter* spawnedAI = world->SpawnActor<AEnemyAICharacter>(bot, spawnLocation, rotator);
					bots.Add(spawnedAI);
					spawnedAI->TeamID = 2;
					spawnedAI->GameplaySpeed = GameplaySpeed;
					actors.push_back(spawnedAI);
					botsSpawned++;
				}
				if (botsSpawned >= botsPerTeam)
				{
					break;
				}
			}
		}
	}
}

void AGeneticMap::CalculateFitness()
{
	float totalFightingTime = 0;
	float fightingTimeCount = 0;
	float TempFitness = 0;

	for (int i = 0; i < NumberOfBots; i++)
	{
		TempFitness += bots[i]->tension;

		//get the fighting time from the array of fights on all bots
		for (int j = 0; j < bots[i]->fightingTimes.Num(); j++)
		{
			totalFightingTime += bots[i]->fightingTimes[j];
			fightingTimeCount++;
		}
	}

	float averageFightingTime = 0;

	if (totalFightingTime > 0)
	{
		// calculate average fighting time
		averageFightingTime = totalFightingTime / fightingTimeCount;
	}
	
	MapFitness = ((TempFitness / NumberOfBots) + GetLevelSize()) * averageFightingTime;

	UE_LOG(LogTemp, Warning, TEXT("Map fitness = %f"), MapFitness);
	
}

int AGeneticMap::GetLevelSize()
{
	int size = 0;
	for (int x = 0; x < sizeX; x++)
	{
		for (int y = 0; y < sizeY; y++)
		{
			if (myGrid[x][y] == 0)
			{
				size++;
			}
		}
	}
	return size;
}

void AGeneticMap::StartCrossOver()
{
	myDNA = DNA(p1->myDNA, p2->myDNA, sizeX, sizeY);
	InitGrid();
	CreateArenas();
	CreateCorridors();
	GenerateGrid();
}


bool AGeneticMap::CheckLeft(int x, int y)
{
	UWorld* world = GetWorld();
	if (myGrid[x][y - 1] == 0 || myGrid[x][y - 1] == 2 || myGrid[x][y - 1] == 3)
	{
		FVector thisLocation = GetActorLocation();
		FVector spawnLocation = FVector(x * 150 + thisLocation.X, y * 150 + thisLocation.Y, thisLocation.Z);
		FRotator rotator = FRotator(0, 0, 0);
		AWall* spawnedWall = world->SpawnActor<AWall>(RightWallActor, spawnLocation, rotator);
		actors.push_back(spawnedWall);
		return true;
	}
	else
	return false;
}

bool AGeneticMap::CheckRight(int x, int y)
{
	UWorld* world = GetWorld();
	if (myGrid[x][y + 1] == 0 || myGrid[x][y + 1] == 2 || myGrid[x][y + 1] == 3)
	{
		FVector thisLocation = GetActorLocation();
		FVector spawnLocation = FVector(x * 150 + thisLocation.X, y * 150 + thisLocation.Y, thisLocation.Z);
		FRotator rotator = FRotator(0, 0, 0);
		AWall* spawnedWall = world->SpawnActor<AWall>(LeftWallActor, spawnLocation, rotator);
		actors.push_back(spawnedWall);
		return true;
	}
	else
		return false;
}

bool AGeneticMap::CheckTop(int x, int y)
{
	UWorld* world = GetWorld();
	if (myGrid[x + 1][y] == 0 || myGrid[x + 1][y] == 2 || myGrid[x + 1][y] == 3)
	{
		FVector thisLocation = GetActorLocation();
		FVector spawnLocation = FVector(x * 150 + thisLocation.X, y * 150 + thisLocation.Y, thisLocation.Z);
		FRotator rotator = FRotator(0, 0, 0);
		AWall* spawnedWall = world->SpawnActor<AWall>(BottomWallActor, spawnLocation, rotator);
		actors.push_back(spawnedWall);
		return true;
	}
	else
		return false;
}

bool AGeneticMap::CheckBottom(int x, int y)
{
	UWorld* world = GetWorld();
	if (myGrid[x - 1][y] == 0 || myGrid[x - 1][y] == 2 || myGrid[x - 1][y] == 3)
	{
		FVector thisLocation = GetActorLocation();
		FVector spawnLocation = FVector(x * 150 + thisLocation.X, y * 150 + thisLocation.Y, thisLocation.Z);
		FRotator rotator = FRotator(0, 0, 0);
		AWall* spawnedWall = world->SpawnActor<AWall>(TopWallActor, spawnLocation, rotator);
		actors.push_back(spawnedWall);
		return true;
	}
	else
		return false;
}

void AGeneticMap::DestoryLevel()
{
	for (int i = 0; i < bots.Max(); i++)
	{
		bots[i]->IAmBeingDestroyed();
	}

	while (actors.size() != 0)
	{
		actors.back()->Destroy();
		actors.pop_back();
		//for (int i = 0; i < actors.size(); i++)
		//{
		//	actors[i]->Destroy();
		//}
	}
	//actors.clear();
	myGrid.clear();
	bots.Empty();
	Destroy();
}
