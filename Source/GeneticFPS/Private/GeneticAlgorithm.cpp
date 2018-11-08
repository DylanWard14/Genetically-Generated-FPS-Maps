// Fill out your copyright notice in the Description page of Project Settings.

#include "GeneticAlgorithm.h"
#include "GeneticMap.h"
#include "FileHelper.h"
#include "Paths.h"


// Sets default values
AGeneticAlgorithm::AGeneticAlgorithm()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}



// Called when the game starts or when spawned
void AGeneticAlgorithm::BeginPlay()
{
	Super::BeginPlay();

	InitPopulation();


}

// Called every frame
void AGeneticAlgorithm::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (population.size() == populationSize &&  CheckSimulationCompleted() && !processingGeneration)
	{

		NextGeneration();

		if (survivors.size() > 0)
		{
			if (survivors[0].fitness > 200000)
			{
				UE_LOG(LogTemp, Warning, TEXT("Saving map with fitness value of %f"), survivors[0].fitness);
				FString level = convertLevelToText(survivors[0]);
				SaveTxt(level, "Maps.csv");
			}
		}
	}



	//if (population.size() < populationSize)
	//{
	//	NextGeneration();
	//}

}

bool AGeneticAlgorithm::SaveTxt(FString SaveTextB, FString FileNameB)
{
	return FFileHelper::SaveStringToFile(SaveTextB, *(FPaths::GameDir() + FileNameB));
}

bool AGeneticAlgorithm::LoadTxt(FString FileNameA, FString& SaveTextA)
{
	return FFileHelper::LoadFileToString(SaveTextA, *(FPaths::GameDir() + FileNameA));
}

void AGeneticAlgorithm::InitPopulation()
{

	for (int i = 0; i < populationSize; i++)
	{
		FVector OriginLocation = FVector(0, i * 10000, 0);

		FVector spawnLocation = FVector(i + OriginLocation.X, i + OriginLocation.Y, OriginLocation.Z);
		FRotator rotator = FRotator(0, 0, 0);
		AGeneticMap* spawnedMap = GetWorld()->SpawnActor<AGeneticMap>(map, spawnLocation, rotator);
		spawnedMap->GameplaySpeed = GameSpeed;
		population.push_back(spawnedMap);
		spawnedMap->myDNA = DNA(genomeLength, sizeX, sizeY);
		spawnedMap->InitGrid();
		spawnedMap->CreateArenas();
		spawnedMap->CreateCorridors();
		spawnedMap->GenerateGrid();
		spawnedMap->SpawnBots();

	}
}

bool AGeneticAlgorithm::CheckSimulationCompleted()
{
	if (population.size() == populationSize)
	{
		for (int i = 0; i < populationSize; i++)
		{
			if (!population[i]->calculatedFitness)
			{
				return false;
			}
		}
		return true;
	}
	else
		return false;
}

DNA AGeneticAlgorithm::TournamentSelection(AGeneticMap* p1, AGeneticMap* p2)
{
	if (p1->MapFitness >= p2->MapFitness)
	{
		UE_LOG(LogTemp, Warning, TEXT("Winning Fitness value between p1: %f and p2: %f = %f"), p1->MapFitness, p2->MapFitness, p1->MapFitness);
		return p1->myDNA;
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Winning Fitness value between p1: %f and p2: %f = %f"), p1->MapFitness, p2->MapFitness, p2->MapFitness);
		return p2->myDNA;
	}
}

void AGeneticAlgorithm::NextGeneration()
{
	//std::vector<DNA> survivors;
	processingGeneration = true;
	while (population.size() != 0)
	{
		while (population.size() > 1)
		{
			int RandNum1 = FMath::RandRange(0, population.size() - 1);
			int RandNum2 = FMath::RandRange(0, population.size() - 1);
			while (RandNum2 == RandNum1)
			{
				RandNum2 = FMath::RandRange(0, population.size() - 1);
			}

			population[RandNum1]->myDNA.fitness = population[RandNum1]->MapFitness;
			population[RandNum2]->myDNA.fitness = population[RandNum2]->MapFitness;

			if (population[RandNum1]->MapFitness != 0 && population[RandNum2]->MapFitness != 0)
			{
				DNA winner = TournamentSelection(population[RandNum1], population[RandNum2]);
				survivors.push_back(winner);
				UE_LOG(LogTemp, Warning, TEXT("Doing tournament selection"));
			}
			else if (population[RandNum1]->MapFitness != 0)
			{
				survivors.push_back(population[RandNum1]->myDNA);
				UE_LOG(LogTemp, Warning, TEXT("Saving p1 to survivors"));
			}
			else if (population[RandNum2]->MapFitness != 0)
			{
				survivors.push_back(population[RandNum2]->myDNA);
				UE_LOG(LogTemp, Warning, TEXT("saving p2 to surivors"));
			}


			population[RandNum1]->DestoryLevel();
			population[RandNum2]->DestoryLevel();
			population.erase(population.begin() + RandNum1);
			if(RandNum1 < RandNum2)
				population.erase(population.begin() + RandNum2 - 1);
			else
				population.erase(population.begin() + RandNum2);
		}
		//else
		//{
			survivors.push_back(population.front()->myDNA);
			population.front()->DestoryLevel();
			population.clear();

		//}
	}


	std::vector<DNA> tempSurvivors = survivors;
	while (population.size() < populationSize)
	{

		for (int i = 0; i < populationSize; i++)
		{
			FVector OriginLocation = FVector(0, i * 10000, 0);

			FVector spawnLocation = FVector(i + OriginLocation.X, i + OriginLocation.Y, OriginLocation.Z);
			FRotator rotator = FRotator(0, 0, 0);
			AGeneticMap* spawnedMap = GetWorld()->SpawnActor<AGeneticMap>(map, spawnLocation, rotator);
			spawnedMap->GameplaySpeed = GameSpeed;
			population.push_back(spawnedMap);

			DNA newDNA;
			if (tempSurvivors.size() > 1)
			{
				//UE_LOG(LogTemp, Warning, TEXT("Creating Mixed DNA"));
				// create random number1
				int RandNum1 = FMath::RandRange(0, tempSurvivors.size() - 1);
				int RandNum2 = FMath::RandRange(0, tempSurvivors.size() - 1);
				// create random number2

				// pass them into a newDNA
				newDNA = DNA(tempSurvivors[RandNum1], tempSurvivors[RandNum2], sizeX, sizeY);

				tempSurvivors.erase(tempSurvivors.begin() + RandNum1);

				if (RandNum1 < RandNum2)
				{
					//survivors.erase(survivors.begin() + RandNum2 - 1);
				}
				else
				{
					//survivors.erase(survivors.begin() + RandNum2);
				}
			}
			else if (tempSurvivors.size() == 1)
			{
				//UE_LOG(LogTemp, Warning, TEXT("Creating from single parent DNA"));
				newDNA = DNA(tempSurvivors[0], sizeX, sizeY);
				tempSurvivors.erase(tempSurvivors.begin());
			}
			else
			{
				//UE_LOG(LogTemp, Warning, TEXT("Creating Random DNA"));
				newDNA = DNA(genomeLength, sizeX, sizeY);
			}
			spawnedMap->myDNA = newDNA;
			spawnedMap->InitGrid();
			spawnedMap->CreateArenas();
			spawnedMap->CreateCorridors();
			spawnedMap->GenerateGrid();
			spawnedMap->SpawnBots();

		}
	}

	//clear survivors
	//survivors.clear();
	std::vector<DNA> temp = SortSurvivors(survivors);
	survivors.clear();
	survivors = temp;
	UE_LOG(LogTemp, Warning, TEXT("survivors size = %i"), survivors.size());
	if (!survivors.empty())
	{
		UE_LOG(LogTemp, Warning, TEXT("highest survivor = %f"), survivors[0].fitness);
	}
	processingGeneration = false;
}

std::vector<DNA> AGeneticAlgorithm::SortSurvivors(std::vector<DNA> Survivors)
{
	std::vector<DNA> tempList;
	tempList = Survivors;
	std::vector<DNA> newList;
	while (!tempList.empty())
	{
		int highestIndex = 0;
		for (int i = 0; i < tempList.size(); i++)
		{
			if (tempList[i].fitness > tempList[highestIndex].fitness)
			{
				highestIndex = i;
				//UE_LOG(LogTemp, Warning, TEXT("assigning"));
			}

		}
		newList.push_back(tempList[highestIndex]);
		tempList.erase(tempList.begin() + highestIndex);
	}

	UE_LOG(LogTemp, Warning, TEXT("function highest survivor = %f"), newList[0].fitness);
	return newList;
}

FString AGeneticAlgorithm::convertLevelToText(DNA Level)
{
	FString tempString;
	for (int x = 0; x < Level.myGrid.size(); x++)
	{
		for (int y = 0; y < Level.myGrid[0].size(); y++)
		{
			tempString += FString::FromInt(Level.myGrid[x][y]) + ",";
		}
		tempString += "\n";
	}
	return tempString;
}

