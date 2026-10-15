#include <cstdio>
static unsigned char g_bAggressiveModeAvailable = 1;
struct CPlayer {
    bool AggressiveMode; int m_setups;
    void SetUpControlsBasedOnAggressiveMode() { ++m_setups; }
    void SetAggressiveMode(bool on)
    {
        if (g_bAggressiveModeAvailable) {
            if (!AggressiveMode && on) { AggressiveMode = true; SetUpControlsBasedOnAggressiveMode(); }
            else if (AggressiveMode && !on) { AggressiveMode = on; SetUpControlsBasedOnAggressiveMode(); }
        }
    }
};
int main()
{
    CPlayer p; p.AggressiveMode = false; p.m_setups = 0;
    p.SetAggressiveMode(false); if (p.AggressiveMode || p.m_setups != 0) { std::printf("00487eb0_TEST FAIL\n"); return 1; }
    p.SetAggressiveMode(true); if (!p.AggressiveMode || p.m_setups != 1) { std::printf("00487eb0_TEST FAIL\n"); return 1; }
    p.SetAggressiveMode(true); if (!p.AggressiveMode || p.m_setups != 1) { std::printf("00487eb0_TEST FAIL\n"); return 1; }
    p.SetAggressiveMode(false); if (p.AggressiveMode || p.m_setups != 2) { std::printf("00487eb0_TEST FAIL\n"); return 1; }
    g_bAggressiveModeAvailable = 0;
    p.SetAggressiveMode(true); if (p.AggressiveMode || p.m_setups != 2) { std::printf("00487eb0_TEST FAIL\n"); return 1; }
    std::printf("00487eb0_TEST PASS\n");
    return 0;
}