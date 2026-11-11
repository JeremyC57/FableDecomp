// NHitLocations::CPhysicalPrimitiveMesh::Init(CPersistContext&, CCharString const &) -- returns true
// Leaf: ?Init@CPhysicalPrimitiveMesh@NHitLocations@@UAE_NAAVCPersistContext@@ABVCCharString@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00814B80, thiscall, `ret 8`.
// Identity: slot 4 of NHitLocations::CPhysicalPrimitiveMesh's vtable 0x0125B9B0 (defined here: the slot holds this body in 1 vtable(s), all of NHitLocations::CPhysicalPrimitiveMesh or derived); the Xbox 360 Test PDB virtual order (third source) aligned on 5 TLC-named slots of that vtable puts Init(CPersistContext &, CCharString const &) at this slot; Ego_r/FableWin name set: ?Init@CPhysicalPrimitiveMesh@NHitLocations@@UAE_NAAVCPersistContext@@ABVCCharString@@@Z; `ret` matches the signature.
class CPersistContext;
class CCharString;
namespace NHitLocations {
class CPhysicalPrimitiveMesh {
public:
    virtual bool Init(CPersistContext&, CCharString const &);
};
bool CPhysicalPrimitiveMesh::Init(CPersistContext&, CCharString const &)
{
    return true;
}
}
