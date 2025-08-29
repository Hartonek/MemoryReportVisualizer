// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"
#include "Logic/MemoryReportConfigMemSectionData.h"

class MEMORYREPORTVISUALIZER_API SMemoryReportConfigMemWidget : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SMemoryReportConfigMemWidget)
	{
	}
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs);

	// Populate the widget with ConfigMem section data
	void Populate(TSharedPtr<ConfigMemSectionData> ConfigMemData);

private:
	// Reference to the ConfigMem data
	TSharedPtr<ConfigMemSectionData> ConfigMemSection;
};