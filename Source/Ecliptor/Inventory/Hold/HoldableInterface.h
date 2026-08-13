// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "HoldableInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI, Blueprintable)
class UHoldableInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class ECLIPTOR_API IHoldableInterface
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Holdable")
	void OnEquipped(AActor* Holder);
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Holdable")
	void OnUnEquipped();
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Holdable")
	void OnHeldUse();
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Holdable")
	FName GetHandSocket() const;
};
