// Fill out your copyright notice in the Description page of Project Settings.


#include "MyMagicProjectile.h"
#include "SAttributeComponent.h"
#include "Components/SphereComponent.h"
#include "SGameplayFunctionLibrary.h"


// Sets default values
AMyMagicProjectile::AMyMagicProjectile()
{
  SphereComp->SetSphereRadius( 20.0f );
  SphereComp->OnComponentBeginOverlap.AddDynamic( this, &AMyMagicProjectile::OnActorOverlap );

  DamageAmount = 20.0f;
}

void AMyMagicProjectile::OnActorOverlap( UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult )
{
  if( OtherActor && OtherActor != GetInstigator() )
  {
    /*USAttributeComponent* AttributeComp{Cast< USAttributeComponent>(OtherActor->GetComponentByClass(USAttributeComponent::StaticClass()))};
    if( AttributeComp )
    {
      AttributeComp->ApplyHealthChange( GetInstigator(), - DamageAmount);
      Explode();
    }*/

    if( USGameplayFunctionLibrary::ApplyDirectionalDamage( GetInstigator(), OtherActor, DamageAmount, SweepResult ) )
    {
      Explode();
    }
  }
}
