// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ResourceComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnResourceChanged, float, Current, float, Max);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnResourceDepleted);

UCLASS(Abstract, Blueprintable, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ECLIPTOR_API UResourceComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UResourceComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	UFUNCTION(BlueprintPure, Category="Resource")
	float GetCurrent() const { return Current; }
	
	UFUNCTION(BlueprintPure, Category="Resource")
	float GetMax() const { return Max; }
	
	UFUNCTION(BlueprintPure, Category="Resource")
	float GetPercent() const { return Max > 0.f ? Current / Max : 0.f; }
	
	UFUNCTION(BlueprintCallable, Category="Resource")
	virtual bool Consume(float Amount);
	
	UFUNCTION(BlueprintCallable, Category="Resource")
	virtual void Restore(float Amount);
	
	UPROPERTY(BlueprintAssignable, Category="Resource" )
	FOnResourceChanged OnResourceChanged;
	
	UPROPERTY(BlueprintAssignable, Category="Resource")
	FOnResourceDepleted OnResourceDepleted;
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Resource", meta=(ClampMin=0))
	float Current = 100.f;
	
	UPROPERTY(EditAnywhere, Category="Resource", meta=(ClampMin=0))
	float Max = 100.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Resource")
	float RegenPerSecond = 0.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Resource")
	float RegenDelay = 0.f;
	
	float RegenDelayRemaining = 0.f;
	
	void ClampCurrent();
	void BroadcastChanged();
	
};
