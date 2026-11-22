// Behaviour test for 0x0096DBC0 CCreatureAction_ScreamerIntoAggressive::Initialise(): a model. The creature's mode manager gets a new
// CModeScreamerAggressive built with the target when the target pointer is set and the target is not dead, else built from the
// creature alone; then message 0xC (flags 0xF) is posted at the creature's position naming creature and target, the INTO_COMBAT
// combat animation is set and the combat base Initialise runs, in that order.
#include <cstdio>
#include <string>
#include <vector>

struct Thing { bool dead; float x; };
struct Mode { const Thing* creature; const Thing* target; };
struct World { std::vector<Mode> modes; std::vector<std::string> log; float messageX; const Thing* messageTarget; };

static void Initialise(World& w, const Thing& creature, const Thing* target)
{
    if (target && !target->dead) {
        Mode m = { &creature, target };
        w.modes.push_back(m);
    } else {
        Mode m = { &creature, 0 };
        w.modes.push_back(m);
    }
    w.log.push_back("AddMode");
    w.messageX = creature.x;
    w.messageTarget = target;
    w.log.push_back("AddMessage");
    w.log.push_back("SetCombatAnimation:INTO_COMBAT");
    w.log.push_back("CCombatActionBase::Initialise");
}

int main()
{
    int ok = 1;
    Thing creature = { false, 5.0f }, alive = { false, 1.0f }, dead = { true, 2.0f };
    World w;
    Initialise(w, creature, &alive);
    Initialise(w, creature, &dead);
    Initialise(w, creature, 0);
    ok &= w.modes.size() == 3 && w.modes[0].target == &alive && w.modes[1].target == 0 && w.modes[2].target == 0;
    ok &= w.modes[0].creature == &creature && w.messageX == 5.0f && w.messageTarget == 0;
    ok &= w.log.size() == 12 && w.log[0] == "AddMode" && w.log[2] == "SetCombatAnimation:INTO_COMBAT" && w.log[3] == "CCombatActionBase::Initialise";
    std::printf(ok ? "0096DBC0_TEST PASS\n" : "0096DBC0_TEST FAIL\n");
    return ok ? 0 : 1;
}