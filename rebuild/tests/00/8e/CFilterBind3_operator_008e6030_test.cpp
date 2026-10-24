// Behaviour test for 0x008E6030 CFilter_Bind3<CanCollideOrInteract, OverlappingSphere, HasTC>::operator(): all three
// filters must pass, in order, and the sphere test is a strict overlap of the summed radii.
#include <cstdio>

static int g_calls = 0;

class C3DVector { public: float X, Y, Z; };
class CTCPhysicsBase {
public:
    C3DVector Centre;
    float Radius;
    void GetBoundingInfo(C3DVector& centre, float& radius) const { centre = Centre; radius = Radius; }
};
class CThing {
public:
    bool Collides;
    long TCType;
    CTCPhysicsBase* PhysicsTC;
};
class CThingFilter_CanCollideOrInteractWithThing {
public:
    bool operator()(const CThing& thing) const { ++g_calls; return thing.Collides; }
};
class CThingFilter_HasTC {
public:
    long Type;
    bool operator()(const CThing& thing) const { ++g_calls; return thing.TCType == Type; }
};
class CThingFilter_OverlappingSphere {
public:
    C3DVector SpherePos;
    float Radius;
    bool operator()(const CThing& thing) const
    {
        C3DVector centre;
        float boundRadius;
        thing.PhysicsTC->GetBoundingInfo(centre, boundRadius);
        float dx = centre.X - SpherePos.X;
        float dy = centre.Y - SpherePos.Y;
        float dz = centre.Z - SpherePos.Z;
        float r = boundRadius + Radius;
        if (dx * dx + dy * dy + dz * dz < r * r)
            return true;
        return false;
    }
};
template <class F1, class F2, class F3> class CFilter_Bind3 {
public:
    const F1* PF1;
    const F2* PF2;
    const F3* PF3;
    template <class T> bool operator()(const T& thing) const { return (*PF1)(thing) && (*PF2)(thing) && (*PF3)(thing); }
};

int main()
{
    CThingFilter_CanCollideOrInteractWithThing f1;
    CThingFilter_OverlappingSphere f2;
    f2.SpherePos.X = 0.0f; f2.SpherePos.Y = 0.0f; f2.SpherePos.Z = 0.0f; f2.Radius = 1.0f;
    CThingFilter_HasTC f3;
    f3.Type = 7;
    CFilter_Bind3<CThingFilter_CanCollideOrInteractWithThing, CThingFilter_OverlappingSphere, CThingFilter_HasTC> bind;
    bind.PF1 = &f1; bind.PF2 = &f2; bind.PF3 = &f3;

    CTCPhysicsBase phys;
    phys.Centre.X = 2.0f; phys.Centre.Y = 1.0f; phys.Centre.Z = 0.0f; phys.Radius = 1.5f;   // d2 = 5, r = 2.5 -> 6.25
    CThing thing;
    thing.Collides = true; thing.TCType = 7; thing.PhysicsTC = &phys;
    int fail = 0;
    if (!bind(thing)) fail = 1;
    if (g_calls != 2) fail = 1;
    g_calls = 0;
    thing.Collides = false;
    if (bind(thing) || g_calls != 1) fail = 1;                  // first filter short-circuits
    thing.Collides = true; g_calls = 0;
    phys.Radius = 1.0f;                                          // r = 2 -> 4 < 5: no overlap
    if (bind(thing) || g_calls != 1) fail = 1;
    phys.Centre.X = 2.0f; phys.Centre.Y = 0.0f; phys.Radius = 1.0f;   // d2 = 4 == r*r: not strictly inside
    if (bind(thing)) fail = 1;
    phys.Radius = 1.5f; thing.TCType = 3;
    if (bind(thing)) fail = 1;                                   // wrong TC
    if (fail) { std::printf("008e6030_TEST FAIL\n"); return 1; }
    std::printf("008e6030_TEST PASS\n");
    return 0;
}