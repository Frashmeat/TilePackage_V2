// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "TilePackageComponent.h"
#include "ItemObject.generated.h"



/**
 * 
 */
UCLASS()
class TILEPACKAGE_V2_API UItemObject : public UObject
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadWrite , VisibleAnywhere , Category = Item)
	FItemStoredInfo ItemStoredInfo; 
};
