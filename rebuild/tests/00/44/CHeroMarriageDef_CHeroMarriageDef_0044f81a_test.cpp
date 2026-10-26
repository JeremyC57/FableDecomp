#include <cstdio>
#include <cstdlib>
#include <cstring>
static char g_seq[64] = "";
struct CDefBase { virtual void v00() { } CDefBase() { std::strcat(g_seq, "B"); } };
static void* g_strings[8]; static long g_nStrings = 0;
class CCharString { public: void* m_pStringData; CCharString() { m_pStringData = 0; g_strings[g_nStrings++] = this; std::strcat(g_seq, "s"); } ~CCharString() { g_strings[g_nStrings++] = this; std::strcat(g_seq, "d"); } };
struct CBase_009fc550 { virtual void v00() { } ~CBase_009fc550() { std::strcat(g_seq, "M"); } };
class CDefPointeeBase : public CBase_009fc550 { public: virtual void v00() { } ~CDefPointeeBase() { std::strcat(g_seq, "P"); } };
class CHeroMarriageDef : public CDefPointeeBase { public: unsigned char _pad04[0x78 - sizeof(void*) + 4]; CCharString m_string7c; unsigned char _pad80[0x24]; CCharString m_stringa4; CCharString m_stringa8; unsigned char _padac[0xc]; CCharString m_stringb8; unsigned char _padbc[8]; CCharString m_stringc4; virtual void v00() { } ~CHeroMarriageDef() { } };

int main() {
    CHeroMarriageDef* d = new CHeroMarriageDef; g_seq[0] = 0; g_nStrings = 0; void* c4 = &d->m_stringc4; void* s7c = &d->m_string7c; delete d; if (std::strcmp(g_seq, "dddddPM") || g_strings[0] != c4 || g_strings[4] != s7c) { std::printf("0044f81a_TEST FAIL\n"); return 1; }
    std::printf("0044f81a_TEST PASS\n"); return 0;
}