// Behaviour test for 0x007F4B60 CTCHasName::GetNPCNameAndFullTitle: "<short name>" + L" - " (retail 0x0122E070) + the occupation's text
// when the bank has that entry, else the short name, else L"Missing text tag" (0x01271DA8).
// NF420 (2026-11-25): LINKED test -- it links the row object (the row function is declared here, not defined); the callees are defined
// under the row's own spellings over a small wide-string model (strings are never freed), so the test asserts the exact texts; a
// mutated literal fails.
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
class CWideString { public: void* PStringData; CWideString(const wchar16* text); CWideString(const CWideString& other); ~CWideString(); long GetLength() const; };
CWideString __fastcall operator+(const CWideString& left, const wchar16* right);
CWideString __fastcall operator+(const CWideString& left, const CWideString& right);
struct CDefVirtual { virtual void v00(); virtual void Destroy() const; };
struct CRefCounted { mutable long m_count; };
struct CIVCountedPointee : CDefVirtual, CRefCounted { };
template <class T> class CIVCountedPointer { public: T* Object; ~CIVCountedPointer() { } };
namespace NGameText {
class CTextEntry : public CIVCountedPointee { public: CWideString GetText() const; };
class CDataBank { public: bool EntryExists(unsigned long index); CIVCountedPointer<const CTextEntry> GetPTextEntry(unsigned long index, bool load) const; };
}
class CTCHasName { public: CWideString GetNPCShortName(); CWideString GetNPCNameAndFullTitle(); };
static int g_destroyed = 0, g_bad = 0; static bool g_exists = true; static const char* g_short = "Bob";
CWideString::CWideString(const wchar16* text) { PStringData = (void*)text; }
CWideString::CWideString(const CWideString& other) { PStringData = other.PStringData; }
CWideString::~CWideString() { }
long CWideString::GetLength() const { return Len((const unsigned short*)PStringData); }
CWideString __fastcall operator+(const CWideString& l, const wchar16* r) { return CWideString(Cat((unsigned short*)l.PStringData, r)); }
CWideString __fastcall operator+(const CWideString& l, const CWideString& r) { return CWideString(Cat((unsigned short*)l.PStringData, (unsigned short*)r.PStringData)); }
void CDefVirtual::v00() { }
void CDefVirtual::Destroy() const { ++g_destroyed; }
static NGameText::CTextEntry g_entry;
CWideString NGameText::CTextEntry::GetText() const { return CWideString(Widen("Smith")); }
bool NGameText::CDataBank::EntryExists(unsigned long index) { if (index != 77) g_bad = 1; return g_exists; }
CIVCountedPointer<const NGameText::CTextEntry> NGameText::CDataBank::GetPTextEntry(unsigned long index, bool load) const
{ if (index != 77 || !load) g_bad = 1; CIVCountedPointer<const CTextEntry> p; p.Object = &g_entry; g_entry.m_count = 1; return p; }
CWideString CTCHasName::GetNPCShortName() { return CWideString(Widen(g_short)); }
extern "C" unsigned char* NF420_Main = 0;   // CMainGameComponent::PMainGameComponent
#pragma comment(linker, "/alternatename:?PMainGameComponent@CMainGameComponent@@1PAV1@A=_NF420_Main")
// Row-object externals this test does not model (generated): a call reaches a trap that prints FAIL and exits; a data
// symbol reads a zeroed buffer.
// (none)
int main()
{
    NGameText::CDataBank bank; static double mainRaw[3]; NF420_Main = (unsigned char*)mainRaw; *(NGameText::CDataBank**)(NF420_Main + 0x14) = &bank;
    static double defRaw[7]; *(unsigned long*)((unsigned char*)defRaw + 0x30) = 77;               // Occupation
    static double tcRaw[2]; *(void**)((unsigned char*)tcRaw + 0xc) = defRaw;                        // PDef
    CTCHasName* tc = (CTCHasName*)tcRaw;
    CWideString a = tc->GetNPCNameAndFullTitle();
    bool ok = Same(a.PStringData, "Bob - Smith") && g_destroyed == 1;
    g_exists = false; CWideString b = tc->GetNPCNameAndFullTitle();
    ok = ok && Same(b.PStringData, "Bob");
    g_short = ""; CWideString c = tc->GetNPCNameAndFullTitle();
    ok = ok && Same(c.PStringData, "Missing text tag") && !g_bad;
    if (ok) printf("007f4b60_TEST PASS\n");
    else printf("FAIL destroyed=%d bad=%d\n", g_destroyed, g_bad);
    return ok ? 0 : 1;
}
