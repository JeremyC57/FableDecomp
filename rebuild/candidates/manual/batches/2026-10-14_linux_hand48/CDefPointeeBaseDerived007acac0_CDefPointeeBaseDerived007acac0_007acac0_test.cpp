#include <cstdio>
#include <cstdlib>
#include <cstring>
static char g_seq[64] = "";
struct CDefBase { virtual void v00() { } CDefBase() { std::strcat(g_seq, "B"); } };
static void* g_strings[8]; static long g_nStrings = 0;
struct CCharString { void* m_pStringData; CCharString() { m_pStringData = 0; g_strings[g_nStrings++] = this; std::strcat(g_seq, "s"); } ~CCharString() { g_strings[g_nStrings++] = this; std::strcat(g_seq, "d"); } };
struct CBase_009fc550 { virtual void v00() { } ~CBase_009fc550() { std::strcat(g_seq, "M"); } };
struct CDefPointeeBase : CBase_009fc550 { virtual void v00() { } ~CDefPointeeBase() { std::strcat(g_seq, "P"); } };
struct CDerived : CDefPointeeBase { unsigned char _pad04[0x60]; CCharString m_string64; unsigned char _pad68[4]; CCharString m_string6c; unsigned char _pad70[0x1c]; CCharString m_string8c; CCharString m_string90; virtual void v00() { } ~CDerived() { } };

int main() {
    CDerived* d = new CDerived; g_seq[0] = 0; g_nStrings = 0; void* s90 = &d->m_string90; void* s64 = &d->m_string64; delete d; if (std::strcmp(g_seq, "ddddPM") || g_strings[0] != s90 || g_strings[3] != s64) { std::printf("007acac0_TEST FAIL\n"); return 1; }
    std::printf("007acac0_TEST PASS\n"); return 0;
}
