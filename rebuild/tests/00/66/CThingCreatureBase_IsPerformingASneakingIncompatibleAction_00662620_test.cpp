// Behaviour test for CThingCreatureBase::IsPerformingASneakingIncompatibleAction (0x00662620)
#include <cstdio>
class CCreatureActionBase { public:
    bool m_incompatible;
    unsigned char m_finished;
    CCreatureActionBase(bool inc, unsigned char fin) : m_incompatible(inc), m_finished(fin) {}
    virtual bool IsIncompatibleWithSneaking() { return m_incompatible; }
};
class CThingCreatureBase { public:
    CCreatureActionBase* m_pCurrentAction;
    long IsPerformingASneakingIncompatibleAction()
    {
        CCreatureActionBase* action = m_pCurrentAction;
        if (action != 0 && !action->m_finished && action->IsIncompatibleWithSneaking())
            return 1;
        return 0;
    }
};
int main()
{
    CThingCreatureBase c;
    CCreatureActionBase a1(true, 0), a2(true, 1), a3(false, 0);
    c.m_pCurrentAction = 0;
    if (c.IsPerformingASneakingIncompatibleAction() != 0) { std::printf("00662620_TEST FAIL\n"); return 1; }
    c.m_pCurrentAction = &a1;
    if (c.IsPerformingASneakingIncompatibleAction() != 1) { std::printf("00662620_TEST FAIL\n"); return 1; }
    c.m_pCurrentAction = &a2;
    if (c.IsPerformingASneakingIncompatibleAction() != 0) { std::printf("00662620_TEST FAIL\n"); return 1; }
    c.m_pCurrentAction = &a3;
    if (c.IsPerformingASneakingIncompatibleAction() != 0) { std::printf("00662620_TEST FAIL\n"); return 1; }
    std::printf("00662620_TEST PASS\n");
    return 0;
}