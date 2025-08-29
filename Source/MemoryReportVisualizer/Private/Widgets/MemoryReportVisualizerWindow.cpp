// Copyright Epic Games, Inc. All Rights Reserved.

#include "Widgets/MemoryReportVisualizerWindow.h"
#include "Widgets/MemoryReportFileVisualizerWidget.h"
#include "Widgets/Layout/SBox.h"
#include "Widgets/Docking/SDockTab.h"

#define LOCTEXT_NAMESPACE "SMemoryReportVisualizerWindow"

void SMemoryReportVisualizerWindow::Construct(const FArguments& InArgs)
{
	ChildSlot
	[
		SNew(SBox)
		.Padding(10)
		[
			SAssignNew(FileVisualizerWidget, SMemoryReportFileVisualizerWidget)
		]
	];
}

TSharedRef<SDockTab> SMemoryReportVisualizerWindow::CreateTab(const FSpawnTabArgs& SpawnTabArgs)
{
	return SNew(SDockTab)
		.TabRole(ETabRole::NomadTab)
		[
			SNew(SMemoryReportVisualizerWindow)
		];
}

#undef LOCTEXT_NAMESPACE