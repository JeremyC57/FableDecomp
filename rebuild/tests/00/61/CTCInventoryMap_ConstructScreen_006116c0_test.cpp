#include <cstdio>
// Behaviour of CTCInventoryMap::vfunc_77 (0x006116C0): forwards the argument with both options false.
class CTCInventoryMap { public:
    long m_screen; int m_force; int m_keep; long m_calls;
    void BuildMapTeleportUI(long screen, bool forceRebuild, bool keepSelection) { ++m_calls; m_screen = screen; m_force = forceRebuild; m_keep = keepSelection; }
    void ConstructScreen(long screen) { BuildMapTeleportUI(screen, false, false); }
};
int main()
{
    CTCInventoryMap m; m.m_calls = 0; m.m_force = 1; m.m_keep = 1; m.m_screen = 0;
    m.ConstructScreen(9);
    if (m.m_calls != 1 || m.m_screen != 9 || m.m_force != 0 || m.m_keep != 0) { std::printf("006116c0_TEST FAIL\n"); return 1; }
    std::printf("006116c0_TEST PASS\n");
    return 0;
}