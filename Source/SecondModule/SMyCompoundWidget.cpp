// Fill out your copyright notice in the Description page of Project Settings.

#include "SMyCompoundWidget.h"
#include "SlateOptMacros.h"
#include "Widgets/Input/SButton.h"
#include "Widgets/Text/STextBlock.h"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION
void SMyCompoundWidget::Construct(const FArguments& InArgs)
{
	ChildSlot
	[
		SNew(SVerticalBox)
		+ SVerticalBox::Slot()
		.AutoHeight()
		.VAlign(VAlign_Top)
		.HAlign(HAlign_Left)
		[
			SNew(SBox)
			.WidthOverride(300.f)
			.HeightOverride(300.f)
			[
				SNew(SButton)
				.OnClicked(this, &SMyCompoundWidget::OutPut)
				.HAlign(HAlign_Center)
				.VAlign(VAlign_Center)
				.Content()
				[
					SNew(STextBlock)
					.Text(FText::FromString(TEXT("Click")))
					.Font(FCoreStyle::GetDefaultFontStyle("Regular", 50))
				]
			]
		]
	];
	
}
END_SLATE_FUNCTION_BUILD_OPTIMIZATION

FReply SMyCompoundWidget::OutPut()
{
	GEngine->AddOnScreenDebugMessage(-1, 3, FColor::Red, TEXT("Click over"));

	return FReply::Handled();
}