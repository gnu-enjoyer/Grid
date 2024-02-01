// copyright 2022 gnu-enjoyer / mike

using UnrealBuildTool;

public class GridEditor : ModuleRules
{
	public GridEditor(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
		CppStandard = CppStandardVersion.Cpp20;

		PublicDependencyModuleNames.AddRange(
			new[]
			{
				"Core",
				"Grid",
				"GameplayTags"
			}
		);

		PrivateDependencyModuleNames.AddRange(
			new[]
			{
				"CoreUObject",
				"Engine",
				"Slate",
				"SlateCore",
				"InputCore",
				
				"EditorFramework",
				"EditorStyle",
				"UnrealEd",
				"LevelEditor",
				"InteractiveToolsFramework",
				"EditorInteractiveToolsFramework",
				"Json",
				"JsonUtilities",
				"DesktopPlatform"
			}
		);
	}
}