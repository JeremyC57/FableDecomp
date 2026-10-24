// NHitLocations::CPhysicalPrimitiveMesh::GetBoundingSphere(const CThing& thing, C3DVector& centre, float& radius) const -- a mesh primitive's bounding sphere is the thing's own physics bounding info
// Leaf: ?GetBoundingSphere@CPhysicalPrimitiveMesh@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00815200, 25 bytes.
// Name: FableWin.pdb ?GetBoundingSphere@CPhysicalPrimitiveMesh@NHitLocations@@UBEXABVCThing@@AAVC3DVector@@AAM@Z;
// slot 11 of the vtable 0x0125B9B0 (rtti-vtables.tsv; the xref is that vtable slot, 0x0125B9DC).
// Callees: CTCPhysicsBase::GetBoundingInfo(C3DVector*, float*) @ 0x006AF640 (landed, __fastcall, ret 8).
// Layout: retail CThing holds the physics component pointer at +0x60 (as the landed
// CThingFilter_IsInFOV::operator() 0x006A7390, no null test).
// Shape: a straight forward of the two out references to the component; the three argument loads
// are all hoisted (radius, centre, thing), the pushes follow right to left, then `this` = thing->PhysicsTC.
// `ret 0Ch`.
struct C3DVector {
    float X, Y, Z;
};

struct CTCPhysicsBase {
    void GetBoundingInfo(C3DVector* centre, float* radius);   // 0x006AF640
};

struct CThing {
    unsigned char   _pad00[0x60];
    CTCPhysicsBase* PhysicsTC;                      // +0x60
};

namespace NHitLocations {

struct CPhysicalPrimitiveMesh {
    virtual void v00();
    void GetBoundingSphere(const CThing& thing, C3DVector& centre, float& radius) const;
};

void CPhysicalPrimitiveMesh::GetBoundingSphere(const CThing& thing, C3DVector& centre, float& radius) const
{
    thing.PhysicsTC->GetBoundingInfo(&centre, &radius);
}

} // namespace NHitLocations