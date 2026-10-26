// Behaviour test for 00815ce0: final.cpp copied verbatim, callees stubbed below so calls and arguments are observable.
// NHitLocations::CPhysicalPrimitiveCylinder::IsPosInside(const CThing&, const C3DVector&)
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
// ---- source (verbatim) ----
// NHitLocations::CPhysicalPrimitiveCylinder::IsPosInside(const CThing& thing, const C3DVector& pos) -- true when the position lies within the thing's cylinder: its projection on the base-to-top axis is inside the length and its distance from the axis is below the radius (each widened by the epsilon)
// Leaf: ?IsPosInside@CPhysicalPrimitiveCylinder@
#pragma intrinsic(sqrt)
extern "C" double __cdecl sqrt(double);
extern float g_fEpsilon;                           // 0x0129BA3C
class C3DVector { public:
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    C3DVector operator-(const C3DVector& o) const { return C3DVector(X - o.X, Y - o.Y, Z - o.Z); }
    C3DVector operator*(float s) const { return C3DVector(X * s, Y * s, Z * s); }
    C3DVector& operator*=(float s) { X *= s; Y *= s; Z *= s; return *this; }
    C3DVector& operator-=(const C3DVector& o) { X -= o.X; Y -= o.Y; Z -= o.Z; return *this; }
    float operator*(const C3DVector& o) const { return Z * o.Z + Y * o.Y + X * o.X; }
    float MagSquared() const { return Z * Z + Y * Y + X * X; }
    float Length() const { return (float)sqrt(Z * Z + Y * Y + X * X); }
};
struct CThingDef {
    unsigned char _pad00[0x34];
    float         Radius;                          // +0x34
};
class CThing {
public:
    unsigned char _pad00[0x60];
    CThingDef*    Def;                             // +0x60
    unsigned char _pad64[8];
    unsigned char Flags;                           // +0x6c (bit 3: creature)
};
class CThingCreatureBase : public CThing {
public:
    float GetCombatRadius() const;                 // 0x00661F90
};
namespace NHitLocations {
struct CPhysicalPrimitiveCylinder {
    virtual void v00();
    void*         BaseName;                        // +0x04 CDefString
    float         Radius;                          // +0x08
    float         Height;                          // +0x0c
    void GetCylinderBaseAndTop(const CThing& thing, C3DVector& base, C3DVector& top) const;   // 0x008158D0
    float GetCylinderRadius(const CThing& thing) const
    {
        float radius = Radius;
        if (radius < g_fEpsilon) {
            radius = thing.Def->Radius;
            if (thing.Flags & 8)
                radius = static_cast<const CThingCreatureBase&>(thing).GetCombatRadius();
        }
        return radius;
    }
    bool IsPosInside(const CThing& thing, const C3DVector& pos) const;
};

bool CPhysicalPrimitiveCylinder::IsPosInside(const CThing& thing, const C3DVector& pos) const
{
    C3DVector base, top;
    GetCylinderBaseAndTop(thing, base, top);
    float radius = GetCylinderRadius(thing);
    C3DVector axis = top - base;
    float length = axis.Length();
    if (length > g_fEpsilon)
        axis *= 1.0f / length;
    C3DVector offset = pos - base;
    float along = offset * axis;
    if (-g_fEpsilon < along && along < g_fEpsilon + length) {
        offset -= axis * along;
        float limit = g_fEpsilon + radius;
        if (offset.MagSquared() < limit * limit)
            return true;
    }
    return false;
}
}

// ---- stubs and driver ----
float g_fEpsilon = 0.001f;
void NHitLocations::CPhysicalPrimitiveCylinder::v00() { }
static int g_baseTop = 0, g_combat = 0;
static C3DVector g_base, g_top;
static float g_combatRadius = 0.0f;
static const CThing* g_lastThing = 0;
static const NHitLocations::CPhysicalPrimitiveCylinder* g_lastCyl = 0;
void NHitLocations::CPhysicalPrimitiveCylinder::GetCylinderBaseAndTop(const CThing& thing, C3DVector& base, C3DVector& top) const
{ ++g_baseTop; g_lastThing = &thing; g_lastCyl = this; base = g_base; top = g_top; }
float CThingCreatureBase::GetCombatRadius() const { ++g_combat; g_lastThing = this; return g_combatRadius; }
static unsigned char g_thingBuf[0x80];
static CThingDef g_def;
int main()
{
    memset(g_thingBuf, 0, sizeof(g_thingBuf)); memset(&g_def, 0, sizeof(g_def));
    CThing* thing = (CThing*)g_thingBuf; thing->Def = &g_def;
    NHitLocations::CPhysicalPrimitiveCylinder cyl; memset(&cyl, 0, sizeof(cyl));
    // vertical cylinder from (0,0,0) to (0,0,10) with the primitive's own radius 2
    g_base = C3DVector(0.0f, 0.0f, 0.0f); g_top = C3DVector(0.0f, 0.0f, 10.0f); cyl.Radius = 2.0f;
    CHECK(cyl.IsPosInside(*thing, C3DVector(1.0f, 1.0f, 5.0f)) == true);        // inside (dist 1.41 < 2)
    CHECK(g_baseTop == 1 && g_lastThing == thing && g_lastCyl == &cyl && g_combat == 0);
    CHECK(cyl.IsPosInside(*thing, C3DVector(1.9f, 0.0f, 9.9f)) == true);        // near the rim and the top
    CHECK(cyl.IsPosInside(*thing, C3DVector(2.5f, 0.0f, 5.0f)) == false);       // outside the radius
    CHECK(cyl.IsPosInside(*thing, C3DVector(0.0f, 0.0f, 10.5f)) == false);      // beyond the top
    CHECK(cyl.IsPosInside(*thing, C3DVector(0.0f, 0.0f, -0.5f)) == false);      // below the base
    CHECK(cyl.IsPosInside(*thing, C3DVector(0.0f, 0.0f, 10.0005f)) == true);    // inside the epsilon band at the top
    // zero radius: the def's radius is used for a non-creature thing
    cyl.Radius = 0.0f; g_def.Radius = 3.0f;
    CHECK(cyl.IsPosInside(*thing, C3DVector(2.5f, 0.0f, 5.0f)) == true && g_combat == 0);
    CHECK(cyl.IsPosInside(*thing, C3DVector(3.5f, 0.0f, 5.0f)) == false);
    // a creature uses its combat radius instead
    thing->Flags = 8; g_combatRadius = 4.0f;
    CHECK(cyl.IsPosInside(*thing, C3DVector(3.5f, 0.0f, 5.0f)) == true && g_combat == 1 && g_lastThing == thing);
    CHECK(cyl.IsPosInside(*thing, C3DVector(4.5f, 0.0f, 5.0f)) == false && g_combat == 2);
    thing->Flags = 0; cyl.Radius = 2.0f;
    // a tilted axis: base (0,0,0) top (10,0,0), point just off the axis
    g_base = C3DVector(0.0f, 0.0f, 0.0f); g_top = C3DVector(10.0f, 0.0f, 0.0f);
    CHECK(cyl.IsPosInside(*thing, C3DVector(5.0f, 1.5f, 0.0f)) == true);
    CHECK(cyl.IsPosInside(*thing, C3DVector(5.0f, 0.0f, 2.5f)) == false);
    // a degenerate (zero-length) cylinder: the axis is left unnormalised, only the base point itself is inside
    g_top = g_base;
    CHECK(cyl.IsPosInside(*thing, C3DVector(0.5f, 0.0f, 0.0f)) == true);        // along = 0, within epsilon + length
    CHECK(cyl.IsPosInside(*thing, C3DVector(2.5f, 0.0f, 0.0f)) == false);
    CHECK(g_baseTop == 14);
    if (fails == 0) printf("00815ce0_TEST PASS\n"); else printf("00815ce0_TEST FAIL\n");
    return fails;
}