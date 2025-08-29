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
	
	virtual ~SectionData() = default;

	// Virtual function to extract content internally - to be overridden by child classes
	virtual void ExtractContentInternally() {}

	FString ID;
	FString Content;
};