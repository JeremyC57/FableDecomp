// CTCPhysicsRigidBody::SetFacingAnglesFromRHSet(const CRightHandedSet& rh_set) -- store the rigid body's right-handed set and mark it as moving
// Leaf: ?SetFacingAnglesFromRHSet@CTCPhysicsRigidBody@@UAEXABVCRightHandedSet@@@Z
// Plan phase 3 (2026-11-04), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
// Retail: Fable.exe (TLC, VC7.1) @ 0x007599C0, 56 bytes (vtable 0x0126996C slot 74, rtti-vtables.tsv; the only reference is
//   that slot, 0x01269A94).
// Identity (proven): slot 74 / 75 hold two byte-identical bodies, 0x007599C0 / 0x00759A00, in both CTCPhysicsRigidBody
//   (0x0126996C) and CTCPhysicsStandard (0x01265F84: 0x007238E0 / 0x007239C0). CTCPhysicsStandard's source order is the
//   same in retail and Ego_r.pdb: slot 74 body, UpdateOldRHSet (slot 95, 0x00723930), ResetRHSet (0x00723980, which calls
//   slot 75), slot 75 body; Ego_r lists SetFacingAnglesFromRHSet 0x0073A5A0, UpdateOldRHSet 0x0073A5F0, ResetRHSet
//   0x0073A640, SetRHSet 0x0073A680, so slot 74 is SetFacingAnglesFromRHSet and slot 75 SetRHSet. CTCPhysicsRigidBody's
//   pair keeps that order in retail (0x007599C0, 0x00759A00), Ego_r (SetFacingAnglesFromRHSet 0x007687C0, SetRHSet 0x00768800)
//   and FableWin (0x025C03F0, 0x025C0450). Ego_r.pdb ?SetFacingAnglesFromRHSet@CTCPhysicsRigidBody@@UAEXABVCRightHandedSet@@@Z.
//   The catalog label `CTCPhysicsRigidBody::vfunc_74` is a placeholder.
// Callees: CTCPhysicsBase::SetAsMoving(bool) @ 0x006B0130, tail-jumped.
// Shape: the 24-byte set is copied through a `lea eax,[ecx+68h]` base with esi as the carrier (implicit assignment of the POD
//   CRightHandedSet); the last call is a void thiscall, so VC7.1 tail-jumps it and rewrites the dead argument slot
//   (`mov [esp+4],1 ; jmp`). The rigid body has no UpdateOldRHSet step (unlike CTCPhysicsStandard's slot 74).
// Layout: Ego_r.pdb CTCPhysicsRigidBody : CTCPhysicsBase (0x40) has three std::vectors at +0x40 / +0x50 / +0x60,
//   PPhysicsMeshInfo +0x70 and RHSet +0x74; retail's STLport vectors are 12 bytes, so RHSet sits at +0x68.
//   engine/CTCPhysicsRigidBody.h follows the PDB offsets and engine/CTCPhysicsBase.h is a flattened POD without virtuals,
//   so both are modelled locally (as the landed CTCPhysicsStandard rows do).
namespace std {
template <class _Tp> class allocator { };
template <class _Tp, class _Alloc> class _STLP_alloc_proxy : public _Alloc { public:
    _Tp _M_data;
};
template <class _Tp, class _Alloc> class _Vector_base { public:
    _Tp* _M_start;                                          // +0x0
    _Tp* _M_finish;                                         // +0x4
    _STLP_alloc_proxy<_Tp*, _Alloc> _M_end_of_storage;      // +0x8
};
template <class _Tp, class _Alloc = allocator<_Tp> > class vector : public _Vector_base<_Tp, _Alloc> { };
} // namespace std

#include "engine/C3DVector.h"
class CRightHandedSet { public: C3DVector Up; C3DVector Forward; };
class CThing;
class CThingPhysicsMeshInfo;

class CTCPhysicsBase {                                      // 0x40 bytes
public:
    virtual void SetFacingAnglesFromRHSet(const CRightHandedSet& rh_set);   // slot 74
    virtual void SetRHSet(const CRightHandedSet& rh_set);                   // slot 75
    void SetAsMoving(bool moving);                          // 0x006B0130
private:
    unsigned char _body[0x3c];                              // +0x04 CTCBase, Position .. Moving/OldMoving
};

class CTCPhysicsRigidBody : public CTCPhysicsBase {         // vtable 0x0126996C
public:
    virtual void SetFacingAnglesFromRHSet(const CRightHandedSet& rh_set);
    std::vector<C3DVector, std::allocator<C3DVector> > CollisionNormal;     // +0x40
    std::vector<long, std::allocator<long> >           CollidingVertexIndex;  // +0x4C
    std::vector<CThing*, std::allocator<CThing*> >     Collidors;           // +0x58
    CThingPhysicsMeshInfo* PPhysicsMeshInfo;                // +0x64
    CRightHandedSet        RHSet;                           // +0x68
    CRightHandedSet        OldRHSet;                        // +0x80
};

void CTCPhysicsRigidBody::SetFacingAnglesFromRHSet(const CRightHandedSet& rh_set)
{
    RHSet = rh_set;
    SetAsMoving(true);
}