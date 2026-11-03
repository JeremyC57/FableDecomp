LK="""    C3DVector look = g_pEngineCamera->PeekLookVector();
    C2DVector direction(look.X, look.Y);"""
VARIANTS = [
 ("v1", [("float blend = Clamp((1.0f - params.ThemeChannels[0].TextureSets[0].Blends[0]) * 255.0f / 255.0f, 0.0f, 1.0f);",
          "float alpha = (1.0f - params.ThemeChannels[0].TextureSets[0].Blends[0]) * 255.0f;\n    float blend = Clamp(alpha / 255.0f, 0.0f, 1.0f);")], None, "blend alpha split"),
 ("v2", [("return CameraDesc.Use2DFOV ? CameraDesc.VerticalFOV : 1.0f / AspectRatio * CameraDesc.HorizontalFOV;","XX")], None, "dummy"),
 ("v3", [(LK, """    const C3DVector& look = g_pEngineCamera->PeekLookVector();
    float lookZ = look.Z;
    C2DVector direction(look.X, look.Y);"""), ("acos(look.Z)","acos(lookZ)")], None, "look ref + lookZ"),
 ("v4", [(LK, """    C3DVector look = g_pEngineCamera->PeekLookVector();
    C2DVector direction;
    direction.X = look.X;
    direction.Y = look.Y;""")], None, "direction members assigned"),
 ("v5", [("    float start = angle - g_pEngineCamera->PeekHFOV() * 0.5f / 6.2831855f;","    float halfFOV = g_pEngineCamera->PeekHFOV() * 0.5f;\n    float start = angle - halfFOV / 6.2831855f;")], None, "halfFOV split"),
 ("v6", [("    float start = angle - g_pEngineCamera->PeekHFOV() * 0.5f / 6.2831855f;","    float start = angle - (g_pEngineCamera->PeekHFOV() * 0.5f) * 0.15915494f;")], None, "(h*0.5)*k"),
]
