SCALE_DECL = "    C2DVector GetUIScale();                          // 0x0041CF47\n"
SCALE_BODY = "    C2DVector GetUIScale();                          // 0x0041CF47 (same-TU stand-in body below: probe of the address-exposure lever)\n    float UIScaleX, UIScaleY;\n"
INST_OLD = "    static CManager* __fastcall GetInstance();       // 0x0041E5F2\n};\n"
INST_NEW = INST_OLD + "__declspec(noinline) C2DVector CManager::GetUIScale() { return C2DVector(UIScaleX, UIScaleY); }\n"
AXES = [
 ("scale", [("sb2", "same-TU noinline stand-in body for GetUIScale", [(SCALE_DECL, SCALE_BODY), (INST_OLD, INST_NEW)])]),
 ("pos", [("", "", []),
          ("tmp", "position temp in the call", [("    C2DVector position(GetScreenWidth()", "    PSaveInProgress->ChangePosition(C2DVector(GetScreenWidth()"),
                                                ("52.0f);\n    PSaveInProgress->ChangePosition(position, 0.0f, false);", "52.0f), 0.0f, false);")])]),
 ("half", [("", "", []), ("hl", "0.5f on the left", [("GetScreenWidth() * 0.5f", "0.5f * GetScreenWidth()"), ("GetScreenHeight() * 0.5f", "0.5f * GetScreenHeight()")])]),
]
