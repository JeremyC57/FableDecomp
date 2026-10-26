// Behaviour test for 0x008C4780 CCreatureAction_ScorpionTailWithdraw::FinishAction: resume a suspended animation,
// remove mode 47 through the mode manager, then the base FinishAction.
#include <cstdio>

static int g_log[16]; static int g_n = 0;
static void Log(int v) { if (g_n < 16) g_log[g_n++] = v; }

class CTCCreatureModeManager { public:
    unsigned int m_removed;
    void RemoveMode(unsigned int mode) { m_removed = mode; Log(100 + (int)mode); }
};
class CThing { public:
    unsigned char m_animationSuspended;
    unsigned int m_presentBits[9];
    CTCCreatureModeManager* m_modes;        // stands in for the {0x31, component} map entry
    void SetSuspendAnimation(bool a, bool b) { m_animationSuspended = 0; Log(a || b ? 99 : 1); }
    template<class T> bool GetInterface(T** ppOut, int interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1u << (interfaceType & 31))) { *ppOut = (T*)m_modes; return true; }
        return false;
    }
};
struct CBaseIntelligentPointer { CThing* m_pItem; CThing* GetPItem() const { return m_pItem; } };
class CCombatActionBase { public:
    CBaseIntelligentPointer PCreature;
    void BaseFinishAction() { Log(2); }
};
class CCreatureAction_ScorpionTailWithdraw : public CCombatActionBase { public:
    void FinishAction()
    {
        if (PCreature.GetPItem()->m_animationSuspended == 1) {
            CThing* creature = PCreature.GetPItem();
            creature->SetSuspendAnimation(false, false);
        }
        CTCCreatureModeManager* pModes = 0;   // retail reads garbage on the miss path; the test always provides the component
        PCreature.GetPItem()->GetInterface(&pModes, 0x31);
        pModes->RemoveMode(47);
        BaseFinishAction();
    }
};

int main()
{
    CTCCreatureModeManager modes; modes.m_removed = 0;
    CThing thing; thing.m_animationSuspended = 1; thing.m_modes = &modes;
    for (int i = 0; i < 9; ++i) thing.m_presentBits[i] = 0;
    thing.m_presentBits[0x31 >> 5] |= 1u << (0x31 & 31);
    CCreatureAction_ScorpionTailWithdraw a; a.PCreature.m_pItem = &thing;
    a.FinishAction();
    if (g_n != 3 || g_log[0] != 1 || g_log[1] != 147 || g_log[2] != 2 || thing.m_animationSuspended != 0) { std::printf("008c4780_TEST FAIL\n"); return 1; }
    g_n = 0; a.FinishAction();   // not suspended: no resume call
    if (g_n != 2 || g_log[0] != 147 || g_log[1] != 2) { std::printf("008c4780_TEST FAIL\n"); return 1; }
    std::printf("008c4780_TEST PASS\n");
    return 0;
}