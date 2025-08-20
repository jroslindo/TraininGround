// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Item.generated.h"

UCLASS()
class COLISEU_API AItem : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AItem();
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//UPROPERTY(EditDefaultsOnly)
	//UPROPERTY(EditInstanceOnly)
	//UPROPERTY(VisibleDefaultsOnly) and there's instance
	UPROPERTY(EditAnywhere)
	FString inputParameter = "Nothing";

	//Collisions
	UFUNCTION()
	virtual void OnSphereOverlap(UPrimitiveComponent* OverlappedComponenent, 
						AActor* OtherActor,
						UPrimitiveComponent* OtherComp,
						int32 OtherBodyIndex,
						bool bFromSweep,
						const FHitResult& SweepResult
	);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	bool hover = true;

	UFUNCTION(BlueprintCallable)
	void test_funct( float DeltaTime);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly) class UStaticMeshComponent* ItemMesh;
private:
	AItem* Objective;
	

	float TransformedSin();
	float RunningTime;
	float TimeConstant = 2.5f;
	float Amplitude = 1.f;

};
