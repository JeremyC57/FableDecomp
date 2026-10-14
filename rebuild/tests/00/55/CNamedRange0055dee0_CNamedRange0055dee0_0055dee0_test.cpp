#include <cstdio>
#include <cstring>
static char g_log[512]; static int g_n;
static void Log(const char* s) { int l = (int)std::strlen(s); if (g_n + l + 1 < (int)sizeof(g_log)) { std::memcpy(g_log + g_n, s, l); g_n += l; g_log[g_n++] = ' '; g_log[g_n] = 0; } }
static int Check(const char* addr, const char* want) { if (std::strcmp(g_log, want) != 0) { std::printf("%s_TEST FAIL\n%s\n", addr, g_log); return 1; } std::printf("%s_TEST PASS\n", addr); return 0; }
struct CWideString { void* m_pData; CWideString() : m_pData(0) { Log("W"); } };
struct CCharString { char* m_pData; CCharString() : m_pData(0) { Log("C"); } };
struct CNamedRange_0055dee0 {
    CWideString m_firstName; long m_firstIndex; CWideString m_secondName; CCharString m_tag; long m_secondIndex;
    CNamedRange_0055dee0() : m_firstName(), m_firstIndex(-1), m_secondName(), m_tag(), m_secondIndex(-1) { }
};
int main()
{
    CNamedRange_0055dee0 r;
    if (r.m_firstIndex != -1 || r.m_secondIndex != -1 || r.m_tag.m_pData != 0) return Check("0055dee0", "x");
    return Check("0055dee0", "W W C ");
}