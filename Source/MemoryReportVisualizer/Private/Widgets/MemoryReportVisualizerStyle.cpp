// Copyright Epic Games, Inc. All Rights Reserved.

#include "Widgets/MemoryReportVisualizerStyle.h"
#include "Styling/SlateStyleRegistry.h"
#include "Framework/Application/SlateApplication.h"
#include "Slate/SlateGameResources.h"
#include "Interfaces/IPluginManager.h"
#include "Styling/SlateStyleMacros.h"

#define RootToContentDir Style->RootToContentDir

TSharedPtr<FSlateStyleSet> FMemoryReportVisualizerStyle::StyleInstance = nullptr;

void FMemoryReportVisualizerStyle::Initialize()
{
	if (!StyleInstance.IsValid())
	{
		StyleInstance = Create();
		FSlateStyleRegistry::RegisterSlateStyle(*StyleInstance);
	}
}

void FMemoryReportVisualizerStyle::Shutdown()
{
	FSlateStyleRegistry::UnRegisterSlateStyle(*StyleInstance);
	ensure(StyleInstance.IsUnique());
	StyleInstance.Reset();
}

FName FMemoryReportVisualizerStyle::GetStyleSetName()
{
	static FName StyleSetName(TEXT("MemoryReportVisualizerStyle"));
	return StyleSetName;
}

const FVector2D Icon16x16(16.0f, 16.0f);
const FVector2D Icon20x20(20.0f, 20.0f);

TSharedRef< FSlateStyleSet > FMemoryReportVisualizerStyle::Create()
{
	TSharedRef< FSlateStyleSet > Style = MakeShareable(new FSlateStyleSet("MemoryReportVisualizerStyle"));
	Style->SetContentRoot(IPluginManager::Get().FindPlugin("MemoryReportVisualizer")->GetBaseDir() / TEXT("Resources"));

	Style->Set("MemoryReportVisualizer.OpenPluginWindow", new IMAGE_BRUSH_SVG(TEXT("PlaceholderButtonIcon"), Icon20x20));

	return Style;
}

void FMemoryReportVisualizerStyle::ReloadTextures()
{
	if (FSlateApplication::IsInitialized())
	{
		FSlateApplication::Get().GetRenderer()->ReloadTextureResources();
	}
}

const ISlateStyle& FMemoryReportVisualizerStyle::Get()
{
	return *StyleInstance;
}
