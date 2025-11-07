// Fill out your copyright notice in the Description page of Project Settings.

#include "SAttributeComponent.h"
#include "SGameModeBase.h"


// Sets default values for this component's properties
USAttributeComponent::USAttributeComponent()
{
  Health = 100;
  HealthMax = 100;
  Health = HealthMax;
}


USAttributeComponent* USAttributeComponent::GetAttributes( AActor* FromActor )
{
  if( FromActor )
  {
    return FromActor->FindComponentByClass<USAttributeComponent>();
  }

  return nullptr;
}


bool USAttributeComponent::IsActorAlive( AActor* Actor )
{
  USAttributeComponent* AttributeComp{ GetAttributes( Actor ) };
  return AttributeComp ? AttributeComp->IsAlive() : false;
}


bool USAttributeComponent::IsFullHealth() const
{
  return Health == HealthMax;
}


float USAttributeComponent::GetHealthMax() const
{
  return HealthMax;
}


bool USAttributeComponent::IsAlive() const
{
  return Health > 0.0f;
}


bool USAttributeComponent::ApplyHealthChange( AActor* InstigatorActor, float Delta )
{
  if( GetOwner()->CanBeDamaged() )
  {
    return false;
  }

  float OldHealth = Health;

  Health = FMath::Clamp( Health + Delta, 0.0f, HealthMax );

  float ActualDelta = Health - OldHealth;
  OnHealthChanged.Broadcast( InstigatorActor, this, Health, ActualDelta );

  // Died
  if( ActualDelta < 0.0f && Health == 0.0f )
  {
    if( ASGameModeBase* GM{ ( GetWorld()->GetAuthGameMode<ASGameModeBase>() ) } )
    {
      GM->OnActorKilled( GetOwner(), InstigatorActor );
    }
  }

  return ActualDelta != 0;
}


bool USAttributeComponent::Kill( AActor* InstigatorActor )
{
  return ApplyHealthChange( InstigatorActor, -GetHealthMax() );
}
