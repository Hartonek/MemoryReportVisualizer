// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"
#include "Widgets/Views/SListView.h"
#include "Widgets/Views/STableViewBase.h"
#include "Widgets/Views/STableRow.h"
#include "Logic/MemoryReportConfigMemSectionData.h"

class MEMORYREPORTVISUALIZER_API SMemoryReportConfigMemWidget : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SMemoryReportConfigMemWidget)
	{
	}
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs);

	// Populate the widget with ConfigMem section data
	void Populate(TSharedPtr<ConfigMemSectionData> ConfigMemData);

private:
	// Table row generation
	TSharedRef<ITableRow> OnGenerateRow(TSharedPtr<ConfigMemDescription> Item, const TSharedRef<STableViewBase>& OwnerTable);

	// Column sorting
	void OnColumnSortModeChanged(const EColumnSortPriority::Type SortPriority, const FName& ColumnId, const EColumnSortMode::Type InSortMode);
	EColumnSortMode::Type GetColumnSortMode(const FName ColumnId) const;

	// Reference to the ConfigMem data
	TSharedPtr<ConfigMemSectionData> ConfigMemSection;
	
	// Table widget and data
	TSharedPtr<SListView<TSharedPtr<ConfigMemDescription>>> ConfigMemTable;
	TArray<TSharedPtr<ConfigMemDescription>> ConfigMemTableData;
	
	// Sorting state
	FName SortByColumn;
	EColumnSortMode::Type SortMode;
};