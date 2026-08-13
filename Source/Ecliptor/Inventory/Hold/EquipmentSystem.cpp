// Fill out your copyright notice in the Description page of Project Settings.


#include "EquipmentSystem.h"

#include "HoldableInterface.h"
#include "Components/SkeletalMeshComponent.h"
#include "Ecliptor/InventorySystem.h"
#include "GameFramework/Actor.h"
#include "GameFramework/Pawn.h"
#include "Sound/SoundConcurrency.h"

UEquipmentSystem::UEquipmentSystem()
{
	PrimaryComponentTick.bCanEverTick = false;
	SetIsReplicated(true);
}

void UEquipmentSystem::RequestEquip(FName ItemID)
{
	AActor* Owner = GetOwner();
	if (!IsValid(Owner) || ItemID.IsNone())
	{
		return;
	}
	
	if (Owner->HasAuthority())
	{
		Server_Equip_Implementation(ItemID);
		return;
	}
	
	Server_Equip(ItemID);
}

void UEquipmentSystem::RequestUnEquip()
{
	AActor* Owner = GetOwner();
	if (!IsValid(Owner))
	{
		return;
	}
	
	if (Owner->HasAuthority())
	{
		Server_UnEquip_Implementation();
		return;
	}
	
	Server_UnEquip();
}

void UEquipmentSystem::RequestHeldUse()
{
	AActor* Owner = GetOwner();
	if (!IsValid(Owner))
	{
		return;
	}
	
	if (Owner->HasAuthority())
	{
		Server_HeldUse_Implementation();
		return;
	}
	
	Server_HeldUse();
}

USkeletalMeshComponent* UEquipmentSystem::FindHandsMesh() const
{
	AActor* Owner = GetOwner();
	if (!IsValid(Owner))
	{
		return nullptr;
	}
	
	TArray<USkeletalMeshComponent*> Meshes;
	Owner->GetComponents<USkeletalMeshComponent>(Meshes);
	for (USkeletalMeshComponent* Mesh : Meshes)
	{
		if (Mesh && Mesh->ComponentHasTag(HandMeshTag))
		{
			return Mesh;
		}
	}
	
	return Meshes.IsValidIndex(0) ? Meshes[0] : nullptr;
}

void UEquipmentSystem::Multicast_HeldUse_Implementation()
{
	if (!IsValid(HeldInstance) || !HeldInstance->Implements<UHoldableInterface>())
	{
		return;
	}
	
	IHoldableInterface::Execute_OnHeldUse(HeldInstance);
}

void UEquipmentSystem::Server_Equip_Implementation(FName ItemID)
{
	AActor* Owner = GetOwner();
	if (!IsValid(Owner))
	{
		return;
	}
	
	UInventorySystem* Inventory = Owner->FindComponentByClass<UInventorySystem>();
	if (!Inventory || !Inventory->CanHoldItem(ItemID))
	{
		return;
	}
	
	FItemData Data;
	if (!Inventory->GetItemData(ItemID, Data) || !Data.HeldActor)
	{
		return;
	}
	
	Multicast_Attach(Data.HeldActor);
}

void UEquipmentSystem::Server_UnEquip_Implementation()
{
	Multicast_Detach();
}

void UEquipmentSystem::Server_HeldUse_Implementation()
{
	if (!IsValid(HeldInstance) || !HeldInstance->Implements<UHoldableInterface>())
	{
		return;
	}
	
	Multicast_HeldUse();
}

void UEquipmentSystem::Multicast_Attach_Implementation(TSubclassOf<AActor> Class)
{
	Multicast_Detach_Implementation();
	USkeletalMeshComponent* Hands = FindHandsMesh();
	AActor* Owner = GetOwner();
	if (!Hands || !Class || !IsValid(Owner) || !GetWorld())
	{
		return;
	}
	
	FActorSpawnParameters Params;
	Params.Owner = Owner;
	Params.Instigator = Cast<APawn>(Owner);
	Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	HeldInstance = GetWorld()->SpawnActor<AActor>(Class, Hands->GetComponentTransform(),  Params);
	if (!IsValid(HeldInstance) || !HeldInstance->Implements<UHoldableInterface>())
	{
		if (IsValid(HeldInstance))
		{
			HeldInstance->Destroy();
			HeldInstance = nullptr;
		}
		
		return;
	}
	
	HeldInstance->SetActorEnableCollision(false);
	FName Socket = IHoldableInterface::Execute_GetHandSocket(HeldInstance);
	HeldInstance->AttachToComponent(Hands, FAttachmentTransformRules::SnapToTargetIncludingScale, Socket);
	IHoldableInterface::Execute_OnEquipped(HeldInstance, Owner);
}

void UEquipmentSystem::Multicast_Detach_Implementation()
{
	if (!IsValid(HeldInstance))
	{
		HeldInstance = nullptr;
		return;
	}
	
	if (HeldInstance->Implements<UHoldableInterface>())
	{
		IHoldableInterface::Execute_OnUnEquipped(HeldInstance);
	}
	
	HeldInstance->Destroy();
	HeldInstance = nullptr;
}