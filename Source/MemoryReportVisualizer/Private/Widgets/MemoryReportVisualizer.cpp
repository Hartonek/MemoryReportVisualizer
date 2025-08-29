// Copyright Epic Games, Inc. All Rights Reserved.

#include "Widgets/MemoryReportVisualizer.h"
#include "Widgets/MemoryReportVisualizerStyle.h"
#include "Widgets/MemoryReportVisualizerCommands.h"
#include "Widgets/MemoryReportVisualizerWindow.h"
#include "LevelEditor.h"
#include "Widgets/Docking/SDockTab.h"
#include "ToolMenus.h"

static const FName MemoryReportVisualizerTabName("MemoryReportVisualizer");

#define LOCTEXT_NAMESPACE "FMemoryReportVisualizerModule"

void FMemoryReportVisualizerModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
	
	FMemoryReportVisualizerStyle::Initialize();
	FMemoryReportVisualizerStyle::ReloadTextures();

	FMemoryReportVisualizerCommands::Register();
	
	PluginCommands = MakeShareable(new FUICommandList);

	PluginCommands->MapAction(
		FMemoryReportVisualizerCommands::Get().OpenPluginWindow,
		FExecuteAction::CreateRaw(this, &FMemoryReportVisualizerModule::PluginButtonClicked),
		FCanExecuteAction());

	UToolMenus::RegisterStartupCallback(FSimpleMulticastDelegate::FDelegate::CreateRaw(this, &FMemoryReportVisualizerModule::RegisterMenus));
	
	FGlobalTabmanager::Get()->RegisterNomadTabSpawner(MemoryReportVisualizerTabName, FOnSpawnTab::CreateRaw(this, &FMemoryReportVisualizerModule::OnSpawnPluginTab))
		.SetDisplayName(LOCTEXT("FMemoryReportVisualizerTabTitle", "MemoryReportVisualizer"))
		.SetMenuType(ETabSpawnerMenuType::Hidden);
}

void FMemoryReportVisualizerModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.

	UToolMenus::UnRegisterStartupCallback(this);

	UToolMenus::UnregisterOwner(this);

	FMemoryReportVisualizerStyle::Shutdown();

	FMemoryReportVisualizerCommands::Unregister();

	FGlobalTabmanager::Get()->UnregisterNomadTabSpawner(MemoryReportVisualizerTabName);
}

TSharedRef<SDockTab> FMemoryReportVisualizerModule::OnSpawnPluginTab(const FSpawnTabArgs& SpawnTabArgs)
{
	return SMemoryReportVisualizerWindow::CreateTab(SpawnTabArgs);
}

void FMemoryReportVisualizerModule::PluginButtonClicked()
{
	FGlobalTabmanager::Get()->TryInvokeTab(MemoryReportVisualizerTabName);
}

void FMemoryReportVisualizerModule::RegisterMenus()
{
	// Owner will be used for cleanup in call to UToolMenus::UnregisterOwner
	FToolMenuOwnerScoped OwnerScoped(this);

	{
		UToolMenu* Menu = UToolMenus::Get()->ExtendMenu("LevelEditor.MainMenu.Window");
		{
			FToolMenuSection& Section = Menu->FindOrAddSection("WindowLayout");
			Section.AddMenuEntryWithCommandList(FMemoryReportVisualizerCommands::Get().OpenPluginWindow, PluginCommands);
		}
	}

	{
		UToolMenu* ToolbarMenu = UToolMenus::Get()->ExtendMenu("LevelEditor.LevelEditorToolBar.PlayToolBar");
		{
			FToolMenuSection& Section = ToolbarMenu->FindOrAddSection("PluginTools");
			{
				FToolMenuEntry& Entry = Section.AddEntry(FToolMenuEntry::InitToolBarButton(FMemoryReportVisualizerCommands::Get().OpenPluginWindow));
				Entry.SetCommandList(PluginCommands);
			}
		}
	}
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FMemoryReportVisualizerModule, MemoryReportVisualizer)