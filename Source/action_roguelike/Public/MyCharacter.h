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

public:
  // Sets default values for this character's properties
  AMyCharacter();

protected:

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
  void BlackHoleAttack();
  void Dash();

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
