VARIANTS = [
 ("n1", [], None, "CCountedPointer<CTexture>"),
 ("n2", [("    __forceinline ~CGraphicFrameData() { if (IsInitialised) Uninitialise(); }", "    __forceinline ~CGraphicFrameData() { if (IsInitialised) Uninitialise(); PTexture.ReleaseData(); }")], None, "+ReleaseData"),
]
