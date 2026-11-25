// Behaviour test for 0x00581B33 CTCInventory::HasScreenshots: starts a file scan of <photojournal dir><valid profile name> + L"\\"
// (retail 0x0122DE78) + L"*.jpg" (0x0124FED0) and returns whether a file was found.
// NF420 (2026-11-25): LINKED test -- it links the row object (the row function is declared here, not defined); the callees are defined
// under the row's own spellings over a small wide-string model, so the test asserts the exact scan pattern; a mutated literal fails.
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
extern "C" void __cdecl NF420TrapHit(const char* sym) { printf("FAIL unexpected call %s\n", sym); exit(3); }
extern "C" char NF420TrapData[4096] = { 0 };
static int Len(const unsigned short* s) { int n = 0; while (s[n]) ++n; return n; }
static unsigned short* Cat(const unsigned short* a, const unsigned short* b) { int x = Len(a), y = Len(b); unsigned short* d = (unsigned short*)malloc((x + y + 1) * 2); memcpy(d, a, x * 2); memcpy(d + x, b, y * 2); d[x + y] = 0; return d; }
static unsigned short* Widen(const char* s) { int n = (int)strlen(s); unsigned short* d = (unsigned short*)malloc((n + 1) * 2); for (int i = 0; i <= n; ++i) d[i] = (unsigned char)s[i]; return d; }
static bool Same(const void* w, const char* s) { const unsigned short* p = (const unsigned short*)w; int i = 0; for (; s[i]; ++i) if (p[i] != (unsigned char)s[i]) return false; return p[i] == 0; }
typedef unsigned short wchar_t16;
class CWideString { public: void* PStringData; ~CWideString(); };
CWideString __fastcall operator+(const CWideString& left, const CWideString& right);
CWideString __fastcall operator+(const CWideString& left, const wchar_t16* right);
CWideString __fastcall GetValidProfileName(const CWideString& name);
namespace NResourceDirectoryNames { CWideString __fastcall GetPhotojournalDir(); }
class CUserProfileManager { public: unsigned char _pad_0x00[0x08]; CWideString ProfileName; static CUserProfileManager& __fastcall Get(); };
class CAFile { public:
    class CFileScanInfo { public: bool Stopped; long SearchHandle; };
    static bool __fastcall StartFileScan(const CWideString& pattern, CFileScanInfo& info, wchar_t16* fileName);
};
class CTCInventory { public: bool HasScreenshots(); };
static char g_pattern[128]; static bool g_found = true;
// strings are never freed (the model has no copy constructor, so a returned local is copied bitwise); destructions are counted
static int g_made = 0, g_destroyed = 0;
static CWideString Make(unsigned short* d) { CWideString r; r.PStringData = d; ++g_made; return r; }
CWideString::~CWideString() { ++g_destroyed; }
CWideString __fastcall operator+(const CWideString& l, const CWideString& r) { return Make(Cat((unsigned short*)l.PStringData, (unsigned short*)r.PStringData)); }
CWideString __fastcall operator+(const CWideString& l, const wchar_t16* r) { return Make(Cat((unsigned short*)l.PStringData, r)); }
CWideString __fastcall GetValidProfileName(const CWideString& name) { return Make(Cat((unsigned short*)name.PStringData, Widen("_ok"))); }
CWideString __fastcall NResourceDirectoryNames::GetPhotojournalDir() { return Make(Widen("Photos\\")); }
static double g_mgrRaw[2];
CUserProfileManager& __fastcall CUserProfileManager::Get() { return *(CUserProfileManager*)g_mgrRaw; }
bool __fastcall CAFile::StartFileScan(const CWideString& pattern, CFileScanInfo&, wchar_t16* fileName)
{ const unsigned short* p = (const unsigned short*)pattern.PStringData; int i = 0; for (; p[i] && i < 127; ++i) g_pattern[i] = (char)p[i]; g_pattern[i] = 0; fileName[0] = 0; return g_found; }
// Row-object externals this test does not model (generated): a call reaches a trap that prints FAIL and exits; a data
// symbol reads a zeroed buffer.
// (none)
int main()
{
    ((CUserProfileManager*)g_mgrRaw)->ProfileName.PStringData = Widen("Hero");
    CTCInventory inv;
    bool r1 = inv.HasScreenshots();
    bool ok = r1 && strcmp(g_pattern, "Photos\\Hero_ok\\*.jpg") == 0 && g_destroyed > 0;
    g_found = false; bool r2 = inv.HasScreenshots();
    ok = ok && !r2;
    if (ok) printf("00581b33_TEST PASS\n");
    else printf("FAIL pattern=%s destroyed=%d\n", g_pattern, g_destroyed);
    return ok ? 0 : 1;
}
