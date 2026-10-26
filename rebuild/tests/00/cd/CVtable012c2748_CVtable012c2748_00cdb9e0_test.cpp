#include <cstdio>
#include <cstdlib>
#include <cstring>
static char g_seq[64] = "";
struct CBase { virtual void v00() { } CBase() { std::strcat(g_seq, "B"); } }; class CCharString { public: void* m_p; CCharString() { m_p = 0; std::strcat(g_seq, "s"); } };
struct CObject : CBase { unsigned char _pad04[0x50]; CCharString m_string54; unsigned char _pad58[0xdc]; long m_134, m_138, m_13c; virtual void v00() { } CObject() : CBase(), m_134(0), m_138(0), m_13c(0) { } };

int main() {
    CObject o; if (std::strcmp(g_seq, "Bs") || o.m_134 || o.m_138 || o.m_13c) { std::printf("00cdb9e0_TEST FAIL\n"); return 1; }
    std::printf("00cdb9e0_TEST PASS\n"); return 0;
}