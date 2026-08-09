// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InteractionSystem.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ECLIPTOR_API UInteractionSystem : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UInteractionSystem();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	UFUNCTION(BlueprintCallable, Category="Interaction")
	void TryInteract();
	
	UFUNCTION(BlueprintPure, Category="Interaction")
	AActor* GetFocusedActor() const { return FocusedActor; }
	
protected:
	UPROPERTY(BlueprintReadOnly, Category="Interaction")
	TObjectPtr<AActor> FocusedActor;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Interaction")
	float TraceDistance = 250.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Interaction")
	TEnumAsByte<ECollisionChannel> TraceChannel = ECC_Visibility;
	
	UPROPERTY(BlueprintReadOnly, Category="Interaction")
	TObjectPtr<AActor> PreviousFocusedActor;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Interaction")
	int32 OutlineStencilValue = 1;
	
	AActor* FindInteractable() const;
	
	//
	void UpdateFocusedActor(AActor* NewFocused);
	void SetOutlineEnabled(AActor* Actor, bool bEnabled) const;
};
