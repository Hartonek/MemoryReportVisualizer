// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "HAL/Platform.h"
#include "MemoryReportSectionData.h"
#include "MemoryReportConfigMemSectionData.h"
#include "MemoryReportResourceSizeSortSectionData.h"
#include "MemoryReportRHIMemoryDumpSectionData.h"

class MEMORYREPORTVISUALIZER_API MemoryReportAnalyzer
{
public:
	MemoryReportAnalyzer();
	~MemoryReportAnalyzer();

	bool AnalyzeMemoryReport(const FString& FilePath);
	
	const FString& GetHeader() const { return Header; }
	const TArray<TSharedPtr<SectionData>>& GetSections() const { return Sections; }
	const FString& GetFileContent() const { return FileContent; }

private:
	void ParseFileContent();
	TSharedPtr<SectionData> CreateSpecializedSection(const FString& SectionID, const FString& SectionContent);
	
	FString FileContent;
	FString Header;
	TArray<TSharedPtr<SectionData>> Sections;
};