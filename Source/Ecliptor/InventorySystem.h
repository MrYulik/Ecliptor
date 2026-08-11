// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InventoryEntry.h"
#include "Inventory/ItemData.h"
#include "Components/ActorComponent.h"
#include "Engine/DataTable.h"
#include "InventorySystem.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnInventoryChanged);


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ECLIPTOR_API UInventorySystem : public UActorComponent
{
	GENERATED_BODY()

public:	
	UInventorySystem();

	UFUNCTION(BlueprintCallable, Category="Inventory")
	bool AddItem(FName ItemID, int32 Quantity = 1);
	
	UFUNCTION(BlueprintCallable, Category="Inventory")
	bool RemoveItem(FName ItemID, int32 Quantity = 1);
	
	UFUNCTION(BlueprintPure, Category="Inventory")
	bool HasItem(FName ItemID, int32 Quantity = 1) const;
	
	UFUNCTION(BlueprintPure, Category="Inventory")
	int32 GetItemQuantity(FName ItemID) const;
	
	UFUNCTION(BlueprintPure, Category="Inventory")
	int32 GetSlotCount() const { return MaxSlots; }
	
	UFUNCTION(BlueprintPure, Category="Inventory")
	bool GetSlot(int32 Index, FInventoryEntry& OutEntry) const;
	
	UFUNCTION(BlueprintPure, Category="Inventory")
	bool GetItemData(FName ItemID, FItemData& OutData) const;
	
	UPROPERTY(BlueprintAssignable, Category="Inventory")
	FOnInventoryChanged OnInventoryChanged;
	
	UFUNCTION(BLueprintCallable, Category="Inventory")
	bool DropItem(FName ItemID, int32 Quantity, AActor* Dropper);
	
	UFUNCTION(BlueprintCallable, Category="Inventory")
	bool UseItem(FName ItemID);
	
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Inventory")
	TObjectPtr<UDataTable> ItemDatabase;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Inventory")
	TArray<FInventoryEntry> Items;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Inventory")
	int32 MaxSlots = 4;
};
