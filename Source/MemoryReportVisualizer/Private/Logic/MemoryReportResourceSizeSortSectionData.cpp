// Copyright Epic Games, Inc. All Rights Reserved.

#include "Logic/MemoryReportResourceSizeSortSectionData.h"
#include "HAL/FileManager.h"
#include "Misc/FileHelper.h"

void ResourceSizeSortSectionData::ExtractContentInternally()
{
	ResourceSizeSortEntries.Empty();
	
	if (Content.IsEmpty())
	{
		UE_LOG(LogTemp, Warning, TEXT("ResourceSizeSort section content is empty"));
		return;
	}
	
	// Split content into lines
	TArray<FString> Lines;
	Content.ParseIntoArrayLines(Lines, true); // true = cull empty lines
	
	// Skip the first two lines (header information)
	int32 StartIndex = FMath::Min(2, Lines.Num());
	
	// Parse each line looking for ResourceSizeSort entries
	// Expected format: "Class Count NumKB MaxKB ResExcKB ResExcDedSysKB ResExcDedVidKB ResExcUnkKB"
	for (int32 i = StartIndex; i < Lines.Num(); i++)
	{
		FString TrimmedLine = Lines[i].TrimStartAndEnd();
		if (TrimmedLine.IsEmpty())
		{
			// Skip empty lines
			continue;
		}
		
		// Split line into eight parts based on whitespaces
		TArray<FString> Components;
		TrimmedLine.ParseIntoArray(Components, TEXT(" "), true); // true = cull empty strings
		
		// We expect exactly 8 components: Class, Count, NumKB, MaxKB, ResExcKB, ResExcDedSysKB, ResExcDedVidKB, ResExcUnkKB
		if (Components.Num() >= 8)
		{
			FString Class = Components[0];
			int32 Count = FCString::Atoi(*Components[1]);
			float NumKB = FCString::Atof(*Components[2]);
			float MaxKB = FCString::Atof(*Components[3]);
			float ResExcKB = FCString::Atof(*Components[4]);
			float ResExcDedSysKB = FCString::Atof(*Components[5]);
			float ResExcDedVidKB = FCString::Atof(*Components[6]);
			float ResExcUnkKB = FCString::Atof(*Components[7]);
			
			ResourceSizeSortDescription Entry(Class, Count, NumKB, MaxKB, ResExcKB, ResExcDedSysKB, ResExcDedVidKB, ResExcUnkKB);
			ResourceSizeSortEntries.Add(Entry);
			
			UE_LOG(LogTemp, VeryVerbose, TEXT("Parsed ResourceSizeSort entry: %s (Count: %d, NumKB: %.2f)"), *Class, Count, NumKB);
		}
		else if (Components.Num() > 0)
		{
			UE_LOG(LogTemp, Warning, TEXT("ResourceSizeSort line has unexpected format: %s"), *TrimmedLine);
		}
	}
	
	UE_LOG(LogTemp, Log, TEXT("ResourceSizeSort section parsed: %d entries found"), ResourceSizeSortEntries.Num());
}