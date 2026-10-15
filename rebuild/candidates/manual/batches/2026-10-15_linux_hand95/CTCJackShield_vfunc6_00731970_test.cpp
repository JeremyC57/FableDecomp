#include <cstdio>
// Behaviour of CTCJackShield::vfunc_6 (0x00731970).
struct CEngineGlobals { bool m_answer; virtual bool QuerySlot9() { return m_answer; } };
CEngineGlobals g_engine;
CEngineGlobals* g_pEngineGlobals = &g_engine;
struct CThing { long m_calls; bool m_last; virtual void Slot28(bool flag) { ++m_calls; m_last = flag; } };
struct CTCJackShield {
    CThing* ParentThing; long m_shieldCalls; bool m_shield;
    void EnableShield(bool enable) { ++m_shieldCalls; m_shield = enable; }
    void vfunc_6()
    {
        EnableShield(false);
        if (!g_pEngineGlobals->QuerySlot9())
            ParentThing->Slot28(false);
    }
};
int main()
{
    CThing thing; thing.m_calls = 0; thing.m_last = true;
    CTCJackShield s; s.ParentThing = &thing; s.m_shieldCalls = 0; s.m_shield = true;
    g_engine.m_answer = true; s.vfunc_6();
    int bad = (s.m_shieldCalls != 1 || s.m_shield || thing.m_calls != 0);
    g_engine.m_answer = false; s.vfunc_6();
    bad |= (s.m_shieldCalls != 2 || thing.m_calls != 1 || thing.m_last);
    if (bad) { std::printf("00731970_TEST FAIL\n"); return 1; }
    std::printf("00731970_TEST PASS\n");
    return 0;
}
