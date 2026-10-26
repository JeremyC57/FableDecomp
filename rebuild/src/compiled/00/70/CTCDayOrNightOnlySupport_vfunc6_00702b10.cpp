// CTCDayOrNightOnlySupport::vfunc_6() -- on initial activation read the day/night-only suffix of the thing's section name: "DayOnly" -> 1, "NightOnly" -> 2, anything else -> 0
// Leaf: vfunc_6
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00702B10, 176 bytes (plain `ret`). CTCDayOrNightOnlySupport vtable 0x01264D3C slot 6
// (OnInitialActivate, GOTCHAS chunk 160).
// Callees: CQuestManager::GetDayOrNightOnlyStringFromFullSectionName(const CCharString&) @ 0x004AF350 on g_pQuestManager
// @ 0x013B89FC (CCharString by hidden pointer), Compare_00411570 @ 0x00411570 (`__fastcall`, the inlined CCharString equality),
// ~CCharString @ 0x0099EAE0. Globals: g_DayOnlySuffix @ 0x013BAC18, g_NightOnlySuffix @ 0x013BAC1C (as the landed
// GetSectionNameSuffix 0x00702AC0).
// Layout: Ego_r.pdb CTCDayOrNightOnlySupport DayNightOrBoth +0x0c; retail CThing SerialisationSectionName +0x58 (PDB +0x68).
// Shape: the suffix is a named local; the inline CCharString operator== over references (same block -> equal; both blocks
// set with equal length -> `Compare(...) ? false : true`, the 8-bit `neg ; sbb al,al ; inc al`); three returns, each with its
// own inline destructor call. The suffix block stays in esi across Compare only with Compare's real body (landed 0x00411570)
// compiled above under `auto_inline(off)`, as the landed CTCCameraPointScriptedSpline::HasMessage 0x0075D2B0.
class CCharStringData { public:                                // the shared string block
    const char*   m_pData;                              // +0x00
    unsigned long m_length;                             // +0x04
};
#pragma auto_inline(off)
long __fastcall Compare_00411570(const char* a, const char* b)   // 0x00411570 (the landed body)
{
    for (;;) {
        char ca = *a;
        if (ca == 0 && *b == 0)
            return 0;
        char cb = *b;
        if (ca < cb)
            return -1;
        if (ca > cb)
            return 1;
        ++a;
        ++b;
    }
}
#pragma auto_inline(on)
class CCharString { public:
    CCharStringData* PStringData;
    ~CCharString();                                     // 0x0099EAE0
    bool operator==(const CCharString& other) const
    {
        if (PStringData == other.PStringData)
            return true;
        if (PStringData && other.PStringData && PStringData->m_length == other.PStringData->m_length)
            return Compare_00411570(PStringData->m_pData, other.PStringData->m_pData) ? false : true;
        return false;
    }
};
extern CCharString g_DayOnlySuffix;                     // 0x013BAC18 "DayOnly"
extern CCharString g_NightOnlySuffix;                   // 0x013BAC1C "NightOnly"
class CQuestManager { public:
    CCharString GetDayOrNightOnlyStringFromFullSectionName(const CCharString& sectionName);   // 0x004AF350
};
extern CQuestManager* g_pQuestManager;                  // 0x013B89FC
class CThing { public:
    unsigned char _pad00[0x58];
    CCharString   SerialisationSectionName;             // +0x58
};
class CTCDayOrNightOnlySupport { public:
    virtual void v00();
    CThing* ParentThing;                                // +0x04 (CTCBase)
    unsigned char _pad08[0x0c - 0x08];
    long DayNightOrBoth;                                // +0x0c EDayNightOrBoth
    void vfunc_6();
};

void CTCDayOrNightOnlySupport::vfunc_6()
{
    CCharString suffix = g_pQuestManager->GetDayOrNightOnlyStringFromFullSectionName(ParentThing->SerialisationSectionName);
    if (suffix == g_DayOnlySuffix) {
        DayNightOrBoth = 1;
        return;
    }
    if (suffix == g_NightOnlySuffix) {
        DayNightOrBoth = 2;
        return;
    }
    DayNightOrBoth = 0;
}