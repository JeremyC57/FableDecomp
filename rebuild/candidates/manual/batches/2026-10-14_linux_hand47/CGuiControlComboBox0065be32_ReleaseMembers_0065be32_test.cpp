#include <cstdio>
#include <cstdlib>
#include <cstring>
// behaviour test for 0x0065BE32: exercises the same logic through a copy of the model (compiled separately from the source)
static int g_log[8]; static int g_n = 0;
struct CMember_0065be32 { void* m_p; };
struct C {
    long _pad[19]; CMember_0065be32 m_member4c, m_member50, m_member54, m_member58, m_member5c;
    void Release4c(CMember_0065be32* m) { g_log[g_n++] = (m == &m_member4c) ? 1 : -1; }
    void Release50(CMember_0065be32* m) { g_log[g_n++] = (m == &m_member50) ? 2 : -1; }
    void Release54(CMember_0065be32* m) { g_log[g_n++] = (m == &m_member54) ? 3 : -1; }
    void Release58(CMember_0065be32* m) { g_log[g_n++] = (m == &m_member58) ? 4 : -1; }
    void Release5c(CMember_0065be32* m) { g_log[g_n++] = (m == &m_member5c) ? 5 : -1; }
    void ReleaseMembers() { Release4c(&m_member4c); Release50(&m_member50); Release54(&m_member54); Release58(&m_member58); Release5c(&m_member5c); }
};
static bool Run()
{
    C c; c.ReleaseMembers();
    return g_n == 5 && g_log[0] == 1 && g_log[1] == 2 && g_log[2] == 3 && g_log[3] == 4 && g_log[4] == 5;
}
int main()
{
    if (!Run()) { std::printf("0065be32_TEST FAIL\n"); return 1; }
    std::printf("0065be32_TEST PASS\n");
    return 0;
}
