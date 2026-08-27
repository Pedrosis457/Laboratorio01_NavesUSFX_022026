using UnrealBuildTool;
using System.Collections.Generic;

public class NavesUSFX_022026Target : TargetRules
{
	public NavesUSFX_022026Target(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;
		DefaultBuildSettings = BuildSettingsVersion.V2;
		ExtraModuleNames.AddRange(new string[] { "NavesUSFX_022026" });
	}
}
