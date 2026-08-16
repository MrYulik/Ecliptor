// Fill out your copyright notice in the Description page of Project Settings.


#include "EcliptorGameState.h"

#include "Net/UnrealNetwork.h"

AEcliptorGameState::AEcliptorGameState()
{
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
}

void AEcliptorGameState::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	if (HasAuthority())
	{
		GameTimeSeconds += DeltaSeconds * TimeScale;
	}
}

FText AEcliptorGameState::GetGameTimeText() const
{
	const int32 Total = FMath::Max(0, FMath::FloorToInt(GameTimeSeconds));
	const int32 Hours = (Total / 3600) % 24;
	const int32 Minutes = (Total / 60) % 60;
	const int32 Seconds = Total % 60;
	
	return FText::FromString(FString::Printf(TEXT("%02d:%02d:%02d"), Hours, Minutes, Seconds));
}

void AEcliptorGameState::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AEcliptorGameState, GameTimeSeconds);
}
