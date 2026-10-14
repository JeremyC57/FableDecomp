#include <cstdio>
#include <cstring>
static char g_log[512]; static int g_n;
static void Log(const char* s) { int l = (int)std::strlen(s); if (g_n + l + 1 < (int)sizeof(g_log)) { std::memcpy(g_log + g_n, s, l); g_n += l; g_log[g_n++] = ' '; g_log[g_n] = 0; } }
static int Check(const char* addr, const char* want) { if (std::strcmp(g_log, want) != 0) { std::printf("%s_TEST FAIL\n%s\n", addr, g_log); return 1; } std::printf("%s_TEST PASS\n", addr); return 0; }
struct CWideString { void* m_pData; ~CWideString() { char b[16]; std::sprintf(b, "w%d", (int)(long)m_pData); Log(b); } };
struct CWideStringQuintet_0042f7c1 { unsigned char _pad[0x20]; CWideString m_text0, m_text1, m_text2, m_text3, m_text4; ~CWideStringQuintet_0042f7c1(); };
CWideStringQuintet_0042f7c1::~CWideStringQuintet_0042f7c1() { }
int main()
{
    { CWideStringQuintet_0042f7c1 q; q.m_text0.m_pData = (void*)0; q.m_text1.m_pData = (void*)1; q.m_text2.m_pData = (void*)2; q.m_text3.m_pData = (void*)3; q.m_text4.m_pData = (void*)4; }
    return Check("0042f7c1", "w4 w3 w2 w1 w0 ");
}
