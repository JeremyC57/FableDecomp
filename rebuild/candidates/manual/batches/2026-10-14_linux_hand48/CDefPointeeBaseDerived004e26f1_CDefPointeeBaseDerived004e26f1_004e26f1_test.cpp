#include <cstdio>
#include <cstdlib>
#include <cstring>
static char g_seq[64] = "";
struct CDefBase { virtual void v00() { } CDefBase() { std::strcat(g_seq, "B"); } };
static void* g_strings[8]; static long g_nStrings = 0;
struct CCharString { void* m_pStringData; CCharString() { m_pStringData = 0; g_strings[g_nStrings++] = this; std::strcat(g_seq, "s"); } ~CCharString() { g_strings[g_nStrings++] = this; std::strcat(g_seq, "d"); } };
struct CBase_009fc550 { virtual void v00() { } ~CBase_009fc550() { std::strcat(g_seq, "M"); } };
struct CDefPointeeBase : CBase_009fc550 { virtual void v00() { } ~CDefPointeeBase() { std::strcat(g_seq, "P"); } };
struct CMember16 { unsigned char _body[0x10]; ~CMember16() { std::strcat(g_seq, "6"); } }; struct CMember12 { unsigned char _body[0xc]; ~CMember12() { std::strcat(g_seq, "c"); } };
struct CDerived : CDefPointeeBase { unsigned char _pad04[0x38]; CCharString m_string3c; CMember12 m_member40; CMember12 m_member4c; unsigned char _pad58[0xc]; CMember16 m_member64; CMember16 m_member74; virtual void v00() { } ~CDerived() { } };

int main() {
    CDerived* d = new CDerived; g_seq[0] = 0; delete d; if (std::strcmp(g_seq, "66ccdPM")) { std::printf("004e26f1_TEST FAIL\n"); return 1; }
    std::printf("004e26f1_TEST PASS\n"); return 0;
}
