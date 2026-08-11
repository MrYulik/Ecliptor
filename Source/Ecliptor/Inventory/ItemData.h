// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ItemCategory.h"
#include "Engine/DataTable.h"
#include "Engine/Texture2D.h"
#include "ItemData.generated.h"

class UItemUseAction;

USTRUCT(BlueprintType)
struct ECLIPTOR_API FItemData : public FTableRowBase
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Item")
	FName ID = "item_";
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Item")
	FText Name;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Item")
	FText Description;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Item")
	TObjectPtr<UTexture2D> Icon;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Item")
	TSubclassOf<AActor> WorldObject;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Item")
	TSubclassOf<UItemUseAction> UseAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Item")
	int32 MaxStack = 1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Item")
	EItemCategory Category = EItemCategory::Misc;
};
