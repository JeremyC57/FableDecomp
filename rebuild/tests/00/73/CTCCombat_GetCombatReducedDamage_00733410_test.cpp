#include <cstdio>
class CThing;
class CCreatureActionBase { public: long ActionType; long ActionSubType; bool Finished; };
class CThingCreatureBase { public:
    bool m_stance; CCreatureActionBase* CurrentAction;
    virtual bool IsInCombatStance() { return m_stance; }
    bool IsPerformingAnyAction() { return CurrentAction != 0 && !CurrentAction->Finished; }
};
class CTCCombat { public:
    CThingCreatureBase* ParentCreature;
    float GetCombatReducedDamage(float damage, CThing* attacker)
    {
        if (ParentCreature->IsInCombatStance() && ParentCreature->IsPerformingAnyAction()) {
            CCreatureActionBase* action = ParentCreature->CurrentAction;
            if (action->ActionType == 100 && action->ActionSubType == 250)
                return 0.0f;
        }
        return damage;
    }
};
int main()
{
    CCreatureActionBase blk = { 100, 250, false }, other = { 100, 251, false }, done = { 100, 250, true };
    CThingCreatureBase c; c.m_stance = true; c.CurrentAction = &blk;
    CTCCombat t; t.ParentCreature = &c;
    if (t.GetCombatReducedDamage(12.5f, 0) != 0.0f) { std::printf("00733410_TEST FAIL\n"); return 1; }
    c.CurrentAction = &other;
    if (t.GetCombatReducedDamage(12.5f, 0) != 12.5f) { std::printf("00733410_TEST FAIL\n"); return 1; }
    c.CurrentAction = &done;
    if (t.GetCombatReducedDamage(3.0f, 0) != 3.0f) { std::printf("00733410_TEST FAIL\n"); return 1; }
    c.CurrentAction = &blk; c.m_stance = false;
    if (t.GetCombatReducedDamage(4.0f, 0) != 4.0f) { std::printf("00733410_TEST FAIL\n"); return 1; }
    c.CurrentAction = 0; c.m_stance = true;
    if (t.GetCombatReducedDamage(5.0f, 0) != 5.0f) { std::printf("00733410_TEST FAIL\n"); return 1; }
    std::printf("00733410_TEST PASS\n");
    return 0;
}