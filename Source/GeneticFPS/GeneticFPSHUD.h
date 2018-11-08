// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#pragma once 

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "GeneticFPSHUD.generated.h"

UCLASS()
class AGeneticFPSHUD : public AHUD
{
	GENERATED_BODY()

public:
	AGeneticFPSHUD();

	/** Primary draw call for the HUD */
	virtual void DrawHUD() override;

private:
	/** Crosshair asset pointer */
	class UTexture2D* CrosshairTex;

};
