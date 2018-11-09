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
	Gametime += (DeltaTime * GameplaySpeed); //This speeds up the game time by the gameplay speed

	if (Gametime >= MaxPlaytime && !calculatedFitness)//When the game timer has reached the max play time and we havent calculated fitness
	{
		CalculateFitness(); // call the calculate fitness funciton
		calculatedFitness = true;
	}

}

///This function resizes the 2D array to the correct size of the map and assigns each index to 1
void AGeneticMap::InitGrid()
{
	myGrid.resize(sizeX); //resizes the 2D arrays x size
	for (int x = 0; x < sizeX; x++) // loop through all the values in the x of the 2D array
	{
		myGrid[x].resize(sizeX); //Resize those arrays to the size of the map
	}

	for (int i = 0; i < sizeX; i++) //Loop through the maps X
	{
		for (int j = 0; j < sizeY; j++) //Loop through the maps Y
		{
			myGrid[i][j] = 1; //Set each index to 1. A value of 1 represents a wall
		}
	}
}

///This function loops through all the arenas in the maps DNA and changes the grids index to 0 at the positions of each arena
void AGeneticMap::CreateArenas()
{
	for (int i = 0; i < myDNA.arenas.size(); i++) //Loop through all the arenas in the DNA
	{
		for (int x = myDNA.arenas[i].xPosition; x < (myDNA.arenas[i].size + myDNA.arenas[i].xPosition); x++) //Loop through all the tiles in this arenas X
		{
			for (int y = myDNA.arenas[i].yPosition; y < (myDNA.arenas[i].size + myDNA.arenas[i].yPosition); y++) //Loop through all the tiles in the arenas Y
			{
				if (x > 1 && x < (sizeX - 1) && y > 1 && y < (sizeY - 1)) //If we are inside the grid still
				{
					myGrid[x][y] = 0; //Set the index at position x,y to 0, representing a floor
				}
			}
		}
	}
	CreateCover();
}

///This function  will loops though all the cover in the maps DNA and change the grid index accordingly
///This function is currently broken
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

///This function loops through all the Corridors in the maps DNA and changes the grids index to 0 at the positions of each Corridor
void AGeneticMap::CreateCorridors()
{
	for (int i = 0; i < myDNA.corridors.size(); i++) //Loop through all the corridors in the DNA
	{
		if (myDNA.corridors[i].length >= 0) //If the length is a positive value, make the corridor run along the X
		{
			for (int x = myDNA.corridors[i].xPosition; x < (myDNA.corridors[i].length + myDNA.corridors[i].xPosition); x++) //Loop through all the X positions
			{
				for (int y = myDNA.corridors[i].yPosition; y < (myDNA.corridors[i].yPosition + myDNA.corridors[i].width); y++) //Loop through all the Y positions
				{
					if (x > 1 && x < (sizeX - 1) && y > 1 && y < (sizeY - 1)) //If we are still inside the grid
					{
						myGrid[x][y] = 0; // set index to 0
						//UE_LOG(LogTemp, Warning, TEXT("Going this way"));
					}
				}
			}
		}

		else if (myDNA.corridors[i].length < 0) // if the length is a negative value, Make the corridor run along the Y
		{
			for (int x = std::abs(myDNA.corridors[i].xPosition); x < std::abs(myDNA.corridors[i].xPosition + myDNA.corridors[i].width); x++) //Loop through all the X positions
			{
				for (int y = std::abs(myDNA.corridors[i].yPosition); y < std::abs(myDNA.corridors[i].yPosition + myDNA.corridors[i].length); y++) //Loop through all the Y positions
				{
					if (x > 1 && x < (sizeX - 1) && y > 1 && y < (sizeY - 1)) //If we are still inside the grid
					{
						myGrid[x][y] = 0; //Set index to 0
						
					}
				}
			}
		}
	}
}

///This function spawns the approriate models at each position
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
	for (int x = 1; x < sizeX - 1; x++) //Loop through the maps X
	{
		for (int y = 1; y < sizeY - 1; y++) //Loop through the Maps Y
		{
			UWorld* world = GetWorld();
			if (myGrid[x][y] == 1) //If the currnet loop index equals 1, a Wall tile
			{
				if (world)
				{
					CheckLeft(x, y); //Check if we should spawn a left wall
					CheckRight(x, y); //Check if we should spawn a right wall
					CheckTop(x, y); //Check if we should spawn a Top wall
					CheckBottom(x, y); //Check if we should spawn a bottom wall
				}

				/*FVector thisLocation = GetActorLocation();
				FVector spawnLocation = FVector(x * 300 + thisLocation.X, y * 300 + thisLocation.Y, thisLocation.Z);
				FRotator rotator = FRotator(0, 0, 0);
				GetWorld()->SpawnActor<AWall>(CeilingActor, spawnLocation, rotator);
				*/
			}

			if (myGrid[x][y] == 0 || myGrid[x][y] == 2 || myGrid[x][y] == 3) //if the index is a 0 then we spawn a floor tile
			{
				// spawn floor
				FVector thisLocation = GetActorLocation();
				FVector spawnLocation = FVector(x * 150 + thisLocation.X, y * 150 + thisLocation.Y, thisLocation.Z);
				FRotator rotator = FRotator(0, 0, 0);
				actors.push_back(GetWorld()->SpawnActor<AWall>(FloorActor, spawnLocation, rotator));
			}

			if (myGrid[x][y] == 2) //this is reposible for spawing cover
			{
				// spawn floor
				FVector thisLocation = GetActorLocation();
				FVector spawnLocation = FVector(x * 150 + thisLocation.X, y * 150 + thisLocation.Y, thisLocation.Z);
				FRotator rotator = FRotator(0, 0, 0);
				actors.push_back(GetWorld()->SpawnActor<AWall>(CoverActor, spawnLocation, rotator));
			}
			if (myGrid[x][y] == 3) //this is resposible for spawning a rotated cover actor
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

///This function spawns the bots at the opposite ends of the map
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

///This function is reposible for calculating the fitness value of the map
void AGeneticMap::CalculateFitness()
{
	float totalFightingTime = 0;
	float fightingTimeCount = 0;
	float TempFitness = 0;

	for (int i = 0; i < NumberOfBots; i++) //Loop through all the bots in the map and total their tension and fighting times
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

	if (totalFightingTime > 0) //calculate the averate fighting time
	{
		// calculate average fighting time
		averageFightingTime = totalFightingTime / fightingTimeCount;
	}
	
	MapFitness = ((TempFitness / NumberOfBots) + GetLevelSize()) * averageFightingTime; //Calculate the maps fitness value

	UE_LOG(LogTemp, Warning, TEXT("Map fitness = %f"), MapFitness);
	
}

///This function calcualtes the levels overall size
int AGeneticMap::GetLevelSize()
{
	int size = 0;
	for (int x = 0; x < sizeX; x++) //Loop through the maps X
	{
		for (int y = 0; y < sizeY; y++) //Loop through the maps y
		{
			if (myGrid[x][y] == 0) //for each index of 0 in the array
			{
				size++; //increase the size varaible
			}
		}
	}
	return size;
}

///This function is reposible for performing cross over
void AGeneticMap::StartCrossOver()
{
	myDNA = DNA(p1->myDNA, p2->myDNA, sizeX, sizeY);
	InitGrid();
	CreateArenas();
	CreateCorridors();
	GenerateGrid();
}

/// This function checks to see if there is a floor tile next to this wall on the left side
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

/// This function checks to see if there is a floor tile next to this wall on the right side
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

/// This function checks to see if there is a floor tile next to this wall on the top side
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

/// This function checks to see if there is a floor tile next to this wall on the bottom side
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

/// This function destorys the level and the AI aswell as clearing all arrays and vectors
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
