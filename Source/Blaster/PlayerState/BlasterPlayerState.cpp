// Fill out your copyright notice in the Description page of Project Settings.


#include "BlasterPlayerState.h"
#include "Blaster/Character/BlasterCharacter.h"
#include "Blaster/PlayerController/BlasterPlayerController.h"

void ABlasterPlayerState::OnRep_Score()
{
	Super::OnRep_Score();
	if (Character == nullptr)
	{
		Character = Cast<ABlasterCharacter>(GetPawn());
	}
	if (Character)
	{
		if (Controller == nullptr)
		{
			Controller = Cast<ABlasterPlayerController>(Character->Controller);
		}
		if (Controller)
		{
			Controller->SetHUDScore(GetScore());
		}
	}
}

void ABlasterPlayerState::AddToScore(float ScoreAmount)
{
	SetScore(GetScore() + ScoreAmount);
	if (Character == nullptr)
	{
		Character = Cast<ABlasterCharacter>(GetPawn());
	}
	if (Character)
	{
		if (Controller == nullptr)
		{
			Controller = Cast<ABlasterPlayerController>(Character->Controller);
		}
		if (Controller)
		{
			Controller->SetHUDScore(GetScore());
		}
	}
}
