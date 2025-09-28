// Fill out your copyright notice in the Description page of Project Settings.


#include "MyMagicProjectile.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "SAttributeComponent.h"

// Sets default values
AMyMagicProjectile::AMyMagicProjectile()
{
  // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
  PrimaryActorTick.bCanEverTick = true;

  SphereComp = CreateDefaultSubobject<USphereComponent>( "SphereComp" );
  
  // Method one, setting collision info in code:
  //SphereComp->SetCollisionObjectType( ECC_WorldDynamic );
  //SphereComp->SetCollisionResponseToAllChannels( ECR_Ignore );
  //SphereComp->SetCollisionResponseToChannel( ECC_Pawn, ECR_Overlap );

  // Method two, setting via profile name, defined in Project Settings->Collision->Preset
  // Note that the Projectile Preset was added manually, it's not a default Unreal Preset
  SphereComp->SetCollisionProfileName( "Projectile" );
  SphereComp->OnComponentBeginOverlap.AddDynamic( this, &AMyMagicProjectile::OnActorOverlap );
  RootComponent = SphereComp;

  EffectComp = CreateDefaultSubobject<UParticleSystemComponent>( "EffectComp" );
  EffectComp->SetupAttachment( SphereComp );

  MovementComp = CreateDefaultSubobject<UProjectileMovementComponent>( "MovementComp" );
  MovementComp->InitialSpeed = 1000.f;
  MovementComp->bRotationFollowsVelocity = true;
  MovementComp->bInitialVelocityInLocalSpace = true;
}

void AMyMagicProjectile::OnActorOverlap( UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult )
{
  if( OtherActor && OtherActor != GetInstigator() )
  {
    USAttributeComponent* AttributeComp{ Cast< USAttributeComponent>(OtherActor->GetComponentByClass( USAttributeComponent::StaticClass() ) ) };
    if( AttributeComp )
    {
      AttributeComp->ApplyHealthChange( -20.0f );
      Destroy();
    }
  }
}

// Called when the game starts or when spawned
void AMyMagicProjectile::BeginPlay()
{
  Super::BeginPlay();
}

// Called every frame
void AMyMagicProjectile::Tick( float DeltaTime )
{
  Super::Tick( DeltaTime );
}
