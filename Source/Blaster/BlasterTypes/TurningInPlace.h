#pragma once

UENUM(BlueprintType)
enum class ETurningInPlace: uint8
{
	ETIP_Left UMETA(DisplayName = "Turing Left"),
	ETIP_Right UMETA(DisplayName = "Turing Right"),
	ETIP_NotTurning UMETA(DisplayName = "Not Turning"),
	ETIP_MAX UMETA(DisplayName = "DefaultMAX")
};