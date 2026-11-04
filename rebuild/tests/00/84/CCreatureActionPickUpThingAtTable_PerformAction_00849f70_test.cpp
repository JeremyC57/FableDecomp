// Behaviour test for 0x00849F70 CCreatureAction_PickUpThingAtTable::PerformAction(): a model of the row over stub things; checks
// nothing happens unless both the thing and the creature are alive, the thing goes into the creature's carry slot named by the
// carryable def's ActiveCarrySlot (immediately), and the action's creature mode is added only when it is non-zero.
#include <cstdio>
struct Thing { bool exists, dead; long activeCarrySlot; };
static int g_put, g_slot, g_modes; static unsigned long g_mode; static bool g_now;
static void PerformAction(const Thing& thing, const Thing& creature, unsigned long mode)
{
    if (thing.exists && !thing.dead) {
        if (creature.exists && !creature.dead) {
            ++g_put; g_slot = (int)thing.activeCarrySlot; g_now = true;
            if (mode) { ++g_modes; g_mode = mode; }
        }
    }
}
int main()
{
    int ok = 1;
    Thing t = { true, false, 4 }, c = { true, false, 0 }, dead = { true, true, 0 }, none = { false, false, 0 };
    PerformAction(none, c, 1); PerformAction(dead, c, 1); PerformAction(t, dead, 1); PerformAction(t, none, 1);
    ok &= g_put == 0 && g_modes == 0;
    PerformAction(t, c, 0); ok &= g_put == 1 && g_slot == 4 && g_now && g_modes == 0;
    PerformAction(t, c, 0x21); ok &= g_put == 2 && g_modes == 1 && g_mode == 0x21;
    std::printf(ok ? "00849F70_TEST PASS\n" : "00849F70_TEST FAIL\n");
    return ok ? 0 : 1;
}