#include "WorldItem.h"

#include "InventorySystem.h"
#include "ItemData.h"

void AWorldItem::Interact_Implementation(AActor* Interactor)
{
	if (!IsValid(Interactor))
	{
		return;
	}

	UInventorySystem* Inventory =
		Interactor->FindComponentByClass<UInventorySystem>();

	if (!IsValid(Inventory))
	{
		return;
	}

	if (Inventory->AddItem(ItemID, Quantity))
	{
		UE_LOG(LogTemp, Warning, TEXT("Picked up %s x%d"), *ItemID.ToString(), Quantity);
		Destroy();
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("AddItem FAILED for %s"), *ItemID.ToString());
	}
}

FText AWorldItem::GetInteractionText_Implementation() const
{
	if (!ItemDatabase)
	{
		return FText::FromString(TEXT("Pick Up"));
	}

	const FItemData* ItemData =
		ItemDatabase->FindRow<FItemData>(
			ItemID,
			TEXT("WorldItem::GetInteractionText")
		);

	if (!ItemData)
	{
		return FText::FromString(TEXT("Pick Up"));
	}

	return FText::Format(
		NSLOCTEXT(
			"Interaction",
			"PickUpItem",
			"Pick up {0}"
		),
		ItemData->Name
	);
}

bool AWorldItem::CanInteract_Implementation(AActor* Interactor) const
{
	UInventorySystem* Inventory = Interactor->FindComponentByClass<UInventorySystem>();
	return IsValid(Inventory);
}
