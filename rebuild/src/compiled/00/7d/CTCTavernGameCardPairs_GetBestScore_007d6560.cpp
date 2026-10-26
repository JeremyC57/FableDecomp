// CTCTavernGameCardPairs::GetBestScore() -- in the timed game type (1), the def's "best score" text followed by the hero's best pairs time (one decimal) when one is set; otherwise an empty string
// Leaf: GetBestScore
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007D6560, 268 bytes (`ret 4`, CWideString by hidden pointer);
// CTCTavernGameCardPairs vtable 0x0126F6FC slot 43 (GetBestScore, as ShoveHaPenny 0x007CF5B0 / SpotTheAddition 0x007D7C80).
// Callees: CTCTavernGame::GetTavernGameDef(CDefPointer*) 0x005EA000; CTCTavernGame::GetHeroStats(CTCHeroStats**)
//          0x005EB4B0; CTCTavernGame::GetText(unsigned long) 0x005E9E70; CCharString(const char*, long) 0x0099EBF0;
//          CCharString::ToWideString() const 0x0099E480; ~CCharString 0x0099EAE0; CCharString::FormatDouble(long, double)
//          0x009A08B0 (static `__fastcall`); CWideString::operator+=(const CWideString&) 0x0099B8D0; CWideString(const
//          CWideString&) 0x0099B720 (copy-out, no NRVO); ~CWideString 0x0099B510; the def's virtual slot 1.
// Layout (retail): CTavernGameDef::BestScore +0xb4; CTCTavernGameCardBase::GameType +0x300 (PDB +0x324); CTCHeroStats::
//          BestTimePairs +0x178 (Ego_r.pdb +0x190; engine/CTCHeroStats.h carries PDB offsets, so a local model).
// Shape: the text is fetched before the mode test; a one-case `switch` (`return text;` inside the case, the empty-string return
//        after the switch, its temporary homed in the dead hidden-result slot) gives `dec esi ; je` with the default arm first; `best != 0.0f` is
//        `fld [0.0] ; fld best ; fucompp ; test ah,44h ; jnp`; the stats out pointer is zero-initialised (its CardSorting twin 0x007D5290 leaves it unset).
// Layout (supersedes any local-model note above): engine/CTCHeroStats.h now has the retail STLport 4.6 offsets, so the data
//   members come from the shared header; the class below keeps only the declarations this body needs.
#define CTCHeroStats CTCHeroStatsDonor
#include "engine/CTCHeroStats.h"
#undef CTCHeroStats
class CWideString { public:
    void* m_data;
    CWideString(const CWideString& other);      // 0x0099B720
    ~CWideString();                             // 0x0099B510
    const CWideString& operator+=(const CWideString& other);   // 0x0099B8D0
};
class CCharString { public:
    void* m_data;
    CCharString(const char* text, long length); // 0x0099EBF0
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
class CTavernGameDef : public CDefVirtual, public CRefCounted { public:
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

class CTCHeroStats : public CTCHeroStatsDonor {
 public:                           // retail layout (PDB - 0x18 here)
};

class CTCTavernGame { public:
    virtual void v00();
    void GetTavernGameDef(CDefPointer* out);    // 0x005EA000
    void GetHeroStats(CTCHeroStats** stats);    // 0x005EB4B0
    CWideString GetText(unsigned long id);      // 0x005E9E70
    unsigned char _pad_0x04[0x2fc];
    long          GameType;                     // +0x300
};

struct CTCTavernGameCardPairs : CTCTavernGame {
    CWideString GetBestScore();
};

CWideString CTCTavernGameCardPairs::GetBestScore()
{
    CDefPointer def;
    GetTavernGameDef(&def);
    CTCHeroStats* stats = 0;
    GetHeroStats(&stats);
    CWideString text = GetText(def->BestScore);
    switch (GameType) {
    case 1:
        if (stats->BestTimePairs != 0.0f)
            text += CCharString::FormatDouble(1, stats->BestTimePairs).ToWideString();
        return text;
    }
    return CCharString("", -1).ToWideString();
}