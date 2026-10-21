// Behaviour test for 00a75340: src/00a75340.cpp copied verbatim; a two-plane volume so the outside early-out (0), the
// clamp to 1 deep inside, and the linear 0.5 - d/(2r) proportion near the surface are checked.
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
// ---- source (verbatim) ----
// CContainmentVolume::PeekRoughProportionOfSphereContained(const C3DVector& centre, float radius) const -- rough fraction of a sphere inside the plane volume: 0.5 - (deepest outside distance)/(2r), clamped to [0,1]
// Retry 10b (getsw sweep, FIXES B-15): the X coordinate is read through an inline getter `GetX()`; the plain member read gives the parked register/scheduling difference.
// Leaf: PeekRoughProportionOfSphereContained
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A75340, 174 bytes (176 with padding),
//         `?PeekRoughProportionOfSphereContained@CContainmentVolume@@QBEMABVC3DVector@@M@Z` (FableWin.pdb export list).
// Float pool: -1e30 @ 0x0129DCA4 (start of the running maximum), 0.5 @ 0x0122F59C, 0.0 @ 0x0122DEDC, 1.0 @ 0x0122DED8.
// Shape: for each 16-byte CPlane { C3DVector Normal; float Distance } the outside distance is
// -(Y*Y + Z*Z + X*X - Distance) (fchs); any plane the sphere is fully outside of returns 0 early.
// The running maximum stays on the FPU stack; its value is spilled into the dead `centre` slot.
#include "engine/CContainmentVolume.h"

struct C3DVector {
    float X;
    float Y;
    float Z;
    float GetX() const { return X; }
};

struct CPlane {
    C3DVector Normal;
    float     Distance;
};

static inline float Clamp01(float value)
{
    if (value < 0.0f)
        return 0.0f;
    if (value > 1.0f)
        return 1.0f;
    return value;
}

struct CContainmentVolume_Methods : CContainmentVolume {
    float PeekRoughProportionOfSphereContained(const C3DVector& centre, float radius) const;
};

float CContainmentVolume_Methods::PeekRoughProportionOfSphereContained(const C3DVector& centre, float radius) const
{
    float maxDistance = -1e30f;
    for (unsigned long i = 0; i < Size; ++i) {
        const CPlane& plane = reinterpret_cast<const CPlane*>(ContainmentPlanes)[i];
        float distance = -(plane.Normal.GetX() * centre.GetX() + (centre.Y * plane.Normal.Y + centre.Z * plane.Normal.Z) - plane.Distance);
        if (distance > radius)
            return 0.0f;
        if (maxDistance < distance)
            maxDistance = distance;
    }
    return Clamp01(0.5f - maxDistance / (radius + radius));
}
// ---- end source ----
static C3DVector V(float x, float y, float z) { C3DVector v; v.X = x; v.Y = y; v.Z = z; return v; }
int main()
{
    // planes x <= 10 (normal +X, distance 10) and y <= 10; signed distance = -(n.c - d)
    static CPlane planes[2]; planes[0].Normal = V(1, 0, 0); planes[0].Distance = 10; planes[1].Normal = V(0, 1, 0); planes[1].Distance = 10;
    static unsigned char raw[sizeof(CContainmentVolume_Methods)]; memset(raw, 0, sizeof(raw));
    CContainmentVolume_Methods* v = (CContainmentVolume_Methods*)raw;
    *(CPlane**)&v->ContainmentPlanes = planes; v->Size = 2;
    float p = v->PeekRoughProportionOfSphereContained(V(0, 0, 0), 2.0f);     // distances 10, 10 > r: outside? (10 > 2) -> 0
    CHECK(p == 0.0f);
    p = v->PeekRoughProportionOfSphereContained(V(9, 9.5f, 0), 2.0f);       // distances 1, 0.5: max 1 -> 0.5 - 1/4 = 0.25
    CHECK(p == 0.25f);
    p = v->PeekRoughProportionOfSphereContained(V(14, 13, 0), 2.0f);        // distances -4, -3: max -3 -> 0.5 + 0.75 -> 1
    CHECK(p == 1.0f);
    if (fails == 0) printf("00a75340_TEST PASS\n"); else printf("00a75340_TEST FAIL\n");
    return fails;
}