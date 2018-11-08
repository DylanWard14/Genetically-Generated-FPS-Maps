// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
class GENETICFPS_API Cover
{
public:
	Cover(int x, int y, int myLength, int myRotation);
	~Cover();
	int xPosition;
	int yPosition;
	int length;
	int rotation;
};
