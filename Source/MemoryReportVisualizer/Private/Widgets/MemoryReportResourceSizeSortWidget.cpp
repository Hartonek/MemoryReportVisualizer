// Copyright Epic Games, Inc. All Rights Reserved.

#include "Widgets/MemoryReportResourceSizeSortWidget.h"
#include "Widgets/Layout/SBox.h"
#include "Widgets/Text/STextBlock.h"
#include "Widgets/Layout/SBorder.h"
#include "Widgets/Views/SHeaderRow.h"
#include "Widgets/Views/SListView.h"
#include "Widgets/Views/STableRow.h"
#include "Styling/AppStyle.h"

#define LOCTEXT_NAMESPACE "SMemoryReportResourceSizeSortWidget"

void SMemoryReportResourceSizeSortWidget::Construct(const FArguments& InArgs)
{
	// Initialize sorting state
	SortByColumn = FName("Class");
	SortMode = EColumnSortMode::Ascending;

	ChildSlot
	[
		SNew(SBorder)
		.BorderImage(FAppStyle::GetBrush("ToolPanel.GroupBorder"))
		.Padding(10)
		[
			SAssignNew(ResourceSizeSortTable, SListView<TSharedPtr<ResourceSizeSortDescription>>)
			.ListItemsSource(&ResourceSizeSortTableData)
			.OnGenerateRow(this, &SMemoryReportResourceSizeSortWidget::OnGenerateRow)
			.HeaderRow
			(
				SNew(SHeaderRow)
				+ SHeaderRow::Column(FName("Class"))
				.DefaultLabel(LOCTEXT("ClassColumnHeader", "Class"))
				.SortMode(this, &SMemoryReportResourceSizeSortWidget::GetColumnSortMode, FName("Class"))
				.OnSort(this, &SMemoryReportResourceSizeSortWidget::OnColumnSortModeChanged)
				.FillWidth(0.25f)
				
				+ SHeaderRow::Column(FName("Count"))
				.DefaultLabel(LOCTEXT("CountColumnHeader", "Count"))
				.SortMode(this, &SMemoryReportResourceSizeSortWidget::GetColumnSortMode, FName("Count"))
				.OnSort(this, &SMemoryReportResourceSizeSortWidget::OnColumnSortModeChanged)
				.FillWidth(0.1f)
				
				+ SHeaderRow::Column(FName("NumKB"))
				.DefaultLabel(LOCTEXT("NumKBColumnHeader", "Num KB"))
				.SortMode(this, &SMemoryReportResourceSizeSortWidget::GetColumnSortMode, FName("NumKB"))
				.OnSort(this, &SMemoryReportResourceSizeSortWidget::OnColumnSortModeChanged)
				.FillWidth(0.1f)
				
				+ SHeaderRow::Column(FName("MaxKB"))
				.DefaultLabel(LOCTEXT("MaxKBColumnHeader", "Max KB"))
				.SortMode(this, &SMemoryReportResourceSizeSortWidget::GetColumnSortMode, FName("MaxKB"))
				.OnSort(this, &SMemoryReportResourceSizeSortWidget::OnColumnSortModeChanged)
				.FillWidth(0.1f)
				
				+ SHeaderRow::Column(FName("ResExcKB"))
				.DefaultLabel(LOCTEXT("ResExcKBColumnHeader", "ResExc KB"))
				.SortMode(this, &SMemoryReportResourceSizeSortWidget::GetColumnSortMode, FName("ResExcKB"))
				.OnSort(this, &SMemoryReportResourceSizeSortWidget::OnColumnSortModeChanged)
				.FillWidth(0.11f)
				
				+ SHeaderRow::Column(FName("ResExcDedSysKB"))
				.DefaultLabel(LOCTEXT("ResExcDedSysKBColumnHeader", "ResExcDedSys KB"))
				.SortMode(this, &SMemoryReportResourceSizeSortWidget::GetColumnSortMode, FName("ResExcDedSysKB"))
				.OnSort(this, &SMemoryReportResourceSizeSortWidget::OnColumnSortModeChanged)
				.FillWidth(0.11f)
				
				+ SHeaderRow::Column(FName("ResExcDedVidKB"))
				.DefaultLabel(LOCTEXT("ResExcDedVidKBColumnHeader", "ResExcDedVid KB"))
				.SortMode(this, &SMemoryReportResourceSizeSortWidget::GetColumnSortMode, FName("ResExcDedVidKB"))
				.OnSort(this, &SMemoryReportResourceSizeSortWidget::OnColumnSortModeChanged)
				.FillWidth(0.11f)
				
				+ SHeaderRow::Column(FName("ResExcUnkKB"))
				.DefaultLabel(LOCTEXT("ResExcUnkKBColumnHeader", "ResExcUnk KB"))
				.SortMode(this, &SMemoryReportResourceSizeSortWidget::GetColumnSortMode, FName("ResExcUnkKB"))
				.OnSort(this, &SMemoryReportResourceSizeSortWidget::OnColumnSortModeChanged)
				.FillWidth(0.11f)
			)
		]
	];
}

void SMemoryReportResourceSizeSortWidget::Populate(TSharedPtr<ResourceSizeSortSectionData> ResourceSizeSortData)
{
	ResourceSizeSortSection = ResourceSizeSortData;
	
	// Clear existing data
	ResourceSizeSortTableData.Empty();
	
	if (ResourceSizeSortSection.IsValid())
	{
		// Convert ResourceSizeSortEntries to shared pointers for the table
		for (const ResourceSizeSortDescription& Entry : ResourceSizeSortSection->ResourceSizeSortEntries)
		{
			ResourceSizeSortTableData.Add(MakeShareable(new ResourceSizeSortDescription(Entry)));
		}
		
		// Refresh the table view
		if (ResourceSizeSortTable.IsValid())
		{
			ResourceSizeSortTable->RequestListRefresh();
		}
	}
}

TSharedRef<ITableRow> SMemoryReportResourceSizeSortWidget::OnGenerateRow(TSharedPtr<ResourceSizeSortDescription> Item, const TSharedRef<STableViewBase>& OwnerTable)
{
	class SResourceSizeSortTableRow : public SMultiColumnTableRow<TSharedPtr<ResourceSizeSortDescription>>
	{
	public:
		SLATE_BEGIN_ARGS(SResourceSizeSortTableRow) {}
		SLATE_ARGUMENT(TSharedPtr<ResourceSizeSortDescription>, Item)
		SLATE_END_ARGS()

		void Construct(const FArguments& InArgs, const TSharedRef<STableViewBase>& InOwnerTableView)
		{
			Item = InArgs._Item;
			SMultiColumnTableRow<TSharedPtr<ResourceSizeSortDescription>>::Construct(
				FSuperRowType::FArguments(),
				InOwnerTableView
			);
		}

		virtual TSharedRef<SWidget> GenerateWidgetForColumn(const FName& ColumnName) override
		{
			if (ColumnName == FName("Class"))
			{
				return SNew(STextBlock)
					.Text(FText::FromString(Item.IsValid() ? Item->Class : TEXT("")))
					.Font(FAppStyle::GetFontStyle("PropertyWindow.NormalFont"));
			}
			else if (ColumnName == FName("Count"))
			{
				return SNew(STextBlock)
					.Text(FText::AsNumber(Item.IsValid() ? Item->Count : 0))
					.Font(FAppStyle::GetFontStyle("PropertyWindow.NormalFont"))
					.Justification(ETextJustify::Right);
			}
			else if (ColumnName == FName("NumKB"))
			{
				return SNew(STextBlock)
					.Text(FText::AsNumber(Item.IsValid() ? Item->NumKB : 0.0f))
					.Font(FAppStyle::GetFontStyle("PropertyWindow.NormalFont"))
					.Justification(ETextJustify::Right);
			}
			else if (ColumnName == FName("MaxKB"))
			{
				return SNew(STextBlock)
					.Text(FText::AsNumber(Item.IsValid() ? Item->MaxKB : 0.0f))
					.Font(FAppStyle::GetFontStyle("PropertyWindow.NormalFont"))
					.Justification(ETextJustify::Right);
			}
			else if (ColumnName == FName("ResExcKB"))
			{
				return SNew(STextBlock)
					.Text(FText::AsNumber(Item.IsValid() ? Item->ResExcKB : 0.0f))
					.Font(FAppStyle::GetFontStyle("PropertyWindow.NormalFont"))
					.Justification(ETextJustify::Right);
			}
			else if (ColumnName == FName("ResExcDedSysKB"))
			{
				return SNew(STextBlock)
					.Text(FText::AsNumber(Item.IsValid() ? Item->ResExcDedSysKB : 0.0f))
					.Font(FAppStyle::GetFontStyle("PropertyWindow.NormalFont"))
					.Justification(ETextJustify::Right);
			}
			else if (ColumnName == FName("ResExcDedVidKB"))
			{
				return SNew(STextBlock)
					.Text(FText::AsNumber(Item.IsValid() ? Item->ResExcDedVidKB : 0.0f))
					.Font(FAppStyle::GetFontStyle("PropertyWindow.NormalFont"))
					.Justification(ETextJustify::Right);
			}
			else if (ColumnName == FName("ResExcUnkKB"))
			{
				return SNew(STextBlock)
					.Text(FText::AsNumber(Item.IsValid() ? Item->ResExcUnkKB : 0.0f))
					.Font(FAppStyle::GetFontStyle("PropertyWindow.NormalFont"))
					.Justification(ETextJustify::Right);
			}

			return SNullWidget::NullWidget;
		}

	private:
		TSharedPtr<ResourceSizeSortDescription> Item;
	};

	return SNew(SResourceSizeSortTableRow, OwnerTable)
		.Item(Item);
}

void SMemoryReportResourceSizeSortWidget::OnColumnSortModeChanged(const EColumnSortPriority::Type SortPriority, const FName& ColumnId, const EColumnSortMode::Type InSortMode)
{
	SortByColumn = ColumnId;
	SortMode = InSortMode;
	
	// Sort the data based on the selected column
	if (SortMode != EColumnSortMode::None)
	{
		ResourceSizeSortTableData.Sort([this](const TSharedPtr<ResourceSizeSortDescription>& A, const TSharedPtr<ResourceSizeSortDescription>& B) -> bool
		{
			if (!A.IsValid() || !B.IsValid())
			{
				return false;
			}
			
			bool bResult = false;
			if (SortByColumn == FName("Class"))
			{
				bResult = A->Class < B->Class;
			}
			else if (SortByColumn == FName("Count"))
			{
				bResult = A->Count < B->Count;
			}
			else if (SortByColumn == FName("NumKB"))
			{
				bResult = A->NumKB < B->NumKB;
			}
			else if (SortByColumn == FName("MaxKB"))
			{
				bResult = A->MaxKB < B->MaxKB;
			}
			else if (SortByColumn == FName("ResExcKB"))
			{
				bResult = A->ResExcKB < B->ResExcKB;
			}
			else if (SortByColumn == FName("ResExcDedSysKB"))
			{
				bResult = A->ResExcDedSysKB < B->ResExcDedSysKB;
			}
			else if (SortByColumn == FName("ResExcDedVidKB"))
			{
				bResult = A->ResExcDedVidKB < B->ResExcDedVidKB;
			}
			else if (SortByColumn == FName("ResExcUnkKB"))
			{
				bResult = A->ResExcUnkKB < B->ResExcUnkKB;
			}
			
			// Reverse for descending sort
			if (SortMode == EColumnSortMode::Descending)
			{
				bResult = !bResult;
			}
			
			return bResult;
		});
		
		// Refresh the table view
		if (ResourceSizeSortTable.IsValid())
		{
			ResourceSizeSortTable->RequestListRefresh();
		}
	}
}

EColumnSortMode::Type SMemoryReportResourceSizeSortWidget::GetColumnSortMode(const FName ColumnId) const
{
	if (SortByColumn == ColumnId)
	{
		return SortMode;
	}
	return EColumnSortMode::None;
}

#undef LOCTEXT_NAMESPACE