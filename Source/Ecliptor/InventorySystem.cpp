// Fill out your copyright notice in the Description page of Project Settings.


#include "InventorySystem.h"

#include "ItemData.h"

// Sets default values for this component's properties
UInventorySystem::UInventorySystem()
{
	PrimaryComponentTick.bCanEverTick = false;
}

bool UInventorySystem::AddItem(FName ItemID, int32 Quantity)
{
	if (ItemID.IsNone() || Quantity <= 0)
	{
		return false;
	}
	
	if (!ItemDatabase)
	{
		UE_LOG(LogTemp, Error, TEXT("InventoryComponent: ItemDatabase is not assigned."));
		return false;
	}
	
	const FItemData* ItemData = ItemDatabase->FindRow<FItemData>(ItemID, TEXT("InventoryComponent::AddItem"));
	if (!ItemData)
	{
		UE_LOG(LogTemp, Warning, TEXT("InventoryComponent: Item '%s' not found."), *ItemID.ToString());
		return false;
	}
	
	for (FInventoryEntry& Entry : Items)
	{
		if (Entry.ItemID == ItemID)
		{
			Entry.Quantity += Quantity;
			return true;
		}
	}
	
	FInventoryEntry NewEntry;
	NewEntry.ItemID = ItemID;
	NewEntry.Quantity = Quantity;
	Items.Add(NewEntry);
	
	return true;
}

bool UInventorySystem::RemoveItem(FName ItemID, int32 Quantity)
{
	return true;
}

bool UInventorySystem::HasItem(FName ItemID, int32 Quantity) const
{
	return true;
}

int32 UInventorySystem::GetItemQuantity(FName ItemID) const
{
	return 0;
}
