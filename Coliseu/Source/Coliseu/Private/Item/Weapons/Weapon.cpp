// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/Weapons/Weapon.h"
#include "Characters/SlashCharacter.h"

void AWeapon::Equip(USceneComponent* InParent, FName InSocketName)
{
	this->hover = false;
	FAttachmentTransformRules TransformRules(EAttachmentRule::SnapToTarget, true);
	this->ItemMesh->AttachToComponent(InParent, TransformRules, InSocketName);
}

void AWeapon::OnSphereOverlap(UPrimitiveComponent* OverlappedComponenent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Super::OnSphereOverlap(OverlappedComponenent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);

	ASlashCharacter* SlashCharacter =  Cast<ASlashCharacter>(OtherActor);

	if (SlashCharacter) {
		SlashCharacter->SetOverlappingItem(this);
		/*this->hover = false;
		FAttachmentTransformRules TransformRules(EAttachmentRule::SnapToTarget, true);
		this->ItemMesh->AttachToComponent(SlashCharacter->GetMesh(), TransformRules, FName("RightHandSocket"));

		SlashCharacter->WeaponEquiped = true;*/
	}
}

void AWeapon::OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	ASlashCharacter* SlashCharacter = Cast<ASlashCharacter>(OtherActor);

	if (SlashCharacter) {
		SlashCharacter->SetOverlappingItem(nullptr);
	}
}
