// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item/Item.h"
#include "Weapon.generated.h"

/**
 * 
 */
UCLASS()
class COLISEU_API AWeapon : public AItem
{
	GENERATED_BODY()

	virtual void OnSphereOverlap(UPrimitiveComponent* OverlappedComponenent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult
	) override;
	
};
