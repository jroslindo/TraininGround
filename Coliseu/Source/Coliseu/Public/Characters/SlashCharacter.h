// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "CharacterTypes.h"
#include "SlashCharacter.generated.h"

class UInputMappingContext;
class UInputAction;
class USpringArmComponent;
class UCameraComponent;
class UGroomComponent;
class AItem;
class UAnimMontage;

UCLASS()
class COLISEU_API ASlashCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ASlashCharacter();
	FORCEINLINE ECharacterState GetCharacterState() const { return this->CharacterState; }
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	bool WeaponEquiped = false;

public:
	FORCEINLINE void SetOverlappingItem(AItem* Item) { OverlappingItem = Item; }

protected:
	virtual void BeginPlay() override;
	UPROPERTY(EditAnywhere, Category = Input)
	UInputMappingContext* SlashContext;	

	UPROPERTY(EditAnywhere, Category = Input)
	UInputAction* MoveAction;

	UPROPERTY(EditAnywhere, Category = Input)
	UInputAction* LookAction;

	UPROPERTY(EditAnywhere, Category = Input)
	UInputAction* JumpAction;

	UPROPERTY(EditAnywhere, Category = Input)
	UInputAction* EKeyPressed;

	UPROPERTY(EditAnywhere, Category = Input)
	UInputAction* AttackAction;

	UPROPERTY(EditAnywhere, Category = "Hair")
	UGroomComponent* Hair;

	UPROPERTY(EditAnywhere, Category = "Hair")
	UGroomComponent* Eyebrows;

	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAcess = "true"))
	EActionState ActionState = EActionState::EAS_Unoccupied;
	

	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void Jump(const FInputActionValue& Value);
	void KeyPressed(const FInputActionValue& Value);
	void Attack(const FInputActionValue& Value);

private:
	ECharacterState CharacterState = ECharacterState::ECS_Unequipped;
	

	UPROPERTY(VisibleInstanceOnly)
	AItem* OverlappingItem;

	UPROPERTY(VisibleAnywhere) 
	USpringArmComponent* SpringArm;
	UPROPERTY(VisibleAnywhere) 
	UCameraComponent* ViewCamera;

private:
	/**
	* Animation Montages Section
	*/

	void PlayAttackMontage();

	UPROPERTY(EditDefaultsOnly, Category = Montages)
	UAnimMontage* AttackMontage;
};