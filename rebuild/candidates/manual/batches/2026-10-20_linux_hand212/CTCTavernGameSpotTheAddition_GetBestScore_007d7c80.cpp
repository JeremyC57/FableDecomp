// CTCTavernGameSpotTheAddition::GetBestScore() -- the "best score" text from the tavern-game def, followed by the hero's best spot-the-addition time (one decimal) when one has been set
// Leaf: GetBestScore
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007D7C80, 212 bytes (`ret 4`, CWideString by hidden pointer);
// CTCTavernGameSpotTheAddition vtable 0x0126F814 slot 43. Ego_r.pdb `?GetBestScore@...@@UAE?AVCWideString@@XZ`.
// Callees: CTCTavernGame::GetTavernGameDef(CDefPointer*) 0x005EA000; CTCTavernGame::GetHeroStats(CTCHeroStats**)
//          0x005EB4B0 (out parameter); CTCTavernGame::GetText(unsigned long) 0x005E9E70; CCharString::FormatDouble(long
//          digits, double) 0x009A08B0 (static `__fastcall`, hidden result in ecx, digits in edx; Ego_r.pdb names it
//          `GFFloatToCharString(double, long)`, the landed callers' spelling is kept);
//          CCharString::ToWideString() const 0x0099E480; CWideString::operator+=(const CWideString&) 0x0099B8D0;
//          CWideString(const CWideString&) 0x0099B720 (the copy-out, VC7.1 has no NRVO); ~CWideString 0x0099B510;
//          ~CCharString 0x0099EAE0; the def's slot 1 on release.
// Layout: CTavernGameDef::BestScore +0xb4 (PDB offset holds); retail CTCHeroStats::BestTimeGuessTheAddition +0x190
//         (Ego_r.pdb +0x1a8; engine/CTCHeroStats.h carries PDB offsets, so a local model; the landed vfunc_33 0x007D7BA0
//         reads the same word).
// Shape: the stats out pointer is left uninitialised (no zero store; its ShoveHaPenny twin 0x007CF5B0 zeroes it);
//        `best != 0.0f` is `fld [0.0] ; fld best ; fucompp ; test ah,44h ; jnp`; the appended number is a chain
//        of temporaries destroyed right after the `+=`; the named result string is copied out and destroyed, then the
//        null-tested CDefPointer dtor calls the def's inline ReleaseTemporary().
struct CWideString {
    void* m_data;
    CWideString(const CWideString& other);      // 0x0099B720
    ~CWideString();                             // 0x0099B510
    const CWideString& operator+=(const CWideString& other);   // 0x0099B8D0
};
struct CCharString {
    void* m_data;
    ~CCharString();                             // 0x0099EAE0
    CWideString ToWideString() const;           // 0x0099E480
    static CCharString __fastcall FormatDouble(long digits, double value);   // 0x009A08B0
};

struct CRefCounted {
    long m_count;
    void DecRef() { --m_count; }
    long GetRefCount() const { return m_count; }
};
struct CDefVirtual {
    virtual void v00();
    virtual void Destroy();                     // slot 1
};
struct CTavernGameDef : CDefVirtual, CRefCounted {
    unsigned char _pad_0x08[0xac];
    unsigned long BestScore;                    // +0xb4
    void ReleaseTemporary()
    {
        DecRef();
        if (GetRefCount() == 0)
            Destroy();
    }
};
struct CDefPointer {
    CTavernGameDef* Object;
    CDefPointer() : Object(0) {}
    ~CDefPointer() { if (Object) Object->ReleaseTemporary(); }
    CTavernGameDef* operator->() const { return Object; }
};

struct CTCHeroStats {                           // retail layout (PDB - 0x18 here)
    unsigned char _pad[0x18c];
    float BestScoreShoveHaPenny;                // +0x18c
    float BestTimeGuessTheAddition;             // +0x190
};

struct CTCTavernGame {
    virtual void v00();
    void GetTavernGameDef(CDefPointer* out);    // 0x005EA000
    void GetHeroStats(CTCHeroStats** stats);    // 0x005EB4B0
    CWideString GetText(unsigned long id);      // 0x005E9E70
};

struct CTCTavernGameSpotTheAddition : CTCTavernGame {
    CWideString GetBestScore();
};

CWideString CTCTavernGameSpotTheAddition::GetBestScore()
{
    CDefPointer def;
    GetTavernGameDef(&def);
    CTCHeroStats* stats;
    GetHeroStats(&stats);
    CWideString text = GetText(def->BestScore);
    if (stats->BestTimeGuessTheAddition != 0.0f)
        text += CCharString::FormatDouble(1, stats->BestTimeGuessTheAddition).ToWideString();
    return text;
}
