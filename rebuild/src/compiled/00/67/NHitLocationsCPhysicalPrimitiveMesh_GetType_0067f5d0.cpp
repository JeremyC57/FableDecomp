// NHitLocations::CPhysicalPrimitiveMesh::GetType() const -- returns (EPhysicalPrimitiveType)3
// Leaf: ?GetType@CPhysicalPrimitiveMesh@NHitLocations@@UBE?AW4EPhysicalPrimitiveType@2@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x0067F5D0, thiscall, `ret`.
// Identity: slot 3 of NHitLocations::CPhysicalPrimitiveMesh's vtable 0x0125B9B0 (defined here: the slot holds this body in 1 vtable(s), all of NHitLocations::CPhysicalPrimitiveMesh or derived); the Xbox 360 Test PDB virtual order (third source) aligned on 5 TLC-named slots of that vtable puts GetType(void) at this slot; Ego_r/FableWin name set: ?GetType@CPhysicalPrimitiveMesh@NHitLocations@@UBE?AW4EPhysicalPrimitiveType@2@XZ; `ret` matches the signature.
namespace NHitLocations { enum EPhysicalPrimitiveType { }; }
namespace NHitLocations {
class CPhysicalPrimitiveMesh {
public:
    virtual NHitLocations::EPhysicalPrimitiveType GetType() const;
};
NHitLocations::EPhysicalPrimitiveType CPhysicalPrimitiveMesh::GetType() const
{
    return (EPhysicalPrimitiveType)3;
}
}
