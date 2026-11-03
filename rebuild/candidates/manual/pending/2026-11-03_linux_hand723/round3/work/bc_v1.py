VARIANTS = [
 ("e1", [("#define FABLE_RETAIL_CARRAY\n", "")], None, "no CARRAY"),
 ("e2", [("    CEngineInternalPrimitiveSprite& sprite = static_cast<CEngineInternalPrimitiveSprite&>(*sub.Parent);\n",
          "    CEngineInternalPrimitiveSprite* sprite = static_cast<CEngineInternalPrimitiveSprite*>(sub.Parent);\n#define sprite (*sprite)\n")], None, "sprite pointer"),
 ("e3", [("    bool premultiplyAlpha;\n    CRGBFloatColour colour;\n", "    CRGBFloatColour colour;\n    bool premultiplyAlpha;\n")], None, "decl order colour first"),
 ("e4", [("    if (info->GetFrame(frame, 0, 0, 0)) {\n    CRenderStateCaptureBlock captureBlock;\n    g_pRenderManagerCore->AttachTextureToStage(0, frame.GetTexture());\n    bool premultiplyAlpha;\n    CRGBFloatColour colour;\n",
          "    bool premultiplyAlpha;\n    CRGBFloatColour colour;\n    if (info->GetFrame(frame, 0, 0, 0)) {\n    CRenderStateCaptureBlock captureBlock;\n    g_pRenderManagerCore->AttachTextureToStage(0, frame.GetTexture());\n")], None, "colour/premul at function scope"),
]
