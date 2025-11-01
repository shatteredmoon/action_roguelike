// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SGameplayInterface.h"
#include "SItemChest.generated.h"


UCLASS()
class ACTION_ROGUELIKE_API ASItemChest : public AActor, public ISGameplayInterface
{
  GENERATED_BODY()

  void Interact_Implementation( APawn* instigatorPawn );

public:

  UPROPERTY( EditAnywhere )
  float TargetPitch;

protected:

  UPROPERTY( VisibleAnywhere )
  UStaticMeshComponent* BaseMesh;

  UPROPERTY( VisibleAnywhere, BlueprintReadOnly )
  UStaticMeshComponent* LidMesh;

public:
  // Sets default values for this actor's properties
  ASItemChest();
};
