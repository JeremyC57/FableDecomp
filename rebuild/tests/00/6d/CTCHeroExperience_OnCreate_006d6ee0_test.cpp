#include <cstdio>
// Behaviour model of CTCHeroExperience::OnCreate (0x006D6EE0): sub-def always, combat def only for hero things.
struct Def { long refs; };
struct Thing { bool hero; Def* expDef; Def* combatDef; };
struct Exp { Def* pdef; Def* combat;
    void OnCreate(Thing* t)
    {
        pdef = t->expDef;
        if (t->hero) { Def* tmp = t->combatDef; ++tmp->refs; if (combat != tmp) { combat = tmp; ++tmp->refs; } --tmp->refs; }
    } };
int main()
{
    Def e = { 1 }, c = { 1 };
    Thing t; t.hero = false; t.expDef = &e; t.combatDef = &c;
    Exp x; x.pdef = 0; x.combat = 0;
    x.OnCreate(&t);
    if (x.pdef != &e || x.combat) { std::printf("006d6ee0_TEST FAIL\n"); return 1; }
    t.hero = true; x.OnCreate(&t);
    if (x.combat != &c || c.refs != 2) { std::printf("006d6ee0_TEST FAIL\n"); return 1; }
    std::printf("006d6ee0_TEST PASS\n");
    return 0;
}