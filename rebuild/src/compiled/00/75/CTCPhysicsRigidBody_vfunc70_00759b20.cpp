// CTCPhysicsRigidBody::vfunc_70() -- returns the Forward vector of the rigid body's current right-handed set (RHSet+0x0c, this+0x74) by value
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00759B20, length 24 bytes (vtable 0x0126996C slot 70, rtti-vtables.tsv).
// Callees: none. Same slot and shape as CTCPhysicsStandard::vfunc_70 (0x00723C00, RHSet.Forward at +0x5c).
// Shape: a 12-byte C3DVector returned BY VALUE through the hidden pointer (eax = the pointer at `ret 4`); the interleaved
// dword-per-member copy needs C3DVector's memberwise user copy constructor.
// Layout: Ego_r.pdb CTCPhysicsRigidBody puts RHSet at +0x74 after three 16-byte std::vectors at +0x40; the retail vectors are
// 12 bytes (GOTCHAS wave 23), so retail RHSet sits at +0x68 and its Forward at +0x74 -- matching the bytes. engine/
// CTCPhysicsRigidBody.h follows the PDB offsets (4 bytes high per vector) and has no virtuals, so the class is modelled locally.
struct C3DVector {
    float X, Y, Z;
    C3DVector() {}
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) {}
};
struct CRightHandedSet { C3DVector Up; C3DVector Forward; };
struct CRetailVector { void* m_first; void* m_last; void* m_end; };   // retail std::vector, 12 bytes

struct CTCPhysicsRigidBody {
    virtual void v00();
    unsigned char   _base[0x3c];               // +0x04 CTCPhysicsBase body
    CRetailVector   CollisionNormal;           // +0x40
    CRetailVector   CollidingVertexIndex;      // +0x4c
    CRetailVector   Collidors;                 // +0x58
    void*           PPhysicsMeshInfo;          // +0x64
    CRightHandedSet RHSet;                     // +0x68
    CRightHandedSet OldRHSet;                  // +0x80
    C3DVector vfunc_70();
};

C3DVector CTCPhysicsRigidBody::vfunc_70()
{
    return RHSet.Forward;
}