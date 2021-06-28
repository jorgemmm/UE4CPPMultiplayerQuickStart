// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once 

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "FPQuickStartHUD.generated.h"

UCLASS()
class AFPQuickStartHUD : public AHUD
{
	GENERATED_BODY()

public:
	AFPQuickStartHUD();

	/** Primary draw call for the HUD */
	virtual void DrawHUD() override;

private:
	/** Crosshair asset pointer */
	class UTexture2D* CrosshairTex;

};

