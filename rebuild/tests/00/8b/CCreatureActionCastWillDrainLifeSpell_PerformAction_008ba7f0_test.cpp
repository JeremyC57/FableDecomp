// Behaviour test for CCreatureAction_CastWillDrainLifeSpell::PerformAction (0x008BA7F0): only for a live target and a
// caster with the spell-casting component: a drain is added, the spell aimed, AI casters pass level/power, and a live
// target thing is handed to the drain.
#include <cstdio>

struct CThing;
struct CDrain { long Level, Power; CThing* PTarget; int Added; };
struct CThing { unsigned char m_state; bool Dead; bool HasCasting; CThing* SpellTarget; CDrain Drain; };
struct CAction {
    CThing* PCreature; CThing* PItemRemoved; CThing* PTarget; long SpellLevel, SpellPower;
    void PerformAction()
    {
        if (PTarget && !PTarget->Dead) {
            if (PCreature->HasCasting) {
                CDrain* drain = &PCreature->Drain; ++drain->Added;
                PCreature->SpellTarget = PTarget;
                if (PCreature->m_state == 1) { drain->Level = SpellLevel; drain->Power = SpellPower; }
                CThing* target = PItemRemoved;
                if (target && !target->Dead)
                    drain->PTarget = target;
            }
        }
    }
};

int main()
{
    CThing me = { 1, false, true, 0, { 0, 0, 0, 0 } };
    CThing foe = { 0, false, false, 0, { 0, 0, 0, 0 } };
    CAction a = { &me, &foe, &foe, 3, 7 };
    a.PerformAction();
    bool ok = me.Drain.Added == 1 && me.SpellTarget == &foe && me.Drain.Level == 3 && me.Drain.Power == 7 && me.Drain.PTarget == &foe;
    me.HasCasting = false; a.PerformAction();
    ok = ok && me.Drain.Added == 1;
    me.HasCasting = true; foe.Dead = true; a.PerformAction();
    ok = ok && me.Drain.Added == 1;
    if (!ok) { std::printf("008ba7f0_TEST FAIL\n"); return 1; }
    std::printf("008ba7f0_TEST PASS\n");
    return 0;
}