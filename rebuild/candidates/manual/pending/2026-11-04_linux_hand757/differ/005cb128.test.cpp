// Behaviour test for 0x005CB128 CTCInventoryWeapons::RemoveItemFromInventory: a g++ model of the parked source (the residue is codegen only).
#include <cstdio>
struct Weapon { bool alive; long def; bool carried; bool killed; };
struct Inv { Weapon* melee; Weapon* ranged; long counts[8]; int refreshSheathed, refreshSheathing, setup; };
static void Remove(Inv& v, long def, long n)
{
    v.counts[def] -= n;
    Weapon* ws[2] = { v.melee, v.ranged };
    for (int i = 0; i < 2; ++i) {
        Weapon* w = ws[i];
        if (w && w->alive && v.counts[w->def] == 0) { if (w->carried) { w->carried = false; } v.refreshSheathed++; v.refreshSheathing++; w->killed = true; }
    }
    v.refreshSheathed++; v.setup++;
}
int main() {
    Weapon sword = { true, 1, true, false }, bow = { true, 2, false, false };
    Inv v = { &sword, &bow, { 0, 1, 2 }, 0, 0, 0 };
    Remove(v, 1, 1);
    int ok = sword.killed && !sword.carried && !bow.killed && v.refreshSheathed == 2 && v.setup == 1;
    std::printf(ok ? "005cb128_TEST PASS\n" : "005cb128_TEST FAIL\n");
    return ok ? 0 : 1;
}
