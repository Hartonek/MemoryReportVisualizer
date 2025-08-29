// Copyright Epic Games, Inc. All Rights Reserved.

#include "Widgets/MemoryReportFileVisualizerWidget.h"
#include "Widgets/Layout/SBox.h"
#include "Widgets/Text/STextBlock.h"
#include "Widgets/Layout/SBorder.h"
#include "Widgets/Layout/SScrollBox.h"
#include "Widgets/Layout/SSplitter.h"
#include "Widgets/SOverlay.h"
#include "Widgets/Input/SButton.h"
#include "Widgets/Input/SComboBox.h"
#include "DragAndDrop/DecoratedDragDropOp.h"
#include "DragAndDrop/AssetDragDropOp.h"
#include "Framework/Application/SlateApplication.h"
#include "HAL/PlatformFilemanager.h"
#include "Misc/FileHelper.h"
#include "Styling/AppStyle.h"
#include "DesktopPlatformModule.h"
#include "IDesktopPlatform.h"

#define LOCTEXT_NAMESPACE "SMemoryReportFileVisualizerWidget"

void SMemoryReportFileVisualizerWidget::Construct(const FArguments& InArgs)
{
	ChildSlot
	[
		SNew(SSplitter)
		.Orientation(Orient_Vertical)
		+ SSplitter::Slot()
		.Value(0.3f)
		[
			SNew(SBorder)
			.BorderImage(FAppStyle::GetBrush("ToolPanel.GroupBorder"))
			.Padding(20)
			[
				SNew(SBox)
				.MinDesiredHeight(200)
				.MinDesiredWidth(400)
				[
					SAssignNew(DropZone, SBorder)
					.BorderImage(FAppStyle::GetBrush("Graph.Node.Body"))
					.BorderBackgroundColor(FLinearColor(0.1f, 0.1f, 0.1f, 0.8f))
					.HAlign(HAlign_Center)
					.VAlign(VAlign_Center)
					.Padding(40)
					[
						SNew(SBox)
						.HAlign(HAlign_Center)
						.VAlign(VAlign_Center)
						[
							SAssignNew(StatusOverlay, SOverlay)
							+ SOverlay::Slot()
							.HAlign(HAlign_Center)
							.VAlign(VAlign_Center)
							[
								SAssignNew(StatusText, STextBlock)
								.Text(LOCTEXT("DropZoneText", "Drag & Drop Memory Report File Here\nor Click to Browse"))
								.Font(FAppStyle::GetFontStyle("PropertyWindow.NormalFont"))
								.ColorAndOpacity(FSlateColor::UseForeground())
								.Justification(ETextJustify::Center)
							]
							+ SOverlay::Slot()
							.HAlign(HAlign_Center)
							.VAlign(VAlign_Center)
							.Padding(0, 60, 0, 0)
							[
								SAssignNew(ProgressText, STextBlock)
								.Text(LOCTEXT("LoadingText", "Loading..."))
								.Font(FAppStyle::GetFontStyle("PropertyWindow.BoldFont"))
								.ColorAndOpacity(FLinearColor(0.8f, 0.8f, 0.2f, 1.0f))
								.Visibility(EVisibility::Hidden)
								.Justification(ETextJustify::Center)
							]
						]
					]
				]
			]
		]
		+ SSplitter::Slot()
		.Value(0.1f)
		[
			SAssignNew(SectionSelectionArea, SBorder)
			.BorderImage(FAppStyle::GetBrush("ToolPanel.GroupBorder"))
			.Padding(10)
			.Visibility(EVisibility::Collapsed)
			[
				SNew(SSplitter)
				.Orientation(Orient_Vertical)
				+ SSplitter::Slot()
				.Value(0.3f)
				[
					SNew(SBox)
					.Padding(0, 5)
					.VAlign(VAlign_Center)
					[
						SNew(STextBlock)
						.Text(LOCTEXT("SectionSelectionText", "Select Section:"))
						.Font(FAppStyle::GetFontStyle("PropertyWindow.NormalFont"))
					]
				]
				+ SSplitter::Slot()
				.Value(0.7f)
				[
					SAssignNew(SectionComboBox, SComboBox<TSharedPtr<FString>>)
					.OptionsSource(&SectionOptions)
					.OnGenerateWidget(this, &SMemoryReportFileVisualizerWidget::OnGenerateComboBoxItem)
					.OnSelectionChanged(this, &SMemoryReportFileVisualizerWidget::OnSectionSelectionChanged)
					[
						SNew(STextBlock)
						.Text_Lambda([this]() 
						{ 
							return SelectedSection.IsValid() ? FText::FromString(*SelectedSection) : LOCTEXT("SelectSectionText", "Select a section..."); 
						})
					]
				]
			]
		]
		+ SSplitter::Slot()
		.Value(0.6f)
		[
			SNew(SBorder)
			.BorderImage(FAppStyle::GetBrush("ToolPanel.GroupBorder"))
			.Padding(10)
			[
				SAssignNew(ContentScrollBox, SScrollBox)
				.Orientation(Orient_Vertical)
				.ScrollBarAlwaysVisible(true)
				+ SScrollBox::Slot()
				.Padding(5)
				[
					SAssignNew(ContentText, STextBlock)
					.Text(LOCTEXT("NoContentText", "File content will appear here after dropping a .memreport file"))
					.Font(FAppStyle::GetFontStyle("PropertyWindow.NormalFont"))
					.ColorAndOpacity(FSlateColor::UseForeground())
					.AutoWrapText(true)
				]
			]
		]
	];
}

FReply SMemoryReportFileVisualizerWidget::OnDrop(const FGeometry& MyGeometry, const FDragDropEvent& DragDropEvent)
{
	TSharedPtr<FDragDropOperation> Operation = DragDropEvent.GetOperation();
	
	if (Operation.IsValid() && Operation->IsOfType<FExternalDragOperation>())
	{
		TSharedPtr<FExternalDragOperation> ExternalDragOp = StaticCastSharedPtr<FExternalDragOperation>(Operation);
		
		if (ExternalDragOp.IsValid() && ExternalDragOp->HasFiles())
		{
			const TArray<FString>& Files = ExternalDragOp->GetFiles();
			if (Files.Num() > 0)
			{
				FString Extension = FPaths::GetExtension(Files[0]).ToLower();
				if (Extension == TEXT("memreport"))
				{
					OnFileDropped(Files[0]);
					return FReply::Handled();
				}
			}
		}
	}
	
	return FReply::Unhandled();
}

bool SMemoryReportFileVisualizerWidget::IsValidFileForDrop(const FDragDropEvent& DragDropEvent, FText& OutText) const
{
	TSharedPtr<FDragDropOperation> Operation = DragDropEvent.GetOperation();
	
	if (Operation.IsValid() && Operation->IsOfType<FExternalDragOperation>())
	{
		TSharedPtr<FExternalDragOperation> ExternalDragOp = StaticCastSharedPtr<FExternalDragOperation>(Operation);
		
		if (ExternalDragOp.IsValid() && ExternalDragOp->HasFiles())
		{
			const TArray<FString>& Files = ExternalDragOp->GetFiles();
			if (Files.Num() > 0)
			{
				FString Extension = FPaths::GetExtension(Files[0]).ToLower();
				if (Extension == TEXT("memreport"))
				{
					OutText = LOCTEXT("DropAllowed", "Drop to analyze memory report");
					return true;
				}
				else
				{
					OutText = LOCTEXT("DropNotAllowed", "Only .memreport files are supported");
					return false;
				}
			}
		}
	}
	
	OutText = LOCTEXT("DropNotAllowed", "Invalid file type");
	return false;
}

void SMemoryReportFileVisualizerWidget::OnDragEnter(const FGeometry& MyGeometry, const FDragDropEvent& DragDropEvent)
{
	if (DropZone.IsValid())
	{
		FText ValidationText;
		if (IsValidFileForDrop(DragDropEvent, ValidationText))
		{
			DropZone->SetBorderBackgroundColor(FLinearColor(0.0f, 0.5f, 0.0f, 0.8f));
			if (StatusText.IsValid())
			{
				StatusText->SetText(ValidationText);
			}
		}
		else
		{
			DropZone->SetBorderBackgroundColor(FLinearColor(0.5f, 0.0f, 0.0f, 0.8f));
			if (StatusText.IsValid())
			{
				StatusText->SetText(ValidationText);
			}
		}
	}
}

void SMemoryReportFileVisualizerWidget::OnDragLeave(const FDragDropEvent& DragDropEvent)
{
	if (DropZone.IsValid())
	{
		DropZone->SetBorderBackgroundColor(FLinearColor(0.1f, 0.1f, 0.1f, 0.8f));
	}
	if (StatusText.IsValid())
	{
		StatusText->SetText(LOCTEXT("DropZoneText", "Drag & Drop Memory Report File Here\nor Click to Browse"));
	}
}

FReply SMemoryReportFileVisualizerWidget::OnMouseButtonDown(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
{
	if (MouseEvent.GetEffectingButton() == EKeys::LeftMouseButton)
	{
		OpenFileDialog();
		return FReply::Handled();
	}
	return FReply::Unhandled();
}

void SMemoryReportFileVisualizerWidget::OpenFileDialog()
{
	IDesktopPlatform* DesktopPlatform = FDesktopPlatformModule::Get();
	if (DesktopPlatform)
	{
		TArray<FString> OutFilenames;
		const FString FileTypes = TEXT("Memory Report Files (*.memreport)|*.memreport");
		
		bool bOpened = DesktopPlatform->OpenFileDialog(
			FSlateApplication::Get().FindBestParentWindowHandleForDialogs(nullptr),
			LOCTEXT("OpenFileDialogTitle", "Select Memory Report File").ToString(),
			FString(),
			FString(),
			FileTypes,
			EFileDialogFlags::None,
			OutFilenames
		);
		
		if (bOpened && OutFilenames.Num() > 0)
		{
			OnFileDropped(OutFilenames[0]);
		}
	}
}

void SMemoryReportFileVisualizerWidget::ShowLoadingProgress()
{
	if (ProgressText.IsValid())
	{
		ProgressText->SetVisibility(EVisibility::Visible);
	}
	if (StatusText.IsValid())
	{
		StatusText->SetText(LOCTEXT("ProcessingFile", "Processing memory report..."));
	}
}

void SMemoryReportFileVisualizerWidget::HideLoadingProgress()
{
	if (ProgressText.IsValid())
	{
		ProgressText->SetVisibility(EVisibility::Hidden);
	}
}

void SMemoryReportFileVisualizerWidget::OnFileDropped(const FString& FilePath)
{
	ShowLoadingProgress();

	if (Analyzer.AnalyzeMemoryReport(FilePath))
	{
		if (FFileHelper::LoadFileToString(LoadedFileContent, *FilePath))
		{
			HideLoadingProgress();
			
			if (StatusText.IsValid())
			{
				FText StatusMessage = FText::Format(
					LOCTEXT("FileLoadedSuccess", "Successfully loaded: {0}\nFile size: {1} characters"),
					FText::FromString(FPaths::GetCleanFilename(FilePath)),
					FText::AsNumber(LoadedFileContent.Len())
				);
				StatusText->SetText(StatusMessage);
			}
			
			// Populate section dropdown and display content
			PopulateSectionDropdown();
			UpdateContentDisplay();
			
			UE_LOG(LogTemp, Log, TEXT("Memory report loaded successfully. Content length: %d"), LoadedFileContent.Len());
		}
		else
		{
			HideLoadingProgress();
			
			if (StatusText.IsValid())
			{
				StatusText->SetText(LOCTEXT("FileLoadError", "Error: Failed to load file content"));
			}
			
			// Clear content area on error
			if (ContentText.IsValid())
			{
				ContentText->SetText(LOCTEXT("NoContentText", "File content will appear here after dropping a .memreport file"));
			}
			
			// Hide section selection area
			if (SectionSelectionArea.IsValid())
			{
				SectionSelectionArea->SetVisibility(EVisibility::Collapsed);
			}
		}
	}
	else
	{
		HideLoadingProgress();
		
		if (StatusText.IsValid())
		{
			StatusText->SetText(LOCTEXT("AnalysisError", "Error: Failed to analyze memory report"));
		}
		
		// Clear content area on error
		if (ContentText.IsValid())
		{
			ContentText->SetText(LOCTEXT("NoContentText", "File content will appear here after dropping a .memreport file"));
		}
		
		// Hide section selection area
		if (SectionSelectionArea.IsValid())
		{
			SectionSelectionArea->SetVisibility(EVisibility::Collapsed);
		}
	}
}

void SMemoryReportFileVisualizerWidget::OnSectionSelectionChanged(TSharedPtr<FString> SelectedItem, ESelectInfo::Type SelectInfo)
{
	SelectedSection = SelectedItem;
	UpdateContentDisplay();
}

TSharedRef<SWidget> SMemoryReportFileVisualizerWidget::OnGenerateComboBoxItem(TSharedPtr<FString> InItem)
{
	return SNew(STextBlock)
		.Text(FText::FromString(InItem.IsValid() ? *InItem : TEXT("Invalid")))
		.Font(FAppStyle::GetFontStyle("PropertyWindow.NormalFont"));
}

void SMemoryReportFileVisualizerWidget::PopulateSectionDropdown()
{
	SectionOptions.Empty();
	
	// Add "Show All" option
	SectionOptions.Add(MakeShareable(new FString(TEXT("Show All"))));
	
	// Add header option if exists
	const FString& Header = Analyzer.GetHeader();
	if (!Header.IsEmpty())
	{
		SectionOptions.Add(MakeShareable(new FString(TEXT("Header"))));
	}
	
	// Add each section ID
	const TArray<SectionData>& Sections = Analyzer.GetSections();
	for (const SectionData& Section : Sections)
	{
		if (!Section.ID.IsEmpty())
		{
			SectionOptions.Add(MakeShareable(new FString(Section.ID)));
		}
	}
	
	// Refresh the combo box
	if (SectionComboBox.IsValid())
	{
		SectionComboBox->RefreshOptions();
		
		// Set default selection to "Show All"
		if (SectionOptions.Num() > 0)
		{
			SelectedSection = SectionOptions[0];
			SectionComboBox->SetSelectedItem(SelectedSection);
		}
	}
	
	// Show the section selection area if we have options
	if (SectionSelectionArea.IsValid())
	{
		SectionSelectionArea->SetVisibility(SectionOptions.Num() > 1 ? EVisibility::Visible : EVisibility::Collapsed);
	}
}

void SMemoryReportFileVisualizerWidget::UpdateContentDisplay()
{
	if (!ContentText.IsValid() || !SelectedSection.IsValid())
	{
		return;
	}
	
	FString ContentToDisplay;
	
	if (*SelectedSection == TEXT("Show All"))
	{
		// Display the entire file content
		ContentToDisplay = LoadedFileContent;
	}
	else if (*SelectedSection == TEXT("Header"))
	{
		// Display just the header
		ContentToDisplay = Analyzer.GetHeader();
	}
	else
	{
		// Find and display the specific section
		const TArray<SectionData>& Sections = Analyzer.GetSections();
		for (const SectionData& Section : Sections)
		{
			if (Section.ID == *SelectedSection)
			{
				ContentToDisplay = Section.Content;
				break;
			}
		}
		
		if (ContentToDisplay.IsEmpty())
		{
			ContentToDisplay = TEXT("Section not found or empty.");
		}
	}
	
	ContentText->SetText(FText::FromString(ContentToDisplay));
}

#undef LOCTEXT_NAMESPACE