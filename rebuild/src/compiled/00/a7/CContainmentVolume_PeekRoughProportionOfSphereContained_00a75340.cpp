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

class C3DVector { public:
    float X;
    float Y;
    float Z;
    float GetX() const { return X; }
};

class CPlane { public:
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