// Behaviour test for 0x008B8CB0 CCreatureAction_ScreamerAdvance::FrameUpdate: past 30% of the animation, TotalTime = 1 once mode 0x44 is held.
#include <cstdio>
static int g_base = 0;
struct CTCCreatureModeManager {
    unsigned int m_mode;
    bool HasMode(unsigned int mode) { return mode == m_mode; }
};
struct CThing {
    unsigned int m_presentBits[9];
    CTCCreatureModeManager* m_modes;
    template<class T> bool GetInterface(T** ppOut, int interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1u << (interfaceType & 31))) { *ppOut = (T*)m_modes; return true; }
        return false;
    }
};
struct CBaseIntelligentPointer { CThing* m_pItem; CThing* GetPItem() const { return m_pItem; } };
struct CAction {
    CBaseIntelligentPointer PCreature;
    int TotalTime;
    float m_fraction;
    float GetAnimationFraction() { return m_fraction; }
    void FrameUpdate()
    {
        if (GetAnimationFraction() > 0.3f) {
            CTCCreatureModeManager* pModes = 0;
            PCreature.GetPItem()->GetInterface(&pModes, 0x31);
            if (pModes->HasMode(0x44)) TotalTime = 1;
        }
        ++g_base;
    }
};
static int Fail() { std::printf("008b8cb0_TEST FAIL\n"); return 1; }
int main()
{
    CTCCreatureModeManager modes; modes.m_mode = 0x44;
    CThing thing; for (int i = 0; i < 9; ++i) thing.m_presentBits[i] = 0;
    thing.m_presentBits[0x31 >> 5] |= 1u << (0x31 & 31); thing.m_modes = &modes;
    CAction a; a.PCreature.m_pItem = &thing; a.TotalTime = 50;
    a.m_fraction = 0.3f; a.FrameUpdate();
    if (a.TotalTime != 50 || g_base != 1) return Fail();        // not past the threshold
    a.m_fraction = 0.5f; modes.m_mode = 3; a.FrameUpdate();
    if (a.TotalTime != 50 || g_base != 2) return Fail();        // mode not held
    modes.m_mode = 0x44; a.FrameUpdate();
    if (a.TotalTime != 1 || g_base != 3) return Fail();
    std::printf("008b8cb0_TEST PASS\n");
    return 0;
}