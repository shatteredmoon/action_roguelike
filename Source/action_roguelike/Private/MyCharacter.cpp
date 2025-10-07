// Fill out your copyright notice in the Description page of Project Settings.

#include "MyCharacter.h"

#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Camera/CameraComponent.h"
#include "SInteractionComponent.h"
#include "SAttributeComponent.h"

// Sets default values
AMyCharacter::AMyCharacter()
{
  // Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
  PrimaryActorTick.bCanEverTick = true;

  SpringArmComp = CreateDefaultSubobject<USpringArmComponent>("MySpringArmComp");
  SpringArmComp->bUsePawnControlRotation = true;
  SpringArmComp->SetupAttachment( RootComponent );

  CameraComp = CreateDefaultSubobject<UCameraComponent>( "MyCameraComp" );
  CameraComp->SetupAttachment( SpringArmComp );

  InteractionComp = CreateDefaultSubobject<USInteractionComponent>( "InteractionComp" );

  AttributeComp = CreateDefaultSubobject<USAttributeComponent>( "AttributeComp" );

  GetCharacterMovement()->bOrientRotationToMovement = true;

  bUseControllerRotationYaw = false;
  AttackAnimDelay = 0.2f;
}

void AMyCharacter::PostInitializeComponents()
{
  Super::PostInitializeComponents();

  // Trigger when health is changed (damage/healing)
  AttributeComp->OnHealthChanged.AddDynamic( this, &AMyCharacter::OnHealthChanged );
}

void AMyCharacter::MoveForward( float Value )
{
  FRotator ControlRot = GetControlRotation();
  ControlRot.Pitch = 0.f;
  ControlRot.Roll = 0.f;

  AddMovementInput( ControlRot.Vector(), Value);
}

void AMyCharacter::MoveRight( float Value )
{
  FRotator ControlRot = GetControlRotation();
  ControlRot.Pitch = 0.f;
  ControlRot.Roll = 0.f;

  // X = Forward (Red)
  // Y = Right (Green)
  // Z = Up (Blue)

  FVector RightVector{ FRotationMatrix( ControlRot ).GetScaledAxis( EAxis::Y ) };
  AddMovementInput( RightVector, Value );
}

void AMyCharacter::PrimaryAttack()
{
  PlayAnimMontage( AttackAnim );
  GetWorldTimerManager().SetTimer( TimerHandle_PrimaryAttack, this, &AMyCharacter::PrimaryAttack_TimeElapsed, AttackAnimDelay );
}

void AMyCharacter::PrimaryAttack_TimeElapsed()
{
  // if( check( ProjectileClass ) ) // fatal exception
  // if( ensure( ProjectileClass ) ) // only triggers once
  if( ensureAlways( ProjectileClass ) ) // triggers every time
  {
    FVector HandLocation{ GetMesh()->GetSocketLocation( "Muzzle_01" ) };
    FTransform SpawnTM{ FTransform( GetControlRotation(), HandLocation ) };
    FActorSpawnParameters SpawnParams;
    SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
    SpawnParams.Instigator = this;
    GetWorld()->SpawnActor<AActor>( ProjectileClass, SpawnTM, SpawnParams );
  }
}

void AMyCharacter::BlackHoleAttack()
{
  PlayAnimMontage( AttackAnim );

  GetWorldTimerManager().SetTimer( TimerHandle_BlackholeAttack, this, &AMyCharacter::BlackholeAttack_TimeElapsed, AttackAnimDelay );
}


void AMyCharacter::BlackholeAttack_TimeElapsed()
{
  SpawnProjectile( BlackHoleProjectileClass );
}


void AMyCharacter::Dash()
{
  PlayAnimMontage( AttackAnim );

  GetWorldTimerManager().SetTimer( TimerHandle_Dash, this, &AMyCharacter::Dash_TimeElapsed, AttackAnimDelay );
}


void AMyCharacter::Dash_TimeElapsed()
{
  SpawnProjectile( DashProjectileClass );
}

void AMyCharacter::SpawnProjectile( TSubclassOf<AActor> ClassToSpawn )
{
  if( ensureAlways( ClassToSpawn ) )
  {
    FVector HandLocation = GetMesh()->GetSocketLocation( "Muzzle_01" );

    FActorSpawnParameters SpawnParams;
    SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
    SpawnParams.Instigator = this;

    FHitResult Hit;
    FVector TraceStart = CameraComp->GetComponentLocation();
    // endpoint far into the look-at distance (not too far, still adjust somewhat towards crosshair on a miss)
    FVector TraceEnd = CameraComp->GetComponentLocation() + ( GetControlRotation().Vector() * 5000 );

    FCollisionShape Shape;
    Shape.SetSphere( 20.0f );

    // Ignore Player
    FCollisionQueryParams Params;
    Params.AddIgnoredActor( this );

    FCollisionObjectQueryParams ObjParams;
    ObjParams.AddObjectTypesToQuery( ECC_WorldDynamic );
    ObjParams.AddObjectTypesToQuery( ECC_WorldStatic );
    ObjParams.AddObjectTypesToQuery( ECC_Pawn );

    FRotator ProjRotation;
    // true if we got to a blocking hit (Alternative: SweepSingleByChannel with ECC_WorldDynamic)
    if( GetWorld()->SweepSingleByObjectType( Hit, TraceStart, TraceEnd, FQuat::Identity, ObjParams, Shape, Params ) )
    {
      // Adjust location to end up at crosshair look-at
      ProjRotation = FRotationMatrix::MakeFromX( Hit.ImpactPoint - HandLocation ).Rotator();
    }
    else
    {
      // Fall-back since we failed to find any blocking hit
      ProjRotation = FRotationMatrix::MakeFromX( TraceEnd - HandLocation ).Rotator();
    }

    FTransform SpawnTM = FTransform( ProjRotation, HandLocation );
    GetWorld()->SpawnActor<AActor>( ClassToSpawn, SpawnTM, SpawnParams );
  }
}

void AMyCharacter::PrimaryInteract()
{
  InteractionComp->PrimaryInteract();
}

// Called to bind functionality to input
void AMyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
  Super::SetupPlayerInputComponent( PlayerInputComponent );

  PlayerInputComponent->BindAxis( "MoveForward", this, &AMyCharacter::MoveForward );
  PlayerInputComponent->BindAxis( "MoveRight", this, &AMyCharacter::MoveRight );

  PlayerInputComponent->BindAxis( "Turn", this, &APawn::AddControllerYawInput );
  PlayerInputComponent->BindAxis( "LookUp", this, &APawn::AddControllerPitchInput );

  PlayerInputComponent->BindAction( "PrimaryAttack", IE_Pressed, this, &AMyCharacter::PrimaryAttack );
  PlayerInputComponent->BindAction( "Jump", IE_Pressed, this, &ACharacter::Jump );
  PlayerInputComponent->BindAction( "PrimaryInteract", IE_Pressed, this, &AMyCharacter::PrimaryInteract );

  PlayerInputComponent->BindAction( "SecondaryAttack", IE_Pressed, this, &AMyCharacter::BlackHoleAttack );
  PlayerInputComponent->BindAction( "Dash", IE_Pressed, this, &AMyCharacter::Dash );
}

void AMyCharacter::OnHealthChanged( AActor* InstigatorActor, USAttributeComponent* OwningComp, float NewHealth, float Delta )
{
  if( NewHealth <= 0.0f && Delta < 0.0f )
  {
    auto PC{ Cast<APlayerController>( GetController() ) };
    DisableInput( PC );
  }
}
