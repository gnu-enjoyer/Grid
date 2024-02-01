// Fill out your copyright notice in the Description page of Project Settings.

#include "GridEditorToolProperties.h"
#include "DesktopPlatformModule.h"
#include "GridDataAsset.h"
#include "JsonObjectConverter.h"

#define LOCTEXT_NAMESPACE "UGridEditorInteractiveToolProps"

namespace
{
	const auto GridDir = FPaths::ProjectPluginsDir() + TEXT("Grid/");
}

void UGridEditorToolProperties::SaveDataAsset()
{
	if (GridData)
	{
		GridData->Modify(true);
	}
}

void UGridEditorToolProperties::ExportJSON()
{
	if (!GridData)
	{
		return;
	}

	FString OutString{};
	if (FJsonObjectConverter::UStructToJsonObjectString(GridData->Grid, OutString))
	{
		const auto Filename = GridDir + FDateTime::UtcNow().ToString() + TEXT(".json");
		if (!IFileManager::Get().DirectoryExists(*Filename))
		{
			FFileHelper::SaveStringToFile(OutString, *Filename, FFileHelper::EEncodingOptions::ForceUTF8);
		}
	}
}

void UGridEditorToolProperties::ImportJSON()
{
	if (!GridData)
	{
		return;
	}

	const FText Title = LOCTEXT("Import JSON", "Import JSON from file");
	const FString FileTypes = TEXT("JSON (*.json)|*.json|Text (*.txt)|*.txt");
	TArray<FString> Files{};

	if (auto Fs = FSlateApplication::Get().GetActiveTopLevelWindow()->GetNativeWindow()->GetOSWindowHandle())
	{
		FDesktopPlatformModule::Get()->OpenFileDialog(Fs,
		                                              Title.ToString(),
		                                              *GridDir,
		                                              *GridDir,
		                                              FileTypes,
		                                              0,
		                                              Files
		);
	}

	if (!Files.IsEmpty())
	{
		auto Fd = Files.Last();
		FString Buffer{};

		if (FFileHelper::LoadFileToString(Buffer, *Fd))
		{
			FJsonObjectConverter::JsonObjectStringToUStruct<FGridData>(Buffer, &GridData->Grid);
			JSONDelegate.Broadcast();
		}
	}
}

#undef LOCTEXT_NAMESPACE
