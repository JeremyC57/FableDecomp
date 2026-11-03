VARIANTS = [
 ("m1", [("SetUserVertexShaderConstants(1, 3, &ConstantBlock[0].X);", "SetUserVertexShaderConstants(1, 3, (float*)ConstantBlock);")], None, "cast arg"),
 ("m2", [("    ConstantBlock[0].W = sprite.OffsetFactorY;\n", "    ConstantBlock->W = sprite.OffsetFactorY;\n")], None, "arrow W"),
 ("m3", [("    const CMatrix3x4& view = g_pEngineCamera->PeekInverseNonHomogenousViewTransform();\n    ConstantBlock[0].X = view.E11;\n    ConstantBlock[0].Y = view.E21;\n",
          "    ConstantBlock[0].X = g_pEngineCamera->PeekInverseNonHomogenousViewTransform().E11;\n    ConstantBlock[0].Y = g_pEngineCamera->PeekInverseNonHomogenousViewTransform().E21;\n    const CMatrix3x4& view = g_pEngineCamera->PeekInverseNonHomogenousViewTransform();\n")], None, "first two direct"),
 ("m4", [("        Info = 0;\n    }\n};", "        Info = 0;\n    }\n};")], None, "noop"),
 ("k1", [("        Data = 0;\n        Info = 0;\n", "        Info = 0;\n        Data = 0;\n")], None, "texptr info first"),
]
