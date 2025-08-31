// Copyright Epic Games, Inc. All Rights Reserved.

#include "Logic/MemoryReportRHIMemoryDumpSectionData.h"
#include "HAL/FileManager.h"
#include "Misc/FileHelper.h"

void RHIMemoryDumpSectionData::ExtractContentInternally()
{
	RHIMemoryDumpEntries.Empty();
	TotalAmountOfMemory.Empty();
	
	if (Content.IsEmpty())
	{
		UE_LOG(LogTemp, Warning, TEXT("RHIMemoryDump section content is empty"));
		return;
	}
	
	// Split content into lines for processing
	TArray<FString> Lines;
	Content.ParseIntoArrayLines(Lines, true); // true = to cull empty lines
	
	if (Lines.Num() < 2)
	{
		UE_LOG(LogTemp, Warning, TEXT("RHIMemoryDump section has insufficient lines (%d), expected at least 2 (header + footer)"), Lines.Num());
		return;
	}
	
	// Skip first line (header)
	int32 StartIndex = 1;
	
	// Remember last line as TotalAmountOfMemory (footer)
	int32 EndIndex = Lines.Num() - 1;
	TotalAmountOfMemory = Lines[EndIndex].TrimStartAndEnd();
	
	// Remove "total" word and add space before "MB"
	TotalAmountOfMemory = TotalAmountOfMemory.Replace(TEXT("total"), TEXT(""), ESearchCase::IgnoreCase);
	TotalAmountOfMemory = TotalAmountOfMemory.Replace(TEXT("MB"), TEXT(" MB"));
	TotalAmountOfMemory = TotalAmountOfMemory.TrimStartAndEnd();
	
	// Parse middle lines with format: "floatMB - description"
	for (int32 i = StartIndex; i < EndIndex; i++)
	{
		FString Line = Lines[i].TrimStartAndEnd();
		if (Line.IsEmpty())
		{
			continue;
		}
		
		// Find the pattern: number + "MB - "
		int32 MBIndex = Line.Find(TEXT("MB"));
		if (MBIndex == INDEX_NONE)
		{
			UE_LOG(LogTemp, Warning, TEXT("RHIMemoryDump line missing 'MB' pattern: %s"), *Line);
			continue;
		}
		
		// Extract the float number before "MB"
		FString NumberPart = Line.Left(MBIndex).TrimStartAndEnd();
		float MemoryMB = FCString::Atof(*NumberPart);
		
		// Find the " - " separator after "MB"
		FString AfterMB = Line.Mid(MBIndex + 2); // Skip "MB"
		int32 SeparatorIndex = AfterMB.Find(TEXT(" - "));
		if (SeparatorIndex == INDEX_NONE)
		{
			UE_LOG(LogTemp, Warning, TEXT("RHIMemoryDump line missing ' - ' separator: %s"), *Line);
			continue;
		}
		
		// Extract description after " - "
		FString Description = AfterMB.Mid(SeparatorIndex + 3).TrimStartAndEnd();
		
		// Create entry and add to array
		RHIMemoryDumpDescription Entry(MemoryMB, Description);
		RHIMemoryDumpEntries.Add(Entry);
	}
	
	UE_LOG(LogTemp, Log, TEXT("RHIMemoryDump section parsed: %d entries, TotalAmountOfMemory: %s"), 
		RHIMemoryDumpEntries.Num(), *TotalAmountOfMemory);
}