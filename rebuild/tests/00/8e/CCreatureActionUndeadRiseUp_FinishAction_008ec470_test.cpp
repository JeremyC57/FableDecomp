// Behaviour test for 0x008ec470 CCreatureAction_UndeadRiseUp (FinishAction): the
// rising flag gates the component flag store, the underground-mode removal and the flag clear; the base call always runs.
#include <cstdio>
static int g_removed = 0, g_base = 0, g_unsuspend = 0;
struct CComponent { unsigned char m_flag; };
struct CThing {
    unsigned int m_bits;
    CComponent* m_comp9;
    bool GetInterface(CComponent** out, int type) { if (m_bits & (1u << type)) { *out = m_comp9; return true; } return false; }
    void SetSuspendAnimation(bool, bool) { ++g_unsuspend; }
};
struct CAction {
    CThing* m_creature;
    float m_ratio;
    unsigned char m_rising;
    CThing* GetPItem() { return m_creature; }
    float GetProgressRatio() { return m_ratio; }
    void RemoveUndergroundMode() { ++g_removed; }
    void BaseCall() { ++g_base; }
    void FrameUpdate()
    {
        if (m_rising) {
            if (GetProgressRatio() > 0.5f) {
                CComponent* c;
                if (GetPItem()->GetInterface(&c, 9))
                    c->m_flag = 1;
                RemoveUndergroundMode();
                m_rising = 0;
            }
        }
        BaseCall();
    }
    void FinishAction()
    {
        if (m_rising) {
            CComponent* c;
            if (GetPItem()->GetInterface(&c, 9))
                c->m_flag = 1;
            RemoveUndergroundMode();
            m_rising = 0;
        }
        CThing* creature = GetPItem();
        creature->SetSuspendAnimation(false, false);
        BaseCall();
    }
};
int main()
{
    CComponent comp; comp.m_flag = 0;
    CThing t; t.m_bits = 1u << 9; t.m_comp9 = &comp;
    CAction a; a.m_creature = &t; a.m_rising = 1; a.m_ratio = 0.25f;
    bool frame = false;
    if (frame) {
        a.FrameUpdate();
        if (g_removed != 0 || comp.m_flag != 0 || a.m_rising != 1 || g_base != 1) { std::printf("008ec470_TEST FAIL\n"); return 1; }
        a.m_ratio = 0.75f;
        a.FrameUpdate();
        if (g_removed != 1 || comp.m_flag != 1 || a.m_rising != 0 || g_base != 2) { std::printf("008ec470_TEST FAIL\n"); return 1; }
        a.FrameUpdate();
        if (g_removed != 1 || g_base != 3) { std::printf("008ec470_TEST FAIL\n"); return 1; }
    } else {
        t.m_bits = 0;
        a.FinishAction();
        if (g_removed != 1 || comp.m_flag != 0 || a.m_rising != 0 || g_base != 1 || g_unsuspend != 1) { std::printf("008ec470_TEST FAIL\n"); return 1; }
        a.m_rising = 1; t.m_bits = 1u << 9;
        a.FinishAction();
        if (g_removed != 2 || comp.m_flag != 1 || g_base != 2 || g_unsuspend != 2) { std::printf("008ec470_TEST FAIL\n"); return 1; }
        a.FinishAction();
        if (g_removed != 2 || g_base != 3) { std::printf("008ec470_TEST FAIL\n"); return 1; }
    }
    std::printf("008ec470_TEST PASS\n");
    return 0;
}