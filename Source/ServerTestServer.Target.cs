// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class ServerTestServerTarget : TargetRules
{
	public ServerTestServerTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Server;
		ExtraModuleNames.Add("ServerTest");
		ExtraModuleNames.Add("SecondModule");
	}
}
