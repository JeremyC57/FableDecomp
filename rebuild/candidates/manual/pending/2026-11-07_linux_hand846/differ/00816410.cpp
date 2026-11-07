// NHitLocations::CPhysicalPrimitiveCylinder::GetClosestPointToPos2D(const CThing& thing, const C3DVector& pos, C3DVector& normal) const -- the point of the cylinder's XY footprint (the ellipse of its tilted base-to-top axis and radius) closest to a position, with the XY normal from the footprint centre to it
// Grade: DIFFER(1122v1174) -- md=194, frame 0x6c v 0x60: the roots vector gets its own slot where retail reuses top's 0x5c, axis lives at 0x08 instead of 0x20, and perp stays in x87 registers where retail spills it to 0x14 and packs major over it (base/top block + functor temporary already put base, top, length and the functor where retail has them)
// Next idea: find the spelling that ends top's scope before roots is declared in a scope VC7.1 packs (an inline helper returning centre/axis by reference, or roots constructed inside the block's sibling), and keep perp addressable so it spills like retail
// Leaf: ?GetClosestPointToPos2D@CPhysicalPrimitiveCylinder@NHitLocations@@UBE?AVC3DVector@@ABVCThing@@ABV3@AAV3@@Z
#include "stlport/fable_containers.h"
extern "C" { double __cdecl sin(double); double __cdecl cos(double); double __cdecl sqrt(double); }
#pragma intrinsic(sin, cos, sqrt)
const double TWO_PI = 6.283185482025146484375;                  // 0x0124F2B8 ?TWO_PI@@3NB (established spelling)
extern const float DELTA_NOTIONAL_ZERO;                         // 0x0129BA3C
#include "engine/C3DVector.h"
struct CTCPhysicsBase {
    unsigned char _pad00[0x34];
    float         Radius;                                       // +0x34
};
#define FABLE_RETAIL_CTHING
#include "engine/CThing.h"
class CThingCreatureBase : public CThing {
public:
    float GetCombatRadius() const;                              // 0x00661F90
};

namespace {
// Ego_r.pdb: the anonymous-namespace function object of this TU (Ca +0, Cb +4, Cc +8; FableWin ??0...@QAE@MMM@Z).
class CVeryRandomFunctionToFindThePointOnAnElipseThatIsClosestToAPoint {
    float Ca, Cb, Cc;
public:
    CVeryRandomFunctionToFindThePointOnAnElipseThatIsClosestToAPoint(float ca, float cb, float cc) : Ca(ca), Cb(cb), Cc(cc) { }
    bool IsValid() const;
    float GetValue(float x) const;
};
}
namespace NMaths {
template <class F> std::vector<float> __fastcall GetRootsNumerically(float range_start, float range_end, const F& func, long num_roots,
                                                                     long num_asymptotes);   // 0x008185B0
}

namespace NHitLocations {
class CPhysicalPrimitiveCylinder {
public:
    virtual void v00();
    void*         BaseName;                                     // +0x04 CDefString
    float         Radius;                                       // +0x08
    float         Height;                                       // +0x0c
    void GetCylinderBaseAndTop(const CThing& thing, C3DVector& base, C3DVector& top) const;   // 0x008158D0
    float GetCylinderRadius(const CThing& thing) const
    {
        float radius = Radius;
        if (radius < DELTA_NOTIONAL_ZERO) {
            radius = thing.GetPhysicsTC()->Radius;
            if (thing.GetTypeFlags() & 8)
                radius = static_cast<const CThingCreatureBase&>(thing).GetCombatRadius();
        }
        return radius;
    }
    virtual C3DVector GetClosestPointToPos2D(const CThing& thing, const C3DVector& pos, C3DVector& normal) const;
};

C3DVector CPhysicalPrimitiveCylinder::GetClosestPointToPos2D(const CThing& thing, const C3DVector& pos, C3DVector& normal) const
{
    float radius, length;
    C3DVector axis, centre;
    {
        C3DVector base, top;
        GetCylinderBaseAndTop(thing, base, top);
        radius = GetCylinderRadius(thing);
        axis.X = top.X - base.X;
        axis.Y = top.Y - base.Y;
        axis.Z = 0.0f;
        length = (float)sqrt(axis.X * axis.X + axis.Y * axis.Y);
        centre.X = base.X + axis.X * 0.5f;
        centre.Y = base.Y + axis.Y * 0.5f;
        centre.Z = base.Z;
    }
    C3DVector perp(0.0f, 0.0f, 0.0f);
    float halfLength;
    if (length > radius) {
        axis = axis / length;
        perp.X = axis.Y;
        perp.Y = -axis.X;
        halfLength = length * 0.5f;
    } else {
        axis = C3DVector(1.0f, 0.0f, 0.0f);
        perp.X = 0.0f;
        perp.Y = 1.0f;
        halfLength = radius;
    }
    C3DVector major = axis * halfLength;
    C3DVector minor = perp * radius;
    C3DVector offset(centre.X - pos.X, centre.Y - pos.Y, 0.0f);
    std::vector<float> roots = NMaths::GetRootsNumerically(0.0f, 1.0f,
        CVeryRandomFunctionToFindThePointOnAnElipseThatIsClosestToAPoint(major.Dot(offset), -minor.Dot(offset),
                                                                         major.GetSquaredMagnitude() - minor.GetSquaredMagnitude()), 2, 0);
    C3DVector closest = pos;
    if (roots.size() == 2) {
        C3DVector first = centre + major * (float)sin(roots[0] * TWO_PI) + minor * (float)cos(roots[0] * TWO_PI);
        C3DVector second = centre + major * (float)sin(roots[1] * TWO_PI) + minor * (float)cos(roots[1] * TWO_PI);
        if ((first - pos).GetSquaredMagnitude() < (second - pos).GetSquaredMagnitude())
            closest = first;
        else
            closest = second;
    }
    normal = closest - centre;
    normal.Z = 0.0f;
    if (normal.GetSquaredMagnitude2D() > DELTA_NOTIONAL_ZERO * DELTA_NOTIONAL_ZERO)
        normal.AccurateNormalise();
    return closest;
}
}
