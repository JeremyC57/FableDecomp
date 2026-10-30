// Behaviour test for 0x00855880 CCreatureAction_StartBlocking::Initialise (see src/00855880.cpp): run the combat-action base
// hook, then when the creature has its combat TC (interface 0x30) call SetStartingToBlock(true); no TC, no call.
#include <cstdio>
struct Combat { int calls; bool starting; void SetStartingToBlock(bool b) { ++calls; starting = b; } };
struct Creature { bool hasCombat; Combat combat; bool GetTC(Combat** out) { if (!hasCombat) return false; *out = &combat; return true; } };
static int g_base = 0;
struct Action { Creature* PCreature; void Base() { ++g_base; } void Run() { Base(); Combat* nav; if (PCreature->GetTC(&nav)) nav->SetStartingToBlock(true); } };
int main()
{
    Creature c1 = { true, { 0, !true } }; Action a1 = { &c1 }; a1.Run();
    Creature c2 = { false, { 0, !true } }; Action a2 = { &c2 }; a2.Run();
    bool ok = g_base == 2 && c1.combat.calls == 1 && c1.combat.starting == true && c2.combat.calls == 0;
    std::printf(ok ? "00855880_TEST PASS\n" : "00855880_TEST FAIL\n");
    return ok ? 0 : 1;
}