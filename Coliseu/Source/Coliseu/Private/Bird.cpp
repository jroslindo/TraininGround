// Fill out your copyright notice in the Description page of Project Settings.


#include "Bird.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/InputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputAction.h"
#include "EnhancedInputComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"


// Sets default values
ABird::ABird()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//Capsule to detect collision
	this->Capsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule"));
	this->Capsule->SetCapsuleRadius(15.f);
	this->Capsule->SetCapsuleHalfHeight(20.f);	
	this->SetRootComponent(this->Capsule);

	//Mesh (3D) of the Bird
	this->BirdMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("BirdMesh"));
	this->BirdMesh->SetupAttachment(this->GetRootComponent());

	//SpringArm
	this->SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	this->SpringArm->SetupAttachment(this->GetRootComponent());
	this->SpringArm->TargetArmLength = 200.f;
	this->SpringArm->SetRelativeRotation(FRotator(-10.f,0.f, 0.f));

	//Camera
	this->ViewCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	this->ViewCamera->SetupAttachment(this->SpringArm);



	//Make our target of the controller the bird
	//this->AutoPossessPlayer = EAutoReceiveInput::Player1;
}

// Called when the game starts or when spawned
void ABird::BeginPlay()
{
	Super::BeginPlay();
	
	

	if (APlayerController* PlayerController = Cast<APlayerController>(this->GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(this->BirdMappingContext, 0);
		}
	}
}

void ABird::Move(const FInputActionValue& Value)
{
	const float DirectionValue = Value.Get<float>();
	if (this->GetController() && DirectionValue != 0.f)
	{
		UE_LOG(LogTemp, Warning, TEXT("IA_Move triggered"));
		FVector Forward = GetActorForwardVector();
		AddMovementInput(Forward, DirectionValue);
	}	
}

void ABird::Look(const FInputActionValue& Value)
{
	const FVector2D DirectionValue = Value.Get<FVector2D>();

	if (this->GetController())
	{
		this->AddControllerYawInput(DirectionValue.X);
		this->AddControllerPitchInput(DirectionValue.Y);
	}
}

// Called every frame
void ABird::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ABird::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
	//PlayerInputComponent->BindAxis(FName("MoveFoward"), this, &ABird::MoveForward);
	if (UEnhancedInputComponent* EnchancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnchancedInputComponent->BindAction(this->MoveAction, ETriggerEvent::Triggered, this, &ABird::Move);
		EnchancedInputComponent->BindAction(this->LookAction, ETriggerEvent::Triggered, this, &ABird::Look);
	}
}

