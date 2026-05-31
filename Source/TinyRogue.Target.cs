// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class TinyRogueTarget : TargetRules
{
	public TinyRogueTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;
		DefaultBuildSettings = BuildSettingsVersion.V6;
		IncludeOrderVersion = EngineIncludeOrderVersion.Unreal5_7;
		ExtraModuleNames.Add("TinyRogue");
	}
}