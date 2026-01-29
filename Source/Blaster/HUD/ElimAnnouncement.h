// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ElimAnnouncement.generated.h"

/**
 * 
 */
UCLASS()
class BLASTER_API UElimAnnouncement : public UUserWidget
{
	GENERATED_BODY()
public:
	void SetElimAnnouncementText(FString AttackerName, FString VictimName);

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UHorizontalBox> AnnouncementBox;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UTextBlock> AnnouncementText;
};
