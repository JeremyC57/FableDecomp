#include <cstdio>
#include <cstring>
static char g_log[512]; static int g_n;
static void Log(const char* s) { int l = (int)std::strlen(s); if (g_n + l + 1 < (int)sizeof(g_log)) { std::memcpy(g_log + g_n, s, l); g_n += l; g_log[g_n++] = ' '; g_log[g_n] = 0; } }
static int Check(const char* addr, const char* want) { if (std::strcmp(g_log, want) != 0) { std::printf("%s_TEST FAIL\n%s\n", addr, g_log); return 1; } std::printf("%s_TEST PASS\n", addr); return 0; }
struct CBaseIntelligentPointer { virtual void v00() {} void* m_pItem; ~CBaseIntelligentPointer() { Log("ptr"); } };
struct CIntelligentPointer : CBaseIntelligentPointer { };
struct CCharString { char* m_pData; ~CCharString() { Log("str"); } };
struct CAnimFilterBase { virtual void v00() {} ~CAnimFilterBase() { Log("fb"); } };
struct CAnimFilterOfGroup : CAnimFilterBase { CCharString m_group; long m_groupIndex; };
struct CModeBase { virtual void v00() {} long m_type; CAnimFilterOfGroup m_idleFilter; long m_a; long m_b; ~CModeBase() { Log("mode"); } };
struct CModeWithTwoFilters_0073a5d0 : CModeBase { CAnimFilterOfGroup m_actionFilter; CIntelligentPointer m_pTarget; ~CModeWithTwoFilters_0073a5d0() { } };
int main()
{
    { CModeWithTwoFilters_0073a5d0 m; (void)m; }
    return Check("0073a5d0", "ptr str fb mode str fb ");
}