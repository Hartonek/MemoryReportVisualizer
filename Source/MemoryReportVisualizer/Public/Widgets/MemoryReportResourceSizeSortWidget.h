// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"
#include "Widgets/Views/SListView.h"
#include "Widgets/Views/STableViewBase.h"
#include "Widgets/Views/STableRow.h"
#include "Logic/MemoryReportResourceSizeSortSectionData.h"

class MEMORYREPORTVISUALIZER_API SMemoryReportResourceSizeSortWidget : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SMemoryReportResourceSizeSortWidget)
	{
	}
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs);

	// Populate the widget with ResourceSizeSort section data
	void Populate(TSharedPtr<ResourceSizeSortSectionData> ResourceSizeSortData);

private:
	// Table row generation
	TSharedRef<ITableRow> OnGenerateRow(TSharedPtr<ResourceSizeSortDescription> Item, const TSharedRef<STableViewBase>& OwnerTable);

	// Column sorting
	void OnColumnSortModeChanged(const EColumnSortPriority::Type SortPriority, const FName& ColumnId, const EColumnSortMode::Type InSortMode);
	EColumnSortMode::Type GetColumnSortMode(const FName ColumnId) const;

	// Reference to the ResourceSizeSort data
	TSharedPtr<ResourceSizeSortSectionData> ResourceSizeSortSection;
	
	// Table widget and data
	TSharedPtr<SListView<TSharedPtr<ResourceSizeSortDescription>>> ResourceSizeSortTable;
	TArray<TSharedPtr<ResourceSizeSortDescription>> ResourceSizeSortTableData;
	
	// Sorting state
	FName SortByColumn;
	EColumnSortMode::Type SortMode;
};