// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FPQuickStart/FPQuickStartProjectile.h"
#include "BaseQSProjectile.generated.h"

/**
 * 
 */

class UParticleSystem;

class UDamageType;


UCLASS(config = Game)
class FPQUICKSTART_API ABaseQSProjectile : public AFPQuickStartProjectile
{
	GENERATED_BODY()


public:

	ABaseQSProjectile();

	// Particle used when the projectile impacts against another object and explodes.
	UPROPERTY(EditAnywhere, Category = "Effects")
		UParticleSystem* ExplosionEffect;

	

	

	virtual void Destroyed() override;
	
};
