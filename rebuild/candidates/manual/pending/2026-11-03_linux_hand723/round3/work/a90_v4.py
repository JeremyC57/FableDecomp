COL="    CRGBFloatColour topColour = params.ThemeChannels[0].Colours[12];\n    CRGBFloatColour bottomColour = params.ThemeChannels[0].Colours[13];\n"
VARIANTS = [
 ("c10", [(COL, COL+"    const CIDrawEnvironmentParams::CThemeChannel& channel = params.ThemeChannels[0];\n"),
          ("(1.0f - params.ThemeChannels[0].TextureSets[0].Blends[0])","(1.0f - channel.TextureSets[0].Blends[0])"),
          ("colour.W = params.ThemeChannels[0].Scalers[2]","colour.W = channel.Scalers[2]")], None, "channel ref after copies"),
 ("c11", [("(1.0f - params.ThemeChannels[0].TextureSets[0].Blends[0])","(1.0f - params.ThemeChannels.begin()->TextureSets[0].Blends[0])")], None, "begin()-> for blends"),
]
