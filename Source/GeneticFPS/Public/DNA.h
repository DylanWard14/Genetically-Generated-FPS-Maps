// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Arena.h"
#include "Corridor.h"
#include "Cover.h"
#include <list>
#include <iostream>
#include <vector>

/**
 * 
 */
class GENETICFPS_API DNA
{
public:
	DNA();
	DNA(int genomeLength, int sizeX, int sizeY);
	DNA(DNA p1, DNA p2, int sizeX, int sizeY, float mutationRate = 0.25f);
	DNA(DNA p1, int sizeX, int sizeY, float mutationRate = 0.1f);
	~DNA();

	int genome [1][1];

	float fitness;

	std::vector<Arena> arenas;
	std::vector<Corridor> corridors;
	std::vector<Cover> cover;
	
	std::vector<std::vector<int>> myGrid;

	void CreateGrid(int sizeX, int sizeY);

	void GenerateArenas(int genomeLength, int sizeX, int sizeY);
	void GenerateCorridors(int genomeLength, int sizeX, int sizeY);
	void GenerateCover(int genomeLength, int sizeX, int sizeY);


};
