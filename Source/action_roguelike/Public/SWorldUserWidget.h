// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SWorldUserWidget.generated.h"

class AActor;
class USizeBox;

/**
 * 
 */
UCLASS()
class ACTION_ROGUELIKE_API USWorldUserWidget : public UUserWidget
{
  GENERATED_BODY()

protected:

  void NativeTick( const FGeometry& MyGeometry, float InDeltaTime );

  UPROPERTY( meta = ( BindWidget ) )
  USizeBox* ParentSizeBox;

public:

  UPROPERTY( EditAnywhere, Category = "UI" )
  FVector WorldOffset;

  UPROPERTY( BlueprintReadWrite, Category = "UI", meta = ( ExposeOnSpawn = true ) )
  AActor* AttachedActor;
};
