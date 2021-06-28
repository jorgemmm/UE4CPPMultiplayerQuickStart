// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseQSProjectile.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"

//Utils
#include "GameFramework/DamageType.h"
#include "Particles/ParticleSystem.h"
#include "Kismet/GameplayStatics.h"
#include "UObject/ConstructorHelpers.h"

ABaseQSProjectile::ABaseQSProjectile()
{


	

	/**SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("RootComponent")); in parent*/
	GetCollisionComp()->InitSphereRadius(37.5f);
	
	//GetCollisionComp()->SetCollisionProfileName(TEXT("BlockAllDynamic"));
	//RootComponent = GetCollisionComp();



	//Set a default value for explosion particle
	static ConstructorHelpers::FObjectFinder<UParticleSystem> DefaultExplosionEffect(TEXT("/Game/StarterContent/Particles/P_Explosion.P_Explosion"));
	if (DefaultExplosionEffect.Succeeded())
	{
		ExplosionEffect = DefaultExplosionEffect.Object;
	}


	/*ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement")); in parent*/
	GetProjectileMovement()->SetUpdatedComponent(GetCollisionComp());
	GetProjectileMovement()->InitialSpeed = 1500.0f;
	GetProjectileMovement()->MaxSpeed = 1500.0f;
	GetProjectileMovement()->bRotationFollowsVelocity = true;
	GetProjectileMovement()->bShouldBounce = true;

	GetProjectileMovement()->ProjectileGravityScale = 0.0f;

	
	SetReplicates(true);

}




void ABaseQSProjectile::Destroyed()
{
	FVector spawnLocation = GetActorLocation();
	UGameplayStatics::SpawnEmitterAtLocation(this, ExplosionEffect, spawnLocation, FRotator::ZeroRotator, true, EPSCPoolMethod::AutoRelease);

}






