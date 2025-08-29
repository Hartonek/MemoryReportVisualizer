// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Framework/Commands/Commands.h"
#include "MemoryReportVisualizerStyle.h"

class FMemoryReportVisualizerCommands : public TCommands<FMemoryReportVisualizerCommands>
{
public:

	FMemoryReportVisualizerCommands()
		: TCommands<FMemoryReportVisualizerCommands>(TEXT("MemoryReportVisualizer"), NSLOCTEXT("Contexts", "MemoryReportVisualizer", "MemoryReportVisualizer Plugin"), NAME_None, FMemoryReportVisualizerStyle::GetStyleSetName())
	{
	}

	// TCommands<> interface
	virtual void RegisterCommands() override;

public:
	TSharedPtr< FUICommandInfo > OpenPluginWindow;
};