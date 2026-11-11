// NHitLocations::CPhysicalPrimitiveMesh::GetClosestPointToPos2D(CThing const&, C3DVector const&, C3DVector&) const -- a mesh primitive's closest point in 2D is the position itself (copied out; the thing and the out vector are not used)
// Leaf: ?GetClosestPointToPos2D@CPhysicalPrimitiveMesh@NHitLocations@@UBE?AVC3DVector@@ABVCThing@@ABV3@AAV3@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00814BC0, thiscall, `ret 16`.
// Identity (agentFAM03 proof, re-verified by agentFAM05): slot 10 of NHitLocations::CPhysicalPrimitiveMesh's vtable 0x0125B9B0 (the slot holds this body in 1 vtable(s), owner NHitLocations::CPhysicalPrimitiveMesh); the Xbox 360 Test PDB virtual order (third source) aligned on 5 TLC-named slots of that vtable puts GetClosestPointToPos2D(CThing const &, C3DVector const &, C3DVector &) at this slot [gap kind GS]; Ego_r/FableWin name set: ?GetClosestPointToPos2D@CPhysicalPrimitiveMesh@NHitLocations@@UBE?AVC3DVector@@ABVCThing@@ABV3@AAV3@@Z; `ret` matches the signature.
// Was: NHitLocations::CPhysicalPrimitiveMesh::vfunc_10 (rebuild/src/compiled/00/81/NHitLocationsCPhysicalPrimitiveMesh_GetClosestPointToPos2D_00814bc0.cpp).
// No shared header for NHitLocations::CPhysicalPrimitiveMesh: minimal local definition on its RTTI base
// NHitLocations::CPhysicalPrimitiveBase (shared header).
#include "engine/NHitLocations_CPhysicalPrimitiveBase.h"
namespace NHitLocations {
class CPhysicalPrimitiveMesh : public CPhysicalPrimitiveBase {
public:
    virtual C3DVector GetClosestPointToPos2D(const CThing&, const C3DVector&, C3DVector&) const;   // slot 10
};
C3DVector CPhysicalPrimitiveMesh::GetClosestPointToPos2D(const CThing&, const C3DVector& pos, C3DVector&) const
{
    return pos;
}
}
