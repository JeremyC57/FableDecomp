A="rebuild/src/compiled/00/41/global_GetFontBankName_00415260.cpp"
AT="rebuild/tests/00/41/global_GetFontBankName_00415260_test.cpp"
B="rebuild/src/compiled/00/40/global_GetWindowTitle_004023f0.cpp"
BT="rebuild/tests/00/40/global_GetWindowTitle_004023f0_test.cpp"
C="rebuild/src/compiled/00/41/CMainGameComponent_InitialiseText_00416832.cpp"
CT="rebuild/tests/00/41/CMainGameComponent_InitialiseText_00416832_test.cpp"
EDITS=[
(A,"// _global::GetFontBankName() -- name of the main text/font bank for the current language (Windows primary language id), English by default\n// Leaf: ?GetFontBankName@@",
 "// NLocalisation::GetTextBankName() -- name of the main text bank for the current language (Windows primary language id), English by default\n// Leaf: ?GetTextBankName@NLocalisation@@YI?AVCCharString@@XZ\n// Identity (naming wave 34): Ego_r.pdb 0x0040FA60 / FableWin.pdb 0x01877CF0 ?GetTextBankName@NLocalisation@@YI?AVCCharString@@XZ, donor\n// order GetLanguage, GetTextBankName, GetFontBankName (retail 0x00415070 / 0x00415260 / 0x00415350); the bodies return the\n// TEXT_<LANGUAGE>_MAIN bank names (0x00415350 is GetFontBankName). Was landed as the global GetFontBankName."),
(A,"CCharString __fastcall GetFontBankName()\n","CCharString __fastcall NLocalisation::GetTextBankName()\n"),
(A,"namespace NLocalisation { unsigned long __fastcall GetLanguage(); }   // 0x00415070                    // 0x00415070",
   "namespace NLocalisation {\n    unsigned long __fastcall GetLanguage();         // 0x00415070\n    CCharString __fastcall GetTextBankName();\n}"),
(AT,"GetFontBankName","GetTextBankName",0),
(B,"GetFontBankName","GetTextBankName",0),
(BT,"GetFontBankName","GetTextBankName",0),
(C,"GetFontBankName","GetTextBankName",0),
(CT,"GetFontBankName","GetTextBankName",0),
]
