// behaviour test for 0073a9f0: IsHoldWeaponWithBothHands needs a carrying TC, a live thing in the both-hands slot with a weapon TC, and a usable weapon
#include <cstdio>
struct SThing;
struct SWeapon { bool usable; bool IsUsableBy(SThing*) const { return usable; } };
struct SCarrying { SThing* bothHands; long askedSlot; SThing* GetThingInCarrySlot(long slot) { askedSlot = slot; return slot == 5 ? bothHands : 0; } };
struct SThing {
    bool hasCarrying, hasWeapon, dead; SCarrying* carrying; SWeapon* weapon;
    long GetDefGlobalIndexFromName(const char*) { return 5; }   // "CARRY_SLOT_BOTH_HANDS"
};
static bool IsHoldWeaponWithBothHands(SThing* parent)
{
    if (parent->hasCarrying)
    {
        SCarrying* carrying = parent->carrying;
        long slot = parent->GetDefGlobalIndexFromName("CARRY_SLOT_BOTH_HANDS");
        SThing* weapon = carrying->GetThingInCarrySlot(slot);
        if (weapon && !weapon->dead && weapon->hasWeapon)
        {
            if (weapon->weapon->IsUsableBy(parent))
                return true;
        }
    }
    return false;
}
int main()
{
    SWeapon w; w.usable = true;
    SThing sword; sword.hasCarrying = false; sword.hasWeapon = true; sword.dead = false; sword.carrying = 0; sword.weapon = &w;
    SCarrying c; c.bothHands = &sword; c.askedSlot = -1;
    SThing hero; hero.hasCarrying = true; hero.hasWeapon = false; hero.dead = false; hero.carrying = &c; hero.weapon = 0;
    int ok = 1;
    if (!IsHoldWeaponWithBothHands(&hero) || c.askedSlot != 5) ok = 0;
    w.usable = false; if (IsHoldWeaponWithBothHands(&hero)) ok = 0;
    w.usable = true; sword.dead = true; if (IsHoldWeaponWithBothHands(&hero)) ok = 0;
    sword.dead = false; sword.hasWeapon = false; if (IsHoldWeaponWithBothHands(&hero)) ok = 0;
    sword.hasWeapon = true; c.bothHands = 0; if (IsHoldWeaponWithBothHands(&hero)) ok = 0;
    c.bothHands = &sword; hero.hasCarrying = false; if (IsHoldWeaponWithBothHands(&hero)) ok = 0;
    if (!ok) { std::printf("0073a9f0_TEST FAIL\n"); return 1; }
    std::printf("0073a9f0_TEST PASS\n");
    return 0;
}