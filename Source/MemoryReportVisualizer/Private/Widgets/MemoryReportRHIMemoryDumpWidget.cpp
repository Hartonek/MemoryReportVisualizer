// Copyright Epic Games, Inc. All Rights Reserved.

#include "Widgets/MemoryReportRHIMemoryDumpWidget.h"
#include "Widgets/Layout/SBox.h"
#include "Widgets/Text/STextBlock.h"
#include "Widgets/Layout/SBorder.h"
#include "Widgets/SBoxPanel.h"
#include "Widgets/Views/SHeaderRow.h"
#include "Widgets/Views/SListView.h"
#include "Widgets/Views/STableRow.h"
#include "Styling/AppStyle.h"

#define LOCTEXT_NAMESPACE "SMemoryReportRHIMemoryDumpWidget"

void SMemoryReportRHIMemoryDumpWidget::Construct(const FArguments& InArgs)
{
	// Initialize sorting state
	SortByColumn = FName("MemorySizeMB");
	SortMode = EColumnSortMode::Ascending;

	ChildSlot
	[
		SNew(SBorder)
		.BorderImage(FAppStyle::GetBrush("ToolPanel.GroupBorder"))
		.Padding(10)
		[
			SNew(SVerticalBox)
			+ SVerticalBox::Slot()
			.AutoHeight()
			.Padding(0, 0, 0, 10)
			[
				SAssignNew(TotalAmountOfMemoryText, STextBlock)
				.Text(LOCTEXT("TotalAmountOfMemoryDefault", "Total Amount of Memory: Not Available"))
				.Font(FAppStyle::GetFontStyle("PropertyWindow.BoldFont"))
				.Justification(ETextJustify::Left)
			]
			+ SVerticalBox::Slot()
			.FillHeight(1.0f)
			[
				SAssignNew(RHIMemoryDumpTable, SListView<TSharedPtr<RHIMemoryDumpDescription>>)
				.ListItemsSource(&RHIMemoryDumpTableData)
				.OnGenerateRow(this, &SMemoryReportRHIMemoryDumpWidget::OnGenerateRow)
				.HeaderRow
				(
					SNew(SHeaderRow)
					+ SHeaderRow::Column(FName("MemorySizeMB"))
					.DefaultLabel(LOCTEXT("MemorySizeMBColumnHeader", "Memory Size (MB)"))
					.SortMode(this, &SMemoryReportRHIMemoryDumpWidget::GetColumnSortMode, FName("MemorySizeMB"))
					.OnSort(this, &SMemoryReportRHIMemoryDumpWidget::OnColumnSortModeChanged)
					.FillWidth(0.3f)
					
					+ SHeaderRow::Column(FName("Description"))
					.DefaultLabel(LOCTEXT("DescriptionColumnHeader", "Description"))
					.SortMode(this, &SMemoryReportRHIMemoryDumpWidget::GetColumnSortMode, FName("Description"))
					.OnSort(this, &SMemoryReportRHIMemoryDumpWidget::OnColumnSortModeChanged)
					.FillWidth(0.7f)
				)
			]
		]
	];
}

void SMemoryReportRHIMemoryDumpWidget::Populate(TSharedPtr<RHIMemoryDumpSectionData> RHIMemoryDumpData)
{
	RHIMemoryDumpSection = RHIMemoryDumpData;
	
	// Clear existing data
	RHIMemoryDumpTableData.Empty();
	
	if (RHIMemoryDumpSection.IsValid())
	{
		// Update TotalAmountOfMemory text
		if (TotalAmountOfMemoryText.IsValid())
		{
			FString TotalMemoryDisplayText = RHIMemoryDumpSection->TotalAmountOfMemory.IsEmpty() ?
				TEXT("Total Amount of Memory: Not Available") :
				FString::Printf(TEXT("Total Amount of Memory: %s"), *RHIMemoryDumpSection->TotalAmountOfMemory);
			
			TotalAmountOfMemoryText->SetText(FText::FromString(TotalMemoryDisplayText));
		}
		
		// Convert RHIMemoryDumpEntries to shared pointers for the table
		for (const RHIMemoryDumpDescription& Entry : RHIMemoryDumpSection->RHIMemoryDumpEntries)
		{
			RHIMemoryDumpTableData.Add(MakeShareable(new RHIMemoryDumpDescription(Entry)));
		}
		
		// Refresh the table view
		if (RHIMemoryDumpTable.IsValid())
		{
			RHIMemoryDumpTable->RequestListRefresh();
		}
	}
	else
	{
		// Reset TotalAmountOfMemory text when no data
		if (TotalAmountOfMemoryText.IsValid())
		{
			TotalAmountOfMemoryText->SetText(LOCTEXT("TotalAmountOfMemoryDefault", "Total Amount of Memory: Not Available"));
		}
	}
}

TSharedRef<ITableRow> SMemoryReportRHIMemoryDumpWidget::OnGenerateRow(TSharedPtr<RHIMemoryDumpDescription> Item, const TSharedRef<STableViewBase>& OwnerTable)
{
	class SRHIMemoryDumpTableRow : public SMultiColumnTableRow<TSharedPtr<RHIMemoryDumpDescription>>
	{
	public:
		SLATE_BEGIN_ARGS(SRHIMemoryDumpTableRow) {}
		SLATE_ARGUMENT(TSharedPtr<RHIMemoryDumpDescription>, Item)
		SLATE_END_ARGS()

		void Construct(const FArguments& InArgs, const TSharedRef<STableViewBase>& InOwnerTableView)
		{
			Item = InArgs._Item;
			SMultiColumnTableRow<TSharedPtr<RHIMemoryDumpDescription>>::Construct(
				FSuperRowType::FArguments(),
				InOwnerTableView
			);
		}

		virtual TSharedRef<SWidget> GenerateWidgetForColumn(const FName& ColumnName) override
		{
			if (ColumnName == FName("MemorySizeMB"))
			{
				return SNew(STextBlock)
					.Text(FText::AsNumber(Item.IsValid() ? Item->MemorySizeMB : 0.0f))
					.Font(FAppStyle::GetFontStyle("PropertyWindow.NormalFont"))
					.Justification(ETextJustify::Center);
			}
			else if (ColumnName == FName("Description"))
			{
				return SNew(STextBlock)
					.Text(FText::FromString(Item.IsValid() ? Item->Description : TEXT("")))
					.Font(FAppStyle::GetFontStyle("PropertyWindow.NormalFont"));
			}

			return SNullWidget::NullWidget;
		}

	private:
		TSharedPtr<RHIMemoryDumpDescription> Item;
	};

	return SNew(SRHIMemoryDumpTableRow, OwnerTable)
		.Item(Item);
}

void SMemoryReportRHIMemoryDumpWidget::OnColumnSortModeChanged(const EColumnSortPriority::Type SortPriority, const FName& ColumnId, const EColumnSortMode::Type InSortMode)
{
	SortByColumn = ColumnId;
	SortMode = InSortMode;
	
	// Sort the data based on the selected column
	if (SortMode != EColumnSortMode::None)
	{
		RHIMemoryDumpTableData.Sort([this](const TSharedPtr<RHIMemoryDumpDescription>& A, const TSharedPtr<RHIMemoryDumpDescription>& B) -> bool
		{
			if (!A.IsValid() || !B.IsValid())
			{
				return false;
			}
			
			bool bResult = false;
			if (SortByColumn == FName("MemorySizeMB"))
			{
				bResult = A->MemorySizeMB < B->MemorySizeMB;
			}
			else if (SortByColumn == FName("Description"))
			{
				bResult = A->Description < B->Description;
			}
			
			// Reverse for descending sort
			if (SortMode == EColumnSortMode::Descending)
			{
				bResult = !bResult;
			}
			
			return bResult;
		});
		
		// Refresh the table view
		if (RHIMemoryDumpTable.IsValid())
		{
			RHIMemoryDumpTable->RequestListRefresh();
		}
	}
}

EColumnSortMode::Type SMemoryReportRHIMemoryDumpWidget::GetColumnSortMode(const FName ColumnId) const
{
	if (SortByColumn == ColumnId)
	{
		return SortMode;
	}
	return EColumnSortMode::None;
}

#undef LOCTEXT_NAMESPACE