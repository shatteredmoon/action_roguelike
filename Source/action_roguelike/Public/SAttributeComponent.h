// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SAttributeComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams( FOnHealthChanged, AActor*, InstigatorActor, USAttributeComponent*, OwningComp, float, NewHealth, float, Delta );


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ACTION_ROGUELIKE_API USAttributeComponent : public UActorComponent
{
  GENERATED_BODY()

public:
  // Sets default values for this component's properties
  USAttributeComponent();

  UFUNCTION( BlueprintCallable, Category = "Attributes" )
  static USAttributeComponent* GetAttributes( AActor* FromActor );

  UFUNCTION( BlueprintCallable, Category = "Attributes", meta = (DisplayName = "IsAlive" ))
  static bool IsActorAlive(AActor* Actor);

protected:

  UPROPERTY( EditDefaultsOnly, BlueprintReadOnly, Replicated, Category = "Attributes" )
  float Health;

  UPROPERTY( EditDefaultsOnly, BlueprintReadOnly, Replicated, Category = "Attributes" )
  float HealthMax;

  UFUNCTION( NetMulticast, Reliable ) // @FIXEME: mark as unreliable once we moved the state out of SCharacter
  void MulticastHealthChanged( AActor* InstigatorActor, float NewHealth, float Delta );

public:

  UFUNCTION( BlueprintCallable )
  bool Kill( AActor* InstigatorActor );

  UFUNCTION( BlueprintCallable )
  bool IsAlive() const;

  UFUNCTION( BlueprintCallable )
  bool IsFullHealth() const;

  UFUNCTION( BlueprintCallable )
  float GetHealthMax() const;

  UPROPERTY( BlueprintAssignable )
  FOnHealthChanged OnHealthChanged;

  UFUNCTION( BlueprintCallable, Category = "Attributes" )
  bool ApplyHealthChange( AActor* InstigatorActor, float Delta );
};
