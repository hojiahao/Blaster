// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "BlasterHUD.generated.h"

USTRUCT(BlueprintType)
struct FHUDPackage
{
	GENERATED_BODY()
public:
	UPROPERTY()
	TObjectPtr<class UTexture2D> CrosshairsCenter;

	UPROPERTY()
	TObjectPtr<UTexture2D> CrosshairsLeft;

	UPROPERTY()
	TObjectPtr<UTexture2D> CrosshairsRight;

	UPROPERTY()
	TObjectPtr<UTexture2D> CrosshairsTop;

	UPROPERTY()
	TObjectPtr<UTexture2D> CrosshairsBottom;

	float CrosshairSpread = 0.f;
	FLinearColor CrosshairsColor = FLinearColor::White;
};

/**
 * 
 */
UCLASS()
class BLASTER_API ABlasterHUD : public AHUD
{
	GENERATED_BODY()
public:
	virtual void DrawHUD() override;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class UUserWidget> CharacterOverlayClass;
	
	UPROPERTY()
	TObjectPtr<class UCharacterOverlay> CharacterOverlay;

protected:
	virtual void BeginPlay() override;
	void AddCharacterOverlay();

private:
	FHUDPackage HUDPackage;

	void DrawCrosshairs(UTexture2D* Texture, FVector2D ViewportCenter, FVector2D Spread, FLinearColor CrosshairsColor);
	
	UPROPERTY(EditAnywhere)
	float CrosshairSpreadMax = 16.f;
public:
	FORCEINLINE void SetHUDPackage(const FHUDPackage& Package) { HUDPackage = Package; }
};
