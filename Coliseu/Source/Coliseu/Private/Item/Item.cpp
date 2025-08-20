// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/Item.h"
#include "DrawDebugHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
AItem::AItem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	
	//Mesh (3D) of the Bird
	this->ItemMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ItemMesh"));
	this->ItemMesh->SetupAttachment(this->GetRootComponent());
	

}

void AItem::OnSphereOverlap(UPrimitiveComponent* OverlappedComponenent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	const FString OtherActorName = OtherActor->GetName();
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(1, 2.f, FColor::Red, OtherActorName);
	}
}

// Called when the game starts or when spawned
void AItem::BeginPlay()
{
	Super::BeginPlay();

	this->ItemMesh->OnComponentBeginOverlap.AddDynamic(this, &AItem::OnSphereOverlap);
	/*if (GEngine != nullptr)
	{
		GEngine->AddOnScreenDebugMessage(-1, 60.f, FColor::Blue, FString("Hello Unreal 5.7!!!"));
		GEngine->AddOnScreenDebugMessage(-1, 60.f, FColor::Blue, this->inputParameter);
	}	 
	
	UWorld* World = GetWorld();

	if (this->GetActorLabel() == FString("BP_Item"))
	{
		GEngine->AddOnScreenDebugMessage(-1, 60.f, FColor::Green, FString("Origin initialized"));
		TArray<AActor*> actorsList;
		UGameplayStatics::GetAllActorsOfClassWithTag(World, AItem::StaticClass(), FName("BP_Item2"), actorsList);
		if (actorsList.Num() == 1) {
			GEngine->AddOnScreenDebugMessage(-1, 60.f, FColor::Green, FString("Objective set"));
			this->Objective = dynamic_cast<AItem*>(actorsList[0]);
		}
	}
	else
	{
		DrawDebugSphere(GetWorld(), GetActorLocation(), 50, 24, FColor::Blue, true);
	}*/
}

void AItem::test_funct(float DeltaTime) {
	
	if (!this->Objective)
	{
		//GEngine->AddOnScreenDebugMessage(-1, 60.f, FColor::Red, FString("No objective!!!"));
		return;
	}

	if (!GetWorld())
	{
		//GEngine->AddOnScreenDebugMessage(-1, 60.f, FColor::Green, FString("No World!!!"));
		return;
	}
	float speed = 400;
	
	if (FVector::Dist(this->Objective->GetActorLocation(), GetActorLocation()) < DeltaTime * speed)
	{
		GEngine->AddOnScreenDebugMessage(0, 60.f, FColor::Green, FString("Arrived"));
		return;
	}

	DrawDebugSphere(GetWorld(), GetActorLocation(), 50, 24, FColor::Red, false, -1.f);
	DrawDebugLine(GetWorld(), GetActorLocation(), this->Objective->GetActorLocation(), FColor::Red, false, DeltaTime, 0, 5.f);
	
	FVector movement = this->Objective->GetActorLocation() - GetActorLocation();
	movement.Normalize();
	this->AddActorWorldOffset(movement * DeltaTime * speed);
	
}

float AItem::TransformedSin()
{
	return Amplitude * FMath::Sin(RunningTime * TimeConstant);
}

// Called every frame
void AItem::Tick(float DeltaTime)
{
	//Super::Tick(DeltaTime);

	//if (GEngine != nullptr)
	//{
	//	FString message = FString::Printf(TEXT("Delta: %.3f"), DeltaTime);
	//	GEngine->AddOnScreenDebugMessage(1, 60.f, FColor::Green, message);
	//}
	//this->test_funct(DeltaTime);
	if (hover) {
		RunningTime += DeltaTime;
		AddActorWorldOffset(FVector(0.f, 0.f, TransformedSin()));
	}
	

}

