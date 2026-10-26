#include <cstdio>
class CThing { public: unsigned char flags91; unsigned long bits[9]; long compType; void* comp; CThing() : flags91(0), compType(-1), comp(0) { for (int i = 0; i < 9; ++i) bits[i] = 0; } };
struct CBaseIntelligentPointer {
    CThing* item; int sets;
    CBaseIntelligentPointer() : item(0), sets(0) { }
    CThing* GetPItem() const { return item; }
    void SetPItem(CThing* p) { item = p; ++sets; }
};
// the inlined GetInterface: presence bit, then the single-entry map
template<class T> bool GetInterface(CThing* t, T** out, long type)
{
    if (t->bits[type >> 5] & (1UL << (type & 31))) { *out = (t->compType == type) ? (T*)t->comp : 0; return true; }
    return false;
}
static void GiveTC(CThing* t, long type, void* comp) { t->bits[type >> 5] |= 1UL << (type & 31); t->compType = type; t->comp = comp; }
class C3DVector { public: float X, Y, Z; };
struct CPot : CThing { C3DVector pos; C3DVector* GetPosition() { return &pos; } };
struct CCreature { C3DVector* near; float radius; bool placeOk; bool AutoPlaceNear(C3DVector* p, float r) { near = p; radius = r; return placeOk; } };
struct CGroup { bool runnable; CCreature* creature; CBaseIntelligentPointer PCookingPot;
    bool IsRunnable() { return runnable; }
    bool PlaceNearPot() { CCreature* c = creature; return c->AutoPlaceNear(((CPot*)PCookingPot.GetPItem())->GetPosition(), 3.0f); }
    bool InitCreaturePosition() { return IsRunnable() && PlaceNearPot(); } };
int main()
{
    CPot pot; CCreature cr; cr.near = 0; cr.radius = 0; cr.placeOk = true; CGroup g; g.creature = &cr; g.PCookingPot.SetPItem(&pot); g.runnable = false;
    bool ok = !g.InitCreaturePosition() && cr.near == 0;
    g.runnable = true; ok = ok && g.InitCreaturePosition() && cr.near == &pot.pos && cr.radius == 3.0f;
    cr.placeOk = false; ok = ok && !g.InitCreaturePosition();
    if (!ok) { std::printf("009460c0_TEST FAIL\n"); return 1; }
    std::printf("009460c0_TEST PASS\n"); return 0;
}