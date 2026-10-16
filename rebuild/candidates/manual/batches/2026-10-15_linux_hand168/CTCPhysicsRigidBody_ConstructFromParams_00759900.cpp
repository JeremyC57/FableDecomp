// CTCPhysicsRigidBody::ConstructFromParams(CTCParamsBase* params) -- base CTCPhysicsBase construction from the params (position), then the params' orientation set copied into RHSet and OldRHSet
// Leaf: ConstructFromParams
// Retail: Fable.exe (TLC, VC7.1) @ 0x00759900, 99 bytes. CTCPhysicsRigidBody vtable 0x0126996C slot 3 (catalog "vfunc_3"; the
//   CTCPhysicsBase slot-3 body 0x006AF700 is labelled CTCPhysicsBase::ConstructFromParams).
// Callees: CTCPhysicsBase::ConstructFromParams(CTCParamsBase*) @ 0x006AF700 (called non-virtually).
// Layout: engine/CTCPhysicsRigidBody.h (PDB, 16-byte vectors) with retail 12-byte STLport vectors: CollisionNormal +0x40,
//   CollidingVertexIndex +0x4c, Collidors +0x58, PPhysicsMeshInfo +0x64, RHSet +0x68 (PDB +0x74), OldRHSet +0x80 (PDB +0x8c).
//   The physics params: Position +0x04 (read by the base), RHSet +0x10 (CRightHandedSet: Up, Forward).
// Shape: two implicit 24-byte POD assignments from the same source (the source address kept in eax for the second copy).
struct C3DVector { float X, Y, Z; };
struct CRightHandedSet { C3DVector Up; C3DVector Forward; };
struct CTCParamsBase {
    void*     __vftable;                        // +0x00
};
struct CTCParamsPhysics : CTCParamsBase {
    C3DVector       Position;                   // +0x04
    CRightHandedSet RHSet;                      // +0x10
};
struct CTCPhysicsBase {
    virtual void v000(); virtual void v001(); virtual void v002();
    virtual void ConstructFromParams(CTCParamsBase* params);   // slot 3 (+0xc): 0x006AF700
    unsigned char _pad04[0x3c];                 // +0x04 CTCBase body .. Moving bits (+0x3c)
};
struct CTCPhysicsRigidBody : CTCPhysicsBase {
    void*           CollisionNormal[3];         // +0x40 vector<C3DVector>
    void*           CollidingVertexIndex[3];    // +0x4c vector<long>
    void*           Collidors[3];               // +0x58 vector<CThing*>
    void*           PPhysicsMeshInfo;           // +0x64
    CRightHandedSet RHSet;                      // +0x68
    CRightHandedSet OldRHSet;                   // +0x80
    virtual void ConstructFromParams(CTCParamsBase* params);
};

void CTCPhysicsRigidBody::ConstructFromParams(CTCParamsBase* params)
{
    CTCPhysicsBase::ConstructFromParams(params);
    CTCParamsPhysics* physicsParams = static_cast<CTCParamsPhysics*>(params);
    RHSet = physicsParams->RHSet;
    OldRHSet = physicsParams->RHSet;
}
