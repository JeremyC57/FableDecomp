// Behaviour test for 007ed060 (CTCCarriedActionUseDropGenericBox::vfunc_28): the base hook always runs; a creature with flag 8 is given an alert-to-player action, which is destroyed after the call.
#include <cstdio>

class CThingAICreature;

static int g_baseCalls = 0, g_setCalls = 0, g_dtorCalls = 0;
static CThingAICreature* g_actionCreature = 0;

class CCreatureActionBase { public:
    CThingAICreature* m_creature;
    ~CCreatureActionBase() { ++g_dtorCalls; }
};

namespace N_00842150 { struct CCreatureAction_AlertToPlayer : CCreatureActionBase {
    CCreatureAction_AlertToPlayer(CThingAICreature* creature) { m_creature = creature; }
}; } using N_00842150::CCreatureAction_AlertToPlayer;

class CThingAICreature { public:
    unsigned char m_flags6c;
    bool SetCurrentAction(CCreatureActionBase* action) { ++g_setCalls; g_actionCreature = action->m_creature; return true; }
};

class CTCCarriedActionUseBase { public:
    void OnUseBase(CThingAICreature*) { ++g_baseCalls; }
};

struct CTCCarriedActionUseDropGenericBox : CTCCarriedActionUseBase {
    void vfunc_28(CThingAICreature* user)
    {
        CTCCarriedActionUseBase::OnUseBase(user);
        if (user->m_flags6c & 8) {
            CCreatureAction_AlertToPlayer action(user);
            user->SetCurrentAction(&action);
        }
    }
};

int main()
{
    CThingAICreature c; c.m_flags6c = 0x10;
    CTCCarriedActionUseDropGenericBox a;
    a.vfunc_28(&c);
    if (g_baseCalls != 1 || g_setCalls != 0 || g_dtorCalls != 0) { std::printf("007ed060_TEST FAIL\n"); return 1; }
    c.m_flags6c = 0x18;
    a.vfunc_28(&c);
    if (g_baseCalls != 2 || g_setCalls != 1 || g_dtorCalls != 1 || g_actionCreature != &c) { std::printf("007ed060_TEST FAIL\n"); return 1; }
    std::printf("007ed060_TEST PASS\n");
    return 0;
}