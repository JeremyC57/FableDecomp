// Behaviour test for 0x00C6EC80 (standalone copy of the model and logic)
#include <cstdio>
#include <cstring>

struct G { float m_volume; int updates; int lastAlt;
    void UpdateGainRamp(bool alternate) { updates++; lastAlt = alternate ? 1 : 0; }
    void SetParameters(int a0, int a1, int a2, float volume, int a4, bool alternate, bool force)
    {
        if (force || m_volume != volume) { m_volume = volume; UpdateGainRamp(alternate); }
    } };
int main()
{
    G g; g.m_volume = 0.5f; g.updates = 0; g.lastAlt = -1;
    g.SetParameters(0, 0, 0, 0.5f, 0, true, false);
    if (g.updates != 0) { std::printf("00c6ec80_TEST FAIL\n"); return 1; }
    g.SetParameters(0, 0, 0, 0.5f, 0, true, true);
    if (g.updates != 1 || g.lastAlt != 1) { std::printf("00c6ec80_TEST FAIL\n"); return 1; }
    g.SetParameters(0, 0, 0, 0.25f, 0, false, false);
    if (g.updates != 2 || g.lastAlt != 0 || g.m_volume != 0.25f) { std::printf("00c6ec80_TEST FAIL\n"); return 1; }
    std::printf("00c6ec80_TEST PASS\n");
    return 0;
}