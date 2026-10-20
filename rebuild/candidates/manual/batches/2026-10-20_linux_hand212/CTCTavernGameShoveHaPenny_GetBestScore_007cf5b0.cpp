// CTCTavernGameShoveHaPenny::GetBestScore() -- the "best score" text from the tavern-game def, followed by the hero's best shove-ha'penny score when one has been set
// Leaf: GetBestScore
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007CF5B0, 216 bytes (`ret 4`, CWideString by hidden pointer);
// CTCTavernGameShoveHaPenny vtable 0x0126F2F4 slot 43. Ego_r.pdb `?GetBestScore@...@@UAE?AVCWideString@@XZ`.
// Callees: CTCTavernGame::GetTavernGameDef(CDefPointer*) 0x005EA000; CTCTavernGame::GetHeroStats(CTCHeroStats**)
//          0x005EB4B0 (out parameter); CTCTavernGame::GetText(unsigned long) 0x005E9E70; __ftol2 0x00BFEA70 (the
//          `(long)` cast); GFIntToCharString(long) 0x0099F830 (__fastcall, hidden result in ecx);
//          CCharString::ToWideString() const 0x0099E480; CWideString::operator+=(const CWideString&) 0x0099B8D0;
//          CWideString(const CWideString&) 0x0099B720 (the copy-out, VC7.1 has no NRVO); ~CWideString 0x0099B510;
//          ~CCharString 0x0099EAE0; the def's slot 1 on release.
// Layout: CTavernGameDef::BestScore +0xb4 (PDB offset holds); retail CTCHeroStats::BestScoreShoveHaPenny +0x18c
//         (Ego_r.pdb +0x1a4; engine/CTCHeroStats.h carries PDB offsets, so a local model).
// Shape: the stats out pointer is zero-initialised after the def lookup (its SpotTheAddition twin 0x007D7C80 leaves it
//        unset); `best != 0.0f` is `fld [0.0] ; fld best ; fucompp ; test ah,44h ; jnp`; the appended number is a chain
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
};
CCharString __fastcall GFIntToCharString(long value);   // 0x0099F830

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

struct CTCTavernGameShoveHaPenny : CTCTavernGame {
    CWideString GetBestScore();
};

CWideString CTCTavernGameShoveHaPenny::GetBestScore()
{
    CDefPointer def;
    GetTavernGameDef(&def);
    CTCHeroStats* stats = 0;
    GetHeroStats(&stats);
    CWideString text = GetText(def->BestScore);
    if (stats->BestScoreShoveHaPenny != 0.0f)
        text += GFIntToCharString((long)stats->BestScoreShoveHaPenny).ToWideString();
    return text;
}
