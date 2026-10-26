// NHitLocations::CPhysicalPrimitiveMesh::Alloc() const -- vtable slot 1: returns a freshly constructed (empty) mesh primitive
// Leaf: ?Alloc@CPhysicalPrimitiveMesh@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0067F790, 24 bytes (+8 int3 padding).
// Name: FableWin.pdb ?Alloc@CPhysicalPrimitiveMesh@NHitLocations@@UBEPAVCPhysicalPrimitiveBase@2@XZ; slot 1 of the
// vtable 0x0125B9B0 (rtti-vtables.tsv: slot 0 the deleting dtor 0x0067F7B0, slot 2 GetToken, slot 3 GetType).
// Callees: operator new @ 0x00BFEA1A (`push 4 ; call ; add esp,4`).
// Shape: `new CPhysicalPrimitiveMesh` with the inline default constructor -- the mesh primitive has no data
// members (Ego_r.pdb: 4 bytes, the vptr only), so the null-checked construction is one vtable store.
namespace NHitLocations {

class CPhysicalPrimitiveBase { public:
    virtual ~CPhysicalPrimitiveBase() {}
    virtual CPhysicalPrimitiveBase* Alloc() const = 0;   // slot 1
};

struct CPhysicalPrimitiveMesh : CPhysicalPrimitiveBase {
    virtual CPhysicalPrimitiveBase* Alloc() const;
};

CPhysicalPrimitiveBase* CPhysicalPrimitiveMesh::Alloc() const
{
    return new CPhysicalPrimitiveMesh;
}

} // namespace NHitLocations