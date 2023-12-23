// Fill out your copyright notice in the Description page of Project Settings.


#include "TilePackageComponent.h"
#include "ItemActor.h"
#include "ItemObject.h"

#define IsStoredMutexLog

// Sets default values for this component's properties
UTilePackageComponent::UTilePackageComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...


}


bool UTilePackageComponent::HaveSpaceToStoreOnPoint(FItemStoredInfo ItemStoredInfo,FIntPoint LeftTopPoint)
{
	bool HaveSpace = true;
	int j, k;
	for (j = 0; j < ItemStoredInfo.ItemSize.X; ++j)
	{
		int RealX = LeftTopPoint.X + j;
		if (RealX >= PackageSize.X)
		{
			HaveSpace = false;
			break;
		}
		for (k = 0; k < ItemStoredInfo.ItemSize.Y; ++k)
		{
			int RealY = LeftTopPoint.Y + k;
			if (RealY >= PackageSize.Y)
			{
				HaveSpace = false;
				break;
			}
			FIntPoint RealPoint = { RealX, RealY };
			int RealIndex = TurnPointToIndex(RealPoint);
			UE_LOG(LogTemp, Warning, TEXT("Real index : %d"), RealIndex);
			if (RealIndex < 0 && RealIndex >= IsStoredMutex.Num())
			{
				HaveSpace = false;
				break;
			}
			if (IsStoredMutex[RealIndex])
			{
				HaveSpace = false;
				break;
			}
		}
		if (!HaveSpace) break;
	}
	if (HaveSpace && j == ItemStoredInfo.ItemSize.X && k == ItemStoredInfo.ItemSize.Y)
	{
		return true;
	}
	return false;
}

// Called when the game starts
void UTilePackageComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	IsStoredMutex.SetNum(PackageSize.X * PackageSize.Y);
	for (bool& bIsStored : IsStoredMutex)
	{
		bIsStored = false;
	}
#ifdef IsStoredMutexLog
	LogIsStoredMutex();
#endif
}

void UTilePackageComponent::LogIsStoredMutex()
{
	FString str;
	str.Append("\n\n");
	for (int i = 0; i < IsStoredMutex.Num(); ++i)
	{
		str.AppendInt(IsStoredMutex[i]);
		str.Append(" ");
		if(i%PackageSize.X == PackageSize.X-1)
		{
			str.Append("\n");
		}
	}
	UE_LOG(LogTemp, Warning, TEXT("%s"), *str);
}

void UTilePackageComponent::UpdateIsStoredMutex()
{
	for (UItemObject* ItemObject : StoredItems)
	{
		FItemStoredInfo& StoredItem = ItemObject->ItemStoredInfo;
		FIntPoint leftTopPoint = StoredItem.StoredLeftTopPoint;
		for (int i = 0; i < StoredItem.ItemSize.X; ++i)
		{
			for (int j = 0; j < StoredItem.ItemSize.Y; ++j)
			{
				FIntPoint RealPoint = { leftTopPoint.X + i , leftTopPoint.Y + j};
				int RealIndex = TurnPointToIndex(RealPoint);
				if(RealIndex>= 0 && RealIndex< IsStoredMutex.Num())
				IsStoredMutex[RealIndex] = true;
			}
		}
	}
#ifdef IsStoredMutexLog
	LogIsStoredMutex();
#endif
}

bool UTilePackageComponent::HaveSpaceToStored(const FItemStoredInfo& ItemStoredInfo,FIntPoint& LeftTopPoint)
{
	for (int i = 0; i < IsStoredMutex.Num(); ++i)
	{
		FIntPoint leftTop = TurnIndexToPoint(i);
		UE_LOG(LogTemp, Warning, TEXT("Current Point X : %d ,Y : %d"), leftTop.X, leftTop.Y);
		bool HaveSpace = true;
		int j , k;
		UE_LOG(LogTemp, Warning, TEXT("ItemStoredInfo ItemSize X : %d ,Y : %d"), ItemStoredInfo.ItemSize.X, ItemStoredInfo.ItemSize.Y);
		for ( j = 0; j < ItemStoredInfo.ItemSize.X; ++j)
		{
			int RealX = leftTop.X + j;
			if(RealX >= PackageSize.X)
			{
				HaveSpace = false;
				break;
			}
			for ( k = 0; k < ItemStoredInfo.ItemSize.Y; ++k)
			{
				int RealY = leftTop.Y + k;
				if(RealY >= PackageSize.Y)
				{
					HaveSpace = false;
					break;
				}
				FIntPoint RealPoint = { RealX, RealY };
				int RealIndex = TurnPointToIndex(RealPoint);
				UE_LOG(LogTemp, Warning, TEXT("Real index : %d"), RealIndex);
				if(RealIndex < 0 && RealIndex >= IsStoredMutex.Num())
				{
					HaveSpace = false;
					break;
				}
				if(IsStoredMutex[RealIndex])
				{
					HaveSpace = false;
					break;
				}
			}
			if (!HaveSpace) break;
		}
		UE_LOG(LogTemp, Warning, TEXT("Have Space : %d"), HaveSpace);
		if(HaveSpace && j == ItemStoredInfo.ItemSize.X  && k == ItemStoredInfo.ItemSize.Y )
		{
			LeftTopPoint = leftTop;
			return true;
		}
	}
	return false;
}

void UTilePackageComponent::RotateItem(FItemStoredInfo& ItemStoredInfo) const
{
	UMaterialInstance* TempMaterial = ItemStoredInfo.Image_X;
	ItemStoredInfo.Image_X = ItemStoredInfo.Image_Y;
	ItemStoredInfo.Image_Y = TempMaterial;

	int TempInt = ItemStoredInfo.ItemSize.X;
	ItemStoredInfo.ItemSize.X = ItemStoredInfo.ItemSize.Y;
	ItemStoredInfo.ItemSize.Y = TempInt;
}

int UTilePackageComponent::TurnPointToIndex(FIntPoint Point) const
{
	return Point.Y * PackageSize.X + Point.X;
}

FIntPoint UTilePackageComponent::TurnIndexToPoint(int Index) const
{
	return {Index%PackageSize.X,Index/PackageSize.X};
}

FItemStoredInfo UTilePackageComponent::CreateItemStoredInfo(const AItemActor* ItemActor)
{

	if (!ItemActor) return FItemStoredInfo{};
	FItemStoredInfo ItemStoredInfo;
	ItemActor->ItemName;
	if (UDataTable* DataTable = LoadObject<UDataTable>(this, TEXT("/Script/Engine.DataTable'/Game/Item/DT_MyItemInfo.DT_MyItemInfo'")))
	{
			
		if(FMyItemInfo * ItemInfo = DataTable->FindRow<FMyItemInfo>(ItemActor->ItemName, nullptr))
		{
			
			ItemStoredInfo = ItemInfo->ItemStoredInfo;
			ItemStoredInfo.ItemName = ItemActor->ItemName;
		}
	}
	return ItemStoredInfo;
}
bool UTilePackageComponent::TryToStoreItem(const AItemActor* ItemActor)
{
	UpdateIsStoredMutex();
	FItemStoredInfo ItemStoredInfo = CreateItemStoredInfo(ItemActor);
	if (!ItemStoredInfo.ItemName.IsValid()) return false;
	UE_LOG(LogTemp, Warning, TEXT("ItemInfo Get"));
	if(ItemStoredInfo.MaxOverlapNum > 1)
	{
		UE_LOG(LogTemp, Warning, TEXT("Item is Overlaying"));
		for (UItemObject * ItemObject : StoredItems)
		{
			FItemStoredInfo& StoredItem = ItemObject->ItemStoredInfo;
			if(StoredItem.ItemName == ItemStoredInfo.ItemName && StoredItem.Quantity < StoredItem.MaxOverlapNum)
			{
				StoredItem.Quantity += ItemActor->Quantity;
				UE_LOG(LogTemp, Warning, TEXT("Stored by Overlay"));
				return true;
			}
		}
	}
	FIntPoint LeftTopPoint;
	if (!HaveSpaceToStored(ItemStoredInfo, LeftTopPoint))
	{
		RotateItem(ItemStoredInfo);
		if(!HaveSpaceToStored(ItemStoredInfo, LeftTopPoint))
		{
			return false;
		}
	}
	ItemStoredInfo.StoredLeftTopPoint = LeftTopPoint;
	UE_LOG(LogTemp, Warning, TEXT("StoredLeftTopPoint: X : %d , Y : %d"), LeftTopPoint.X, LeftTopPoint.Y);
	ItemStoredInfo.Quantity = 1;
	UItemObject* temp = NewObject<UItemObject>();
	temp->ItemStoredInfo = ItemStoredInfo;
	StoredItems.Emplace(temp);
	UpdateIsStoredMutex();
	UE_LOG(LogTemp, Warning, TEXT("Stored by Create New Info"));
	for (UItemObject* StoredItem : StoredItems)
	{
		UE_LOG(LogTemp, Warning, TEXT("StoredItem Name : %s"), *StoredItem->ItemStoredInfo.ItemName.ToString());
	}
	return true;
}
// Called every frame
void UTilePackageComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}
