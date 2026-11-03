BK="    const CGraphicDataBank* bank = g_pResourceManager->GraphicBanks[0];\n"
T0="    unsigned long skyTexture = params.ThemeChannels[0].TextureSets[0].Textures[0];\n    unsigned long skyBlendTexture = params.ThemeChannels[0].TextureSets[0].Textures[1];\n"
VARIANTS = [
 ("c2", [(BK+T0, T0+BK)], None, "bank after texture reads"),
 ("c3", [("CRGBFloatColour topColour = params.ThemeChannels[0].Colours[12];","CRGBFloatColour topColour(params.ThemeChannels[0].Colours[12]);"),
         ("CRGBFloatColour bottomColour = params.ThemeChannels[0].Colours[13];","CRGBFloatColour bottomColour(params.ThemeChannels[0].Colours[13]);")], None, "direct-init colours"),
 ("c4", [("    CRGBFloatColour topColour = params.ThemeChannels[0].Colours[12];\n    CRGBFloatColour bottomColour = params.ThemeChannels[0].Colours[13];\n    float alpha","    float alpha"),
         ("    C3DVector look","    CRGBFloatColour topColour = params.ThemeChannels[0].Colours[12];\n    CRGBFloatColour bottomColour = params.ThemeChannels[0].Colours[13];\n    C3DVector look")], None, "colours declared later (after pixel constants)"),
 ("c5", [("    unsigned long skyTexture = params.ThemeChannels[0].TextureSets[0].Textures[0];","    long skyTexture = params.ThemeChannels[0].TextureSets[0].Textures[0];"),("    unsigned long skyBlendTexture","    long skyBlendTexture")], None, "long texture indices"),
]
