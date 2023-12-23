// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TilePackageComponent.h"
#include "GameFramework/Actor.h"
#include "ItemActor.generated.h"

UCLASS()
class TILEPACKAGE_V2_API AItemActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AItemActor();

	UPROPERTY(BlueprintReadOnly,VisibleAnywhere,Category = ItemInfo)
	FItemSceneInfo ItemSceneInfo;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = Item)
	FName ItemName; // 用于查询物品信息
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = Item)
	int Quantity; // 物品在场景中的数量

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	USkeletalMeshComponent* SkeletalMeshComponent;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
