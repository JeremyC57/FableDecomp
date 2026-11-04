#include <cstdio>
#include <cstdlib>
#include <cstring>
static char g_seq[64] = "";
struct CScriptBase { virtual void v00() { } unsigned char _body[0x3c]; CScriptBase() { std::strcat(g_seq, "B"); } }; static long g_strings = 0; class CCharString { public: void* m_p; CCharString() { m_p = 0; ++g_strings; } };
struct CKeyedBase : CScriptBase { long m_40; long m_44; CKeyedBase(long a, long b) : CScriptBase() { m_44 = a; m_40 = b; } };
struct CV_SingingStonesScript : CKeyedBase { unsigned char _pad48[0x34]; long m_7c, m_80, m_84; CCharString m_strings[4]; virtual void v00() { } CV_SingingStonesScript(long a, long b) : CKeyedBase(a, b), m_7c(0), m_80(0), m_84(0) { } };

int main() {
    CV_SingingStonesScript o(7, 8); if (std::strcmp(g_seq, "B") || o.m_44 != 7 || o.m_40 != 8 || g_strings != 4 || o.m_7c || o.m_80 || o.m_84) { std::printf("00ed0e60_TEST FAIL\n"); return 1; }
    std::printf("00ed0e60_TEST PASS\n"); return 0;
}