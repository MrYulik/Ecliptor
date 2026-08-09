// Fill out your copyright notice in the Description page of Project Settings.


#include "InventorySystem.h"

#include "ItemData.h"

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

	const int32 MaxStack = FMath::Max(1, ItemData->MaxStack);
	int32 Remaining = Quantity;
	bool bChanged = false;
	
	for (FInventoryEntry& Entry : Items)
	{
		if (Entry.ItemID != ItemID || Remaining <= 0)
		{
			continue;
		}

		const int32 FreeSpace = MaxStack - Entry.Quantity;
		if (FreeSpace <= 0)
		{
			continue;
		}

		const int32 ToAdd = FMath::Min(FreeSpace, Remaining);
		Entry.Quantity += ToAdd;
		Remaining -= ToAdd;
		bChanged = true;
	}
	
	while (Remaining > 0)
	{
		if (Items.Num() >= MaxSlots)
		{
			UE_LOG(LogTemp, Warning, TEXT("Inventory full, could not add all of '%s'."), *ItemID.ToString());
			break;
		}

		FInventoryEntry NewEntry;
		NewEntry.ItemID = ItemID;
		NewEntry.Quantity = FMath::Min(MaxStack, Remaining);
		Items.Add(NewEntry);

		Remaining -= NewEntry.Quantity;
		bChanged = true;
	}

	if (bChanged)
	{
		OnInventoryChanged.Broadcast();
	}
	
	return bChanged && Remaining == 0;
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

bool UInventorySystem::GetSlot(int32 Index, FInventoryEntry& OutEntry) const
{
	if (!Items.IsValidIndex(Index))
	{
		OutEntry = FInventoryEntry();
		return false;
	}

	OutEntry = Items[Index];
	return OutEntry.Quantity > 0 && !OutEntry.ItemID.IsNone();
}

bool UInventorySystem::GetItemData(FName ItemID, FItemData& OutData) const
{
	if (!ItemDatabase || ItemID.IsNone())
	{
		return false;
	}

	const FItemData* Found = ItemDatabase->FindRow<FItemData>(ItemID, TEXT("GetItemData"));
	if (!Found)
	{
		return false;
	}

	OutData = *Found;
	return true;
}
