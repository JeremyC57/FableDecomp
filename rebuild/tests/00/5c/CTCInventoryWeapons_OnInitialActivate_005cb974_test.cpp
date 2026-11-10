// Behaviour test of CTCInventoryWeapons::OnInitialActivate (0x005CB974): saved active weapon def indices (> 0) are re-activated and
// cleared; a weapon flagged as carried on load is moved into its active carry slot (taken off the owner first when it already
// carries it) only when the hero's weapons are usable, and the flag is cleared; the base activation always runs last.
#include <cstdio>
#include <cstring>
static char g_log[128];
static void Log(const char* s) { std::strcat(g_log, s); }
class CThing { public: int id; };
struct CCarryable { bool carried; CThing* carriedBy; long slot; };
struct CWeapons {
    long ActiveMeleeWeaponDefIndex; bool MeleeWeaponCarriedOnLoad; CThing* melee; CCarryable* meleeCarryable;
    long ActiveRangedWeaponDefIndex; bool RangedWeaponCarriedOnLoad; CThing* ranged; CCarryable* rangedCarryable;
    CThing* ParentThing; bool hasHero; bool WeaponsUsable; bool hasCarrying;
    void SetThingAsActiveWeapon(long def, void*) { char b[16]; std::sprintf(b, "A%ld", def); Log(b); }
    void Restore(long& defIndex, bool& carriedOnLoad, CThing* weapon, CCarryable* carryable)
    {
        if (defIndex > 0) {
            SetThingAsActiveWeapon(defIndex, 0);
            defIndex = 0;
            if (carriedOnLoad && weapon) {
                if (hasHero && WeaponsUsable && carryable && hasCarrying) {
                    if (carryable->carried && carryable->carriedBy == ParentThing) Log("R");
                    if (carryable->slot > 0) Log("P");
                }
                carriedOnLoad = false;
            }
        }
    }
    void OnInitialActivate()
    {
        Restore(ActiveMeleeWeaponDefIndex, MeleeWeaponCarriedOnLoad, melee, meleeCarryable);
        Restore(ActiveRangedWeaponDefIndex, RangedWeaponCarriedOnLoad, ranged, rangedCarryable);
        Log("B");
    }
};
int main()
{
    CThing hero = { 1 }, sword = { 2 }, bow = { 3 };
    CCarryable sc = { true, &hero, 2 }, bc = { false, 0, 0 };
    CWeapons w = { 7, true, &sword, &sc, 9, true, &bow, &bc, &hero, true, true, true };
    w.OnInitialActivate();
    int bad = std::strcmp(g_log, "A7RPA9B") != 0 || w.ActiveMeleeWeaponDefIndex != 0 || w.MeleeWeaponCarriedOnLoad
              || w.RangedWeaponCarriedOnLoad || w.ActiveRangedWeaponDefIndex != 0;
    g_log[0] = 0; w.ActiveMeleeWeaponDefIndex = 4; w.MeleeWeaponCarriedOnLoad = true; w.WeaponsUsable = false;
    w.OnInitialActivate();
    bad |= std::strcmp(g_log, "A4B") != 0 || w.MeleeWeaponCarriedOnLoad;
    g_log[0] = 0;
    w.OnInitialActivate();
    bad |= std::strcmp(g_log, "B") != 0;
    if (bad) { std::printf("005cb974_TEST FAIL %s\n", g_log); return 1; }
    std::printf("005cb974_TEST PASS\n");
    return 0;
}