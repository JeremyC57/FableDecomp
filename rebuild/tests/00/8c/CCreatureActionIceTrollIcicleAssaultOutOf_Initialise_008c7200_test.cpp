// behaviour test for 0x008C7200 CCreatureAction_IceTrollIcicleAssaultOutOf::Initialise(): the combat animation is set to "ICICLE_ASSAULT_OUTOF" before the base initialisation runs, and creature mode 0x5D is removed only when the owner has a mode manager
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
        SetCombatAnimation("ICICLE_ASSAULT_OUTOF");
        BaseInitialise();
        ModeManager* modeManager = PCreature->Modes;
        if (modeManager)
            modeManager->RemoveMode(0x5D);
    }
};
int main()
{
    int fail = 0;
    ModeManager mm; Owner withModes; withModes.Modes = &mm; Owner without; without.Modes = 0;
    Action a; a.PCreature = &withModes; gOrder = 0; a.Initialise();
    if (std::strcmp(a.Anim, "ICICLE_ASSAULT_OUTOF") != 0) fail |= 1;
    if (a.animOrder != 1 || a.baseOrder != 2) fail |= 2;
    if (mm.count != 1 || mm.removed != 0x5D) fail |= 4;
    Action b; b.PCreature = &without; b.Initialise();
    if (mm.count != 1 || b.baseOrder == 0) fail |= 8;
    if (fail) { std::printf("008c7200_TEST FAIL %d\n", fail); return 1; }
    std::printf("008c7200_TEST PASS\n");
    return 0;
}