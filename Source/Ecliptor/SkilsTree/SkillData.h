#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "SkillData.generated.h"

USTRUCT(BlueprintType)
struct ECLIPTOR_API FSkillData : public FTableRowBase
{
	GENERATED_BODY();
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Skill");
	FName ID = TEXT("skill_");

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Skill");
	FText Name;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Skill");
	FText Description;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Skill");
	TObjectPtr<UTexture2D> Icon;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Skill");
	int32 MaxRank = 3;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Skill");
	int32 CostPerRank = 1;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Skill");
	FName ParentSkill;
};
