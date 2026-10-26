// Behaviour test for 007d2b70: a self-contained copy of the candidate's model and body, run over stub callees.
// CTCTavernGameCoinGolf::GetCurrentScore() -- the "current score" text from the tavern-game def followed by the score formatted with no decimals
#include <cstdio>
#include <cstring>
#ifndef _MSC_VER
#define __forceinline inline
#define __fastcall
#define __stdcall
#define __cdecl
#define __declspec(x)
#endif
class CWideString { public:
    void* m_data;
    ~CWideString();                             // 0x0099B510
};
class CCharString { public:
    void* m_data;
    ~CCharString();                             // 0x0099EAE0
    CWideString ToWideString() const;           // 0x0099E480
    static CCharString __fastcall FormatDouble(long digits, double value);   // 0x009A08B0
};
CWideString __fastcall operator+(const CWideString& a, const CWideString& b);  // 0x0099BE70

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
    unsigned char _pad_0x08[0xb0];
    unsigned long CurrentScore;                 // +0xb8
    unsigned long RequiredScore;                // +0xbc
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

class CTCTavernGame { public:
    virtual void v00();
    void GetTavernGameDef(CDefPointer* out);    // 0x005EA000
    CWideString GetText(unsigned long id);      // 0x005E9E70
    unsigned char _pad_0x04[0x28];
    float         Score;                        // +0x2c
};

class CTCTavernGameCoinGolf : public CTCTavernGame { public:
    CWideString GetCurrentScore();
};

CWideString CTCTavernGameCoinGolf::GetCurrentScore()
{
    CDefPointer def;
    GetTavernGameDef(&def);
    return GetText(def->CurrentScore) + CCharString::FormatDouble(0, Score).ToWideString();
}

// ---- stubs and driver ----
#include <cstdlib>
static char g_pool[4096]; static int g_used;
static char* Alloc(const char* s) { int n = (int)std::strlen(s) + 1; char* p = g_pool + g_used; g_used += n; std::memcpy(p, s, n); return p; }
CWideString::~CWideString() {}
CCharString::~CCharString() {}
CWideString CCharString::ToWideString() const { CWideString w; w.m_data = Alloc((const char*)m_data); return w; }
static long g_digits = -1;
CCharString __fastcall CCharString::FormatDouble(long digits, double value)
{ char b[64]; std::sprintf(b, "%.0f", value); g_digits = digits; CCharString s; s.m_data = Alloc(b); return s; }
CWideString __fastcall operator+(const CWideString& a, const CWideString& b)
{ char t[256]; std::strcpy(t, (const char*)a.m_data); std::strcat(t, (const char*)b.m_data); CWideString w; w.m_data = Alloc(t); return w; }
static int g_destroyed;
void CDefVirtual::v00() {}
void CDefVirtual::Destroy() { ++g_destroyed; }
static CTavernGameDef g_def;
void CTCTavernGame::v00() {}
void CTCTavernGame::GetTavernGameDef(CDefPointer* out) { out->Object = &g_def; ++g_def.m_count; }
CWideString CTCTavernGame::GetText(unsigned long id) { char b[32]; std::sprintf(b, "T%lu:", id); CWideString w; w.m_data = Alloc(b); return w; }

int main()
{
    CTCTavernGameCoinGolf game;
    g_def.CurrentScore = 77; g_def.m_count = 1;
    game.Score = 12.0f;
    CWideString r = game.GetCurrentScore();
    if (std::strcmp((const char*)r.m_data, "T77:12") != 0 || g_digits != 0 || g_def.m_count != 1 || g_destroyed != 0)
        { std::printf("007d2b70_TEST FAIL (1)\n"); return 1; }
    g_def.m_count = 0; game.Score = 3.0f;
    CWideString r2 = game.GetCurrentScore();
    if (std::strcmp((const char*)r2.m_data, "T77:3") != 0 || g_def.m_count != 0 || g_destroyed != 1)
        { std::printf("007d2b70_TEST FAIL (2)\n"); return 1; }
    std::printf("007d2b70_TEST PASS\n");
    return 0;
}