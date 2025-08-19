// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/Weapons/Weapon.h"

void AWeapon::OnSphereOverlap(UPrimitiveComponent* OverlappedComponenent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	const FString OtherActorName = OtherActor->GetName();
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(1, 2.f, FColor::Red, OtherActorName);
	}
}
