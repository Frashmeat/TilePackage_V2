// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TilePackageComponent.generated.h"


class UItemObject;
class AItemActor;
/*
 *	��Ʒ�ڱ����е���Ϣ
 */
USTRUCT(BlueprintType)
struct FItemStoredInfo
{
	GENERATED_BODY()
	//ͼƬ��Դ
	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	UMaterialInstance* Image_X; 
	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	UMaterialInstance* Image_Y;
	//�洢��Ϣ
	
	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	int MaxOverlapNum; //���洢����
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FIntPoint ItemSize; //��Ʒ�Ĵ洢��С
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FIntPoint StoredLeftTopPoint; //�洢λ�õ����Ͻ�

	FName ItemName; //��Ʒ����
	int Quantity; //�洢����
};

/*
 *	��Ʒ�ڳ����е���Ϣ
 */


USTRUCT(BlueprintType)
struct FItemSceneInfo
{
	GENERATED_BODY()
	//Mesh ��Դ
	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	USkeletalMesh* SkeletalMesh;
	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	UStaticMesh* StaticMesh;
};

USTRUCT(BlueprintType)
struct FMyItemInfo : public FTableRowBase
{
	GENERATED_BODY()
	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	FItemSceneInfo ItemSceneInfo;
	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	FItemStoredInfo ItemStoredInfo;
};



UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TILEPACKAGE_V2_API UTilePackageComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UTilePackageComponent();
	UPROPERTY(BlueprintReadOnly,EditAnywhere,Category = Item)
	FIntPoint PackageSize;
	UPROPERTY(BlueprintReadOnly,EditAnywhere,Category = Item)
	int TileSize;
	UFUNCTION(BlueprintCallable , Category = Item)
	bool HaveSpaceToStoreOnPoint(FItemStoredInfo ItemStoredInfo,FIntPoint LeftTopPoint);
protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	void LogIsStoredMutex();
	UPROPERTY(BlueprintReadWrite,Category = Item)
	TArray<UItemObject*> StoredItems;
	TArray<bool> IsStoredMutex;


	void UpdateIsStoredMutex();
	bool HaveSpaceToStored(const FItemStoredInfo& ItemStoredInfo,FIntPoint& LeftTopPoint);
	void RotateItem(FItemStoredInfo& ItemStoredInfo) const;
	int TurnPointToIndex(FIntPoint Point) const;
	FIntPoint TurnIndexToPoint(int Index) const;
public:	
	// Called every frame
	FItemStoredInfo CreateItemStoredInfo(const AItemActor* ItemActor);
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	bool TryToStoreItem(const AItemActor * ItemActor);
		
};
