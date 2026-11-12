// NHitLocations::CPhysicalPrimitiveMesh::IsPosInside(CThing const &, C3DVector const &) const -- returns false
// Leaf: ?IsPosInside@CPhysicalPrimitiveMesh@NHitLocations@@UBE_NABVCThing@@ABVC3DVector@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00814BB0, thiscall, `ret 8`.
// Identity: slot 9 of NHitLocations::CPhysicalPrimitiveMesh's vtable 0x0125B9B0 (defined here: the slot holds this body in 1 vtable(s), all of NHitLocations::CPhysicalPrimitiveMesh or derived); the Xbox 360 Test PDB virtual order (third source) aligned on 5 TLC-named slots of that vtable puts IsPosInside(CThing const &, C3DVector const &) at this slot; Ego_r/FableWin name set: ?IsPosInside@CPhysicalPrimitiveMesh@NHitLocations@@UBE_NABVCThing@@ABVC3DVector@@@Z; `ret` matches the signature.
class CThing;
class C3DVector;
namespace NHitLocations {
class CPhysicalPrimitiveMesh {
public:
    virtual bool IsPosInside(CThing const &, C3DVector const &) const;
};
bool CPhysicalPrimitiveMesh::IsPosInside(CThing const &, C3DVector const &) const
{
    return false;
}
}
