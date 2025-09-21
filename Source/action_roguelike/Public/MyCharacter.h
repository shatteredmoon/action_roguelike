// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MyCharacter.generated.h"

class UCameraComponent;
class USpringArmComponent;
class USInteractionComponent;
class UAnimMontage;

UCLASS()
class ACTION_ROGUELIKE_API AMyCharacter : public ACharacter
{
  GENERATED_BODY()

protected:
  UPROPERTY( EditAnywhere, Category = "Attack" )
  TSubclassOf<AActor> ProjectileClass;

  UPROPERTY( EditAnywhere, Category = "Attack" )
  UAnimMontage* AttackAnim;

  FTimerHandle TimerHandle_PrimaryAttack;

public:
  // Sets default values for this character's properties
  AMyCharacter();

protected:

  UPROPERTY(VisibleAnywhere)
  USpringArmComponent* MySpringArmComp;

  UPROPERTY( VisibleAnywhere )
  UCameraComponent* MyCameraComp;

  UPROPERTY( VisibleAnywhere )
  USInteractionComponent* InteractionComp;

  // Called when the game starts or when spawned
  virtual void BeginPlay() override;

  void MoveForward( float Value );
  void MoveRight( float Value );

  void Jump();

  void PrimaryAttack();
  void PrimaryAttack_TimeElapsed();

  void PrimaryInteract();

public:
  // Called every frame
  virtual void Tick(float DeltaTime) override;

  // Called to bind functionality to input
  virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
};
