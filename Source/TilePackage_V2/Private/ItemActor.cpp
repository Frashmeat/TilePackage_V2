// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemActor.h"

#include "Kismet/KismetMathLibrary.h"

// Sets default values
AItemActor::AItemActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AItemActor::BeginPlay()
{
	Super::BeginPlay();
 	UDataTable * DataTable = LoadObject<UDataTable>(this, TEXT("/Script/Engine.DataTable'/Game/Item/DT_MyItemInfo.DT_MyItemInfo'"));
	if(!DataTable)
	{
		UE_LOG(LogTemp, Warning, TEXT("DataTable Fail to Load"));
		return;
	}
	if(FMyItemInfo * ItemInfo = DataTable->FindRow<FMyItemInfo>(ItemName,nullptr))
	{
		ItemSceneInfo = ItemInfo->ItemSceneInfo;
	}else
	{
		UE_LOG(LogTemp, Warning, TEXT("Fail to Find Row by ItemName : %s"), *ItemName.ToString());
	}

	FTransform Transform = UKismetMathLibrary::MakeTransform(FVector(0.0, 0.0, 0.0), FRotator(0.0, 0.0, 0.0), FVector(1.0, 1.0, 1.0));

	if(ItemSceneInfo.SkeletalMesh)
	{
		UE_LOG(LogTemp, Warning, TEXT("Skeletal Mesh Loading"));
		UActorComponent * AC = AddComponentByClass(USkeletalMeshComponent::StaticClass(), false, Transform, false);
		if(AC)
		{
			AC->Rename(TEXT("SkeletalMeshComponent"));
			USkeletalMeshComponent* SMC = Cast<USkeletalMeshComponent>(AC);
			if(SMC)
			{
				SMC->SetSkeletalMesh(ItemSceneInfo.SkeletalMesh);
				UE_LOG(LogTemp, Warning, TEXT("Skeletal Mesh Component is Setting"))
			}
		}
		
	}
	else if(ItemSceneInfo.StaticMesh)
	{
		UE_LOG(LogTemp, Warning, TEXT("Static Mesh Loading"));
		UActorComponent* AC = AddComponentByClass(USkeletalMeshComponent::StaticClass(), false, Transform, false);
		if (AC)
		{
			AC->Rename(TEXT("SkeletalMeshComponent"));
			UStaticMeshComponent* SMC = Cast<UStaticMeshComponent>(AC);
			if (SMC)
			{
				SMC->SetStaticMesh(ItemSceneInfo.StaticMesh);
				UE_LOG(LogTemp, Warning, TEXT("Skeletal Mesh Component is Setting"))
			}
		}
	}
}

// Called every frame
void AItemActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

