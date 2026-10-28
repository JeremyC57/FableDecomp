// Behaviour test for 0x00692C30 CCreatureActionBase::GetActionSpeedParameter: the stats def speed, or 0 when there is no stats def (standalone copy of the logic)
#include <cstdio>
struct Def { long Refs; float Speed; };
struct Action { Def* stats; bool GetSubDef(Def** out) const { if (!stats) return false; *out = stats; ++stats->Refs; return true; }
    float GetActionSpeedParameter() const { float speed = 0.0f; Def* def = 0; if (GetSubDef(&def)) speed = def->Speed; if (def) --def->Refs; return speed; } };

int main()
{
    Def d; d.Refs = 1; d.Speed = 2.5f; Action a; a.stats = &d;
    if (a.GetActionSpeedParameter() != 2.5f || d.Refs != 1) { std::printf("00692c30_TEST FAIL\n"); return 1; }
    a.stats = 0; if (a.GetActionSpeedParameter() != 0.0f) { std::printf("00692c30_TEST FAIL\n"); return 1; }
    std::printf("00692c30_TEST PASS\n");
    return 0;
}