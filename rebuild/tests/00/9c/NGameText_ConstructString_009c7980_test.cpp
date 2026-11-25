// Behaviour test for 0x009C7980 NGameText::ConstructString(CWideString&, const CWideString&): replaces the L"%1" placeholder (retail
// 0x0129B450) with the replacement; false when the text has no %1.
// NF420 (2026-11-25): LINKED test -- it links the row object (the row function is declared here, not defined); the CWideString callees
// are defined under the row's own spellings over a small model, so the test asserts the placeholder the row builds; a mutated literal fails.
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
    CWideString(const unsigned short* text); ~CWideString();
    bool Contains(const CWideString& text) const; void Replace(const CWideString& search, const CWideString& replacement);
    void* PData;
};
namespace NGameText { bool __fastcall ConstructString(CWideString& output, const CWideString& replacement); }
static char g_keys[64];
CWideString::CWideString(const unsigned short* text) { char b[128]; Narrow(text, b); PData = Buf(b); strcat(g_keys, b); strcat(g_keys, ";"); }
CWideString::~CWideString() { }
bool CWideString::Contains(const CWideString& text) const { return strstr((char*)PData, (char*)text.PData) != 0; }
void CWideString::Replace(const CWideString& search, const CWideString& replacement) { ::Replace((char*)PData, (char*)search.PData, (char*)replacement.PData); }
// Row-object externals this test does not model (generated): a call reaches a trap that prints FAIL and exits; a data
// symbol reads a zeroed buffer.
// (none)
int main()
{
    CWideString out(Widen("Hello %1!")); CWideString rep(Widen("Bob")); g_keys[0] = 0;
    bool r1 = NGameText::ConstructString(out, rep);
    bool ok = r1 && strcmp((char*)out.PData, "Hello Bob!") == 0 && strcmp(g_keys, "%1;") == 0;
    CWideString none(Widen("No placeholder")); g_keys[0] = 0;
    bool r2 = NGameText::ConstructString(none, rep);
    ok = ok && !r2 && strcmp((char*)none.PData, "No placeholder") == 0 && strcmp(g_keys, "%1;") == 0;
    if (ok) printf("009c7980_TEST PASS\n");
    else printf("FAIL out=%s keys=%s\n", (char*)out.PData, g_keys);
    return ok ? 0 : 1;
}
