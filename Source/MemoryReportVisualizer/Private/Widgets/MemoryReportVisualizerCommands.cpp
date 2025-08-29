// Copyright Epic Games, Inc. All Rights Reserved.

#include "Widgets/MemoryReportVisualizerCommands.h"

#define LOCTEXT_NAMESPACE "FMemoryReportVisualizerModule"

void FMemoryReportVisualizerCommands::RegisterCommands()
{
	UI_COMMAND(OpenPluginWindow, "MemoryReportVisualizer", "Bring up MemoryReportVisualizer window", EUserInterfaceActionType::Button, FInputChord());
}

#undef LOCTEXT_NAMESPACE
