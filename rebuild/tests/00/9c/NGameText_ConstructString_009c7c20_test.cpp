// Behaviour test for 0x009C7C20 NGameText::ConstructString(CWideString&, const vector<CWideString>&): replaces %1..%N with the vector's
// strings; each key is L"%" (retail 0x0129B470) + the index formatted with L"%d" (0x0122DE44); false if any placeholder is missing.
// NF420 (2026-11-25): LINKED test -- it links the row object (the row function is declared here, not defined); the CWideString callees
// are defined under the row's own spellings over a small model (Sprintf records its format), so the test asserts both literals; a
// mutated literal fails.
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
extern "C" void __cdecl NF420TrapHit(const char* sym) { printf("FAIL unexpected call %s\n", sym); exit(3); }
extern "C" char NF420TrapData[4096] = { 0 };
static int Len(const unsigned short* s) { int n = 0; while (s[n]) ++n; return n; }
static unsigned short* Widen(const char* s) { int n = (int)strlen(s); unsigned short* d = (unsigned short*)malloc((n + 1) * 2); for (int i = 0; i <= n; ++i) d[i] = (unsigned char)s[i]; return d; }
static void Narrow(const unsigned short* w, char* out) { int i = 0; for (; w[i] && i < 127; ++i) out[i] = (char)w[i]; out[i] = 0; }
// wide-string model: PData -> a 128-char narrow mirror buffer (all edits are done on narrow text)
static char* Buf(const char* s) { char* d = (char*)malloc(128); strcpy(d, s); return d; }
static void Replace(char* text, const char* key, const char* with) { char* p = strstr(text, key); if (!p) return; char tail[128]; strcpy(tail, p + strlen(key)); strcpy(p, with); strcat(p, tail); }
class CWideString { public:
    CWideString(); CWideString(const CWideString& other); ~CWideString();
    void __cdecl Sprintf(const unsigned short* format, ...);
    bool Contains(const CWideString& text) const; void Replace(const CWideString& search, const CWideString& replacement);
    void* PData;
};
CWideString __fastcall operator+(const unsigned short* left, const CWideString& right);
namespace std {
template <class _Tp> class allocator { };
template <class _Tp, class _Alloc = allocator<_Tp> > class vector { public: _Tp* _M_start; _Tp* _M_finish; _Tp* _M_end_of_storage; };
}
namespace NGameText { bool __fastcall ConstructString(CWideString& output, const std::vector<CWideString>& replacements); }
static char g_formats[64], g_prefixes[64];
CWideString::CWideString() { PData = Buf(""); }
CWideString::CWideString(const CWideString& other) { PData = Buf((char*)other.PData); }
CWideString::~CWideString() { }
void __cdecl CWideString::Sprintf(const unsigned short* format, ...)
{ char f[32]; Narrow(format, f); strcat(g_formats, f); strcat(g_formats, ";"); int* args = (int*)&format + 1; sprintf((char*)PData, f, args[0]); }
CWideString __fastcall operator+(const unsigned short* left, const CWideString& right)
{ char l[32]; Narrow(left, l); strcat(g_prefixes, l); strcat(g_prefixes, ";"); CWideString r; strcpy((char*)r.PData, l); strcat((char*)r.PData, (char*)right.PData); return r; }
bool CWideString::Contains(const CWideString& text) const { return strstr((char*)PData, (char*)text.PData) != 0; }
void CWideString::Replace(const CWideString& search, const CWideString& replacement) { ::Replace((char*)PData, (char*)search.PData, (char*)replacement.PData); }
// Row-object externals this test does not model (generated): a call reaches a trap that prints FAIL and exits; a data
// symbol reads a zeroed buffer.
// (none)
int main()
{
    CWideString reps[2]; strcpy((char*)reps[0].PData, "Bob"); strcpy((char*)reps[1].PData, "Oakvale");
    std::vector<CWideString> v; v._M_start = reps; v._M_finish = reps + 2; v._M_end_of_storage = reps + 2;
    CWideString out; strcpy((char*)out.PData, "%1 lives in %2.");
    bool r1 = NGameText::ConstructString(out, v);
    bool ok = r1 && strcmp((char*)out.PData, "Bob lives in Oakvale.") == 0 && strcmp(g_formats, "%d;%d;") == 0 && strcmp(g_prefixes, "%;%;") == 0;
    CWideString part; strcpy((char*)part.PData, "Only %2");
    bool r2 = NGameText::ConstructString(part, v);
    ok = ok && !r2 && strcmp((char*)part.PData, "Only Oakvale") == 0;
    if (ok) printf("009c7c20_TEST PASS\n");
    else printf("FAIL out=%s formats=%s prefixes=%s\n", (char*)out.PData, g_formats, g_prefixes);
    return ok ? 0 : 1;
}
