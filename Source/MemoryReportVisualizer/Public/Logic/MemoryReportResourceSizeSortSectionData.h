// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "HAL/Platform.h"
#include "MemoryReportSectionData.h"

// Structure for ResourceSizeSort section data
struct MEMORYREPORTVISUALIZER_API ResourceSizeSortDescription
{
	FString Class;
	int32 Count;
	float NumKB;
	float MaxKB;
	float ResExcKB;
	float ResExcDedSysKB;
	float ResExcDedVidKB;
	float ResExcUnkKB;
	
	ResourceSizeSortDescription() : Count(0), NumKB(0.0f), MaxKB(0.0f), ResExcKB(0.0f), ResExcDedSysKB(0.0f), ResExcDedVidKB(0.0f), ResExcUnkKB(0.0f) {}
	ResourceSizeSortDescription(const FString& InClass, int32 InCount, float InNumKB, float InMaxKB, float InResExcKB, float InResExcDedSysKB, float InResExcDedVidKB, float InResExcUnkKB)
		: Class(InClass), Count(InCount), NumKB(InNumKB), MaxKB(InMaxKB), ResExcKB(InResExcKB), ResExcDedSysKB(InResExcDedSysKB), ResExcDedVidKB(InResExcDedVidKB), ResExcUnkKB(InResExcUnkKB) {}
};

// Specialized section data class for ResourceSizeSort sections
class MEMORYREPORTVISUALIZER_API ResourceSizeSortSectionData : public SectionData
{
public:
	ResourceSizeSortSectionData() = default;
	ResourceSizeSortSectionData(const FString& InID, const FString& InContent)
		: SectionData(InID, InContent) {}

	// Override virtual function to parse ResourceSizeSort data
	virtual void ExtractContentInternally() override;

	// Parsed ResourceSizeSort data
	TArray<ResourceSizeSortDescription> ResourceSizeSortEntries;
};