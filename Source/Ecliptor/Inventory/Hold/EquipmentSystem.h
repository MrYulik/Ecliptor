// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "EquipmentSystem.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class ECLIPTOR_API UEquipmentSystem : public UActorComponent
{
	GENERATED_BODY()

public:
	UEquipmentSystem();
	
	UFUNCTION(BlueprintCallable, Category="EquipmentSystem")
	void RequestEquip(FName ItemID);
	
	UFUNCTION(BlueprintCallable, Category="EquipmentSystem")
	void RequestUnEquip();
	
	UFUNCTION(BlueprintCallable, Category="EquipmentSystem")
	void RequestHeldUse();
	
	UFUNCTION(BlueprintCallable, Category="EquipmentSystem")
	bool IsHolding() const { return HeldInstance != nullptr;  }
	
private:
	USkeletalMeshComponent* FindHandsMesh() const;
	
protected:
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="EquipmentSystem")
	FName HandMeshTag;
	
	UPROPERTY()
	TObjectPtr<AActor> HeldInstance;
	
	UFUNCTION(Server, Reliable)
	void Server_Equip(FName ItemID);
	
	UFUNCTION(Server, Reliable)
	void Server_UnEquip();
	
	UFUNCTION(Server, Reliable)
	void Server_HeldUse();
	
	UFUNCTION(NetMulticast, Reliable)
	void Multicast_Attach(TSubclassOf<AActor> Class);

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_HeldUse();
	
	UFUNCTION(NetMulticast, Reliable)
	void Multicast_Detach();
};
