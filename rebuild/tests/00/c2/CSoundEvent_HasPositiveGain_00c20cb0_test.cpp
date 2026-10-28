// Test: CSoundEvent::HasPositiveGain -- state check first, then 1 when the event's gain is above zero, else 0.
#include <cstdio>
static int g_checks = 0;
struct CParams { float gain; float GetGain(void*) { return gain; } };
struct CEvent { CParams m_params; void CheckState(int expected) { if (expected == -1) ++g_checks; }
    int HasPositiveGain(void* mgr) { CheckState(-1); if (m_params.GetGain(mgr) > 0.0f) return 1; return 0; } };
int main()
{
    CEvent e; e.m_params.gain = 0.5f; bool ok = e.HasPositiveGain(0) == 1;
    e.m_params.gain = 0.0f; ok = ok && e.HasPositiveGain(0) == 0;
    e.m_params.gain = -1.0f; ok = ok && e.HasPositiveGain(0) == 0 && g_checks == 3;
    std::printf(ok ? "00c20cb0_TEST PASS\n" : "00c20cb0_TEST FAIL\n");
    return ok ? 0 : 1;
}