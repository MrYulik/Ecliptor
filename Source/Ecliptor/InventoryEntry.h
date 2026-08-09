#pragma once

#include "CoreMinimal.h"
#include "InventoryEntry.generated.h"

USTRUCT(BlueprintType)
struct ECLIPTOR_API FInventoryEntry
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Inventory")
	FName ItemID;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Inventory")
	int32 Quantity = 0;
};
