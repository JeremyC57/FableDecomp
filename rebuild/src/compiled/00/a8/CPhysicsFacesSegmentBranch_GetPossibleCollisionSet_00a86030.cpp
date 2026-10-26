// CPhysicsFacesSegmentBranch::GetPossibleCollisionSet(float radius, const C3DVector& start, const C3DVector& end, std::vector<long>& faces) const -- BSP walk: recurse into each child whose half-space the swept segment (inflated by radius) can reach
// Leaf: GetPossibleCollisionSet
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A86030, 181 bytes (+ int3 padding), `ret 10h`; protected virtual (slot 4, +0x10).
// Callees: the children's own virtual GetPossibleCollisionSet (slot 4, +0x10).
// Shape: both end points' signed distances to ChildDividingPlane (CPlane: Normal +4, Distance +0x10 of the branch) are
//   computed up front into the dead `start`/`end` argument slots (the two pointers stay in edi/ebx); the negative child is visited when either
//   distance is below `radius` (`test ah,5 ; jnp` / `jp`), the positive child when either is above `-radius`
//   (`fchs` once, `test ah,41h ; je` / `jne`, the negated radius popped on both paths). Layout: engine header
//   (ChildDividingPlane +4, PositiveChild +0x14, NegativeChild +0x18).
#include "engine/CPhysicsFacesSegmentBranch.h"

class C3DVector { public: float X, Y, Z; };
struct CPlaneModel {                                   // CPlane (Ego_r.pdb): Normal +0, Distance +0xC
    C3DVector Normal;
    float     Distance;
    float GetDistanceToPoint(const C3DVector& p) const { return Normal.X * p.X + Normal.Y * p.Y + Normal.Z * p.Z - Distance; }
};
template <class T> class allocator;
template <class T, class A> class vector;

class CIPhysicsFacesSegmentBase { public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03();
    virtual void GetPossibleCollisionSet(float radius, const C3DVector& start, const C3DVector& end,
                                         vector<long, allocator<long> >& faces) const;   // slot 4 (+0x10)
};

class CPhysicsFacesSegmentBranch_Methods : public CPhysicsFacesSegmentBranch {
public:
    const CPlaneModel& Plane() const { return *reinterpret_cast<const CPlaneModel*>(ChildDividingPlane); }
    void GetPossibleCollisionSet(float radius, const C3DVector& start, const C3DVector& end,
                                 vector<long, allocator<long> >& faces) const;
};

void CPhysicsFacesSegmentBranch_Methods::GetPossibleCollisionSet(float radius, const C3DVector& start, const C3DVector& end,
                                                                 vector<long, allocator<long> >& faces) const
{
    float startDistance = Plane().GetDistanceToPoint(start);
    float endDistance = Plane().GetDistanceToPoint(end);
    if (startDistance < radius || endDistance < radius)
        NegativeChild->GetPossibleCollisionSet(radius, start, end, faces);
    if (startDistance > -radius || endDistance > -radius)
        PositiveChild->GetPossibleCollisionSet(radius, start, end, faces);
}