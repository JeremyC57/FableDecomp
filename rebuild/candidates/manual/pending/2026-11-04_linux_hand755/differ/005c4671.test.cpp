// Behaviour test for CTCInventoryWeapons::GetFreeAugmentationSlotForWeapon (0x005C4671), g++ model.
#include <cstdio>
struct Weapon { long Def; bool Alive; bool HasAug; long FreeSlot; int ShowEnded; };
static Weapon g_created = { 0, true, true, 2, 0 };
static int g_createCalls;
static Weapon* Create(long def) { ++g_createCalls; g_created.Def = def; return &g_created; }
static long GetFree(Weapon* melee, Weapon* ranged, long def)
{
    long slot = -1; bool created = false; Weapon* w;
    if (melee && melee->Alive && melee->Def == def) w = melee;
    else if (ranged && ranged->Alive && ranged->Def == def) w = ranged;
    else { w = Create(def); created = true; }
    if (w->HasAug) slot = w->FreeSlot;
    if (created) ++w->ShowEnded;
    return slot;
}
int main()
{
    Weapon m = { 5, true, true, 1, 0 }, r = { 6, true, false, 0, 0 };
    int ok = GetFree(&m, &r, 5) == 1 && GetFree(&m, &r, 6) == -1 && g_createCalls == 0;
    ok &= GetFree(&m, &r, 9) == 2 && g_createCalls == 1 && g_created.ShowEnded == 1;
    m.Alive = false; ok &= GetFree(&m, &r, 5) == 2 && g_createCalls == 2;
    std::printf(ok ? "005c4671_TEST PASS\n" : "005c4671_TEST FAIL\n");
    return ok ? 0 : 1;
}
