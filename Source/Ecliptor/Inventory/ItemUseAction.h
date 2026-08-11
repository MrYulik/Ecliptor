// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ItemUseInterface.h"
#include "UObject/Object.h"
#include "ItemUseAction.generated.h"

/**
 * 
 */
UCLASS(Abstract, Blueprintable, BlueprintType)
class ECLIPTOR_API UItemUseAction : public UObject, public IItemUseInterface
{
	GENERATED_BODY()
};
