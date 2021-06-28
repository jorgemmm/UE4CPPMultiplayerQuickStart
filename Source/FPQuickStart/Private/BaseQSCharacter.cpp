// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseQSCharacter.h"


//Requires for this actor
#include "Animation/AnimInstance.h"

#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/InputSettings.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"


#include "MotionControllerComponent.h"
#include "XRMotionControllerBase.h" // for FXRMotionControllerBase::RightHandSourceId

//Enegine
#include "GameFramework/Actor.h"
#include "Engine/World.h"
#include "GameFramework/Actor.h"

//online
#include "Net/UnrealNetwork.h"
#include "Engine/Engine.h"

//Utils
#include  "DrawDebugHelpers.h"


 ABaseQSCharacter::ABaseQSCharacter()
{
	 
	
	//Initialize the player's Health
	MaxHealth = 100.0f;
	CurrentHealth = MaxHealth;


	
}




 FString GetEnumText(ENetRole Role)
 {
	 switch (Role)
	 {
	 case ROLE_None:
		 return "None";
	 case ROLE_SimulatedProxy:
		 return "SimulatedProxy";
	 case ROLE_AutonomousProxy:
		 return "AutonomousProxy";
	 case ROLE_Authority:
		 return "Authority";
	 default:
		 return "ERROR";
	 }
 }

 //Setup Replication
 void ABaseQSCharacter::GetLifetimeReplicatedProps(TArray <FLifetimeProperty> & OutLifetimeProps) const
 {
	 Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	 //Replicate current health.
	 DOREPLIFETIME(ABaseQSCharacter, CurrentHealth);
 }


 //Se puede sustitui por un timer
 void ABaseQSCharacter::Tick(float DeltaTime)
 {
	 Super::Tick(DeltaTime);

	 //Comentar en producción, sólo para debug

	 AActor* MyOwner = GetOwner();
	 if (MyOwner != nullptr) {
		
		 //Para el jugador
		 DrawDebugString(GetWorld(),
			 MyOwner->GetActorLocation(), //FVector(0, 0, 100), 
			  GetEnumText(MyOwner->GetLocalRole()), this, FColor::White, DeltaTime);



	 }
	 else
	 {
		 //Para enemigos
		 DrawDebugString(GetWorld(),
			 GetActorLocation(), //FVector(0, 0, 100), 
			 GetEnumText(GetLocalRole()), this, FColor::White, DeltaTime);
	 }
	 
 }

 void ABaseQSCharacter::OnRep_CurrentHealth()
 {

	 OnHealthUpdate();
	
	
 }


 void ABaseQSCharacter::OnHealthUpdate()
 {
	 //Client-specific functionality
	 if (IsLocallyControlled())
	 {
		 FString healthMessage = FString::Printf(TEXT("You now have %f health remaining."), CurrentHealth);
		 GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, healthMessage);

		 if (CurrentHealth <= 0)
		 {
			FString deathMessage = FString::Printf(TEXT("You have been killed."));
			 GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, deathMessage);

			 // ragdoll the character
			 //ACharacter* MyCharacter = UGameplayStatics::GetPlayerCharacter(this, 0);
			 //if (MyCharacter)
			 //{
				// MyCharacter->GetMesh()->SetSimulatePhysics(true);
				// //MyCharacter->GetMovementComponent()->MovementState.bCanJump = false;
			 //}
		 }
	 }

	 //Server-specific functionality
	 if (GetLocalRole() == ROLE_Authority)
	 {
		 FString healthMessage = FString::Printf(TEXT("%s now has %f health remaining."), *GetFName().ToString(), CurrentHealth);
		 GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, healthMessage);
	 }
	
	 if (GetLocalRole() == ROLE_AutonomousProxy)
	 {
		 //Suelen ser los propietarios o los que controlan al Pawn
		 //Es decir el Jugador o Hero

	 }
	 if (GetLocalRole() == ROLE_SimulatedProxy)
	 {
		 //Suelen ser los enemigos o jugadores o online
		 FString healthMessage = FString::Printf(TEXT("You now have %f health remaining."), CurrentHealth);
		 GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, healthMessage);

		 if (CurrentHealth <= 0)
		 {
			 FString deathMessage = FString::Printf(TEXT("You have been killed."));
			 GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, deathMessage);

			 // ragdoll the character
			 ACharacter* MyCharacter = UGameplayStatics::GetPlayerCharacter(this, 0);
			 if (MyCharacter)
			 {
				MyCharacter->GetMesh()->SetSimulatePhysics(true);
				// //MyCharacter->GetMovementComponent()->MovementState.bCanJump = false;
			 }
		 }

	 }


	 //Functions that occur on all machines. 
	/*

	 
		
		Any special functionality that should occur as a result of damage or death should be placed here.
	*/
 }

 void ABaseQSCharacter::SetCurrentHealth(float healthValue)
 {
	 //En servidor el valor de health se propagará / replicará

	// GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT(" Set Current Health")));
	 if (GetLocalRole() == ROLE_Authority)
	 //if(HasAuthority())
	 {
		 GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Magenta, FString::Printf(TEXT(" Take Damage")));
		 CurrentHealth = FMath::Clamp(healthValue, 0.f, MaxHealth);
		 OnHealthUpdate();
	 }
 }

 float ABaseQSCharacter::TakeDamage(float DamageTaken, FDamageEvent const & DamageEvent, AController * EventInstigator, AActor * DamageCauser)
 {
	 float damageApplied = CurrentHealth - DamageTaken;
	// GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Orange, FString::Printf(TEXT(" Take Damage")));
	 
	 SetCurrentHealth(damageApplied);
	 
	 return damageApplied;
 }



