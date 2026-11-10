// Behaviour test of CTCInventoryWeapons::PutWeaponsInSheathes (0x005C8101): the weapons come out of the hands first, the carrying
// component is looked up (presence test only), then each live active weapon -- melee before ranged -- goes into its carry slot on
// the owner with the one shared stack offset.
#include <cstdio>
class CThing {
public:
    int presenceTests;
    bool HasTC(long type) { ++presenceTests; return type == 70; }
};
struct CIntelligentPointer {
    CThing* item;
    CThing* Get() const { return item; }
};
static bool IS_THING_ALIVE(const CIntelligentPointer& p) { return p.item != 0; }
static int g_calls = 0;
static CThing* g_owner[4];
static CThing* g_weapon[4];
static void PutSheatheObjectInCarrySlot(CThing& owner, CThing& weapon, float& offset)
{
    g_owner[g_calls] = &owner; g_weapon[g_calls] = &weapon; ++g_calls;
    offset += 1.5f;                          // the callee grows the shared offset
}
class CTCInventoryWeapons {
public:
    CThing* ParentThing;
    CIntelligentPointer PActiveMeleeWeapon;
    CIntelligentPointer PActiveRangedWeapon;
    int removed;
    void RemoveWeaponsFromSheathes() { ++removed; }
    void PutWeaponsInSheathes()
    {
        RemoveWeaponsFromSheathes();
        ParentThing->HasTC(70);
        float offset = 0.0f;
        if (IS_THING_ALIVE(PActiveMeleeWeapon)) {
            CThing& owner = *ParentThing;
            PutSheatheObjectInCarrySlot(owner, *PActiveMeleeWeapon.Get(), offset);
        }
        if (IS_THING_ALIVE(PActiveRangedWeapon)) {
            CThing& owner = *ParentThing;
            PutSheatheObjectInCarrySlot(owner, *PActiveRangedWeapon.Get(), offset);
        }
    }
};
int main()
{
    CThing hero = { 0 }, sword = { 0 }, bow = { 0 };
    CTCInventoryWeapons w;
    w.ParentThing = &hero; w.PActiveMeleeWeapon.item = &sword; w.PActiveRangedWeapon.item = &bow; w.removed = 0;
    w.PutWeaponsInSheathes();
    int bad = !(w.removed == 1 && hero.presenceTests == 1 && g_calls == 2 && g_owner[0] == &hero && g_weapon[0] == &sword
                && g_owner[1] == &hero && g_weapon[1] == &bow);
    g_calls = 0; w.PActiveMeleeWeapon.item = 0;
    w.PutWeaponsInSheathes();
    bad |= !(w.removed == 2 && g_calls == 1 && g_weapon[0] == &bow);
    g_calls = 0; w.PActiveRangedWeapon.item = 0;
    w.PutWeaponsInSheathes();
    bad |= !(g_calls == 0 && hero.presenceTests == 3);
    if (bad) { std::printf("005c8101_TEST FAIL\n"); return 1; }
    std::printf("005c8101_TEST PASS\n");
    return 0;
}