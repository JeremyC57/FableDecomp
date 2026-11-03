A="rebuild/src/compiled/00/4b/CQuestManager_ActivateTestQuestCard_004b4c50.cpp"
B="rebuild/src/compiled/00/4b/CQuestManager_ActivateQuestCard_004b4aa0.cpp"
C="rebuild/src/compiled/00/8d/CGameScriptThing_GetDefName_008d02c0.cpp"
EDITS=[
(A,"""namespace N_00415dd0 { struct CDefString {
    long m_index;
    operator CCharString() const;                        // 0x00415DD0
}; } using N_00415dd0::CDefString;""","""class CDefString { public:                             // naming wave 34: was N_00415dd0::CDefString::operator CCharString (as 0x004B4AA0, Codex pass 12)
    long m_index;
    CCharString GetString() const;                       // 0x00415DD0 (FableWin ?GetString@CDefString@@QBE?AVCCharString@@XZ)
};"""),
(A,"    CDefString GetDefName();                             // 0x004C7CC0","    CDefString GetDefName() const;                       // 0x004C7CC0 (Ego_r / FableWin ?GetDefName@CThing@@QBE?AVCDefString@@XZ)"),
(A,"RemoveAvailableQuestCardType(card->GetDefName());","RemoveAvailableQuestCardType(card->GetDefName().GetString());"),
(B,"    CDefString GetDefName();                             // 0x004C7CC0","    CDefString GetDefName() const;                       // 0x004C7CC0 (Ego_r / FableWin ?GetDefName@CThing@@QBE?AVCDefString@@XZ)"),
(C,"class CThing_Retail { public:","class CThing { public:"),
(C,"CThing_Retail","CThing",0),
]
