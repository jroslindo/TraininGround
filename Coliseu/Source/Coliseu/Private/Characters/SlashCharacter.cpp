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
#include "GameFramework/CharacterMovementComponent.h"
#include "GroomComponent.h"
#include "Item/Item.h"
#include "Item/Weapons/Weapon.h"
#include "Animation/AnimMontage.h"


// Sets default values
ASlashCharacter::ASlashCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	this->GetCharacterMovement()->bOrientRotationToMovement = true;
	this->GetCharacterMovement()->RotationRate = FRotator(0,360,0);

	//SpringArm
	this->SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	this->SpringArm->SetupAttachment(this->GetRootComponent());
	this->SpringArm->TargetArmLength = 250.f;
	this->SpringArm->SetRelativeRotation(FRotator(-30.f, 0.f, 0.f));
	this->SpringArm->SetRelativeLocation(FVector(0.f, 0.f, 70));

	//Camera
	this->ViewCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	this->ViewCamera->SetupAttachment(this->SpringArm);

	//Make our target of the controller the bird
	this->AutoPossessPlayer = EAutoReceiveInput::Player0;

	this->Hair = CreateDefaultSubobject<UGroomComponent>(TEXT("Hair"));
	this->Hair->SetupAttachment(this->GetMesh());
	this->Hair->AttachmentName = FString("head");


	this->Eyebrows = CreateDefaultSubobject<UGroomComponent>(TEXT("Eyebrows"));
	this->Eyebrows->SetupAttachment(this->GetMesh());
	this->Eyebrows->AttachmentName = FString("head");

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
	if (this->ActionState != EActionState::EAS_Unoccupied) return;


	const FVector2D MovementVector = Value.Get<FVector2D>();

	const FRotator ControlRotation = this->GetControlRotation();
	const FRotator YawRotation(0.f, ControlRotation.Yaw, 0.f);

	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	this->AddMovementInput(ForwardDirection, MovementVector.Y);
	this->AddMovementInput(RightDirection, MovementVector.X);
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

void ASlashCharacter::KeyPressed(const FInputActionValue& Value)
{
	AWeapon* OverlappingWeapon = Cast<AWeapon>(OverlappingItem);
	if (OverlappingWeapon)
	{
		OverlappingWeapon->Equip(GetMesh(), FName("RightHandSocket"));
		CharacterState = ECharacterState::ECS_EquippedOneHandedWeapon;
		OverlappingItem = nullptr;
		EquippedWeapon = OverlappingWeapon;
	}
	else
	{
		if (CanDisarm())
		{
			this->ActionState = EActionState::EAS_EquippingWeapon;
			PlayEquipMontage(FName("Unequip"));
			CharacterState = ECharacterState::ECS_Unequipped;
		}
		else if (CanArm())
		{
			this->ActionState = EActionState::EAS_EquippingWeapon;
			PlayEquipMontage(FName("Equip"));
			CharacterState = ECharacterState::ECS_EquippedOneHandedWeapon;
		}
	}

}

void ASlashCharacter::Attack(const FInputActionValue& Value)
{
	if (this->CharacterState == ECharacterState::ECS_Unequipped)
		return;

	if (this->ActionState == EActionState::EAS_Unoccupied)
	{
		this->PlayAttackMontage();
		this->ActionState = EActionState::EAS_Attacking;
	}
}

void ASlashCharacter::PlayEquipMontage(FName SectionName)
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && EquipMontage)
	{
		AnimInstance->Montage_Play(EquipMontage);
		AnimInstance->Montage_JumpToSection(SectionName, EquipMontage);
	}
}

bool ASlashCharacter::CanDisarm()
{
	return ActionState == EActionState::EAS_Unoccupied &&
		CharacterState != ECharacterState::ECS_Unequipped;
}

bool ASlashCharacter::CanArm()
{
	return ActionState == EActionState::EAS_Unoccupied &&
		CharacterState == ECharacterState::ECS_Unequipped &&
		EquippedWeapon;
}

void ASlashCharacter::Disarm()
{
	if (this->EquippedWeapon)
	{
		this->EquippedWeapon->AttachMeshToSocket(this->GetMesh(), FName("SpineSocket"));
	}
}

void ASlashCharacter::Arm()
{
	if (this->EquippedWeapon)
	{
		this->EquippedWeapon->AttachMeshToSocket(this->GetMesh(), FName("RightHandSocket"));
	}
}

void ASlashCharacter::PlayAttackMontage()
{
	UAnimInstance* AnimInstance = this->GetMesh()->GetAnimInstance();
	
	if (!AnimInstance)
	{
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(0, 2.f, FColor::Red, FString("AnimInstance Null!!!!!"));
		}
		return;
	}

	if (this->AttackMontage)
	{
		AnimInstance->Montage_Play(this->AttackMontage);
		const int32 Selection = FMath::RandRange(0, 1);
		FName SectionName = FName();
		switch (Selection)
		{
		case 0:
			SectionName = FName("Attack1");
			break;
		case 1:
			SectionName = FName("Attack2");
			break;
		default:
			break;
		}
		AnimInstance->Montage_JumpToSection(SectionName, this->AttackMontage);
	}
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
		EnchancedInputComponent->BindAction(this->EKeyPressed, ETriggerEvent::Started, this, &ASlashCharacter::KeyPressed);
		EnchancedInputComponent->BindAction(this->AttackAction, ETriggerEvent::Triggered, this, &ASlashCharacter::Attack);
	}

}

