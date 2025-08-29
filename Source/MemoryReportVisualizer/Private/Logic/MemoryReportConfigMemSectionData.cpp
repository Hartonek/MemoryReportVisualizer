// Copyright Epic Games, Inc. All Rights Reserved.

#include "Logic/MemoryReportConfigMemSectionData.h"
#include "HAL/FileManager.h"
#include "Misc/FileHelper.h"

void ConfigMemSectionData::ExtractContentInternally()
{
	ConfigMemEntries.Empty();
	
	if (Content.IsEmpty())
	{
		UE_LOG(LogTemp, Warning, TEXT("ConfigMem section content is empty"));
		return;
	}
	
	// Split content into lines
	TArray<FString> Lines;
	Content.ParseIntoArrayLines(Lines, true); // true = cull empty lines
	
	// Skip the first two lines (header information)
	int32 StartIndex = FMath::Min(2, Lines.Num());
	
	// Parse each line looking for ConfigMem entries
	// Expected format: "FileName NumBytes MaxBytes"
	for (int32 i = StartIndex; i < Lines.Num(); i++)
	{
		FString TrimmedLine = Lines[i].TrimStartAndEnd();
		if (TrimmedLine.IsEmpty())
		{
			// Skip empty lines
			continue;
		}
		
		// Split line into three parts based on whitespaces
		TArray<FString> Components;
		TrimmedLine.ParseIntoArray(Components, TEXT(" "), true); // true = cull empty strings
		
		// We expect exactly 3 components: FileName, NumBytes, MaxBytes
		if (Components.Num() >= 3)
		{
			FString FileName = Components[0];
			int32 NumBytes = FCString::Atoi(*Components[1]);
			int32 MaxBytes = FCString::Atoi(*Components[2]);
			
			ConfigMemDescription Entry(FileName, NumBytes, MaxBytes);
			ConfigMemEntries.Add(Entry);
			
			UE_LOG(LogTemp, VeryVerbose, TEXT("Parsed ConfigMem entry: %s (%d/%d bytes)"), *FileName, NumBytes, MaxBytes);
		}
		else if (Components.Num() > 0)
		{
			UE_LOG(LogTemp, Warning, TEXT("ConfigMem line has unexpected format: %s"), *TrimmedLine);
		}
	}
	
	UE_LOG(LogTemp, Log, TEXT("ConfigMem section parsed: %d entries found"), ConfigMemEntries.Num());
}