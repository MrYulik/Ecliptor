// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InteractionInterface.h"
#include "Engine/DataTable.h"
#include "GameFramework/Actor.h"
#include "Sound/SoundWave.h"
#include "WorldItem.generated.h"

UCLASS(Abstract)
class ECLIPTOR_API AWorldItem : public AActor, public IInteractionInterface
{
	GENERATED_BODY()
	
public:
	virtual void Interact_Implementation(AActor* Interactor) override;
	virtual FText GetInteractionText_Implementation() const override;
	virtual bool CanInteract_Implementation(AActor* Interactor) const override;
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Item")
	FName ItemID;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Item")
	int32 Quantity = 1;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Item")
	TObjectPtr<UDataTable> ItemDatabase;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Item")
	TObjectPtr<USoundBase> PickupSound;
};
