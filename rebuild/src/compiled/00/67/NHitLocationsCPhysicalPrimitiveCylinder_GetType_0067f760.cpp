// NHitLocations::CPhysicalPrimitiveCylinder::GetType() const -- returns (EPhysicalPrimitiveType)2
// Leaf: ?GetType@CPhysicalPrimitiveCylinder@NHitLocations@@UBE?AW4EPhysicalPrimitiveType@2@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x0067F760, thiscall, `ret`.
// Identity: slot 3 of NHitLocations::CPhysicalPrimitiveCylinder's vtable 0x0125BA4C (defined here: the slot holds this body in 1 vtable(s), all of NHitLocations::CPhysicalPrimitiveCylinder or derived); the Xbox 360 Test PDB virtual order (third source) aligned on 9 TLC-named slots of that vtable puts GetType(void) at this slot; Ego_r/FableWin name set: ?GetType@CPhysicalPrimitiveCylinder@NHitLocations@@UBE?AW4EPhysicalPrimitiveType@2@XZ; `ret` matches the signature.
namespace NHitLocations { enum EPhysicalPrimitiveType { }; }
namespace NHitLocations {
class CPhysicalPrimitiveCylinder {
public:
    virtual NHitLocations::EPhysicalPrimitiveType GetType() const;
};
NHitLocations::EPhysicalPrimitiveType CPhysicalPrimitiveCylinder::GetType() const
{
    return (EPhysicalPrimitiveType)2;
}
}
