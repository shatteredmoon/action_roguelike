// Fill out your copyright notice in the Description page of Project Settings.


#include "SGameModeBase.h"
#include "EnvironmentQuery/EnvQuerymanager.h"
#include "AI/SAICharacter.h"
#include "SAttributeComponent.h"
#include "EngineUtils.h"


ASGameModeBase::ASGameModeBase()
{
  SpawnTimerInterval = 2.0f;
}


void ASGameModeBase::StartPlay()
{
  Super::StartPlay();

  GetWorldTimerManager().SetTimer( TimerHandle_SpawnBots, this, &ASGameModeBase::SpawnBotTimerElapsed, SpawnTimerInterval, true );
}


void ASGameModeBase::SpawnBotTimerElapsed()
{
  UEnvQueryInstanceBlueprintWrapper* QueryInstance{ UEnvQueryManager::RunEQSQuery( this, SpawnBotQuery, this, EEnvQueryRunMode::RandomBest5Pct, nullptr ) };
  if( ensure( QueryInstance ) )
  {
    QueryInstance->GetOnQueryFinishedEvent().AddDynamic( this, &ASGameModeBase::OnQueryCompleted );
  }
}

void ASGameModeBase::OnQueryCompleted( UEnvQueryInstanceBlueprintWrapper* QueryInstance, EEnvQueryStatus::Type QueryStatus )
{
  if( QueryStatus != EEnvQueryStatus::Success )
  {
    UE_LOG( LogTemp, Warning, TEXT( " Spawn bot EQS Query Failed!" ) );
    return;
  }

  int32 NumAliveBots{ 0 };
  for( TActorIterator<ASAICharacter> Iter( GetWorld() ); Iter; ++Iter )
  {
    ASAICharacter* Bot{ *Iter };
    auto* AttributeComp{ Cast< USAttributeComponent>( Bot->GetComponentByClass( USAttributeComponent::StaticClass() ) ) };
    if( AttributeComp && AttributeComp->IsAlive() )
    {
      ++NumAliveBots;
    }
  }

  /*int32 NumOfAliveBots = 0;
  for( ASAICharacter* Bot : TActorRange<ASAICharacter>( GetWorld() ) )
  {
    USAttributeComponent* AIAttributeComp = USAttributeComponent::GetAttributeComp( Bot );
    if( ensure( AIAttributeComp ) && AIAttributeComp->IsAlive() )
    {
      ++NumOfAliveBots;
    }
  }*/

  float MaxBotCount{ 10.0f };

  if( DifficultyCurve )
  {
    MaxBotCount = DifficultyCurve->GetFloatValue( GetWorld()->TimeSeconds );
  }

  if( NumAliveBots >= MaxBotCount )
  {
    return;
  }

  TArray<FVector> Locations{ QueryInstance->GetResultsAsLocations() };
  if( Locations.IsValidIndex(0) )
  {
    GetWorld()->SpawnActor<AActor>( MinionClass, Locations[0], FRotator::ZeroRotator );
  }
}
