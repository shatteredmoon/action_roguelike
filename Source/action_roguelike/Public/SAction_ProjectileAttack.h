// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SAction.h"
#include "SAction_ProjectileAttack.generated.h"

class UAnimMontage;


/**
 * 
 */
UCLASS()
class ACTION_ROGUELIKE_API USAction_ProjectileAttack : public USAction
{
  GENERATED_BODY()

protected:
  UPROPERTY( EditAnywhere, Category = "Attack" )
  TSubclassOf<AActor> ProjectileClass;

  UPROPERTY( EditDefaultsOnly, Category = "Attack" )
  float AttackAnimDelay;

  UPROPERTY( EditAnywhere, Category = "Attack" )
  UAnimMontage* AttackAnim;

  UFUNCTION()
  void AttackDelay_Elapsed( ACharacter* InstigatorCharacter );

public:

  void StartAction_Implementation( AActor* Instigator ) override;

  USAction_ProjectileAttack();
};
