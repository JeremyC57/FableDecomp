// Behaviour test for 0x00663340 CThingCreatureBase::GetPHeroCombatDef() const: fetch the hero combat def only for a positive index (standalone copy of the logic)
#include <cstdio>
struct Def { int Refs; };
struct Manager { Def defs[4]; int lookups; bool GetDef(long i, Def** out) { ++lookups; *out = &defs[i]; ++defs[i].Refs; return true; } };
struct Creature { long HeroCombatDef; Manager* M; Def* GetPHeroCombatDef() { Def* d = 0; long index = HeroCombatDef; if (index > 0) M->GetDef(index, &d); return d; } };

int main()
{
    Manager m; m.lookups = 0; for (int i = 0; i < 4; ++i) m.defs[i].Refs = 0; Creature c; c.M = &m;
    c.HeroCombatDef = 0; if (c.GetPHeroCombatDef() || m.lookups) { std::printf("00663340_TEST FAIL\n"); return 1; }
    c.HeroCombatDef = -1; if (c.GetPHeroCombatDef() || m.lookups) { std::printf("00663340_TEST FAIL\n"); return 1; }
    c.HeroCombatDef = 2; if (c.GetPHeroCombatDef() != &m.defs[2] || m.lookups != 1 || m.defs[2].Refs != 1) { std::printf("00663340_TEST FAIL\n"); return 1; }
    std::printf("00663340_TEST PASS\n");
    return 0;
}