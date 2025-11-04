// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/SAICharacter.h"
#include "Perception/PawnSensingComponent.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "DrawDebugHelpers.h"
#include "SAttributeComponent.h"
#include "BrainComponent.h"
#include "SWorldUserWidget.h"


// Sets default values
ASAICharacter::ASAICharacter()
{
  PawnSensingComp = CreateDefaultSubobject<UPawnSensingComponent>( "PawnSensingComp" );
  AttributeComp = CreateDefaultSubobject<USAttributeComponent>( "AttriuteComp" );

  AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

  TimeToHitParamName = "TimeToHit";
}


void ASAICharacter::PostInitializeComponents()
{
  Super::PostInitializeComponents();

  PawnSensingComp->OnSeePawn.AddDynamic( this, &ASAICharacter::OnPawnSeen );
  AttributeComp->OnHealthChanged.AddDynamic( this, &ASAICharacter::OnHealthChanged );
}


void ASAICharacter::OnHealthChanged( AActor* InstigatorActor, USAttributeComponent* OwningComp, float NewHealth, float Delta )\
{
  if( Delta < 0.0f )
  {
    if( InstigatorActor != this )
    {
      SetTargetActor( InstigatorActor );
    }

    if( ActiveHealthBar == nullptr )
    {
      ActiveHealthBar = CreateWidget<USWorldUserWidget>( GetWorld(), HealthBarWidgetClass);
      if( ActiveHealthBar )
      {
        ActiveHealthBar->AttachedActor = this;
        ActiveHealthBar->AddToViewport();
      }
    }

    GetMesh()->SetScalarParameterValueOnMaterials( TimeToHitParamName, GetWorld()->TimeSeconds );

    if( NewHealth <= 0.0f )
    {
      // Stop behavior tree
      AAIController* AIC{ Cast<AAIController>( GetController() ) };
      if( AIC )
      {
        AIC->GetBrainComponent()->StopLogic( "Killed" );
      }

      // Ragdoll
      GetMesh()->SetAllBodiesSimulatePhysics( true );
      GetMesh()->SetCollisionProfileName( "Ragdoll" );

      // Set lifespan (before cleanup)
      SetLifeSpan( 10.0f );
    }
  }
}


void ASAICharacter::OnPawnSeen( APawn* Pawn )
{
  SetTargetActor( Pawn );
  DrawDebugString( GetWorld(), GetActorLocation(), "PLAYER SPOTTED", nullptr, FColor::White, 4.0f, true );
}


void ASAICharacter::SetTargetActor( AActor* NewTarget )
{
  AAIController* AIC{ Cast<AAIController>( GetController() ) };
  if( AIC )
  {
    AIC->GetBlackboardComponent()->SetValueAsObject( "TargetActor", NewTarget );
  }
}


// Called to bind functionality to input
void ASAICharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
  Super::SetupPlayerInputComponent( PlayerInputComponent );
}
