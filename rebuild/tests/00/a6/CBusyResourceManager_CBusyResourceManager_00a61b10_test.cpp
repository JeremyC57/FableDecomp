// Behaviour test for 0x00A61B10: release-all runs first, then the second list, then the first list is destroyed.
#include <cstdio>
static int g_log[8]; static int g_n = 0;
struct CInterfaceList { int id; ~CInterfaceList() { g_log[g_n++] = id; } };
struct CLists {
    CInterfaceList m_devices; CInterfaceList m_effects;
    void ReleaseAll() { g_log[g_n++] = 0; }
    ~CLists() { ReleaseAll(); }
};
int main()
{
    { CLists l; l.m_devices.id = 1; l.m_effects.id = 2; }
    if (g_n != 3 || g_log[0] != 0 || g_log[1] != 2 || g_log[2] != 1) { std::printf("00a61b10_TEST FAIL\n"); return 1; }
    std::printf("00a61b10_TEST PASS\n");
    return 0;
}