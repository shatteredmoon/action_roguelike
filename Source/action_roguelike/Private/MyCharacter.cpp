// Fill out your copyright notice in the Description page of Project Settings.

#include "MyCharacter.h"

#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Camera/CameraComponent.h"
#include "SInteractionComponent.h"
#include "SAttributeComponent.h"
#include "SActionComponent.h"


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

  ActionComp = CreateDefaultSubobject<USActionComponent>( "ActionComp" );

  GetCharacterMovement()->bOrientRotationToMovement = true;

  bUseControllerRotationYaw = false;
}

void AMyCharacter::PostInitializeComponents()
{
  Super::PostInitializeComponents();

  // Trigger when health is changed (damage/healing)
  AttributeComp->OnHealthChanged.AddDynamic( this, &AMyCharacter::OnHealthChanged );
}


FVector AMyCharacter::GetPawnViewLocation() const
{
  return CameraComp->GetComponentLocation();
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


void AMyCharacter::SprintStart()
{
  ActionComp->StartActionByName( this, "Sprint" );
}


void AMyCharacter::SprintStop()
{
  ActionComp->StopActionByName( this, "Sprint" );
}


void AMyCharacter::PrimaryAttack()
{
  ActionComp->StartActionByName( this, "PrimaryAttack" );
}


void AMyCharacter::BlackHoleAttack()
{
  ActionComp->StartActionByName( this, "Blackhole" );
  //PlayAnimMontage( AttackAnim );
  //GetWorldTimerManager().SetTimer( TimerHandle_BlackholeAttack, this, &AMyCharacter::BlackholeAttack_TimeElapsed, AttackAnimDelay );
}


void AMyCharacter::Dash()
{
  ActionComp->StartActionByName( this, "Dash" );

  //PlayAnimMontage( AttackAnim );
  //GetWorldTimerManager().SetTimer( TimerHandle_Dash, this, &AMyCharacter::Dash_TimeElapsed, AttackAnimDelay );
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

  PlayerInputComponent->BindAction( "Sprint", IE_Pressed, this, &AMyCharacter::SprintStart );
  PlayerInputComponent->BindAction( "Sprint", IE_Released, this, &AMyCharacter::SprintStop );
}


void AMyCharacter::HealSelf( float Amount /* = 100 */ )
{
  AttributeComp->ApplyHealthChange( this, Amount );
}


void AMyCharacter::OnHealthChanged( AActor* InstigatorActor, USAttributeComponent* OwningComp, float NewHealth, float Delta )
{
  if( NewHealth <= 0.0f && Delta < 0.0f )
  {
    auto PC{ Cast<APlayerController>( GetController() ) };
    DisableInput( PC );

    SetLifeSpan( 5.0f );
  }
}
