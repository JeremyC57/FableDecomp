// Behaviour test for 007d7c80: a self-contained copy of the candidate's model and body, run over stub callees.
// CTCTavernGameSpotTheAddition::GetBestScore() -- the "best score" text from the tavern-game def, followed by the hero's best spot-the-addition time (one decimal) when one has been set
#include <cstdio>
#include <cstring>
#ifndef _MSC_VER
#define __forceinline inline
#define __fastcall
#define __stdcall
#define __cdecl
#define __declspec(x)
#endif
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

// ---- stubs and driver ----
static char g_pool[4096]; static int g_used;
static char* Alloc(const char* s) { int n = (int)std::strlen(s) + 1; char* p = g_pool + g_used; g_used += n; std::memcpy(p, s, n); return p; }
static int g_wdtors;
CWideString::CWideString(const CWideString& other) { m_data = Alloc((const char*)other.m_data); }
CWideString::~CWideString() { ++g_wdtors; }
const CWideString& CWideString::operator+=(const CWideString& other)
{ char t[256]; std::strcpy(t, (const char*)m_data); std::strcat(t, (const char*)other.m_data); m_data = Alloc(t); return *this; }
CCharString::~CCharString() {}
CWideString CCharString::ToWideString() const { CWideString* w = (CWideString*)Alloc("\x01\x01\x01\x01\x01\x01\x01\x01"); w->m_data = m_data; return *w; }
CCharString __fastcall CCharString::FormatDouble(long digits, double value) { char b[32]; std::sprintf(b, "%.*f", (int)digits, value); CCharString s; s.m_data = Alloc(b); return s; }
static int g_destroyed;
void CDefVirtual::v00() {}
void CDefVirtual::Destroy() { ++g_destroyed; }
static CTavernGameDef g_def;
static CTCHeroStats g_stats;
void CTCTavernGame::v00() {}
void CTCTavernGame::GetTavernGameDef(CDefPointer* out) { out->Object = &g_def; ++g_def.m_count; }
void CTCTavernGame::GetHeroStats(CTCHeroStats** stats) { *stats = &g_stats; }
CWideString CTCTavernGame::GetText(unsigned long id)
{ char b[32]; std::sprintf(b, "T%lu:", id); CWideString* w = (CWideString*)Alloc("\x01\x01\x01\x01\x01\x01\x01\x01"); w->m_data = Alloc(b); return *w; }

int main()
{
    CTCTavernGameSpotTheAddition game;
    g_def.BestScore = 180; g_def.m_count = 1;
    g_stats.BestTimeGuessTheAddition = 42.75f;
    CWideString r = game.GetBestScore();
    if (std::strcmp((const char*)r.m_data, "T180:42.8") != 0 || g_def.m_count != 1 || g_destroyed != 0)
        { std::printf("007d7c80_TEST FAIL (1: %s)\n", (const char*)r.m_data); return 1; }
    g_stats.BestTimeGuessTheAddition = 0.0f; g_def.m_count = 0;
    CWideString r2 = game.GetBestScore();
    if (std::strcmp((const char*)r2.m_data, "T180:") != 0 || g_def.m_count != 0 || g_destroyed != 1)
        { std::printf("007d7c80_TEST FAIL (2)\n"); return 1; }
    std::printf("007d7c80_TEST PASS\n");
    return 0;
}