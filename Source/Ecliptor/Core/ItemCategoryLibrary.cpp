// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemCategoryLibrary.h"
#include "Internationalization/Text.h"

inline FText UItemCategoryLibrary::GetCategoryDisplayName(EItemCategory Category)
{
	switch (Category)
	{
		case EItemCategory::Medicine: return FText::FromString(TEXT("[MEDICINE]"));
		case EItemCategory::Resource:     return FText::FromString(TEXT("[RESOURCE]"));
		case EItemCategory::Tools:    return FText::FromString(TEXT("[TOOLS]"));
		case EItemCategory::Misc:     return FText::FromString(TEXT("[MISC]"));
		default:                      return FText::FromString(TEXT("[UNKNOWN]"));
	}
}
