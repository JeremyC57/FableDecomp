// Grade: DIFFER(ours v retail) -- 95v95: only the square order differs (retail squares dx first: `fld st(2) ; fmul st(3)` twice; ours squares dy first: `fld st(1) ; fmul st(2)` then `fld st(3) ; fmul st(4)`)
// CThingFilter_OverlappingCircle::operator()(CThing* thing) -- true when the thing's physics footprint circle (map-who position, component radius +0x34) overlaps the filter's circle (SpherePos, Radius) in the XY plane
// Leaf: ??RCThingFilter_OverlappingCircle
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0063EF50, length 95 bytes (+ int3 padding).
// Callees: CTCPhysicsBase::GetMapwhoPosition @ 0x006AFCB0 (C3DVector through the hidden pointer, a 12-byte frame local).
// Layout: PDB CThingFilter_OverlappingCircle { C3DVector SpherePos; float Radius; }; retail CThing PhysicsTC +0x60; the
// physics component's own radius float at +0x34 (local model).
// Shape: C3DVector has a user constructor (hidden-pointer return); the component pointer stays in edi across the
// call; `if (dist2 < r * r) return true; return false;` gives byte-constant exits.
struct C3DVector {
    float x, y, z;
    C3DVector() {}
};

struct CTCPhysicsBase {
    unsigned char _pad00[0x34];
    float         m_radius;                         // +0x34
    C3DVector GetMapwhoPosition() const;            // 0x006AFCB0
};

struct CThing {
    unsigned char   _pad00[0x60];
    CTCPhysicsBase* PhysicsTC;                      // +0x60
};

struct CThingFilter_OverlappingCircle {
    C3DVector SpherePos;                            // +0x00
    float     Radius;                               // +0x0c
    bool Overlaps(const C3DVector& pos, const CTCPhysicsBase* physics) const
    {
        float dx = pos.x - SpherePos.x;
        float dy = pos.y - SpherePos.y;
        float r = physics->m_radius + Radius;
        if (dx * dx + dy * dy < r * r)
            return true;
        return false;
    }
    bool operator()(CThing* thing) const;
};

bool CThingFilter_OverlappingCircle::operator()(CThing* thing) const
{
    CTCPhysicsBase* physics = thing->PhysicsTC;
    if (physics)
        return Overlaps(physics->GetMapwhoPosition(), physics);
    return false;
}
