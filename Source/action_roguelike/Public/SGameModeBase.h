// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "EnvironmentQuery/EnvQueryTypes.h"
#include "SGameModeBase.generated.h"

class AController;
class UEnvQuery;
class UEnvQueryInstanceBlueprintWrapper;
class UCurveFloat;


/**
 * 
 */
UCLASS()
class ACTION_ROGUELIKE_API ASGameModeBase : public AGameModeBase
{
  GENERATED_BODY()

protected:

  UPROPERTY( EditDefaultsOnly, Category = "AI" )
  TSubclassOf<AActor> MinionClass;

  UPROPERTY( EditDefaultsOnly, Category = "AI" )
  UEnvQuery* SpawnBotQuery{ nullptr };

  UPROPERTY( EditDefaultsOnly, Category = "AI" )
  UCurveFloat* DifficultyCurve;

  FTimerHandle TimerHandle_SpawnBots;

  UPROPERTY( EditDefaultsOnly, Category = "AI")
  float SpawnTimerInterval{ 2.0f };

  UFUNCTION()
  void SpawnBotTimerElapsed();

  UFUNCTION()
  void OnQueryCompleted( UEnvQueryInstanceBlueprintWrapper* QueryInstance, EEnvQueryStatus::Type QueryStatus);

  UFUNCTION()
  void RespawnPlayerElapsed( AController* Controller );

public:

  ASGameModeBase();

  virtual void OnActorKilled( AActor* VictimActor, AActor* Killer );

  void StartPlay() override;

  UFUNCTION( Exec )
  void KillAll();
};
