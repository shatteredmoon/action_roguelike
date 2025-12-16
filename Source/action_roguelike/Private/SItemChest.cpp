// Fill out your copyright notice in the Description page of Project Settings.

#include "SItemChest.h"
#include "Net/UnrealNetwork.h"


// Sets default values
ASItemChest::ASItemChest()
{
  BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>( TEXT( "BaseMesh" ) );
  RootComponent = BaseMesh;

  LidMesh = CreateDefaultSubobject<UStaticMeshComponent>( TEXT( "LidMesh" ) );
  LidMesh->SetupAttachment( BaseMesh );

  TargetPitch = 110.f;

  bReplicates = true;
}


void ASItemChest::Interact_Implementation( APawn* instigatorPawn )
{
  bLidOpened = !bLidOpened;
  OnRep_LidOpened();
}


void ASItemChest::OnActorLoaded_Implementation()
{
  OnRep_LidOpened();
}


void ASItemChest::OnRep_LidOpened()
{
  float CurrPitch = bLidOpened ? TargetPitch : 0.0f;
  LidMesh->SetRelativeRotation( FRotator( TargetPitch, 0, 0 ) );
}


void ASItemChest::GetLifetimeReplicatedProps( TArray<FLifetimeProperty>& OutLifetimeProps ) const
{
  Super::GetLifetimeReplicatedProps( OutLifetimeProps );

  // Send to all clients
  DOREPLIFETIME( ASItemChest, bLidOpened );
}
