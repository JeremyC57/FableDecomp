#include <cstdio>
#include <cstring>
static char g_log[512]; static int g_n;
static void Log(const char* s) { int l = (int)std::strlen(s); if (g_n + l + 1 < (int)sizeof(g_log)) { std::memcpy(g_log + g_n, s, l); g_n += l; g_log[g_n++] = ' '; g_log[g_n] = 0; } }
static int Check(const char* addr, const char* want) { if (std::strcmp(g_log, want) != 0) { std::printf("%s_TEST FAIL\n%s\n", addr, g_log); return 1; } std::printf("%s_TEST PASS\n", addr); return 0; }
struct CCharString { char* m_pData; CCharString() : m_pData(0) { Log("str"); } };
struct CAnimFilterBase { virtual void v00() {} CAnimFilterBase() { } };
struct CAnimFilterOf2Groups : CAnimFilterBase {
    CCharString m_groupA; CCharString m_groupB;
    CAnimFilterOf2Groups(const CCharString& a, const CCharString& b) : m_groupA(), m_groupB() { SetGroups(a, b); }
    void SetGroups(const CCharString& a, const CCharString& b) { Log(a.m_pData); Log(b.m_pData); }
};
int main()
{
    CCharString a; a.m_pData = (char*)"left"; CCharString b; b.m_pData = (char*)"right"; g_n = 0; g_log[0] = 0;
    CAnimFilterOf2Groups f(a, b);
    if (f.m_groupA.m_pData != 0 || f.m_groupB.m_pData != 0) return Check("0073e9d0", "x");
    return Check("0073e9d0", "str str left right ");
}