// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "SAICharacter.generated.h"

class UPawnSensingComponent;


UCLASS()
class ACTION_ROGUELIKE_API ASAICharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ASAICharacter();

protected:

  UPROPERTY( VisibleAnywhere, Category = "Components" )
  UPawnSensingComponent* PawnSensingComp;

  UFUNCTION()
  void OnPawnSeen( APawn* Pawn );

  void PostInitializeComponents() override;

public:

  // Called to bind functionality to input
  void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
};
