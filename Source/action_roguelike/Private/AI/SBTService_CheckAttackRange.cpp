// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/SBTService_CheckAttackRange.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"

void USBTService_CheckAttackRange::TickNode( UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds )
{
  Super::TickNode( OwnerComp, NodeMemory, DeltaSeconds );

  // Check distance between ai pawn and target actor

  auto* BlackBoardComp{ OwnerComp.GetBlackboardComponent() };
  if( ensure( BlackBoardComp ) )
  {
    const AActor* TargetActor{ Cast<AActor>( BlackBoardComp->GetValueAsObject( "TargetActor" ) ) };
    if( TargetActor )
    {
      const auto* MyController{ OwnerComp.GetAIOwner() };
      if( ensure( MyController ) )
      {
        const APawn* AIPawn{ MyController->GetPawn() };
        if( AIPawn )
        {
          const double DistanceTo{ FVector::Distance( TargetActor->GetActorLocation(), AIPawn->GetActorLocation() ) };
          const bool bWithinRange{ DistanceTo < 2000.0 };
          bool bHasLOS{ false };
          if( bWithinRange )
          {
            bHasLOS = MyController->LineOfSightTo( TargetActor );
          }

          BlackBoardComp->SetValueAsBool( AttackRangeKey.SelectedKeyName, bWithinRange && bHasLOS );
        }
      }
    }
  }
}
