// Fill out your copyright notice in the Description page of Project Settings.

#include "SProjectileBase.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "SAttributeComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ASProjectileBase::ASProjectileBase()
{
  SphereComp = CreateDefaultSubobject<USphereComponent>( "SphereComp" );

  // Method one, setting collision info in code:
  //SphereComp->SetCollisionObjectType( ECC_WorldDynamic );
  //SphereComp->SetCollisionResponseToAllChannels( ECR_Ignore );
  //SphereComp->SetCollisionResponseToChannel( ECC_Pawn, ECR_Overlap );

  // Method two, setting via profile name, defined in Project Settings->Collision->Preset
  // Note that the Projectile Preset was added manually, it's not a default Unreal Preset
  SphereComp->SetCollisionProfileName( "Projectile" );
  SphereComp->OnComponentHit.AddDynamic( this, &ASProjectileBase::OnActorHit );
  RootComponent = SphereComp;

  EffectComp = CreateDefaultSubobject<UParticleSystemComponent>( "EffectComp" );
  EffectComp->SetupAttachment( SphereComp );

  MovementComp = CreateDefaultSubobject<UProjectileMovementComponent>( "MovementComp" );
  MovementComp->bRotationFollowsVelocity = true;
  MovementComp->bInitialVelocityInLocalSpace = true;
  MovementComp->ProjectileGravityScale = 0.0f;
  MovementComp->InitialSpeed = 8000.f;

  SetReplicates( true );
}

void ASProjectileBase::OnActorHit( UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit )
{
  Explode();
}

// Note that the native override requires _Implementation be added to the function name
void ASProjectileBase::Explode_Implementation()
{
  //if( ensure( !IsPendingKill() ) )
  {
    UGameplayStatics::SpawnEmitterAtLocation( this, ImpactVFX, GetActorLocation(), GetActorRotation() );
    Destroy();
  }
}

void ASProjectileBase::PostInitializeComponents()
{
  Super::PostInitializeComponents();
}
