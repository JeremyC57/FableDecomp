// Behaviour test for CCreatureAction_RemovePhysicalShieldSpell::PerformAction 0x008BA7E0: performing the action reads the action's
// creature pointer (one GetPItem on PCreature) and changes no state of the action or the creature.
#include <cstdio>
struct CThingCreatureBase { int shield; };
static int g_reads = 0;
struct CIntelligentPointer { CThingCreatureBase* Item; CThingCreatureBase* Get() const { ++g_reads; return Item; } };
struct CCreatureActionBase { virtual ~CCreatureActionBase() {} virtual void PerformAction() = 0; int Base; CIntelligentPointer PCreature; };
struct CCreatureAction_RemovePhysicalShieldSpell : CCreatureActionBase { void PerformAction() { PCreature.Get(); } };
int main()
{
    CThingCreatureBase c = { 1 };
    CCreatureAction_RemovePhysicalShieldSpell a; a.Base = 7; a.PCreature.Item = &c;
    CCreatureActionBase& base = a;
    base.PerformAction();
    if (g_reads != 1 || c.shield != 1 || a.Base != 7 || a.PCreature.Item != &c) { std::printf("008ba7e0_TEST FAIL\n"); return 1; }
    std::printf("008ba7e0_TEST PASS\n");
    return 0;
}