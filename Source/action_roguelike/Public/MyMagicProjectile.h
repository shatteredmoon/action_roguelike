// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SProjectileBase.h"
#include "MyMagicProjectile.generated.h"

UCLASS()
class ACTION_ROGUELIKE_API AMyMagicProjectile : public ASProjectileBase
{
  GENERATED_BODY()

public:
  // Sets default values for this actor's properties
  AMyMagicProjectile();

protected:

  UPROPERTY( EditDefaultsOnly, Category = "Damage" )
  float DamageAmount;

  UFUNCTION()
  void OnActorOverlap( UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult );
};
