// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/SlashCharacter.h"

#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/InputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputAction.h"
#include "EnhancedInputComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"


// Sets default values
ASlashCharacter::ASlashCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//SpringArm
	this->SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	this->SpringArm->SetupAttachment(this->GetRootComponent());
	this->SpringArm->TargetArmLength = 200.f;
	this->SpringArm->SetRelativeRotation(FRotator(-30.f, 0.f, 0.f));
	this->SpringArm->SetRelativeLocation(FVector(0.f, 0.f, 70));

	//Camera
	this->ViewCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	this->ViewCamera->SetupAttachment(this->SpringArm);

	//Make our target of the controller the bird
	this->AutoPossessPlayer = EAutoReceiveInput::Player0;

}

// Called when the game starts or when spawned
void ASlashCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	if (APlayerController* PlayerController = Cast<APlayerController>(this->GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(this->SlashContext, 0);
		}
	}
}

void ASlashCharacter::Move(const FInputActionValue& Value)
{
	const FVector2D MovementVector = Value.Get<FVector2D>();

	this->AddMovementInput(GetActorForwardVector(), MovementVector.Y);
	this->AddMovementInput(GetActorRightVector(), MovementVector.X);
}

void ASlashCharacter::Look(const FInputActionValue& Value)
{
	const FVector2D DirectionValue = Value.Get<FVector2D>();

	if (this->GetController())
	{
		this->AddControllerYawInput(DirectionValue.X);
		this->AddControllerPitchInput(DirectionValue.Y);
	}
}

void ASlashCharacter::Jump(const FInputActionValue& Value)
{
	const bool Jump = Value.Get<bool>();

	if (Jump)
	{
		// Call the built-in jump function from ACharacter
		ACharacter::Jump();
	}
	else
	{
		// Stop jumping when input is released
		StopJumping();
	}

}

// Called every frame
void ASlashCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ASlashCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	//PlayerInputComponent->BindAxis(FName("MoveFoward"), this, &ABird::MoveForward);
	if (UEnhancedInputComponent* EnchancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnchancedInputComponent->BindAction(this->MoveAction, ETriggerEvent::Triggered, this, &ASlashCharacter::Move);
		EnchancedInputComponent->BindAction(this->LookAction, ETriggerEvent::Triggered, this, &ASlashCharacter::Look);
		EnchancedInputComponent->BindAction(this->JumpAction, ETriggerEvent::Triggered, this, &ASlashCharacter::Jump);
	}

}

