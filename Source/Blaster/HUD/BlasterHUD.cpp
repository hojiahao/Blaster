// Fill out your copyright notice in the Description page of Project Settings.


#include "BlasterHUD.h"

void ABlasterHUD::DrawHUD()
{
	Super::DrawHUD();
	// Draw custom HUD elements here
	FVector2D ViewportSize;
	if (GEngine && GEngine->GameViewport)
	{
		GEngine->GameViewport->GetViewportSize(ViewportSize);
		const FVector2D ViewportCenter(ViewportSize.X / 2.f, ViewportSize.Y / 2.f);
		if (HUDPackage.CrosshairsCenter)
		{
			DrawCrosshairs(HUDPackage.CrosshairsCenter, ViewportCenter);
		}
		if (HUDPackage.CrosshairsLeft)
		{
			DrawCrosshairs(HUDPackage.CrosshairsCenter, ViewportCenter);
		}
		if (HUDPackage.CrosshairsRight)
		{
			DrawCrosshairs(HUDPackage.CrosshairsCenter, ViewportCenter);
		}
		if (HUDPackage.CrosshairsTop)
		{
			DrawCrosshairs(HUDPackage.CrosshairsCenter, ViewportCenter);
		}
		if (HUDPackage.CrosshairsBottom)
		{
			DrawCrosshairs(HUDPackage.CrosshairsCenter, ViewportCenter);
		}
	}
}

void ABlasterHUD::DrawCrosshairs(UTexture2D* Texture, FVector2D ViewportCenter)
{
	const float TextureWidth = Texture ? Texture->GetSizeX() : 0.f;
	const float TextureHeight = Texture ? Texture->GetSizeY() : 0.f;
	const FVector2D TextureDrawPosition(
		ViewportCenter.X - (TextureWidth / 2.f),
		ViewportCenter.Y - (TextureHeight / 2.f)
	);
	DrawTexture(
		Texture,
		TextureDrawPosition.X,
		TextureDrawPosition.Y,
		TextureWidth,
		TextureHeight,
		0.f,
		0.f,
		1.f,
		1.f,
		FLinearColor::White
	);
}
