// CTCPhysicsRigidBody::vfunc_75(const CRightHandedSet& set) -- store the rigid body's right-handed set and mark it as moving (SetFacingAnglesFromRHSet slot, no old-set snapshot)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00759A00, length 56 bytes (vtable 0x0126996C slot 75, rtti-vtables.tsv; slot 75
// of CTCPhysicsStandard is SetFacingAnglesFromRHSet 0x007239C0, which also calls UpdateOldRHSet first).
// Callees: CTCPhysicsBase::SetAsMoving(bool) @ 0x006B0130, tail-jumped.
// Shape: the 24-byte set is copied through a `lea eax,[ecx+68h]` base with esi as the carrier (implicit assignment of a POD
// CRightHandedSet); the last call is a void thiscall, so VC7.1 tail-jumps it and rewrites the dead argument slot
// (`mov [esp+4],1 ; jmp`).
// Layout: Ego_r.pdb CTCPhysicsRigidBody has RHSet at +0x74 after three 16-byte std::vectors at +0x40; retail vectors are 12
// bytes (GOTCHAS wave 23), so retail RHSet sits at +0x68. engine/CTCPhysicsRigidBody.h follows the PDB offsets and has no
// virtuals, so the class is modelled locally.
struct C3DVector { float X, Y, Z; };
struct CRightHandedSet { C3DVector Up; C3DVector Forward; };
struct CRetailVector { void* m_first; void* m_last; void* m_end; };   // retail std::vector, 12 bytes

struct CTCPhysicsBase {                        // 0x40 bytes
    virtual void v00();
    unsigned char _body[0x3c];                 // +0x04 CTCBase, Position .. Moving/OldMoving
    void SetAsMoving(bool moving);             // 0x006B0130
};

struct CTCPhysicsRigidBody : CTCPhysicsBase {
    CRetailVector   CollisionNormal;           // +0x40
    CRetailVector   CollidingVertexIndex;      // +0x4c
    CRetailVector   Collidors;                 // +0x58
    void*           PPhysicsMeshInfo;          // +0x64
    CRightHandedSet RHSet;                     // +0x68
    CRightHandedSet OldRHSet;                  // +0x80
    void vfunc_75(const CRightHandedSet& set);
};

void CTCPhysicsRigidBody::vfunc_75(const CRightHandedSet& set)
{
    RHSet = set;
    SetAsMoving(true);
}
