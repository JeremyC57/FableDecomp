// Behaviour test for CTCInventoryWeapons::RemoveWeaponsFromSheathes (0x005C552C): a model of the carrier / weapon defs.
#include <cstdio>
struct Thing { bool alive; bool hasDef; bool hasWeaponTC; long sheatheObject; bool hasItemTC; float scale; float renderSize; bool returned; bool killed; };
struct Carrier { Thing* slots[4]; bool primary[4];
    bool IsCarrying(const Thing* t) const { for (int i = 0; i < 4; ++i) if (slots[i] == t) return true; return false; }
    bool IsPrimary(const Thing* t) const { for (int i = 0; i < 4; ++i) if (slots[i] == t) return primary[i]; return false; }
    Thing* Get(long s) const { return (s >= 0 && s < 4) ? slots[s] : 0; }
    void Remove(Thing* t) { for (int i = 0; i < 4; ++i) if (slots[i] == t) slots[i] = 0; }
};
static void RemoveWeaponsFromSheathes(Carrier& c, Thing* melee, Thing* ranged)
{
    Thing* weapons[2]; int n = 0;
    if (melee && melee->alive) weapons[n++] = melee;
    if (ranged && ranged->alive) weapons[n++] = ranged;
    for (int i = 0; i < n; ++i) {
        Thing* w = weapons[i];
        if (c.IsCarrying(w) && c.IsPrimary(w)) continue;
        if (w->hasDef && w->hasWeaponTC) {
            Thing* s = 0;
            if (w->sheatheObject > 0) s = c.Get(w->sheatheObject);
            else if (w->sheatheObject != 0) s = w;
            if (s && s->alive) {
                s->scale = s->renderSize;
                if (c.IsCarrying(s)) c.Remove(s);
                if (s->hasItemTC) s->returned = true; else s->killed = true;
            }
        }
    }
}
int main()
{
    Thing sheath = { true, false, false, 0, false, 0.5f, 2.0f, false, false };
    Thing melee = { true, true, true, 2, false, 1.0f, 1.0f, false, false };
    Thing ranged = { true, true, true, -1, true, 0.25f, 3.0f, false, false };
    Carrier c = { { 0, 0, &sheath, &ranged }, { false, false, false, false } };
    RemoveWeaponsFromSheathes(c, &melee, &ranged);
    if (!sheath.killed || sheath.scale != 2.0f || c.slots[2] != 0 || !ranged.returned || ranged.scale != 3.0f || c.slots[3] != 0) {
        std::printf("005c552c_TEST FAIL\n"); return 1;
    }
    std::printf("005c552c_TEST PASS\n");
    return 0;
}
