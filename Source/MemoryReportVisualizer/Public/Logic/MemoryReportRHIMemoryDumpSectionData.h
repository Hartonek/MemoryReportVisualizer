// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "HAL/Platform.h"
#include "MemoryReportSectionData.h"

// Structure for RHIMemoryDump section data
struct MEMORYREPORTVISUALIZER_API RHIMemoryDumpDescription
{
	float MemorySizeMB;
	FString Description;
	
	RHIMemoryDumpDescription() : MemorySizeMB(0.0f) {}
	RHIMemoryDumpDescription(float InMemorySizeMB, const FString& InDescription)
		: MemorySizeMB(InMemorySizeMB), Description(InDescription) {}
};

// Specialized section data class for RHIMemoryDump sections
class MEMORYREPORTVISUALIZER_API RHIMemoryDumpSectionData : public SectionData
{
public:
	RHIMemoryDumpSectionData() = default;
	RHIMemoryDumpSectionData(const FString& InID, const FString& InContent)
		: SectionData(InID, InContent) {}

	// Override virtual function to parse RHIMemoryDump data
	virtual void ExtractContentInternally() override;

	// Parsed RHIMemoryDump data
	TArray<RHIMemoryDumpDescription> RHIMemoryDumpEntries;
	
	// Total amount of memory from footer line
	FString TotalAmountOfMemory;
};