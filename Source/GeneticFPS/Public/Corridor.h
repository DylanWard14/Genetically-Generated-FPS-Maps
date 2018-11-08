// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
class GENETICFPS_API Corridor
{
public:
	Corridor(int x, int y, int _Length);
	~Corridor();

	int xPosition;
	int yPosition;
	int length;
	int width = 3;
};
