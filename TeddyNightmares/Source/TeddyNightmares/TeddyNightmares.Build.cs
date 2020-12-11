// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class TeddyNightmares : ModuleRules
{
	public TeddyNightmares(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] {
			"Core",
			"CoreUObject",
			"Engine",
			"InputCore",
			"HeadMountedDisplay",
			"AIModule",
			"GameplayTasks",
			"NavigationSystem",
			"UMG",
			"Slate",
			"SlateCore"
		});
	}
}
