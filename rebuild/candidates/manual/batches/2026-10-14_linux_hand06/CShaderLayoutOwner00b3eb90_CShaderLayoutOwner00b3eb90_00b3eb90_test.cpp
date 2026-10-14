#include <cstdio>
#include <cstring>
static char g_log[512]; static int g_n;
static void Log(const char* s) { int l = (int)std::strlen(s); if (g_n + l + 1 < (int)sizeof(g_log)) { std::memcpy(g_log + g_n, s, l); g_n += l; g_log[g_n++] = ' '; g_log[g_n] = 0; } }
static int Check(const char* addr, const char* want) { if (std::strcmp(g_log, want) != 0) { std::printf("%s_TEST FAIL\n%s\n", addr, g_log); return 1; } std::printf("%s_TEST PASS\n", addr); return 0; }
struct CBaseClass { virtual void v00() {} ~CBaseClass() { Log("cb"); } };
struct CLayoutEntry : CBaseClass { long m_a; long m_b; };
struct CLayoutBase { virtual void v00() {} ~CLayoutBase() { Log("layout"); } unsigned char _pad[0x148]; };
struct CShaderLayoutOwner_00b3eb90 : CLayoutBase { CLayoutEntry m_entry0, m_entry1, m_entry2, m_entry3; ~CShaderLayoutOwner_00b3eb90() { } };
int main()
{
    { CShaderLayoutOwner_00b3eb90 o; (void)o; }
    return Check("00b3eb90", "cb cb cb cb layout ");
}
