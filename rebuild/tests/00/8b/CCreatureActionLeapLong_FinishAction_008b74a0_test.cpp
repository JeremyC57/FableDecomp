// Test: CCreatureAction_LeapLong::FinishAction() removes thing component 77 only when the creature has it, then runs the base FinishAction.
#include <cstdio>
static int g_removed = 0, g_baseFinish = 0, g_getItem = 0;
class CThing { public:
    unsigned int PresentTCs[9];
    bool HasTC(int type) const { return (PresentTCs[type >> 5] & (1u << (type & 31))) != 0; }
    void RemoveTC(int type) { ++g_removed; PresentTCs[type >> 5] &= ~(1u << (type & 31)); }
};
class CIntelligentPointer { public:
    CThing* Item;
    CThing* GetPItem() const { ++g_getItem; return Item; }
};
class CCombatActionBase { public:
    virtual ~CCombatActionBase() { }
    CIntelligentPointer PCreature;
protected:
    virtual void FinishAction() { ++g_baseFinish; }
};
class CCreatureAction_LeapLong : public CCombatActionBase { public:
    virtual void FinishAction();
};
void CCreatureAction_LeapLong::FinishAction()
{
    if (PCreature.GetPItem()->HasTC(77))
        PCreature.GetPItem()->RemoveTC(77);
    CCombatActionBase::FinishAction();
}
int main()
{
    CThing thing;
    for (int i = 0; i < 9; ++i) thing.PresentTCs[i] = 0;
    CCreatureAction_LeapLong action;
    action.PCreature.Item = &thing;
    action.FinishAction();
    if (g_removed != 0 || g_baseFinish != 1 || g_getItem != 1) { std::printf("008b74a0_TEST FAIL\n"); return 1; }
    thing.PresentTCs[2] = 1u << 13;               // component 77
    action.FinishAction();
    if (g_removed != 1 || g_baseFinish != 2 || g_getItem != 3 || thing.HasTC(77)) { std::printf("008b74a0_TEST FAIL\n"); return 1; }
    std::printf("008b74a0_TEST PASS\n");
    return 0;
}