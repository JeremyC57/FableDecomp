// Behaviour test for 005ea310: a self-contained copy of the candidate's model and body, run over stub callees.
// CTCTavernGame::GetTotalWinnings() -- the "total winnings" text from the tavern-game def followed by the total winnings as a number
#include <cstdio>
#include <cstring>
#ifndef _MSC_VER
#define __forceinline inline
#define __fastcall
#define __stdcall
#define __cdecl
#define __declspec(x)
#endif
#include <stddef.h>

class CWideString { public:
    void* m_data;
    CWideString();                              // 0x0099AED0
    CWideString(const CWideString& other);      // 0x0099B720
    ~CWideString();                             // 0x0099B510
    void __cdecl Sprintf(const wchar_t* format, ...);   // 0x0099BA70
};
CWideString __fastcall operator+(const CWideString& a, const CWideString& b);  // 0x0099BE70

inline CWideString IntToWideString(long value)
{
    CWideString text;
    text.Sprintf(L"%d", value);      // 0x0122DE44
    return text;
}

struct CDefVirtual {
    virtual void v00();
    virtual void Destroy();                     // slot 1
};
class CTavernGameDef : public CDefVirtual { public:
    long m_count;                               // +0x04
    unsigned char _pad_0x08[0xec];
    unsigned long TotalWinnings;                // +0xf4
    void Release() { if (--m_count == 0) Destroy(); }
};
struct CDefPointer {
    CTavernGameDef* Object;
    CDefPointer() : Object(0) {}
    ~CDefPointer() { if (Object) Object->Release(); }
    CTavernGameDef* operator->() const { return Object; }
};
class CDefinitionManager { public:
    CTavernGameDef* PeekDefFromGlobalIndex(long index);   // stand-in only
};
class CDefClassBase { public:
    virtual long* GetSubDefIndex(const char* name);       // stand-in only (retail: slot 14, CCharString name)
    unsigned char       _pad_0x04[0x18];
    CDefinitionManager* m_pManager;             // +0x1c
    bool GetSubDef_005ed4a0(CDefPointer& out);  // 0x005ED4A0 (GetSubDef<CTavernGameDef>)
};

// Stand-in for the same-module GetSubDef<CTavernGameDef> body (0x005ED4A0, landed separately; not graded here):
// compiled above the caller without inlining so VC7.1 sees, as retail's whole-program build did, that the def pointer
// it fills does not escape -- the caller then keeps the def in edi and releases it with `dec [edi+4] ; jne`.
bool CDefClassBase::GetSubDef_005ed4a0(CDefPointer& out)
{
    long* index = GetSubDefIndex("CTavernGameDef");
    if (index == 0)
        return false;
    CTavernGameDef* def = m_pManager->PeekDefFromGlobalIndex(*index);
    if (out.Object != def) {
        if (out.Object)
            out.Object->Release();
        out.Object = def;
        if (def)
            ++def->m_count;
    }
    return true;
}
class CThing { public:
    unsigned char  _pad_0x00[0x70];
    CDefClassBase* PDef;                        // +0x70
};

class CTCTavernGame { public:
    virtual void v00();
    CThing*       ParentThing;                  // +0x04 (CTCBase)
    unsigned char _pad_0x08[0x34];
    long          TotalWinnings;                // +0x3c
    void GetDef(CDefPointer* out) { ParentThing->PDef->GetSubDef_005ed4a0(*out); }   // out of line 0x005EA000
    CWideString GetText(unsigned long id);      // 0x005E9E70
    virtual CWideString GetTotalWinnings();
};

CWideString CTCTavernGame::GetTotalWinnings()
{
    CDefPointer def;
    GetDef(&def);
    return GetText(def->TotalWinnings) + IntToWideString(TotalWinnings);
}

// ---- stubs and driver ----
#include <cstdarg>
static char g_pool[4096]; static int g_used;
static char* Alloc(const char* s) { int n = (int)std::strlen(s) + 1; char* p = g_pool + g_used; g_used += n; std::memcpy(p, s, n); return p; }
CWideString::CWideString() { m_data = Alloc(""); }
CWideString::CWideString(const CWideString& other) { m_data = Alloc((const char*)other.m_data); }
CWideString::~CWideString() {}
void __cdecl CWideString::Sprintf(const wchar_t* format, ...)
{ CWideString* text = this; va_list ap; va_start(ap, format); long v = va_arg(ap, long); va_end(ap); char b[32]; std::sprintf(b, "%ld", v); text->m_data = Alloc(b); }
CWideString __fastcall operator+(const CWideString& a, const CWideString& b)
{ char t[256]; std::strcpy(t, (const char*)a.m_data); std::strcat(t, (const char*)b.m_data); CWideString w; w.m_data = Alloc(t); return w; }
static int g_destroyed;
void CDefVirtual::v00() {}
void CDefVirtual::Destroy() { ++g_destroyed; }
static CTavernGameDef g_def;
static long g_index = 5;
long* CDefClassBase::GetSubDefIndex(const char* name) { return std::strcmp(name, "CTavernGameDef") == 0 ? &g_index : 0; }
CTavernGameDef* CDefinitionManager::PeekDefFromGlobalIndex(long index) { return index == 5 ? &g_def : 0; }
void CTCTavernGame::v00() {}
CWideString CTCTavernGame::GetText(unsigned long id) { char b[32]; std::sprintf(b, "T%lu:", id); CWideString w; w.m_data = Alloc(b); return w; }

int main()
{
    static CDefinitionManager manager;
    static CDefClassBase thingDef;
    static CThing thing;
    thingDef.m_pManager = &manager;
    thing.PDef = &thingDef;
    CTCTavernGame game;
    game.ParentThing = &thing;
    game.TotalWinnings = 345;
    g_def.TotalWinnings = 61; g_def.m_count = 1;
    CWideString r = game.GetTotalWinnings();
    if (std::strcmp((const char*)r.m_data, "T61:345") != 0 || g_def.m_count != 1 || g_destroyed != 0)
        { std::printf("005ea310_TEST FAIL (1: %s)\n", (const char*)r.m_data); return 1; }
    g_def.m_count = 0; game.TotalWinnings = -2;
    CWideString r2 = game.GetTotalWinnings();
    if (std::strcmp((const char*)r2.m_data, "T61:-2") != 0 || g_def.m_count != 0 || g_destroyed != 1)
        { std::printf("005ea310_TEST FAIL (2)\n"); return 1; }
    std::printf("005ea310_TEST PASS\n");
    return 0;
}