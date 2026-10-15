// Behaviour test for 0x0096D260: EatAtTavern::GetBuilding asks the creature's village for the nearest property-0x20 building.
#include <cstdio>
struct C3DVector { float X, Y, Z; };
struct CBuilding { int id; };
struct CTCVillage { CBuilding* tavern; const C3DVector* asked; unsigned long prop;
    CBuilding* FindNearest(const C3DVector& p, unsigned long property) { asked = &p; prop = property; return tavern; } };
struct CTCVillager { CTCVillage* village; };
struct CCreature { C3DVector pos; CTCVillager* villager; };
static CTCVillage* GetVillage(CTCVillager* v) { return v->village; }
static CBuilding* GetBuilding(CCreature* c)
{
    if (c->villager) {
        if (GetVillage(c->villager) != 0) return GetVillage(c->villager)->FindNearest(c->pos, 0x20);
    }
    return 0;
}
int main()
{
    CBuilding t = { 3 };
    CTCVillage v = { &t, 0, 0 };
    CTCVillager vg = { &v };
    CCreature c = { { 1, 2, 3 }, &vg };
    int bad = 0;
    if (GetBuilding(&c) != &t || v.asked != &c.pos || v.prop != 0x20) bad = 1;
    vg.village = 0; if (GetBuilding(&c) != 0) bad = 2;
    c.villager = 0; if (GetBuilding(&c) != 0) bad = 3;
    if (bad) { std::printf("0096d260_TEST FAIL\n"); return 1; }
    std::printf("0096d260_TEST PASS\n");
    return 0;
}
