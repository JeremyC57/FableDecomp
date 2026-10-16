// CThingFilter_OverlappingSphere::operator()(CThing* thing) -- true when the thing's physics bounding sphere overlaps the filter sphere (SpherePos, Radius)
// Leaf: ??RCThingFilter_OverlappingSphere
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006689E0, length 107 bytes (+ int3 padding).
// Callees: CTCPhysicsBase::GetBoundingInfo(C3DVector*, float*) @ 0x006AF640 (landed).
// Layout: PDB CThingFilter_OverlappingSphere { C3DVector SpherePos (+0); float Radius (+0xc); }; retail CThing PhysicsTC +0x60
// (no null test: the filter is only run on physical things).
// Shape: the bounding centre is a C3DVector local, the bounding radius lands in the dead thing argument slot; the deltas
// are thing minus SpherePos, the radii summed as `boundRadius + Radius`; `if (d2 < r*r) return true; return false;`
// gives the byte-constant exits.
struct C3DVector {
    float X, Y, Z;
};
struct CTCPhysicsBase {
    void GetBoundingInfo(C3DVector* centre, float* radius);   // 0x006AF640
};
struct CThing {
    unsigned char   _pad00[0x60];
    CTCPhysicsBase* PhysicsTC;                  // +0x60
};
struct CThingFilter_OverlappingSphere {
    C3DVector SpherePos;                        // +0x00
    float     Radius;                           // +0x0c
    bool operator()(CThing* thing) const;
};

bool CThingFilter_OverlappingSphere::operator()(CThing* thing) const
{
    C3DVector centre;
    float boundRadius;
    thing->PhysicsTC->GetBoundingInfo(&centre, &boundRadius);
    float dx = centre.X - SpherePos.X;
    float dy = centre.Y - SpherePos.Y;
    float dz = centre.Z - SpherePos.Z;
    float r = boundRadius + Radius;
    if (dx * dx + dy * dy + dz * dz < r * r)
        return true;
    return false;
}