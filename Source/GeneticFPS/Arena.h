// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
class GENETICFPS_API Arena
{
public:
	Arena(int x, int y, int _size);
	~Arena();

	int size;
	int xPosition;
	int yPosition;
};
