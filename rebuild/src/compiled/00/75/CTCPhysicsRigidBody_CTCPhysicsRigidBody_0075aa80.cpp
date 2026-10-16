// CTCPhysicsRigidBody::CTCPhysicsRigidBody(CThing* parent) -- CTCPhysicsBase(parent), own vtable, the three collision vectors and the rigid body's three vertex vectors empty, no physics mesh info yet
// Leaf: ??0CTCPhysicsRigidBody
// Retail: Fable.exe (TLC, VC7.1) @ 0x0075AA80, 111 bytes. Vtable 0x0126996C (CTCPhysicsRigidBody: slot 3 ConstructFromParams
//   0x00759900, slot 4 OnCreate 0x0075B2D0, slot 97 GetPossibleCollidedWithArea 0x00759870).
// Callees: CTCPhysicsBase::CTCPhysicsBase(CThing*) @ 0x006B0730.
// Layout: PDB CTCPhysicsRigidBody (engine/CTCPhysicsRigidBody.h) with retail 12-byte STLport vectors: CollisionNormal +0x40,
//   CollidingVertexIndex +0x4c, Collidors +0x58, PPhysicsMeshInfo +0x64, RHSet +0x68, OldRHSet +0x80, OffsetVector +0x98,
//   Body +0xa0 (CRigidBody: BodyBoundingVertices +0x44, CurrentConfig +0x50 and LastConfig +0x104, each CRigidBodyConfig 0xb4
//   with BoundingVertices at +0xa8 -> +0xe4, +0x198, +0x24c). Matrices / vectors of floats are left uninitialised.
// Shape: member vectors zeroed by their inline ctors in declaration order with one shared zero register; PPhysicsMeshInfo is a
//   body assignment (stored last).
struct CThing;
template <class T> struct CRetailVector {       // STLport vector (12 bytes)
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    CRetailVector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) {}
};
struct C3DVector { float X, Y, Z; };
struct CRightHandedSet { C3DVector Up; C3DVector Forward; };
struct C2DVector { float X, Y; };
struct CRigidBodyConfig {                       // retail 0xb4
    unsigned char _pad00[0xa8];                 // CMPosition, Orientation, CMVelocity, AngularMomentum, CMForce, Torque,
                                                // InverseWorldInertiaTensor, AngularVelocity
    CRetailVector<C3DVector> BoundingVertices;  // +0xa8
};
struct CRigidBody {
    unsigned char _pad00[0x44];                 // OneOverMass, CoefficientOfRestitution, InverseBodyInertiaTensor, CMPosition
    CRetailVector<C3DVector> BodyBoundingVertices;   // +0x44
    CRigidBodyConfig CurrentConfig;             // +0x50
    CRigidBodyConfig LastConfig;                // +0x104
};
struct CTCPhysicsBase {
    virtual ~CTCPhysicsBase();
    unsigned char _pad04[0x3c];
    CTCPhysicsBase(CThing* parent);             // 0x006B0730
};
struct CThingPhysicsMeshInfo;
struct CTCPhysicsRigidBody : CTCPhysicsBase {
    CRetailVector<C3DVector> CollisionNormal;   // +0x40
    CRetailVector<long>      CollidingVertexIndex;   // +0x4c
    CRetailVector<CThing*>   Collidors;         // +0x58
    CThingPhysicsMeshInfo*   PPhysicsMeshInfo;  // +0x64
    CRightHandedSet RHSet;                      // +0x68
    CRightHandedSet OldRHSet;                   // +0x80
    C2DVector       OffsetVector;               // +0x98
    CRigidBody      Body;                       // +0xa0
    virtual ~CTCPhysicsRigidBody();
    CTCPhysicsRigidBody(CThing* parent);
};

CTCPhysicsRigidBody::CTCPhysicsRigidBody(CThing* parent) : CTCPhysicsBase(parent)
{
    PPhysicsMeshInfo = 0;
}