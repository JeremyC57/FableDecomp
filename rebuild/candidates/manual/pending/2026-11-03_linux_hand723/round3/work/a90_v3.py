COL="    CRGBFloatColour topColour = params.ThemeChannels[0].Colours[12];\n    CRGBFloatColour bottomColour = params.ThemeChannels[0].Colours[13];\n"
BL="    float alpha = (1.0f - params.ThemeChannels[0].TextureSets[0].Blends[0]) * 255.0f;\n    float blend = Clamp(alpha / 255.0f, 0.0f, 1.0f);\n"
LK="    const C3DVector& look = g_pEngineCamera->PeekLookVector();\n"
VARIANTS = [
 ("c6", [(COL, ""), (LK, COL+LK)], None, "colours declared before look"),
 ("c7", [(COL+BL, BL+COL)], None, "blend before colours"),
 ("c8", [(COL, "    CRGBFloatColour bottomColour = params.ThemeChannels[0].Colours[13];\n    CRGBFloatColour topColour = params.ThemeChannels[0].Colours[12];\n")], None, "bottom first"),
 ("c9", [(COL, ""), ("    g_pShaderRenderManager->SetUserVertexShaderConstant(0, topColour);\n    g_pShaderRenderManager->SetUserVertexShaderConstant(1, bottomColour);\n",
                     "    g_pShaderRenderManager->SetUserVertexShaderConstant(0, params.ThemeChannels[0].Colours[12]);\n    g_pShaderRenderManager->SetUserVertexShaderConstant(1, params.ThemeChannels[0].Colours[13]);\n")], None, "no colour copies (direct refs)"),
]
