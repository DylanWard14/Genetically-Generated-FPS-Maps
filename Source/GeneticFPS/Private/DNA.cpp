// Fill out your copyright notice in the Description page of Project Settings.

#include "DNA.h"
DNA::DNA()
{

}

DNA::DNA(int genomeLength, int sizeX, int sizeY)
{


	GenerateArenas(genomeLength, sizeX, sizeY);
	GenerateCorridors(genomeLength, sizeX, sizeY);


	CreateGrid(sizeX, sizeY);

	//GenerateCover(genomeLength, sizeX, sizeY);
}

DNA::DNA(DNA p1, DNA p2, int sizeX, int sizeY, float mutationRate)
{
	float mutationChance = FMath::RandRange(0.0f, 1.0f); //This will be used to determine if mutation will occur

	for (int i = 0; i < p1.arenas.size(); i++) //loop through all the needed arenas
	{
		if (mutationChance <= mutationRate) //Check if the generated mutationChance is less than the mutation rate
		{
			int xPos = FMath::RandRange(1, sizeX - 10); //Create a new Random Arena
			int yPos = FMath::RandRange(1, sizeY - 10);
			int size = FMath::RandRange(5, 10);
			arenas.push_back(Arena(xPos, yPos, size));
		}
		else //Mutate arena from one of the parents
		{
			int parentChance = FMath::RandRange(0, 2); //Generate random number to choose which parent will be used

			if (parentChance == 0) //use parent 1 
			{
				arenas.push_back(p1.arenas[i]); //add the arena from parent 1
			}
			else //use parent 2
			{
				arenas.push_back(p2.arenas[i]); //add the arena from parent 2
			}
		}
	}

	for (int i = 0; i < p1.corridors.size(); i++)
	{
		if (mutationChance <= mutationRate)
		{
			int xPos = FMath::RandRange(1, sizeX - 5);
			int yPos = FMath::RandRange(1, sizeY - 5);
			int length;

			int chance = FMath::RandRange(0, 100);

			if (chance < 50)
			{
				length = FMath::RandRange(-40, -5);
			}
			else
			{
				length = FMath::RandRange(5, 40);
			}
			corridors.push_back(Corridor(xPos, yPos, length));
		}

		else
		{
			int parentChance = FMath::RandRange(0, 2);

			if (parentChance == 0)
			{
				corridors.push_back(p1.corridors[i]);
			}
			else
			{
				corridors.push_back(p2.corridors[i]);
			}

		}
	}

	
	CreateGrid(sizeX, sizeY);
	/*
	UE_LOG(LogTemp, Warning, TEXT("p1 cover size = %i p2 cover size = %i"), p1.cover.size(), p2.cover.size());
	for (int i = 0; i < p1.cover.size(); i++)
	{
		if (mutationChance <= mutationRate)
		{
			int xPos = 0;
			int yPos = 0;

			int myRandom = FMath::RandRange(1, 2); // 1 == arena, 2 == corridor


			int rotation = FMath::RandRange(1, 2); // if 1 then it goes along the y axis, if 2 goes along x

			//if (myRandom == 1)
			//{
			//Do the arena stuff
			//Generate random number for array index
			int index = FMath::RandRange(0, arenas.size() - 1);
			//UE_LOG(LogTemp, Warning, TEXT("Creating from arena"));
			if (rotation == 1) // going along the y axis
			{
				xPos = FMath::RandRange(arenas[index].xPosition, (arenas[index].xPosition + arenas[index].size));
				yPos = FMath::RandRange(arenas[index].yPosition + 2, (arenas[index].yPosition + arenas[index].size) - 2);
				//UE_LOG(LogTemp, Warning, TEXT("Setting pos"));
			}

			else if (rotation == 2)
			{
				xPos = FMath::RandRange(arenas[index].xPosition + 2, (arenas[index].xPosition + arenas[index].size) - 2);
				yPos = FMath::RandRange(arenas[index].yPosition, (arenas[index].yPosition + arenas[index].size));
			}
			

			int length = FMath::RandRange(1, 3);
			if (myGrid[xPos][yPos] == 0)
			{
				cover.push_back(Cover(xPos, yPos, length, rotation));
				myGrid[xPos][yPos] = 2;
				//UE_LOG(LogTemp, Warning, TEXT("My Cover location = x %i y %i "), xPos, yPos);
			}


		}

		else
		{
			int parentChance = FMath::RandRange(0, 2);

			if (parentChance == 0)
			{
				if (i < p1.cover.size())
				{
					if (myGrid[p1.cover[i].xPosition][p1.cover[i].yPosition] == 0)
					{
						cover.push_back(p1.cover[i]);
					}
				}
			}
			else 
			{
				if (i < p2.cover.size())
				{
					if (myGrid[p2.cover[i].xPosition][p2.cover[i].yPosition] == 0)
					{
						cover.push_back(p2.cover[i]);
					}
				}
			}
		}
	}*/
	
}

DNA::DNA(DNA p1, int sizeX, int sizeY, float mutationRate)
{
	float mutationChance = FMath::RandRange(0.0f, 1.0f);

	for (int i = 0; i < p1.arenas.size(); i++)
	{
		if (mutationChance <= mutationRate)
		{
			int xPos = FMath::RandRange(1, sizeX - 10);
			int yPos = FMath::RandRange(1, sizeY - 10);
			int size = FMath::RandRange(5, 10);
			arenas.push_back(Arena(xPos, yPos, size));
		}
		else
		{
			arenas.push_back(p1.arenas[i]);
		
		}
	}

	for (int i = 0; i < p1.corridors.size(); i++)
	{
		if (mutationChance <= mutationRate)
		{
			int xPos = FMath::RandRange(1, sizeX - 5);
			int yPos = FMath::RandRange(1, sizeY - 5);
			int length;

			int chance = FMath::RandRange(0, 100);

			if (chance < 50)
			{
				length = FMath::RandRange(-40, -5);
			}
			else
			{
				length = FMath::RandRange(5, 40);
			}
			corridors.push_back(Corridor(xPos, yPos, length));
		}

		else
		{
			corridors.push_back(p1.corridors[i]);
		}
	}

	
	CreateGrid(sizeX, sizeY);
	/*
	UE_LOG(LogTemp, Warning, TEXT("p1 cover size = %i"), p1.cover.size());
	for (int i = 0; i < p1.cover.size(); i++)
	{
		if (mutationChance <= mutationRate)
		{
			int xPos = 0;
			int yPos = 0;

			int myRandom = FMath::RandRange(1, 2); // 1 == arena, 2 == corridor


			int rotation = FMath::RandRange(1, 2); // if 1 then it goes along the y axis, if 2 goes along x

			//if (myRandom == 1)
			//{
			//Do the arena stuff
			//Generate random number for array index
			int index = FMath::RandRange(0, arenas.size() - 1);
			//UE_LOG(LogTemp, Warning, TEXT("Creating from arena"));
			if (rotation == 1) // going along the y axis
			{
				xPos = FMath::RandRange(arenas[index].xPosition, (arenas[index].xPosition + arenas[index].size));
				yPos = FMath::RandRange(arenas[index].yPosition + 2, (arenas[index].yPosition + arenas[index].size) - 2);
				//UE_LOG(LogTemp, Warning, TEXT("Setting pos"));
			}

			else if (rotation == 2)
			{
				xPos = FMath::RandRange(arenas[index].xPosition + 2, (arenas[index].xPosition + arenas[index].size) - 2);
				yPos = FMath::RandRange(arenas[index].yPosition, (arenas[index].yPosition + arenas[index].size));
			}

			int length = FMath::RandRange(1, 3);

			if (myGrid[xPos][yPos] == 0)
			{
				cover.push_back(Cover(xPos, yPos, length, rotation));
				myGrid[xPos][yPos] = 2;
				//UE_LOG(LogTemp, Warning, TEXT("My Cover location = x %i y %i "), xPos, yPos);
			}

		}
		else if (myGrid[p1.cover[i].xPosition][p1.cover[i].yPosition] == 0)
		{
			cover.push_back(p1.cover[i]);

		}
	}*/
}

DNA::~DNA()
{
}

void DNA::CreateGrid(int sizeX, int sizeY)
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

	//adds the arenas to the list
	for (int i = 0; i < arenas.size(); i++)
	{
		for (int x = arenas[i].xPosition; x < (arenas[i].size + arenas[i].xPosition); x++)
		{
			for (int y = arenas[i].yPosition; y < (arenas[i].size + arenas[i].yPosition); y++)
			{
				if (x > 1 && x < (sizeX - 1) && y > 1 && y < (sizeY - 1))
				{
					myGrid[x][y] = 0;
				}
			}

		}
	}

	//adds corridors to the list
	for (int i = 0; i < corridors.size(); i++)
	{
		if (corridors[i].length >= 0)
		{
			for (int x = corridors[i].xPosition; x < (corridors[i].length + corridors[i].xPosition); x++)
			{
				for (int y = corridors[i].yPosition; y < (corridors[i].yPosition + corridors[i].width); y++)
				{
					if (x > 1 && x < (sizeX - 1) && y > 1 && y < (sizeY - 1))
					{
						myGrid[x][y] = 0;
					}
				}
			}
		}

		else if (corridors[i].length < 0)
		{
			for (int x = std::abs(corridors[i].xPosition); x < std::abs(corridors[i].xPosition + corridors[i].width); x++)
			{
				for (int y = std::abs(corridors[i].yPosition); y < std::abs(corridors[i].yPosition + corridors[i].length); y++)
				{
					if (x > 1 && x < (sizeX - 1) && y > 1 && y < (sizeY - 1))
					{
						myGrid[x][y] = 0;
					}
				}
			}
		}
	}

}

void DNA::GenerateArenas(int genomeLength, int sizeX, int sizeY)
{
	for (int i = 0; i < genomeLength; i++)
	{
		int xPos = FMath::RandRange(1, sizeX - 10);
		int yPos = FMath::RandRange(1, sizeY - 10);
		int size = FMath::RandRange(5, 10);
		arenas.push_back(Arena(xPos, yPos, size));
	}
}

void DNA::GenerateCorridors(int genomeLength, int sizeX, int sizeY)
{
	for (int i = 0; i < genomeLength; i++)
	{
		int xPos = FMath::RandRange(1, sizeX - 5);
		int yPos = FMath::RandRange(1, sizeY - 5);
		int length;

		int chance = FMath::RandRange(0, 100);

		if (chance < 50)
		{
			length = FMath::RandRange(-40, -5);
		}
		else
		{
			length = FMath::RandRange(5, 40);
		}

		corridors.push_back(Corridor(xPos, yPos, length));
	}
}

void DNA::GenerateCover(int genomeLength, int sizeX, int sizeY)
{
	int xPos = 0;
	int yPos = 0;
	for (int i = 0; i < genomeLength * 2; i++)
	{
		//Select a random arena or corridor
		int myRandom = 1; //FMath::RandRange(1, 2); // 1 == arena, 2 == corridor


		int rotation = FMath::RandRange(1, 2); // if 1 then it goes along the y axis, if 2 goes along x

		if (myRandom == 1)
		{
			//Do the arena stuff
			//Generate random number for array index
			int index = FMath::RandRange(0, arenas.size() - 1);
			//UE_LOG(LogTemp, Warning, TEXT("Creating from arena"));
			if (rotation == 1) // going along the y axis
			{
				xPos = FMath::RandRange(arenas[index].xPosition, (arenas[index].xPosition + arenas[index].size));
				yPos = FMath::RandRange(arenas[index].yPosition + 2, (arenas[index].yPosition + arenas[index].size) - 2);
				//UE_LOG(LogTemp, Warning, TEXT("Setting pos"));
			}

			else if (rotation == 2)
			{
				xPos = FMath::RandRange(arenas[index].xPosition + 2, (arenas[index].xPosition + arenas[index].size) - 2);
				yPos = FMath::RandRange(arenas[index].yPosition, (arenas[index].yPosition + arenas[index].size));
			}
		}
		/*else if (myRandom == 2)
		{
			// do corridor stuff
			int index = FMath::RandRange(0, corridors.size() - 1);
			UE_LOG(LogTemp, Warning, TEXT("Creating cover in corridor"));
			if (corridors[index].length >= 0)
			{
				
				//corridors length is along the x
				//x + length ... y + width
				rotation = 2;
					xPos = FMath::RandRange(corridors[index].xPosition, (corridors[index].xPosition + corridors[index].length));
					yPos = FMath::RandRange(corridors[index].yPosition, (corridors[index].yPosition + corridors[index].width));
			}
			else
			{
				//corridors length is along the y
				//y + length ... x + width
				rotation = 1;
					xPos = FMath::RandRange(corridors[index].xPosition, (corridors[index].xPosition + corridors[index].width));
					yPos = FMath::RandRange(corridors[index].yPosition, (corridors[index].yPosition + abs(corridors[index].length)));
			}
		} */

		int length = FMath::RandRange(1, 3);
		//UE_LOG(LogTemp, Warning, TEXT("My Cover location = x %i y %i "), xPos, yPos);
		if (xPos < sizeX && yPos < sizeY)
		{
			if (myGrid[xPos][yPos] == 0)
			{
				cover.push_back(Cover(xPos, yPos, length, rotation));
				//UE_LOG(LogTemp, Warning, TEXT("My Cover location = x %i y %i "), xPos, yPos);
			}
		}

	}

	

	//UE_LOG(LogTemp, Warning, TEXT("Placed all cover locations, cover list size = %i "), cover.size());
}
