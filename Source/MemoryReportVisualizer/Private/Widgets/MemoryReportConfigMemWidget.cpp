// Copyright Epic Games, Inc. All Rights Reserved.

#include "Widgets/MemoryReportConfigMemWidget.h"
#include "Widgets/Layout/SBox.h"
#include "Widgets/Text/STextBlock.h"
#include "Widgets/Layout/SBorder.h"
#include "Widgets/Views/SHeaderRow.h"
#include "Widgets/Views/SListView.h"
#include "Widgets/Views/STableRow.h"
#include "Styling/AppStyle.h"

#define LOCTEXT_NAMESPACE "SMemoryReportConfigMemWidget"

void SMemoryReportConfigMemWidget::Construct(const FArguments& InArgs)
{
	// Initialize sorting state
	SortByColumn = FName("FileName");
	SortMode = EColumnSortMode::Ascending;

	ChildSlot
	[
		SNew(SBorder)
		.BorderImage(FAppStyle::GetBrush("ToolPanel.GroupBorder"))
		.Padding(10)
		[
			SAssignNew(ConfigMemTable, SListView<TSharedPtr<ConfigMemDescription>>)
			.ListItemsSource(&ConfigMemTableData)
			.OnGenerateRow(this, &SMemoryReportConfigMemWidget::OnGenerateRow)
			.HeaderRow
			(
				SNew(SHeaderRow)
				+ SHeaderRow::Column(FName("FileName"))
				.DefaultLabel(LOCTEXT("FileNameColumnHeader", "File Name"))
				.SortMode(this, &SMemoryReportConfigMemWidget::GetColumnSortMode, FName("FileName"))
				.OnSort(this, &SMemoryReportConfigMemWidget::OnColumnSortModeChanged)
				.FillWidth(0.6f)
				
				+ SHeaderRow::Column(FName("NumBytes"))
				.DefaultLabel(LOCTEXT("NumBytesColumnHeader", "Num Bytes"))
				.SortMode(this, &SMemoryReportConfigMemWidget::GetColumnSortMode, FName("NumBytes"))
				.OnSort(this, &SMemoryReportConfigMemWidget::OnColumnSortModeChanged)
				.FillWidth(0.2f)
				
				+ SHeaderRow::Column(FName("MaxBytes"))
				.DefaultLabel(LOCTEXT("MaxBytesColumnHeader", "Max Bytes"))
				.SortMode(this, &SMemoryReportConfigMemWidget::GetColumnSortMode, FName("MaxBytes"))
				.OnSort(this, &SMemoryReportConfigMemWidget::OnColumnSortModeChanged)
				.FillWidth(0.2f)
			)
		]
	];
}

void SMemoryReportConfigMemWidget::Populate(TSharedPtr<ConfigMemSectionData> ConfigMemData)
{
	ConfigMemSection = ConfigMemData;
	
	// Clear existing data
	ConfigMemTableData.Empty();
	
	if (ConfigMemSection.IsValid())
	{
		// Convert ConfigMemEntries to shared pointers for the table
		for (const ConfigMemDescription& Entry : ConfigMemSection->ConfigMemEntries)
		{
			ConfigMemTableData.Add(MakeShareable(new ConfigMemDescription(Entry)));
		}
		
		// Refresh the table view
		if (ConfigMemTable.IsValid())
		{
			ConfigMemTable->RequestListRefresh();
		}
	}
}

TSharedRef<ITableRow> SMemoryReportConfigMemWidget::OnGenerateRow(TSharedPtr<ConfigMemDescription> Item, const TSharedRef<STableViewBase>& OwnerTable)
{
	class SConfigMemTableRow : public SMultiColumnTableRow<TSharedPtr<ConfigMemDescription>>
	{
	public:
		SLATE_BEGIN_ARGS(SConfigMemTableRow) {}
		SLATE_ARGUMENT(TSharedPtr<ConfigMemDescription>, Item)
		SLATE_END_ARGS()

		void Construct(const FArguments& InArgs, const TSharedRef<STableViewBase>& InOwnerTableView)
		{
			Item = InArgs._Item;
			SMultiColumnTableRow<TSharedPtr<ConfigMemDescription>>::Construct(
				FSuperRowType::FArguments(),
				InOwnerTableView
			);
		}

		virtual TSharedRef<SWidget> GenerateWidgetForColumn(const FName& ColumnName) override
		{
			if (ColumnName == FName("FileName"))
			{
				return SNew(STextBlock)
					.Text(FText::FromString(Item.IsValid() ? Item->FileName : TEXT("")))
					.Font(FAppStyle::GetFontStyle("PropertyWindow.NormalFont"));
			}
			else if (ColumnName == FName("NumBytes"))
			{
				return SNew(STextBlock)
					.Text(FText::AsNumber(Item.IsValid() ? Item->NumBytes : 0))
					.Font(FAppStyle::GetFontStyle("PropertyWindow.NormalFont"))
					.Justification(ETextJustify::Right);
			}
			else if (ColumnName == FName("MaxBytes"))
			{
				return SNew(STextBlock)
					.Text(FText::AsNumber(Item.IsValid() ? Item->MaxBytes : 0))
					.Font(FAppStyle::GetFontStyle("PropertyWindow.NormalFont"))
					.Justification(ETextJustify::Right);
			}

			return SNullWidget::NullWidget;
		}

	private:
		TSharedPtr<ConfigMemDescription> Item;
	};

	return SNew(SConfigMemTableRow, OwnerTable)
		.Item(Item);
}

void SMemoryReportConfigMemWidget::OnColumnSortModeChanged(const EColumnSortPriority::Type SortPriority, const FName& ColumnId, const EColumnSortMode::Type InSortMode)
{
	SortByColumn = ColumnId;
	SortMode = InSortMode;
	
	// Sort the data based on the selected column
	if (SortMode != EColumnSortMode::None)
	{
		ConfigMemTableData.Sort([this](const TSharedPtr<ConfigMemDescription>& A, const TSharedPtr<ConfigMemDescription>& B) -> bool
		{
			if (!A.IsValid() || !B.IsValid())
			{
				return false;
			}
			
			bool bResult = false;
			if (SortByColumn == FName("FileName"))
			{
				bResult = A->FileName < B->FileName;
			}
			else if (SortByColumn == FName("NumBytes"))
			{
				bResult = A->NumBytes < B->NumBytes;
			}
			else if (SortByColumn == FName("MaxBytes"))
			{
				bResult = A->MaxBytes < B->MaxBytes;
			}
			
			// Reverse for descending sort
			if (SortMode == EColumnSortMode::Descending)
			{
				bResult = !bResult;
			}
			
			return bResult;
		});
		
		// Refresh the table view
		if (ConfigMemTable.IsValid())
		{
			ConfigMemTable->RequestListRefresh();
		}
	}
}

EColumnSortMode::Type SMemoryReportConfigMemWidget::GetColumnSortMode(const FName ColumnId) const
{
	if (SortByColumn == ColumnId)
	{
		return SortMode;
	}
	return EColumnSortMode::None;
}

#undef LOCTEXT_NAMESPACE