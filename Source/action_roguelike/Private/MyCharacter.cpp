// Fill out your copyright notice in the Description page of Project Settings.

#include "MyCharacter.h"

#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"

// Sets default values
AMyCharacter::AMyCharacter()
{
  // Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
  PrimaryActorTick.bCanEverTick = true;

  MySpringArmComp = CreateDefaultSubobject<USpringArmComponent>("MySpringArmComp");
  MySpringArmComp->SetupAttachment( RootComponent );

  MyCameraComp = CreateDefaultSubobject<UCameraComponent>( "MyCameraComp" );
  MyCameraComp->SetupAttachment( MySpringArmComp );
}

// Called when the game starts or when spawned
void AMyCharacter::BeginPlay()
{
  Super::BeginPlay();
}

void AMyCharacter::MoveForward( float Value )
{
  AddMovementInput(GetActorForwardVector(), Value);
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
  PlayerInputComponent->BindAxis( "Turn", this, &APawn::AddControllerYawInput );
}
