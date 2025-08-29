// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"
#include "Input/Reply.h"
#include "Input/DragAndDrop.h"
#include "DragAndDrop/DecoratedDragDropOp.h"
#include "Logic/MemoryReportAnalyzer.h"
#include "Widgets/MemoryReportConfigMemWidget.h"
#include "Widgets/MemoryReportResourceSizeSortWidget.h"

class MEMORYREPORTVISUALIZER_API SMemoryReportFileVisualizerWidget : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SMemoryReportFileVisualizerWidget)
	{}
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs);

protected:
	virtual FReply OnDrop(const FGeometry& MyGeometry, const FDragDropEvent& DragDropEvent) override;
	virtual void OnDragEnter(const FGeometry& MyGeometry, const FDragDropEvent& DragDropEvent) override;
	virtual void OnDragLeave(const FDragDropEvent& DragDropEvent) override;
	virtual FReply OnMouseButtonDown(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) override;
	
	bool IsValidFileForDrop(const FDragDropEvent& DragDropEvent, FText& OutText) const;

private:
	void OnFileDropped(const FString& FilePath);
	void OpenFileDialog();
	void ShowLoadingProgress();
	void HideLoadingProgress();
	void OnSectionSelectionChanged(TSharedPtr<FString> SelectedItem, ESelectInfo::Type SelectInfo);
	TSharedRef<SWidget> OnGenerateComboBoxItem(TSharedPtr<FString> InItem);
	void PopulateSectionDropdown();
	void UpdateContentDisplay();
	
	TSharedPtr<SBorder> DropZone;
	TSharedPtr<STextBlock> StatusText;
	TSharedPtr<SScrollBox> ContentScrollBox;
	TSharedPtr<STextBlock> ContentText;
	TSharedPtr<STextBlock> ProgressText;
	TSharedPtr<SOverlay> StatusOverlay;
	TSharedPtr<class SComboBox<TSharedPtr<FString>>> SectionComboBox;
	TSharedPtr<SBorder> SectionSelectionArea;
	TSharedPtr<SMemoryReportConfigMemWidget> ConfigMemWidget;
	TSharedPtr<SMemoryReportResourceSizeSortWidget> ResourceSizeSortWidget;
	TArray<TSharedPtr<FString>> SectionOptions;
	TSharedPtr<FString> SelectedSection;
	MemoryReportAnalyzer Analyzer;
	FString LoadedFileContent;
};