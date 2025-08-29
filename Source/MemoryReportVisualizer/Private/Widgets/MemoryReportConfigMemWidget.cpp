// Copyright Epic Games, Inc. All Rights Reserved.

#include "Widgets/MemoryReportConfigMemWidget.h"
#include "Widgets/Layout/SBox.h"
#include "Widgets/Text/STextBlock.h"
#include "Widgets/Layout/SBorder.h"
#include "Styling/AppStyle.h"

#define LOCTEXT_NAMESPACE "SMemoryReportConfigMemWidget"

void SMemoryReportConfigMemWidget::Construct(const FArguments& InArgs)
{
	ChildSlot
	[
		SNew(SBorder)
		.BorderImage(FAppStyle::GetBrush("ToolPanel.GroupBorder"))
		.Padding(10)
		[
			SNew(SBox)
			.HAlign(HAlign_Center)
			.VAlign(VAlign_Center)
			[
				SNew(STextBlock)
				.Text(LOCTEXT("ConfigMemWidgetPlaceholder", "ConfigMem specialized view - implementation pending"))
				.Font(FAppStyle::GetFontStyle("PropertyWindow.NormalFont"))
				.ColorAndOpacity(FSlateColor::UseForeground())
				.Justification(ETextJustify::Center)
			]
		]
	];
}

void SMemoryReportConfigMemWidget::Populate(TSharedPtr<ConfigMemSectionData> ConfigMemData)
{
	ConfigMemSection = ConfigMemData;
	
	// TODO: Implement specialized ConfigMem data visualization
	// For now, this function is empty as requested
}

#undef LOCTEXT_NAMESPACE