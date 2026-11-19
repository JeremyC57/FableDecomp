// CThingFilter_OverlappingCircle::operator()(const CThing& test_thing) const -- true when the thing's physics footprint circle (map-who position, component radius +0x34) overlaps the filter's circle (SpherePos, Radius) in the XY plane
// Grade: RELOCATION_MATCH -- the distance is accumulated in two statements (`float d2 = dx * dx; d2 += dy * dy;`), which squares dx first as retail does; a single `dx * dx + dy * dy` squares dy first
// Leaf: ??RCThingFilter_OverlappingCircle@@QBE_NABVCThing@@@Z
// Plan phase 4 (2026-11-05), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12). Member accesses x/y/z respelled to the PDB X/Y/Z.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0063EF50, length 95 bytes (+ int3 padding).
// Callees: CTCPhysicsBase::GetMapwhoPosition @ 0x006AFCB0 (C3DVector through the hidden pointer, a 12-byte frame local).
// Layout: PDB CThingFilter_OverlappingCircle { C3DVector SpherePos; float Radius; }; retail CThing PhysicsTC +0x60; the
// physics component's own radius float at +0x34 (local model).
// Shape: C3DVector has a user constructor (hidden-pointer return); the component pointer stays in edi across the
// call; `if (dist2 < r * r) return true; return false;` gives byte-constant exits.
#include "engine/C3DVector.h"

class CTCPhysicsBase { public:
    unsigned char _pad00[0x34];
    float         m_radius;                         // +0x34
    C3DVector GetMapwhoPosition() const;            // 0x006AFCB0
};

#include "engine/CThing.h"

// Name (NF94 q11401): Ego_r.pdb / FableWin.pdb ??RCThingFilter_OverlappingCircle@@QBE_NABVCThing@@@Z (const CThing&); the shared
//   engine/CThingFilter_OverlappingCircle.h declares it (SpherePos / Radius private) and the landed caller 0x005ED7D0 binds it so.
//   The header's BODY opt-in text is not used: its note measures that text squaring dy first here; the row keeps its own
//   two-statement sum (dx first, as retail 0x0063EF7E..84). Was a local class model with operator()(CThing*).
#include "engine/CThingFilter_OverlappingCircle.h"

// The overlap test: the landed text's inline helper (the position by reference to the call's frame temporary), here a file-local
//   inline function since the shared header declares no such member. SpherePos and Radius go by reference (as the member helper
//   read them through `this`); by value copies Radius before the call (DIFFER 103v95), and the body written inline in operator()
//   gives DIFFER 98v95.
inline bool OverlapsFootprint(const C3DVector& pos, const CTCPhysicsBase* physics, const C3DVector& sphere_pos, const float& radius)
{
    float dx = pos.X - sphere_pos.X;
    float dy = pos.Y - sphere_pos.Y;
    float r = physics->m_radius + radius;
    float d2 = dx * dx;
    d2 += dy * dy;
    if (d2 < r * r)
        return true;
    return false;
}

bool CThingFilter_OverlappingCircle::operator()(const CThing& test_thing) const
{
    const CTCPhysicsBase* physics = test_thing.GetPhysicsTC();
    if (physics)
        return OverlapsFootprint(physics->GetMapwhoPosition(), physics, SpherePos, Radius);
    return false;
}
