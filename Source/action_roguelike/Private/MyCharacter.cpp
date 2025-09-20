// Fill out your copyright notice in the Description page of Project Settings.

#include "MyCharacter.h"

#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Camera/CameraComponent.h"

// Sets default values
AMyCharacter::AMyCharacter()
{
  // Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
  PrimaryActorTick.bCanEverTick = true;

  MySpringArmComp = CreateDefaultSubobject<USpringArmComponent>("MySpringArmComp");
  MySpringArmComp->bUsePawnControlRotation = true;
  MySpringArmComp->SetupAttachment( RootComponent );

  MyCameraComp = CreateDefaultSubobject<UCameraComponent>( "MyCameraComp" );
  MyCameraComp->SetupAttachment( MySpringArmComp );

  GetCharacterMovement()->bOrientRotationToMovement = true;

  bUseControllerRotationYaw = false;
}

// Called when the game starts or when spawned
void AMyCharacter::BeginPlay()
{
  Super::BeginPlay();
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
  FVector HandLocation{ GetMesh()->GetSocketLocation( "Muzzle_01" ) };
  FTransform SpawnTM{ FTransform(GetControlRotation(), HandLocation) };
  FActorSpawnParameters SpawnParams;
  SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
  GetWorld()->SpawnActor<AActor>( ProjectileClass, SpawnTM, SpawnParams );
}

// Called every frame
void AMyCharacter::Tick(float DeltaTime)
{
  Super::Tick( DeltaTime );
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
}
