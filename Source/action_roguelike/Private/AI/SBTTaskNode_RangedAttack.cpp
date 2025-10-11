// Fill out your copyright notice in the Description page of Project Settings.

#include "AI/SBTTaskNode_RangedAttack.h"
#include "AIController.h"
#include "GameFramework/Character.h"
#include "BehaviorTree/BlackboardComponent.h"


EBTNodeResult::Type USBTTaskNode_RangedAttack::ExecuteTask( UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory )
{
  auto* MyController{ OwnerComp.GetAIOwner() };
  if( ensure( MyController ) )
  {
    auto* MyPawn{ Cast<ACharacter>( MyController->GetPawn() ) };
    if( nullptr == MyPawn )
    {
      return EBTNodeResult::Failed;
    }

    FVector MuzzleLocation{ MyPawn->GetMesh()->GetSocketLocation( "Muzzle_01" ) };

    auto* TargetActor{ Cast<AActor>( OwnerComp.GetBlackboardComponent()->GetValueAsObject( "TargetActor" ) ) };
    if( TargetActor == nullptr )
    {
      return EBTNodeResult::Failed;
    }

    FVector Direction{ TargetActor->GetActorLocation() - MuzzleLocation };
    FRotator MuzzleRotation = Direction.Rotation();

    FActorSpawnParameters Params;
    Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

    auto NewProj{ GetWorld()->SpawnActor<AActor>( ProjectileClass, MuzzleLocation, MuzzleRotation, Params ) };

    return NewProj ? EBTNodeResult::Succeeded : EBTNodeResult::Failed;
  }

  return EBTNodeResult::Failed;
}
