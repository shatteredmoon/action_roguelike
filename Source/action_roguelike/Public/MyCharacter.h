// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MyCharacter.generated.h"

class UCameraComponent;
class USpringArmComponent;
class USInteractionComponent;
class UAnimMontage;
class USAttributeComponent;
class USActionComponent;


UCLASS()
class ACTION_ROGUELIKE_API AMyCharacter : public ACharacter
{
  GENERATED_BODY()

protected:
  UPROPERTY( EditAnywhere, Category = "Attack" )
  TSubclassOf<AActor> ProjectileClass;

  UPROPERTY( EditAnywhere, Category = "Attack" )
  TSubclassOf<AActor> BlackHoleProjectileClass;

  UPROPERTY( EditAnywhere, Category = "Attack" )
  TSubclassOf<AActor> DashProjectileClass;

  UPROPERTY( EditAnywhere, Category = "Attack" )
  UAnimMontage* AttackAnim;

  FTimerHandle TimerHandle_PrimaryAttack;
  FTimerHandle TimerHandle_BlackholeAttack;
  FTimerHandle TimerHandle_Dash;

public:
  // Sets default values for this character's properties
  AMyCharacter();

protected:

  UPROPERTY( EditDefaultsOnly, Category = "Attack" )
  float AttackAnimDelay;

  UPROPERTY(VisibleAnywhere)
  USpringArmComponent* SpringArmComp;

  UPROPERTY( VisibleAnywhere )
  UCameraComponent* CameraComp;

  UPROPERTY( VisibleAnywhere )
  USInteractionComponent* InteractionComp;

  UPROPERTY( VisibleAnywhere, BlueprintReadOnly, Category = "Components")
  USAttributeComponent* AttributeComp;

  UPROPERTY( VisibleAnywhere, BlueprintReadOnly, Category = "Components" )
  USActionComponent* ActionComp;

  void MoveForward( float Value );
  void MoveRight( float Value );

  void SprintStart();
  void SprintStop();

  void PrimaryAttack();
  void PrimaryAttack_TimeElapsed();

  void BlackHoleAttack();

  void BlackholeAttack_TimeElapsed();

  void Dash();

  void Dash_TimeElapsed();

  // Re-use spawn logic between attacks
  void SpawnProjectile( TSubclassOf<AActor> ClassToSpawn );

  void PrimaryInteract();

  UFUNCTION()
  void OnHealthChanged( AActor* InstigatorActor, USAttributeComponent* OwningComp, float NewHealth, float Delta );

  void PostInitializeComponents() override;

  FVector GetPawnViewLocation() const override;

public:

  // Called to bind functionality to input
  void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

  UFUNCTION(Exec)
  void HealSelf( float Amount = 100 );
};
