#pragma once

#include "CoreMinimal.h"
#include "ItemCategory.generated.h"

UENUM(BlueprintType)
enum class EItemCategory : uint8
{
	Resource UMETA(DisplayName = "Resource"),
	Tools UMETA(DisplayName = "Tools"),
	Medicine UMETA(DisplayName = "Medicine"),
	Misc UMETA(DisplayName = "Misc")
};