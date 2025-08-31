// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"
#include "Widgets/Views/SListView.h"
#include "Widgets/Views/STableViewBase.h"
#include "Widgets/Views/STableRow.h"
#include "Widgets/Text/STextBlock.h"
#include "Logic/MemoryReportRHIMemoryDumpSectionData.h"

class MEMORYREPORTVISUALIZER_API SMemoryReportRHIMemoryDumpWidget : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SMemoryReportRHIMemoryDumpWidget)
	{
	}
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs);

	// Populate the widget with RHIMemoryDump section data
	void Populate(TSharedPtr<RHIMemoryDumpSectionData> RHIMemoryDumpData);

private:
	// Table row generation
	TSharedRef<ITableRow> OnGenerateRow(TSharedPtr<RHIMemoryDumpDescription> Item, const TSharedRef<STableViewBase>& OwnerTable);

	// Column sorting
	void OnColumnSortModeChanged(const EColumnSortPriority::Type SortPriority, const FName& ColumnId, const EColumnSortMode::Type InSortMode);
	EColumnSortMode::Type GetColumnSortMode(const FName ColumnId) const;

	// Reference to the RHIMemoryDump data
	TSharedPtr<RHIMemoryDumpSectionData> RHIMemoryDumpSection;
	
	// Table widget and data
	TSharedPtr<SListView<TSharedPtr<RHIMemoryDumpDescription>>> RHIMemoryDumpTable;
	TArray<TSharedPtr<RHIMemoryDumpDescription>> RHIMemoryDumpTableData;
	
	// TotalAmountOfMemory text widget
	TSharedPtr<STextBlock> TotalAmountOfMemoryText;
	
	// Sorting state
	FName SortByColumn;
	EColumnSortMode::Type SortMode;
};