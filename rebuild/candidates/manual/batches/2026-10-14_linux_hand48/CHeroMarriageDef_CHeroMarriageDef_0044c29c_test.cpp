#include <cstdio>
#include <cstdlib>
#include <cstring>
static char g_seq[64] = "";
struct CDefBase { virtual void v00() { } CDefBase() { std::strcat(g_seq, "B"); } };
static void* g_strings[8]; static long g_nStrings = 0;
struct CCharString { void* m_pStringData; CCharString() { m_pStringData = 0; g_strings[g_nStrings++] = this; std::strcat(g_seq, "s"); } ~CCharString() { g_strings[g_nStrings++] = this; std::strcat(g_seq, "d"); } };
struct CHeroMarriageDef : CDefBase { unsigned char _pad04[0x78]; CCharString m_string7c; unsigned char _pad80[0x24]; CCharString m_stringa4; CCharString m_stringa8; unsigned char _padac[0xc]; CCharString m_stringb8; unsigned char _padbc[8]; CCharString m_stringc4; virtual void v00() { } CHeroMarriageDef() : CDefBase() { } };

int main() {
    CHeroMarriageDef* d = new CHeroMarriageDef; if (std::strcmp(g_seq, "Bsssss") || g_strings[0] != &d->m_string7c || g_strings[4] != &d->m_stringc4) { std::printf("0044c29c_TEST FAIL\n"); return 1; }
    std::printf("0044c29c_TEST PASS\n"); return 0;
}
