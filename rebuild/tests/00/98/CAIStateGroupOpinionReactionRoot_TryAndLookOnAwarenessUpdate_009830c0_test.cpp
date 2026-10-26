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
class C3DVector { public: float X, Y, Z; C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) { } C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { } };
struct CThingPos { C3DVector pos; CThingPos() : pos(1.f, 2.f, 3.f) { } C3DVector* GetPosition() { return &pos; } };
struct CGroup { CThingPos* thing; long result; float gotZ; long flags;
    CThingPos* GetPItem() const { return thing; }
    long AIAction_LookAtPos(long repeat, C3DVector p, long f, float s) { gotZ = p.Z; flags = f; return repeat == 1 && s > 0.00277f && s < 0.00278f ? result : -1; }
    bool TryAndLook() { CThingPos* t = GetPItem(); return AIAction_LookAtPos(1, *t->GetPosition(), 0x21, 0.0027777f) == 2; } };
int main()
{
    CThingPos t; CGroup g; g.thing = &t; g.result = 2; g.gotZ = 0; g.flags = 0;
    bool ok = g.TryAndLook() && g.gotZ == 3.f && g.flags == 0x21;
    g.result = 3; ok = ok && !g.TryAndLook();
    if (!ok) { std::printf("009830c0_TEST FAIL\n"); return 1; }
    std::printf("009830c0_TEST PASS\n"); return 0;
}