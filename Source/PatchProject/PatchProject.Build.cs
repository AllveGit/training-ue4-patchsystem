// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class PatchProject : ModuleRules
{
	public PatchProject(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(new string[] { 
			"Core", 
			"CoreUObject", 
			"Engine", 
			"InputCore", 
			"HeadMountedDisplay", 
			"NavigationSystem", 
			"AIModule" });

		PrivateDependencyModuleNames.AddRange(new string[]
		{
		});
	}
}
