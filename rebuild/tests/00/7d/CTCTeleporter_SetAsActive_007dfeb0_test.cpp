#include <cstdio>
// Behaviour test for CTCTeleporter::SetAsActive (0x007DFEB0).
class CPlayerGui { public: int refreshes; long lastFlag; void Refresh(long flag) { ++refreshes; lastFlag = flag; } };
static CPlayerGui g_gui;
static CPlayerGui* g_pPlayerGui = &g_gui;
class CTCTeleporter { public:
    bool m_teleportActive;
    bool m_teleportActivated;
    void SetAsActive(bool active);
};
void CTCTeleporter::SetAsActive(bool active)
{
    if (active != m_teleportActive) {
        m_teleportActive = active;
        if (active)
            m_teleportActivated = true;
        g_pPlayerGui->Refresh(0);
    }
}
int main()
{
    g_gui.refreshes = 0; g_gui.lastFlag = 9;
    CTCTeleporter t; t.m_teleportActive = false; t.m_teleportActivated = false;
    t.SetAsActive(false);
    if (g_gui.refreshes != 0 || t.m_teleportActivated) { std::printf("007dfeb0_TEST FAIL\n"); return 1; }
    t.SetAsActive(true);
    if (g_gui.refreshes != 1 || g_gui.lastFlag != 0 || !t.m_teleportActive || !t.m_teleportActivated) { std::printf("007dfeb0_TEST FAIL\n"); return 1; }
    t.SetAsActive(false);
    if (g_gui.refreshes != 2 || t.m_teleportActive || !t.m_teleportActivated) { std::printf("007dfeb0_TEST FAIL\n"); return 1; }
    std::printf("007dfeb0_TEST PASS\n");
    return 0;
}