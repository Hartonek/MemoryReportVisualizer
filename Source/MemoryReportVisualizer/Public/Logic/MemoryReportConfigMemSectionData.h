// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "HAL/Platform.h"
#include "MemoryReportSectionData.h"

// Structure for ConfigMem section data
struct MEMORYREPORTVISUALIZER_API ConfigMemDescription
{
	FString FileName;
	int32 NumBytes;
	int32 MaxBytes;
	
	ConfigMemDescription() : NumBytes(0), MaxBytes(0) {}
	ConfigMemDescription(const FString& InFileName, int32 InNumBytes, int32 InMaxBytes)
		: FileName(InFileName), NumBytes(InNumBytes), MaxBytes(InMaxBytes) {}
};

// Specialized section data class for ConfigMem sections
class MEMORYREPORTVISUALIZER_API ConfigMemSectionData : public SectionData
{
public:
	ConfigMemSectionData() = default;
	ConfigMemSectionData(const FString& InID, const FString& InContent)
		: SectionData(InID, InContent) {}

	// Override virtual function to parse ConfigMem data
	virtual void ExtractContentInternally() override;

	// Parsed ConfigMem data
	TArray<ConfigMemDescription> ConfigMemEntries;
};