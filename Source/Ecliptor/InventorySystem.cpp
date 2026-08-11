// Fill out your copyright notice in the Description page of Project Settings.


#include "InventorySystem.h"

#include "WorldItem.h"
#include "Engine/World.h"
#include "GameFramework/Actor.h"
#include "Inventory/ItemUseAction.h"
#include "Inventory/ItemUseInterface.h"

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
	if (ItemID.IsNone() || Quantity <= 0)
	{
		return false;
	}
	
	for (int32 i = 0; i < Items.Num(); i++)
	{
		if (Items[i].ItemID != ItemID)
		{
			continue;
		}
		
		if (Items[i].Quantity > Quantity)
		{
			Items[i].Quantity -= Quantity;
		}
		else
		{
			Items.RemoveAt(i);
		}
		
		OnInventoryChanged.Broadcast();
		return true;
	}
	
	return false;
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

bool UInventorySystem::DropItem(FName ItemID, int32 Quantity, AActor* Dropper)
{
	if (!IsValid(Dropper) || ItemID.IsNone() || Quantity <= 0)
	{
		return false;
	}
	
	FItemData ItemData;
	if (!GetItemData(ItemID, ItemData) || !ItemData.WorldObject)
	{
		UE_LOG(LogTemp, Warning, TEXT("DropItem: no WorldObject for '%s"), *ItemID.ToString());
		return false;
	}
	
	if (!RemoveItem(ItemID, Quantity))
	{
		return false;
	}
	
	UWorld* World = Dropper->GetWorld();
	if (!World)
	{
		return false;
	}
	
	const FVector DropLocation = Dropper->GetActorLocation() + Dropper->GetActorForwardVector() * 150.f + FVector(0.f, 0.f, 50.f);
	FActorSpawnParameters Params;
	Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
	AActor* Spawned = World->SpawnActor<AActor>(ItemData.WorldObject, DropLocation, Dropper->GetActorRotation(), Params);
	if (!Spawned)
	{
		AddItem(ItemID, Quantity);
		return false;
	}
	
	if (AWorldItem* WorldItem = Cast<AWorldItem>(Spawned))
	{
		WorldItem->SetupDroppedItem(ItemID, Quantity);
	}
	
	return true;
}

bool UInventorySystem::UseItem(FName ItemID)
{
	if (ItemID.IsNone())
	{
		return false;
	}
	
	AActor* Owner = GetOwner();
	if (!IsValid(Owner))
	{
		return false;
	}
	
	FItemData ItemData;
	if (!GetItemData(ItemID, ItemData))
	{
		return false;
	}
	
	if (!ItemData.UseAction)
	{
		return false;
	}
	
	UItemUseAction* Action = NewObject<UItemUseAction>(Owner, ItemData.UseAction);
	if (!IsValid(Action))
	{
		return false;
	}
	
	if (!IItemUseInterface::Execute_TryUseItem(Action, Owner, ItemData))
	{
		return false;
	}
	
	return RemoveItem(ItemID, 1);
}
