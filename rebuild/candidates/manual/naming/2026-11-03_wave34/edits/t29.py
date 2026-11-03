# T29 (c718 rows): NProgressDisplay::StopProgress (Ego_r 0x00B3BFC0), CPlayer::SetPlayerCharacter (Ego_r 0x00493400),
# NCameraModes::CScriptModuleTransitionBetweenPoints ctors (Ego_r 0x00827D60 / 0x00827D10, FableWin 0x0283A830 / 0x0283A680).
A = "rebuild/src/compiled/00/43/CDisplayEngine_WorldUpdate_00434f60.cpp"; B = "rebuild/src/compiled/00/89/CGameScriptInterface_TurnCreatureInto_0089f4a0.cpp"; C = "rebuild/src/compiled/00/89/CGameScriptInterface_CameraMoveBetweenLookingAt_008926d0.cpp"; D = "rebuild/src/compiled/00/89/CGameScriptInterface_CameraMoveBetweenLookingAt_00892530.cpp"
EDITS = [
    (A, "void __fastcall StopProgress(bool stopNow, bool keepScreen);   // 0x009E9F20", "namespace NProgressDisplay { void __fastcall StopProgress(bool stopNow, bool keepScreen); }   // 0x009E9F20"),
    (A, "            StopProgress(true, false);", "            NProgressDisplay::StopProgress(true, false);"),
    (A, "// Callees: StopProgress(bool, bool) @ 0x009E9F20", "// Callees: NProgressDisplay::StopProgress(bool, bool) @ 0x009E9F20"),
    (B, "SetPlayerCharacter_00487B80", "SetPlayerCharacter", 2),
    (C, "CScriptModuleMoveBetweenLookingAt", "CScriptModuleTransitionBetweenPoints", 0),
    (D, "CScriptModuleMoveBetweenLookingAt is descriptive", "CScriptModuleTransitionBetweenPoints per Ego_r.pdb (naming wave 34)"),
    (D, "CScriptModuleMoveBetweenLookingAt", "CScriptModuleTransitionBetweenPoints", 0),
]
