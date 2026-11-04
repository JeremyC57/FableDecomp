// NHitLocations::CPhysicalPrimitiveSphere::GetClosestPointToPos2D(const CThing& thing, const C3DVector& pos, C3DVector& direction) const -- the point on the sphere's horizontal circle closest to the position, and the unit 2D direction to it
// Grade: DIFFER(238v238) -- x87 order: the Z product of offset * radius is fld z ; fmul radius in ours, fld radius ; fmul z in retail (X and Y match)
// Leaf: ?GetClosestPointToPos2D@CPhysicalPrimitiveSphere@NHitLocations@@UBE?AVC3DVector@@ABVCThing@@ABV3@AAV3@@Z
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x00814EA0, 238 bytes, `ret 10h` (hidden C3DVector result + 3 arguments); vtable slot 10.
// Identity: Ego_r.pdb ?GetClosestPointToPos2D@CPhysicalPrimitiveSphere@NHitLocations@@UBE?AVC3DVector@@ABVCThing@@ABV3@AAV3@@Z.
// Callees: NHitLocations::CPhysicalPrimitiveSphere::GetDummyObjectOrigin(CThing*) const 0x00814C70 (landed binding),
//   CThingCreatureBase::GetCombatRadius() const 0x00661F90, C3DVector::AccurateNormalise() 0x00A14440.
//   Constant: DELTA_NOTIONAL_ZERO (0x0129BA3C, 1e-4f).
// Shape: the landed sphere family (IsPosInside 0x00814E10): the radius falls back to the thing's physics radius when
//   below 1e-4 (written out in the body) and is homed in the dead `thing` argument slot. The offset is a C3DVector(dx, dy, 0)
//   local, normalised only when its 2D squared length (the default-path GetSquaredMagnitude2D `X*X+Y*Y`, Y loaded first)
//   exceeds DELTA_NOTIONAL_ZERO squared; the direction is copied out dword by dword and the result is `offset * radius + centre`.
#include "engine/C3DVector.h"
extern const float DELTA_NOTIONAL_ZERO;                           // 0x0129BA3C ?DELTA_NOTIONAL_ZERO@@3MB (1e-4f)

class CThingDef_Physics {                                          // the thing's definition (only the collision radius is read)
public:
    unsigned char _pad[0x34];
    float         CollisionRadius;                                 // +0x34
};
class CThing {
public:
    unsigned char      _pad00[0x60];
    CThingDef_Physics* m_pDef;                                     // +0x60
    unsigned char      _pad64[0x08];
    unsigned char      m_flags;                                    // +0x6c (bit 3: a creature)
};
class CThingCreatureBase : public CThing {
public:
    float GetCombatRadius() const;                                 // 0x00661F90
};
class CDefString { public: long TablePos; };

namespace NHitLocations {
class CPhysicalPrimitiveSphere {
public:
    virtual void v00();
    C3DVector GetDummyObjectOrigin(CThing* thing) const;           // 0x00814C70
    virtual C3DVector GetClosestPointToPos2D(const CThing& thing, const C3DVector& pos, C3DVector& direction) const;
    CDefString BaseName;                                           // +0x04
    float      Radius;                                             // +0x08
};

C3DVector CPhysicalPrimitiveSphere::GetClosestPointToPos2D(const CThing& thing, const C3DVector& pos, C3DVector& direction) const
{
    C3DVector centre = GetDummyObjectOrigin(const_cast<CThing*>(&thing));
    float radius = Radius;
    if (radius < DELTA_NOTIONAL_ZERO) {
        radius = thing.m_pDef->CollisionRadius;
        if (thing.m_flags & 8)
            radius = static_cast<const CThingCreatureBase&>(thing).GetCombatRadius();
    }
    C3DVector offset = pos - centre;
    offset.Z = 0.0f;
    if (offset.GetSquaredMagnitude2D() > DELTA_NOTIONAL_ZERO * DELTA_NOTIONAL_ZERO)
        offset.AccurateNormalise();
    direction = offset;
    return offset * radius + centre;
}
}
