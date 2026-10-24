// NHitLocations::CPhysicalPrimitiveMesh::GetClosestPointToPos2D(const CThing& thing, const C3DVector& pos, C3DVector& normal) const -- a mesh primitive has no 2D shape: the closest point is the position itself
// Leaf: ?GetClosestPointToPos2D@CPhysicalPrimitiveMesh@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00814BC0, 27 bytes (+5 int3 padding).
// Name: FableWin.pdb ?GetClosestPointToPos2D@CPhysicalPrimitiveMesh@NHitLocations@@UBE?AVC3DVector@@ABVCThing@@ABV3@AAV3@@Z;
// slot 10 of the vtable 0x0125B9B0 (rtti-vtables.tsv; the sphere/cylinder slot 10 is the same method).
// Callees: none.
// Shape: the C3DVector result comes back through the hidden pointer ([esp+4], returned in eax); `pos`
// is the third stack word ([esp+0Ch]) and is copied word by word through edx by C3DVector's memberwise
// USER copy constructor (the implicit copy uses esi and a `mov edx,eax` copy of the result pointer); `thing` and `normal` are unused. `ret 10h`: hidden pointer + three arguments.
struct CThing;

struct C3DVector {
    float X, Y, Z;
    C3DVector(const C3DVector& v) : X(v.X), Y(v.Y), Z(v.Z) {}
};

namespace NHitLocations {

struct CPhysicalPrimitiveMesh {
    virtual void v00();
    C3DVector GetClosestPointToPos2D(const CThing& thing, const C3DVector& pos, C3DVector& normal) const;
};

C3DVector CPhysicalPrimitiveMesh::GetClosestPointToPos2D(const CThing& thing, const C3DVector& pos, C3DVector& normal) const
{
    return pos;
}

} // namespace NHitLocations