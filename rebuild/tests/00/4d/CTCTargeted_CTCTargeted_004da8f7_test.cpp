// Behaviour test for CTCTargeted::CTCTargeted @ 0x004DA8F7: flags 0, targetable with glow, no LOS exemption, no def.
#include <cstdio>
struct CThing { int id; };
struct CTCBase { CThing* ParentThing; CTCBase(CThing* o) : ParentThing(o) {} };
struct CTCTargeted : CTCBase {
    unsigned long TargetingFlags; bool Targetable, UseTargetGlow, ValidTargetWithoutLOS; void* PTargetingDef;
    CTCTargeted(CThing* o) : CTCBase(o), TargetingFlags(0), Targetable(true), UseTargetGlow(true), ValidTargetWithoutLOS(false), PTargetingDef(0) {}
};
int main()
{
    CThing th; th.id = 0; CTCTargeted t(&th);
    if (t.ParentThing != &th || t.TargetingFlags || !t.Targetable || !t.UseTargetGlow || t.ValidTargetWithoutLOS || t.PTargetingDef) { std::printf("004da8f7_TEST FAIL\n"); return 1; }
    std::printf("004da8f7_TEST PASS\n");
    return 0;
}