// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"
#include "Widgets/Docking/SDockTab.h"

class MEMORYREPORTVISUALIZER_API SMemoryReportVisualizerWindow : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SMemoryReportVisualizerWindow)
	{}
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs);

	static TSharedRef<SDockTab> CreateTab(const FSpawnTabArgs& SpawnTabArgs);

private:
	TSharedPtr<class SMemoryReportFileVisualizerWidget> FileVisualizerWidget;
};