// Behaviour test of CTCInventoryWeapons::RemoveItemFromInventory (0x005CB128): after the base removal, an active weapon with none
// left in the inventory is taken off the carrier (when carried), the weapons come out of their sheathes, and the weapon is killed;
// the sheathes are refreshed and the carrier set up again at the end in every case.
#include <cstdio>
#include <cstring>
static char g_log[256];
static void Log(const char* s) { std::strcat(g_log, s); }
class CThing {
public:
    unsigned long defIndex; bool killed;
    unsigned long GetDefGlobalIndex() const { return defIndex; }
    void Kill(bool) { killed = true; Log("K"); }
};
class CTCCarrying { public:
    CThing* carried[2];
    bool IsCarryingThing(const CThing& t) const { return carried[0] == &t || carried[1] == &t; }
    void RemoveThing(CThing& t) { if (carried[0] == &t) carried[0] = 0; if (carried[1] == &t) carried[1] = 0; Log("R"); }
};
struct COwner {
    CTCCarrying* carrying;
    bool GetTC(CTCCarrying** out) { if (!carrying) return false; *out = carrying; return true; }
};
struct CIntelligentPointer { CThing* item; CThing* Get() const { return item; } };
static bool IS_THING_ALIVE(const CIntelligentPointer& p) { return p.item != 0 && !p.item->killed; }
class CTCInventoryWeapons {
public:
    COwner* ParentThing;
    CIntelligentPointer PActiveMeleeWeapon, PActiveRangedWeapon;
    long counts[64];
    void BaseRemoveItemFromInventory(long def, long count) { counts[def] -= count; Log("B"); }
    long GetNumberOfItemsOfTypeInInventory(unsigned long def) const { return counts[def]; }
    void RefreshSheathedWeapons() { Log("S"); }
    void RemoveWeaponsFromSheathes() { Log("O"); }
    void SetupActiveWeaponsOnCarrier() { Log("C"); }
    void RemoveItemFromInventory(long itemDefIndex, long count)
    {
        BaseRemoveItemFromInventory(itemDefIndex, count);
        if (IS_THING_ALIVE(PActiveMeleeWeapon) && GetNumberOfItemsOfTypeInInventory(PActiveMeleeWeapon.Get()->GetDefGlobalIndex()) == 0) {
            CTCCarrying* carrying;
            if (ParentThing->GetTC(&carrying) && carrying->IsCarryingThing(*PActiveMeleeWeapon.Get())) {
                carrying->RemoveThing(*PActiveMeleeWeapon.Get());
                RefreshSheathedWeapons();
            }
            RemoveWeaponsFromSheathes();
            PActiveMeleeWeapon.Get()->Kill(false);
        }
        if (IS_THING_ALIVE(PActiveRangedWeapon) && GetNumberOfItemsOfTypeInInventory(PActiveRangedWeapon.Get()->GetDefGlobalIndex()) == 0) {
            CTCCarrying* carrying;
            if (ParentThing->GetTC(&carrying) && carrying->IsCarryingThing(*PActiveRangedWeapon.Get())) {
                carrying->RemoveThing(*PActiveRangedWeapon.Get());
                RefreshSheathedWeapons();
            }
            RemoveWeaponsFromSheathes();
            PActiveRangedWeapon.Get()->Kill(false);
        }
        RefreshSheathedWeapons();
        SetupActiveWeaponsOnCarrier();
    }
};
int main()
{
    CThing sword = { 5, false }, bow = { 7, false };
    CTCCarrying carrying = { { &sword, 0 } };
    COwner owner = { &carrying };
    CTCInventoryWeapons w; std::memset(w.counts, 0, sizeof w.counts);
    w.ParentThing = &owner; w.PActiveMeleeWeapon.item = &sword; w.PActiveRangedWeapon.item = &bow;
    w.counts[5] = 1; w.counts[7] = 2;
    w.RemoveItemFromInventory(5, 1);                 // last sword: carried -> removed; bow still owned
    int bad = std::strcmp(g_log, "BRSOKSC") != 0 || !sword.killed || bow.killed || carrying.carried[0] != 0;
    g_log[0] = 0;
    w.RemoveItemFromInventory(7, 2);                 // last bow: not carried -> no RemoveThing
    bad |= std::strcmp(g_log, "BOKSC") != 0 || !bow.killed;
    g_log[0] = 0; owner.carrying = 0; w.counts[9] = 3;
    w.RemoveItemFromInventory(9, 1);                 // nothing active any more
    bad |= std::strcmp(g_log, "BSC") != 0;
    if (bad) { std::printf("005cb128_TEST FAIL %s\n", g_log); return 1; }
    std::printf("005cb128_TEST PASS\n");
    return 0;
}