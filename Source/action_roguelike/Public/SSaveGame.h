// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "SSaveGame.generated.h"


USTRUCT()
struct FActorSaveData
{
  GENERATED_BODY()

public:

  // Actor Identifier
  UPROPERTY()
  FString ActorName;

  // For movable Actors, keep location, rotation, and scale
  UPROPERTY()
  FTransform Transform;

  UPROPERTY()
  TArray<uint8> ByteData;
};


/**
 * 
 */
UCLASS()
class ACTION_ROGUELIKE_API USSaveGame : public USaveGame
{
  GENERATED_BODY()

public:

  UPROPERTY()
  int32 Credits;

  UPROPERTY()
  TArray<FActorSaveData> SavedActors;
};
