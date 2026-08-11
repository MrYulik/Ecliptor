#pragma once

#include "CoreMinimal.h"
#include "ItemData.h"
#include "UObject/Interface.h"
#include "ItemUseInterface.generated.h"

UINTERFACE(MinimalAPI, Blueprintable)
class UItemUseInterface : public UInterface
{
	GENERATED_BODY()
};

class ECLIPTOR_API IItemUseInterface
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Item")
	bool TryUseItem(AActor* User, const FItemData& ItemData);
};