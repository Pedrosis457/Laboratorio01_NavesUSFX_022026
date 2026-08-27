using UnrealBuildTool;
using System.Collections.Generic;

public class NavesUSFX_022026EditorTarget : TargetRules
{
	public NavesUSFX_022026EditorTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;
		DefaultBuildSettings = BuildSettingsVersion.V2;
		ExtraModuleNames.AddRange(new string[] { "NavesUSFX_022026" });
	}
}
