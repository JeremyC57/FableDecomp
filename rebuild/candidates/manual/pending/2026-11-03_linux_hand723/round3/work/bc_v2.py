INFO="    const CGraphicInfo* info = g_pResourceManager->GraphicBanks[0]->GetGraphicInfo(sprite.BankIndex);\n    CGraphicFrame frame;\n"
PM="    bool premultiplyAlpha;\n    CRGBFloatColour colour;\n"
LT="        CEnginePrimitiveManagerParticleGroup* particleGroupManager =\n            static_cast<CEnginePrimitiveManagerParticleGroup*>(g_pEnginePrimitiveTypeInfo->PrimitiveManagers[24]);\n        unsigned long lights = 0;\n"
VARIANTS = [
 ("f3", [(INFO, "    CGraphicFrame frame;\n    const CGraphicInfo* info = g_pResourceManager->GraphicBanks[0]->GetGraphicInfo(sprite.BankIndex);\n")], None, "frame decl before info"),
 ("f4", [(PM, PM+"    unsigned long lights;\n"), ("        unsigned long lights = 0;\n", "        lights = 0;\n")], None, "lights outer"),
 ("f5", [(PM, PM+"    CEnginePrimitiveManagerParticleGroup* particleGroupManager;\n    unsigned long lights;\n"), (LT, "        particleGroupManager =\n            static_cast<CEnginePrimitiveManagerParticleGroup*>(g_pEnginePrimitiveTypeInfo->PrimitiveManagers[24]);\n        lights = 0;\n")], None, "mgr+lights outer"),
 ("f6", [(PM, "    CRGBFloatColour colour;\n    bool premultiplyAlpha;\n    unsigned long lights;\n"), ("        unsigned long lights = 0;\n", "        lights = 0;\n")], None, "colour,premul,lights outer"),
 ("f7", [("    if (info->GetFrame(frame, 0, 0, 0)) {\n", "    if (!info->GetFrame(frame, 0, 0, 0))\n        ;\n    else {\n")], None, "inverted if"),
]
