// CPhysicsFacesSegmentBranch::GetPossibleCollisionSet(float radius, const C3DVector& start, const C3DVector& end, std::vector<long>& faces) const -- BSP walk: recurse into each child whose half-space the swept segment (inflated by radius) can reach
// Leaf: ?GetPossibleCollisionSet@CPhysicsFacesSegmentBranch@@MBEXMABVC3DVector@@0AAV?$vector@JV?$allocator@J@std@@@std@@@Z
// Class-model move (NF325 draft, landed by NF327 2026-11-24 with the base's friend opt-in): defined on the real class of engine/CPhysicsFacesSegmentBranch.h under FABLE_RETAIL_CPHYSICSFACESSEGMENTBRANCH_VIRTUAL_MODEL
//   (the opt-in polymorphic model; was CPhysicsFacesSegmentBranch_Methods with a local CIPhysicsFacesSegmentBase model and a local
//   global-namespace vector template); the retail RTTI vftable placement names this PDB member here. The plane accessor moves from the
//   alias class to a file-static inline (no symbol; same reinterpretation of ChildDividingPlane).
// Plan phase 3 (2026-11-04), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A86030, 181 bytes (+ int3 padding), `ret 10h`; protected virtual (slot 4, +0x10).
// Callees: the children's own virtual GetPossibleCollisionSet (slot 4, +0x10).
// Shape: both end points' signed distances to ChildDividingPlane (CPlane: Normal +4, Distance +0x10 of the branch) are
//   computed up front into the dead `start`/`end` argument slots (the two pointers stay in edi/ebx); the negative child is visited when either
//   distance is below `radius` (`test ah,5 ; jnp` / `jp`), the positive child when either is above `-radius`
//   (`fchs` once, `test ah,41h ; je` / `jne`, the negated radius popped on both paths). Layout: engine header
//   (ChildDividingPlane +4, PositiveChild +0x14, NegativeChild +0x18).
#define FABLE_RETAIL_CPHYSICSFACESSEGMENTBRANCH_VIRTUAL_MODEL
#define FABLE_RETAIL_CIPHYSICSFACESSEGMENTBASE_FRIEND_BRANCH   // NF327 (2026-11-24, NF325 Q1): the base grants the protected slot-4 recursion
#include "engine/CPhysicsFacesSegmentBranch.h"

#include "engine/C3DVector.h"
struct CPlaneModel {                                   // CPlane (Ego_r.pdb): Normal +0, Distance +0xC
    C3DVector Normal;
    float     Distance;
    float GetDistanceToPoint(const C3DVector& p) const { return Normal.X * p.X + Normal.Y * p.Y + Normal.Z * p.Z - Distance; }
};
class CPhysicsFacesSegmentBranch_Methods : public CPhysicsFacesSegmentBranch {   // HEAD's helper scaffolding, kept so its pick-any COMDAT is unchanged
public:
    const CPlaneModel& Plane() const { return *reinterpret_cast<const CPlaneModel*>(ChildDividingPlane); }
};
static inline const CPlaneModel& Plane(const CPhysicsFacesSegmentBranch& branch) { return static_cast<const CPhysicsFacesSegmentBranch_Methods&>(branch).Plane(); }
void CPhysicsFacesSegmentBranch::GetPossibleCollisionSet(float radius, const C3DVector& start, const C3DVector& end,
                                                         std::vector<long>& faces) const
{
    float startDistance = Plane(*this).GetDistanceToPoint(start);
    float endDistance = Plane(*this).GetDistanceToPoint(end);
    if (startDistance < radius || endDistance < radius)
        NegativeChild->GetPossibleCollisionSet(radius, start, end, faces);
    if (startDistance > -radius || endDistance > -radius)
        PositiveChild->GetPossibleCollisionSet(radius, start, end, faces);
}