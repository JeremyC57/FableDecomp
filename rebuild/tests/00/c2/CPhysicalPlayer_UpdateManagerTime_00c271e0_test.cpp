// Test: CPhysicalPlayer::UpdateManagerTime -- calls the manager's GetFaderOutput once and changes nothing.
#include <cstdio>
static int g_calls = 0; static const void* g_this = 0;
struct CSoundEventManager { float m_level; float GetFaderOutput() const { ++g_calls; g_this = this; return m_level; } };
struct CPhysicalPlayer { int m_state; void UpdateManagerTime(CSoundEventManager* manager) { manager->GetFaderOutput(); } };
int main()
{
    CSoundEventManager m; m.m_level = 0.25f; CPhysicalPlayer p; p.m_state = 7;
    p.UpdateManagerTime(&m);
    bool ok = g_calls == 1 && g_this == &m && p.m_state == 7 && m.m_level == 0.25f;
    std::printf(ok ? "00c271e0_TEST PASS\n" : "00c271e0_TEST FAIL\n");
    return ok ? 0 : 1;
}
