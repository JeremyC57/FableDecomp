// Behaviour test for 0x0072B940 CTCSpecialAbilities::CalculateWeaponState: unarmed without a carrying component, an empty
// primary slot or a dead held thing; armed when the held thing is not the active ranged weapon; for the active ranged weapon,
// bow unless it has a weapon component of type 4 (armed), and bow when it has no weapon component.
#include <cstdio>
enum EWEAPON_STATE { UNARMED = 0, ARMED = 1, BOW = 2 };
struct Thing { bool Dead; bool HasWeapon; int WeaponType; };
struct Creature { bool HasCarrying; Thing* Primary; bool HasInventory; Thing* ActiveRanged; };
static EWEAPON_STATE CalculateWeaponState(const Creature& c)
{
    if (c.HasCarrying) {
        Thing* weapon = c.Primary;
        if (weapon && !weapon->Dead) {
            if (weapon == c.ActiveRanged) {
                if (weapon->HasWeapon) return weapon->WeaponType == 4 ? ARMED : BOW;
                return BOW;
            }
            return ARMED;
        }
    }
    return UNARMED;
}
int main()
{
    Thing sword = { false, true, 1 }, bow = { false, true, 2 }, crossbow = { false, true, 4 }, rock = { false, false, 0 };
    Thing dead = { true, true, 2 };
    Creature none = { false, &sword, true, &bow };
    Creature empty = { true, 0, true, &bow };
    Creature corpse = { true, &dead, true, &dead };
    Creature melee = { true, &sword, true, &bow };
    Creature archer = { true, &bow, true, &bow };
    Creature cross = { true, &crossbow, true, &crossbow };
    Creature thrower = { true, &rock, true, &rock };
    bool ok = CalculateWeaponState(none) == UNARMED && CalculateWeaponState(empty) == UNARMED &&
              CalculateWeaponState(corpse) == UNARMED && CalculateWeaponState(melee) == ARMED &&
              CalculateWeaponState(archer) == BOW && CalculateWeaponState(cross) == ARMED && CalculateWeaponState(thrower) == BOW;
    if (!ok) { std::printf("0072b940_TEST FAIL\n"); return 1; }
    std::printf("0072b940_TEST PASS\n");
    return 0;
}