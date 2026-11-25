// Behaviour test for 0x004069E0 CUserProfileManager::GetAutoSaveDisplayName: the game's (else the front end's) text bank looks up
// "TEXT_GUI_PROFILE_DEFAULT" (retail 0x0122DF40); with neither component the name is L"Default" (0x0122DE80).
// NF420 (2026-11-25): LINKED test -- it links the row object (the row function is declared here, not defined); the callees are defined
// under the row's own spellings and the two component statics are aliased to test data, so the test asserts both literals; a mutated
// literal fails.
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
extern "C" void __cdecl NF420TrapHit(const char* sym) { printf("FAIL unexpected call %s\n", sym); exit(3); }
extern "C" char NF420TrapData[4096] = { 0 };
static int Len(const unsigned short* s) { int n = 0; while (s[n]) ++n; return n; }
static unsigned short* Cat(const unsigned short* a, const unsigned short* b) { int x = Len(a), y = Len(b); unsigned short* d = (unsigned short*)malloc((x + y + 1) * 2); memcpy(d, a, x * 2); memcpy(d + x, b, y * 2); d[x + y] = 0; return d; }
static unsigned short* Widen(const char* s) { int n = (int)strlen(s); unsigned short* d = (unsigned short*)malloc((n + 1) * 2); for (int i = 0; i <= n; ++i) d[i] = (unsigned char)s[i]; return d; }
static bool Same(const void* w, const char* s) { const unsigned short* p = (const unsigned short*)w; int i = 0; for (; s[i]; ++i) if (p[i] != (unsigned char)s[i]) return false; return p[i] == 0; }
typedef unsigned short wchar16;
class CWideString { public: const wchar16* p; CWideString(const wchar16* s); ~CWideString(); };
class CCharString { public: const char* p; CCharString(const char* s, long n); ~CCharString(); };
namespace NGameText { class CDataBank { public: int Id; const CWideString GetTextBySymbol(const CCharString& symbol) const; }; }
class CUserProfileManager { public: CWideString GetAutoSaveDisplayName(); };
static char g_symbol[64]; static int g_bank = 0, g_bad = 0;
CWideString::CWideString(const wchar16* s) { p = s; }
CWideString::~CWideString() { }
CCharString::CCharString(const char* s, long n) { p = s; if (n != -1) g_bad = 1; }
CCharString::~CCharString() { }
static unsigned short kFound[] = { 'F', 'O', 'U', 'N', 'D', 0 };
const CWideString NGameText::CDataBank::GetTextBySymbol(const CCharString& symbol) const { strcpy(g_symbol, symbol.p); g_bank = Id; return CWideString(kFound); }
// The component instance statics (CMainGameComponent::PMainGameComponent, CNewFrontendGameComponent::m_pNewFrontendGameComponent).
extern "C" unsigned char* NF420_Main = 0;
extern "C" unsigned char* NF420_Frontend = 0;
#pragma comment(linker, "/alternatename:?PMainGameComponent@CMainGameComponent@@1PAV1@A=_NF420_Main")
#pragma comment(linker, "/alternatename:?m_pNewFrontendGameComponent@CNewFrontendGameComponent@@0PAV1@A=_NF420_Frontend")
// Row-object externals this test does not model (generated): a call reaches a trap that prints FAIL and exits; a data
// symbol reads a zeroed buffer.
// (none)
int main()
{
    CUserProfileManager mgr;
    CWideString none = mgr.GetAutoSaveDisplayName();
    bool ok = Same(none.p, "Default") && g_symbol[0] == 0;
    static double feRaw[0x68 / 8]; NGameText::CDataBank feBank; feBank.Id = 2; *(NGameText::CDataBank**)((unsigned char*)feRaw + 0x60) = &feBank;
    NF420_Frontend = (unsigned char*)feRaw;
    CWideString fe = mgr.GetAutoSaveDisplayName();
    ok = ok && fe.p == kFound && g_bank == 2 && strcmp(g_symbol, "TEXT_GUI_PROFILE_DEFAULT") == 0;
    static double mainRaw[0x18 / 8]; NGameText::CDataBank mainBank; mainBank.Id = 1; *(NGameText::CDataBank**)((unsigned char*)mainRaw + 0x14) = &mainBank;
    NF420_Main = (unsigned char*)mainRaw; g_symbol[0] = 0;
    CWideString mn = mgr.GetAutoSaveDisplayName();
    ok = ok && mn.p == kFound && g_bank == 1 && strcmp(g_symbol, "TEXT_GUI_PROFILE_DEFAULT") == 0 && !g_bad;
    if (ok) printf("004069e0_TEST PASS\n");
    else printf("FAIL symbol=%s bank=%d bad=%d\n", g_symbol, g_bank, g_bad);
    return ok ? 0 : 1;
}
