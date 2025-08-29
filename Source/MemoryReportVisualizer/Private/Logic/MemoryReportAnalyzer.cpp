// Copyright Epic Games, Inc. All Rights Reserved.

#include "Logic/MemoryReportAnalyzer.h"
#include "Logic/MemoryReportConfigMemSectionData.h"
#include "Logic/MemoryReportResourceSizeSortSectionData.h"
#include "HAL/FileManager.h"
#include "Misc/FileHelper.h"

MemoryReportAnalyzer::MemoryReportAnalyzer()
{
}

MemoryReportAnalyzer::~MemoryReportAnalyzer()
{
}

bool MemoryReportAnalyzer::AnalyzeMemoryReport(const FString& FilePath)
{
	if (!IFileManager::Get().FileExists(*FilePath))
	{
		UE_LOG(LogTemp, Warning, TEXT("Memory report file does not exist: %s"), *FilePath);
		return false;
	}

	if (!FFileHelper::LoadFileToString(FileContent, *FilePath))
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to load memory report file: %s"), *FilePath);
		FileContent.Empty();
		return false;
	}

	UE_LOG(LogTemp, Log, TEXT("Successfully loaded memory report file: %s (Size: %d characters)"), *FilePath, FileContent.Len());
	
	ParseFileContent();
	
	UE_LOG(LogTemp, Log, TEXT("Parsed memory report: Header size: %d characters, Sections found: %d"), Header.Len(), Sections.Num());
	
	return true;
}

void MemoryReportAnalyzer::ParseFileContent()
{
	Header.Empty();
	Sections.Empty();
	
	if (FileContent.IsEmpty())
	{
		UE_LOG(LogTemp, Warning, TEXT("File content is empty, nothing to parse"));
		return;
	}
	
	const FString BeginMarker = TEXT("MemReport: Begin command ");
	const FString EndMarker = TEXT("MemReport: End command ");
	
	// Find the first section start to separate header
	int32 FirstSectionStart = FileContent.Find(BeginMarker);
	
	if (FirstSectionStart == INDEX_NONE)
	{
		// No sections found, entire content is header
		Header = FileContent;
		UE_LOG(LogTemp, Log, TEXT("No sections found in file, treating entire content as header"));
		return;
	}
	
	// Extract header (everything before first section)
	Header = FileContent.Left(FirstSectionStart);
	UE_LOG(LogTemp, Log, TEXT("Extracted header: %d characters"), Header.Len());
	
	// Parse sections
	int32 SearchStart = FirstSectionStart;
	
	while (SearchStart < FileContent.Len())
	{
		// Find section begin
		int32 SectionBeginPos = FileContent.Find(BeginMarker, ESearchCase::IgnoreCase, ESearchDir::FromStart, SearchStart);
		if (SectionBeginPos == INDEX_NONE)
		{
			break; // No more sections
		}
		
		// Extract section ID from begin marker line
		int32 LineEndPos = FileContent.Find(TEXT("\n"), ESearchCase::IgnoreCase, ESearchDir::FromStart, SectionBeginPos);
		if (LineEndPos == INDEX_NONE)
		{
			LineEndPos = FileContent.Len();
		}
		
		FString BeginLine = FileContent.Mid(SectionBeginPos, LineEndPos - SectionBeginPos);
		FString SectionID = BeginLine.Mid(BeginMarker.Len()).TrimStartAndEnd();
		
		if (SectionID.IsEmpty())
		{
			UE_LOG(LogTemp, Warning, TEXT("Found section begin marker without ID at position %d"), SectionBeginPos);
			SearchStart = LineEndPos + 1;
			continue;
		}
		
		// Find matching section end
		FString EndMarkerWithID = EndMarker + SectionID;
		int32 SectionEndPos = FileContent.Find(EndMarkerWithID, ESearchCase::IgnoreCase, ESearchDir::FromStart, SectionBeginPos);
		
		if (SectionEndPos == INDEX_NONE)
		{
			UE_LOG(LogTemp, Warning, TEXT("Section '%s' has no matching end marker"), *SectionID);
			SearchStart = LineEndPos + 1;
			continue;
		}
		
		// Find the end of the end marker line
		int32 EndLineEndPos = FileContent.Find(TEXT("\n"), ESearchCase::IgnoreCase, ESearchDir::FromStart, SectionEndPos);
		if (EndLineEndPos == INDEX_NONE)
		{
			EndLineEndPos = FileContent.Len();
		}
		
		// Extract section content (between begin and end markers, excluding the marker lines)
		int32 ContentStart = LineEndPos + 1; // Skip the begin marker line
		int32 ContentLength = SectionEndPos - ContentStart;
		
		if (ContentLength > 0)
		{
			FString SectionContent = FileContent.Mid(ContentStart, ContentLength);
			TSharedPtr<SectionData> Section = CreateSpecializedSection(SectionID, SectionContent);
			Sections.Add(Section);
			
			UE_LOG(LogTemp, Log, TEXT("Parsed section '%s': %d characters"), *SectionID, SectionContent.Len());
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Section '%s' has no content"), *SectionID);
		}
		
		// Move search start to after this section's end marker
		SearchStart = EndLineEndPos + 1;
	}
	
	UE_LOG(LogTemp, Log, TEXT("Parsing complete: %d sections extracted"), Sections.Num());
}

TSharedPtr<SectionData> MemoryReportAnalyzer::CreateSpecializedSection(const FString& SectionID, const FString& SectionContent)
{
	TSharedPtr<SectionData> Section;

	// Remove parentheses from SectionID if they exist
	FString InternalSectionID = SectionID.Replace(TEXT("\""), TEXT(""));
	
	// Create specialized section based on section ID
	if (InternalSectionID == TEXT("ConfigMem"))
	{
		// Create ConfigMem specialized section
		TSharedPtr<ConfigMemSectionData> ConfigMemSection = MakeShareable(new ConfigMemSectionData(InternalSectionID, SectionContent));
		ConfigMemSection->ExtractContentInternally();
		Section = ConfigMemSection;
		
		UE_LOG(LogTemp, Log, TEXT("Created specialized ConfigMem section with %d entries"), ConfigMemSection->ConfigMemEntries.Num());
	}
	else if (InternalSectionID == TEXT("obj list -resourcesizesort"))
	{
		// Create ResourceSizeSort specialized section
		TSharedPtr<ResourceSizeSortSectionData> ResourceSizeSortSection = MakeShareable(new ResourceSizeSortSectionData(InternalSectionID, SectionContent));
		ResourceSizeSortSection->ExtractContentInternally();
		Section = ResourceSizeSortSection;
		
		UE_LOG(LogTemp, Log, TEXT("Created specialized ResourceSizeSort section with %d entries"), ResourceSizeSortSection->ResourceSizeSortEntries.Num());
	}
	else
	{
		// Create generic section for unknown types
		Section = MakeShareable(new SectionData(InternalSectionID, SectionContent));
	}
	
	return Section;
}

