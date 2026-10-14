// Behaviour test for CAnimationEnvironment::InitializeWithDefaults @ 0x00c3ec30: bind the global device; negative Initialise -> unbind + false; else Reset + true.
#include <cstdio>
static int g_reset = 0; static long g_result = 0; static long g_a = 0, g_b = 0;
struct CAnimationDevice {
    virtual void v0() { }
    virtual long Initialise(long a, long b) { g_a = a; g_b = b; return g_result; }
    virtual void v2() { }
    virtual void Reset() { ++g_reset; }
};
static CAnimationDevice g_animationDevice;
struct CAnimationEnvironment {
    unsigned char _pad[0x1d0]; long m_value1d0; CAnimationDevice* m_pDevice;
    bool InitializeWithDefaults(long a, long b)
    {
        m_value1d0 = 0; m_pDevice = &g_animationDevice;
        if (m_pDevice->Initialise(a, b) < 0) { m_pDevice = 0; return false; }
        m_pDevice->Reset(); return true;
    }
};
int main()
{
    CAnimationEnvironment env; env.m_value1d0 = 123; env.m_pDevice = 0;
    g_result = -1;
    bool ok = env.InitializeWithDefaults(3, 4) == false && env.m_pDevice == 0 && env.m_value1d0 == 0 && g_reset == 0 && g_a == 3 && g_b == 4;
    g_result = 0;
    if (!(env.InitializeWithDefaults(5, 6) == true && env.m_pDevice == &g_animationDevice && g_reset == 1 && g_a == 5 && g_b == 6)) ok = false;
    g_result = 7;
    if (!(env.InitializeWithDefaults(0, 0) == true && g_reset == 2)) ok = false;
    if (!ok) { std::printf("00c3ec30_TEST FAIL\n"); return 1; }
    std::printf("00c3ec30_TEST PASS\n");
    return 0;
}
