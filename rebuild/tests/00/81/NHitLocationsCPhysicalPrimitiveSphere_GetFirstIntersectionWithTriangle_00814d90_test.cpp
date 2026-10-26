#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <new>
#ifndef _MSC_VER
#define __cdecl
#define __stdcall
#define __fastcall
#define __fastcall
#define __forceinline inline
#define __int64 long long
#endif
static int g_ok = 1;
#define CHECK(c) do { if (!(c)) { g_ok = 0; std::printf("check failed line %d\n", __LINE__); } } while (0)
// ---- the candidate model (copied from src/00814d90.cpp) ----
// NHitLocations::CPhysicalPrimitiveSphere::GetFirstIntersectionWithTriangle(const CThing& thing, const vector<C3DVector>& triangle, C3DVector& point, C3DVector& normal, float unused) const -- build the world sphere (dummy-object origin, radius or the thing's physics radius when unset) and intersect the triangle with it
// Leaf: ?GetFirstIntersectionWithTriangle@CPhysicalPrimitiveSphere@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00814D90, 122 bytes (+6 int3 padding), `ret 14h`.
// Name: FableWin.pdb ?GetFirstIntersectionWithTriangle@CPhysicalPrimitiveSphere@NHitLocations@@UBE_NABVCThing@@ABV?$vector@VC3DVector@@...@std@@AAVC3DVector@@2M@Z
// (vtable slot 8, catalog vfunc_8); the twin of the landed GetFirstIntersectionWithFiniteLine 0x00814D10 (same model).
// Callees: CPhysicalPrimitiveSphere::GetDummyObjectOrigin @ 0x00814C70 (C3DVector by hidden pointer),
// CThingCreatureBase::GetCombatRadius @ 0x00661F90, NBBBMaths::CSphere::GetFirstIntersectionWithTriangle @ 0x00A58B50.
// Constant: 0.0001f @ 0x0129BA3C (the "radius unset" threshold).
// Layout: Ego_r.pdb CPhysicalPrimitiveSphere {vptr, CDefString BaseName +4, float Radius +8}; CSphere {Radius, Base}.
// The thing is modelled as in the landed twin: def +0x60 (collision radius at def+0x34), flags byte +0x6c (bit 3 = creature).
// Shape: radius kept on the x87 stack across the test, inline GetPhysicsRadius, the CSphere temporary takes the radius then
// the centre; the trailing float argument is unused.
class C3DVector { public:
    float x, y, z;
};
template<class T> struct vector {                  // STLport vector (12 bytes)
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
};
struct CThingDef_Physics {                         // the thing's definition (only the collision radius is read)
    unsigned char _pad[0x34];
    float         CollisionRadius;                 // +0x34
};
class CThing { public:
    unsigned char      _pad00[0x60];
    CThingDef_Physics* m_pDef;                     // +0x60
    unsigned char      _pad64[0x08];
    unsigned char      m_flags;                    // +0x6c (bit 3: a creature)
    float GetPhysicsRadius();
};
class CThingCreatureBase : public CThing { public:
    float GetCombatRadius();                       // 0x00661F90
};
inline float CThing::GetPhysicsRadius()
{
    float radius = m_pDef->CollisionRadius;
    if (m_flags & 8)
        radius = ((CThingCreatureBase*)this)->GetCombatRadius();
    return radius;
}
struct CSphere {
    float     Radius;                              // +0x00
    C3DVector Base;                                // +0x04
    CSphere(float radius, const C3DVector& base) : Radius(radius), Base(base) { }
    bool GetFirstIntersectionWithTriangle(const vector<C3DVector>& triangle, C3DVector& point, C3DVector& normal) const;   // 0x00A58B50
};
namespace NHitLocations {
struct CDefString { long TablePos; };
struct CPhysicalPrimitiveSphere {
    virtual void v00();
    CDefString BaseName;                           // +0x04
    float      Radius;                             // +0x08
    C3DVector GetDummyObjectOrigin(CThing* thing) const;   // 0x00814C70
    float GetRadius(CThing* thing) const
    {
        float radius = Radius;
        if (radius < 0.0001f)
            radius = thing->GetPhysicsRadius();
        return radius;
    }
    virtual bool GetFirstIntersectionWithTriangle(const CThing& thing, const vector<C3DVector>& triangle, C3DVector& point, C3DVector& normal, float unused) const;
};

bool CPhysicalPrimitiveSphere::GetFirstIntersectionWithTriangle(const CThing& thing, const vector<C3DVector>& triangle, C3DVector& point, C3DVector& normal, float unused) const
{
    C3DVector centre = GetDummyObjectOrigin(const_cast<CThing*>(&thing));
    CSphere sphere(GetRadius(const_cast<CThing*>(&thing)), centre);
    return sphere.GetFirstIntersectionWithTriangle(triangle, point, normal);
}
} // namespace NHitLocations

// ---- stubs for the external callees ----

static float g_sphereRadius = 0.0f;
static C3DVector g_sphereBase;
static const vector<C3DVector>* g_triangle = 0;
static C3DVector* g_point = 0;
static C3DVector* g_normal = 0;
static CThing* g_originThing = 0;
C3DVector NHitLocations::CPhysicalPrimitiveSphere::GetDummyObjectOrigin(CThing* thing) const
{
    g_originThing = thing;
    C3DVector v = { 1.0f, 2.0f, 3.0f };
    return v;
}
float CThingCreatureBase::GetCombatRadius() { return 7.0f; }
bool CSphere::GetFirstIntersectionWithTriangle(const vector<C3DVector>& triangle, C3DVector& point, C3DVector& normal) const
{
    g_sphereRadius = Radius; g_sphereBase = Base; g_triangle = &triangle; g_point = &point; g_normal = &normal;
    return Radius > 5.0f;
}
void NHitLocations::CPhysicalPrimitiveSphere::v00() {}

static void run()
{

    CThingDef_Physics def;
    def.CollisionRadius = 2.5f;
    CThing thing;
    thing.m_pDef = &def;
    thing.m_flags = 0;
    vector<C3DVector> triangle;
    triangle._M_start = 0; triangle._M_finish = 0; triangle._M_end_of_storage = 0;
    C3DVector point = { 0.0f, 0.0f, 0.0f };
    C3DVector normal = { 0.0f, 0.0f, 0.0f };
    NHitLocations::CPhysicalPrimitiveSphere sphere;
    sphere.BaseName.TablePos = 0;
    sphere.Radius = 6.0f;                      // own radius
    CHECK(sphere.GetFirstIntersectionWithTriangle(thing, triangle, point, normal, 0.0f) == true);
    CHECK(g_sphereRadius == 6.0f && g_originThing == &thing);
    CHECK(g_sphereBase.x == 1.0f && g_sphereBase.y == 2.0f && g_sphereBase.z == 3.0f);
    CHECK(g_triangle == &triangle && g_point == &point && g_normal == &normal);
    sphere.Radius = 0.0f;                      // unset: the thing's collision radius
    CHECK(sphere.GetFirstIntersectionWithTriangle(thing, triangle, point, normal, 0.0f) == false);
    CHECK(g_sphereRadius == 2.5f);
    thing.m_flags = 8;                         // a creature: its combat radius
    CHECK(sphere.GetFirstIntersectionWithTriangle(thing, triangle, point, normal, 0.0f) == true);
    CHECK(g_sphereRadius == 7.0f);
}

int main()
{
    run();
    if (!g_ok) { std::printf("00814d90_TEST FAIL\n"); return 1; }
    std::printf("00814d90_TEST PASS\n");
    return 0;
}