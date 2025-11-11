// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "SAction.generated.h"

class AActor;
class UWorld;


/**
 * 
 */
UCLASS(Blueprintable)
class ACTION_ROGUELIKE_API USAction : public UObject
{
  GENERATED_BODY()

public:

  UFUNCTION( BlueprintNativeEvent, Category = "Actions" )
  void StartAction( AActor* Instigator );

  UFUNCTION( BlueprintNativeEvent, Category = "Actions" )
  void StopAction( AActor* Instigator );

  UPROPERTY( EditDefaultsOnly, Category = "Actions" );
  FName ActionName;

  UWorld* GetWorld() const override;
};
