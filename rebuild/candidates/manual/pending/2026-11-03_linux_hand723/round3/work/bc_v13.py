VARIANTS = [
 ("s2", [("    __forceinline ~CGraphicFrame() { ReleaseData(); }", "    __forceinline ~CGraphicFrame() { if (FrameData) FrameData->Release(); }")], None, "dtor no zero"),
 ("s3", [("        if (--ReferenceCount == 0)\n            delete this;", "        --ReferenceCount;\n        if (ReferenceCount == 0)\n            delete this;")], None, "split dec"),
 ("s4", [("    __forceinline ~CGraphicFrameData() { if (IsInitialised) Uninitialise(); }", "    __forceinline ~CGraphicFrameData()\n    {\n        if (IsInitialised)\n            Uninitialise();\n    }")], None, "noop fmt"),
]
