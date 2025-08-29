// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "HAL/Platform.h"

class MEMORYREPORTVISUALIZER_API SectionData
{
public:
	SectionData() = default;
	SectionData(const FString& InID, const FString& InContent)
		: ID(InID), Content(InContent) {}

	FString ID;
	FString Content;
};

class MEMORYREPORTVISUALIZER_API MemoryReportAnalyzer
{
public:
	MemoryReportAnalyzer();
	~MemoryReportAnalyzer();

	bool AnalyzeMemoryReport(const FString& FilePath);
	
	const FString& GetHeader() const { return Header; }
	const TArray<SectionData>& GetSections() const { return Sections; }
	const FString& GetFileContent() const { return FileContent; }

private:
	void ParseFileContent();
	
	FString FileContent;
	FString Header;
	TArray<SectionData> Sections;
};