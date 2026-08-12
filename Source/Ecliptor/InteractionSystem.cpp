// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractionSystem.h"

#include "CollisionQueryParams.h"
#include "InteractionInterface.h"
#include "Components/MeshComponent.h"
#include "Engine/World.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/PlayerController.h"

// Sets default values for this component's properties
UInteractionSystem::UInteractionSystem()
{
	PrimaryComponentTick.bCanEverTick = true;
	SetIsReplicatedByDefault(true);
}

void UInteractionSystem::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	UpdateFocusedActor(FindInteractable());
}

void UInteractionSystem::TryInteract()
{
	if (!FocusedActor)
		return;
		
		IInteractionInterface::Execute_Interact(FocusedActor, GetOwner());
}

AActor* UInteractionSystem::FindInteractable() const
{
	const APawn* OwnerPawn = Cast<APawn>(GetOwner());
	if (!OwnerPawn)
		return nullptr;
	
	const APlayerController* PC = Cast<APlayerController>(OwnerPawn->GetController());
	if (!PC || !PC->PlayerCameraManager)
		return nullptr;
	
	const FVector Start = PC->PlayerCameraManager->GetCameraLocation();
	const FVector End = Start + PC->PlayerCameraManager->GetActorForwardVector() * TraceDistance;
	FHitResult Hit;
	FCollisionQueryParams Params(SCENE_QUERY_STAT(InteractionTrace), false, GetOwner());
	if (!GetWorld()->LineTraceSingleByChannel(Hit, Start, End, TraceChannel, Params))
	{
		return nullptr;
	}
	
	AActor* HitActor = Hit.GetActor();
	if (!HitActor || !HitActor->Implements<UInteractionInterface>())
	{
		return nullptr;
	}
	
	if (!IInteractionInterface::Execute_CanInteract(HitActor, GetOwner()))
	{
		return nullptr;
	}
	
	return HitActor;
}

void UInteractionSystem::UpdateFocusedActor(AActor* NewFocused)
{
	if (NewFocused == PreviousFocusedActor)
	{
		FocusedActor = NewFocused;
		return;
	}
	
	SetOutlineEnabled(PreviousFocusedActor, false);
	FocusedActor = NewFocused;
	PreviousFocusedActor = NewFocused;
	SetOutlineEnabled(FocusedActor, true);
}

void UInteractionSystem::SetOutlineEnabled(AActor* Actor, bool bEnabled) const
{
	if (!Actor)
		return;
	
	TArray<UMeshComponent*> Meshes;
	Actor->GetComponents<UMeshComponent>(Meshes);
	for (UMeshComponent* Mesh : Meshes)
	{
		if (!Mesh)
			continue;
		Mesh->SetRenderCustomDepth(bEnabled);
		Mesh->SetCustomDepthStencilValue(bEnabled ? OutlineStencilValue : 0);
	}
}

void UInteractionSystem::Server_TryInteract_Implementation(AActor* Target)
{
	if (!FocusedActor)
	{
		return;
	}
	
	AActor* Owner = GetOwner();
	if (!IsValid(Owner))
	{
		return;
	}
	
	if (Owner->HasAuthority())
	{
		IInteractionInterface::Execute_Interact(Owner, GetOwner());
		return;
	}
	
	Server_TryInteract(FocusedActor);
}

bool UInteractionSystem::Server_TryInteract_Validate(AActor* Target)
{
	return true;
}

