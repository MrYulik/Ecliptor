// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Ecliptor/Inventory/ItemCategory.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "ItemCategoryLibrary.generated.h"

/**
 * 
 */
UCLASS()
class ECLIPTOR_API UItemCategoryLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintPure, Category="Item")
	static FText GetCategoryDisplayName(EItemCategory Category);
};