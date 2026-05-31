// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class TinyRogueEditorTarget : TargetRules
{
	public TinyRogueEditorTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;
		DefaultBuildSettings = BuildSettingsVersion.V6;
		IncludeOrderVersion = EngineIncludeOrderVersion.Unreal5_7;
		ExtraModuleNames.Add("TinyRogue");
		RegisterModulesCreatedByRider();
	}

	private void RegisterModulesCreatedByRider()
	{
		ExtraModuleNames.AddRange(new[] { "TinyRogueEditor" });
	}
}