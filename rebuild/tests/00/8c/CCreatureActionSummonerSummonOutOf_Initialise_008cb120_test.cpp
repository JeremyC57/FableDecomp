// behaviour test for 0x008CB120 CCreatureAction_SummonerSummonOutOf::Initialise(): the combat animation is set to "SUMMON_OUT_OF" before the base initialisation runs, and creature mode 0x59 is removed only when the owner has a mode manager
#include <cstdio>
#include <cstring>
static int gOrder = 0;
struct ModeManager { int removed; int count; ModeManager() : removed(0), count(0) { } void RemoveMode(unsigned int m) { removed = (int)m; count++; } };
struct Owner { ModeManager* Modes; };
struct Action {
    Owner* PCreature; char Anim[64]; int animOrder, baseOrder;
    Action() : PCreature(0), animOrder(0), baseOrder(0) { Anim[0] = 0; }
    void SetCombatAnimation(const char* name) { std::strcpy(Anim, name); animOrder = ++gOrder; }
    void BaseInitialise() { baseOrder = ++gOrder; }
    void Initialise()                                    // the function under test
    {
        SetCombatAnimation("SUMMON_OUT_OF");
        BaseInitialise();
        ModeManager* modeManager = PCreature->Modes;
        if (modeManager)
            modeManager->RemoveMode(0x59);
    }
};
int main()
{
    int fail = 0;
    ModeManager mm; Owner withModes; withModes.Modes = &mm; Owner without; without.Modes = 0;
    Action a; a.PCreature = &withModes; gOrder = 0; a.Initialise();
    if (std::strcmp(a.Anim, "SUMMON_OUT_OF") != 0) fail |= 1;
    if (a.animOrder != 1 || a.baseOrder != 2) fail |= 2;
    if (mm.count != 1 || mm.removed != 0x59) fail |= 4;
    Action b; b.PCreature = &without; b.Initialise();
    if (mm.count != 1 || b.baseOrder == 0) fail |= 8;
    if (fail) { std::printf("008cb120_TEST FAIL %d\n", fail); return 1; }
    std::printf("008cb120_TEST PASS\n");
    return 0;
}