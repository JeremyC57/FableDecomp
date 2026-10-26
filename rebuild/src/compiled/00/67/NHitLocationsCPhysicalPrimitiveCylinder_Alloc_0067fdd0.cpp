// NHitLocations::CPhysicalPrimitiveCylinder::Alloc() const -- vtable slot 1: returns a freshly constructed cylinder primitive (BaseName unset)
// Leaf: ?Alloc@CPhysicalPrimitiveCylinder@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0067FDD0, 31 bytes.
// Name: FableWin.pdb ?Alloc@CPhysicalPrimitiveCylinder@NHitLocations@@UBEPAVCPhysicalPrimitiveBase@2@XZ; slot 1 of the
// vtable 0x0125BA4C (rtti-vtables.tsv; slot 0 is the deleting dtor 0x0067F770).
// Callees: operator new @ 0x00BFEA1A (`push 10h`).
// Layout: Ego_r.pdb CPhysicalPrimitiveCylinder (16 bytes) = { vptr, CDefString BaseName +4, float Radius +8, float Height +0xC }.
// Shape: `new CPhysicalPrimitiveCylinder` with the inline default constructor: the vtable store, then the
// CDefString member's own constructor (index -1); Radius and Height are left uninitialised (no store), as retail.
namespace NHitLocations {

struct CDefString {
    long Index;
    CDefString() : Index(-1) {}
};

class CPhysicalPrimitiveBase { public:
    virtual ~CPhysicalPrimitiveBase() {}
    virtual CPhysicalPrimitiveBase* Alloc() const = 0;   // slot 1
};

struct CPhysicalPrimitiveCylinder : CPhysicalPrimitiveBase {
    CDefString BaseName;   // +0x04
    float      Radius;     // +0x08
    float      Height;     // +0x0C
    virtual CPhysicalPrimitiveBase* Alloc() const;
};

CPhysicalPrimitiveBase* CPhysicalPrimitiveCylinder::Alloc() const
{
    return new CPhysicalPrimitiveCylinder;
}

} // namespace NHitLocations