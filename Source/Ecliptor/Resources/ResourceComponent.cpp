// Fill out your copyright notice in the Description page of Project Settings.


#include "ResourceComponent.h"

UResourceComponent::UResourceComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void UResourceComponent::BeginPlay()
{
	Super::BeginPlay();
	ClampCurrent();
	BroadcastChanged();
}


// Called every frame
void UResourceComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if (RegenDelayRemaining > 0.f)
	{
		RegenDelayRemaining -= DeltaTime;
		return;
	}
	
	if (RegenPerSecond > 0.f && Current < Max)
	{
		Restore(RegenPerSecond * DeltaTime);
	}
}

bool UResourceComponent::Consume(float Amount)
{
	if (Amount <= 0.f || Current <= 0.f)
		return false;
	
	const float Old = Current;
	Current = FMath::Max(0.f, Current - Amount);
	RegenDelayRemaining = RegenDelay;
	if (!FMath::IsNearlyEqual(Old, Current))
	{
		BroadcastChanged();
	}
	
	if (Current <= 0.f && Old > 0.f)
		OnResourceChanged.Broadcast(Current, Max);
	
	return Current < Old;
}

void UResourceComponent::Restore(float Amount)
{
	if (Amount <= 0.f || Current >= Max)
		return;
	
	const float Old = Current;
	Current = FMath::Min(Max, Current + Amount);
	if (!FMath::IsNearlyEqual(Old, Current))
	{
		BroadcastChanged();
	}
}

void UResourceComponent::ClampCurrent()
{
	Current = FMath::Clamp(Current, 0.f, Max);
}

void UResourceComponent::BroadcastChanged()
{
	OnResourceChanged.Broadcast(Current, Max);
}

