// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/SlashAnimInstance.h"
#include "Characters/SlashCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "kismet/kismetMathLibrary.h"

void USlashAnimInstance::NativeInitializeAnimation()
{
	Super::NativeUninitializeAnimation();

	this->SlashCharacter = Cast<ASlashCharacter>(this->TryGetPawnOwner());

	if (this->SlashCharacter)
	{
		this->SlashCharacterMovement = this->SlashCharacter->GetCharacterMovement();
	}
	 
}

void USlashAnimInstance::NativeUpdateAnimation(float DeltaTime)
{
	Super::NativeUpdateAnimation(DeltaTime);

	if (this->SlashCharacterMovement)
	{
		this->GroundSpeed = UKismetMathLibrary::VSizeXY(this->SlashCharacterMovement->Velocity);
		this->IsFalling = this->SlashCharacterMovement->IsFalling();
	}
}
