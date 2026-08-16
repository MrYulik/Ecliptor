// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "EcliptorGameState.generated.h"

/**
 * 
 */
UCLASS()
class ECLIPTOR_API AEcliptorGameState : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	AEcliptorGameState();
	
	virtual void Tick(float DeltaSeconds) override;
	
	UFUNCTION(BlueprintPure, Category="Time")
	float GetGameTimeSeconds() const { return GameTimeSeconds; }
	
	UFUNCTION(BlueprintPure, Category="Time")
	FText GetGameTimeText() const;
	
protected:
	UPROPERTY(Replicated, BlueprintReadOnly, Category="Time")
	float GameTimeSeconds = 0.f;
	
	UPROPERTY(Replicated, BlueprintReadOnly, Category="Time")
	float TimeScale = 1.f;
	
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	
};
