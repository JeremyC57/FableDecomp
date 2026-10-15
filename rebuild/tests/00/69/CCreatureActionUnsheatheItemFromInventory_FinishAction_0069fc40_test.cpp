// Behaviour test for 0x0069FC40 CCreatureAction_UnsheatheItemFromInventory::FinishAction: perform-if-needed, base finish,
// follow-up handed to the creature (refusal notifies the follow-up), mode manager notified only when present.
#include <cstdio>
static int g_perform = 0, g_baseFinish = 0, g_rejected = 0, g_setCalls = 0, g_modeNotified = 0;
struct CAction {
    unsigned char PerformedAction;
    virtual void PerformAction() { ++g_perform; PerformedAction = 1; }
    virtual void OnRejectedByCreature() { ++g_rejected; }
    virtual ~CAction() {}
};
struct CModes { void OnUnsheatheFinished() { ++g_modeNotified; } };
struct CThing {
    bool m_accepts;
    unsigned int m_bits[9];
    CModes* m_modes;
    bool SetCurrentAction(CAction*) { ++g_setCalls; return m_accepts; }
    bool GetInterface(CModes** out, int type)
    {
        if (m_bits[type >> 5] & (1u << (type & 31))) { *out = m_modes; return true; }
        return false;
    }
};
struct CUnsheathe : CAction {
    CThing* m_creature;
    CAction* m_followUp;
    CThing* GetPItem() { return m_creature; }
    bool GetPerformedAction() { return PerformedAction != 0; }
    void BaseFinish() { ++g_baseFinish; }
    void FinishAction()
    {
        if (!GetPerformedAction())
            PerformAction();
        BaseFinish();
        if (m_followUp) {
            if (!GetPItem()->SetCurrentAction(m_followUp))
                m_followUp->OnRejectedByCreature();
        }
        CModes* modes;
        if (GetPItem()->GetInterface(&modes, 0x31))
            modes->OnUnsheatheFinished();
    }
};
int main()
{
    CModes modes; CThing t; t.m_accepts = false; for (int i = 0; i < 9; ++i) t.m_bits[i] = 0; t.m_modes = &modes;
    CAction follow; follow.PerformedAction = 0;
    CUnsheathe a; a.PerformedAction = 0; a.m_creature = &t; a.m_followUp = 0;
    a.FinishAction();
    if (g_perform != 1 || g_baseFinish != 1 || g_setCalls != 0 || g_modeNotified != 0) { std::printf("0069fc40_TEST FAIL\n"); return 1; }
    a.m_followUp = &follow; t.m_bits[1] = 0x20000;
    a.FinishAction();
    if (g_perform != 1 || g_setCalls != 1 || g_rejected != 1 || g_modeNotified != 1) { std::printf("0069fc40_TEST FAIL\n"); return 1; }
    t.m_accepts = true;
    a.FinishAction();
    if (g_setCalls != 2 || g_rejected != 1 || g_modeNotified != 2) { std::printf("0069fc40_TEST FAIL\n"); return 1; }
    std::printf("0069fc40_TEST PASS\n");
    return 0;
}