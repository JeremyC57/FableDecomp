P7="""        premultiplyAlpha = false;
        g_pRenderManagerCore->SetAlphaTestEnable(true);
        g_pRenderManagerCore->SetAlphaRef(0x10);
        g_pRenderManagerCore->SetAlphaTestFunc(ALPHA_TEST_FUNC_GREATER);
"""
VARIANTS = [
 ("i1", [(P7, """        g_pRenderManagerCore->SetAlphaTestEnable(true);
        premultiplyAlpha = false;
        g_pRenderManagerCore->SetAlphaRef(0x10);
        g_pRenderManagerCore->SetAlphaTestFunc(ALPHA_TEST_FUNC_GREATER);
""")], None, "premul after alpha enable"),
 ("i2", [(P7, """        g_pRenderManagerCore->SetAlphaTestEnable(true);
        g_pRenderManagerCore->SetAlphaRef(0x10);
        g_pRenderManagerCore->SetAlphaTestFunc(ALPHA_TEST_FUNC_GREATER);
        premultiplyAlpha = false;
""")], None, "premul last"),
 ("i3", [("        g_pRenderManagerCore->SetAlphaTestEnable(true);\n", "")], None, "EXPERIMENT no alpha enable"),
 ("i4", [("        g_pRenderManagerCore->SetAlphaRef(0x10);\n", "        g_pRenderManagerCore->SetAlphaRef((unsigned char)0x10);\n")], None, "uchar ref"),
]
