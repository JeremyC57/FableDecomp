// Behaviour test for CTCHero::SetWeaponsAsUsable (0x006CA380): a model of the row -- the flag is always recorded; only making weapons
// unusable while a weapon is carried removes the primary-slot thing (without dropping it: the argument is false).
#include <stdio.h>
struct Carrying { int removed; bool lastArg; void* RemoveThingFromPrimarySlot(bool drop) { ++removed; lastArg = drop; return 0; } };
struct Hero { bool WeaponsUsable; bool carryingWeapon; Carrying carrying;
    void SetWeaponsAsUsable(bool usable) { WeaponsUsable = usable; if (!usable && carryingWeapon) carrying.RemoveThingFromPrimarySlot(false); } };
int main()
{
    Hero h = { true, true, { 0, true } };
    h.SetWeaponsAsUsable(true);
    int ok = h.WeaponsUsable && h.carrying.removed == 0;
    h.SetWeaponsAsUsable(false);
    ok = ok && !h.WeaponsUsable && h.carrying.removed == 1 && !h.carrying.lastArg;
    h.carryingWeapon = false; h.SetWeaponsAsUsable(false);
    ok = ok && h.carrying.removed == 1;
    printf(ok ? "006ca380_TEST PASS\n" : "006ca380_TEST FAIL\n");
    return ok ? 0 : 1;
}