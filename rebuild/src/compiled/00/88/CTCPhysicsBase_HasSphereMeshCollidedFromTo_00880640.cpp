// CTCPhysicsBase::HasSphereMeshCollidedFromTo(const CTCPhysicsBase& other, const C3DVector& from, const C3DVector& to, CPhysicsCollideResponse& response) const -- sweep this body's sphere (raised by its radius; the player uses a fixed 0.4) from `from` to `to` through the other body's physics mesh and report the hit position (lowered back) and normal
// Retail: Fable.exe (TLC, VC7.1) @ 0x00880640, 230 bytes (`ret 10h`; PDB ?HasSphereMeshCollidedFromTo@CTCPhysicsBase@@IBE_NABV1@ABVC3DVector@@1AAVCPhysicsCollideResponse@@@Z).
// Callees: CTCPhysicsBase::GetPhysicsMeshObject() const @ 0x006B0850 (CPhysicsMeshObject by hidden pointer), CPhysicsMeshObject::
//   GetSphereIntersectionFromTo(float, const C3DVector&, const C3DVector&, CMeshIntersectionData&) const @ 0x00A120D0,
//   ~CPhysicsMeshObject @ 0x00A127D0 (inlined once per return). 0.4f is an immediate store.
// Layout: Ego_r.pdb CPhysicsMeshObject (0x40: Mesh +0x04, PositionMatrix +0x08, Scale +0x38, OO_Scale +0x3c), CMeshIntersectionData
//   {IntersectionPosition, Normal, DistanceSquared} (0x1c), CPhysicsCollideResponse {CollidePos, CollideNormal, Friction};
//   CTCPhysicsBase Radius +0x34; CThing +0x10 type byte (2 = player).
// Shape: the radius is a float local assigned in an if/else (dword copies, homed at [esp]); the raised start/end points are
//   named locals built by the 3-float constructor (Z first: arguments evaluate right to left); the normal is copied before the
//   position with the implicit assignment (address formed with `lea ecx,[eax+0Ch]`), then `CollidePos.Z -= radius`.
struct C3DVector {
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
};
struct CPhysicsCollideResponse {
    C3DVector CollidePos;                               // +0x00
    C3DVector CollideNormal;                            // +0x0c
    float     Friction;                                 // +0x18
};
struct CMeshIntersectionData {
    C3DVector IntersectionPosition;                     // +0x00
    C3DVector Normal;                                   // +0x0c
    float     DistanceSquared;                          // +0x18
};
struct CPhysicsMeshObject {                             // 0x40 bytes
    void*         __vftable;                            // +0x00
    void*         Mesh[1];                              // +0x04 CIVCountedPointer<const CPhysicsMesh>
    unsigned char PositionMatrix[0x30];                 // +0x08 CMatrix3x4
    float         Scale;                                // +0x38
    float         OO_Scale;                             // +0x3c
    ~CPhysicsMeshObject();                              // 0x00A127D0
    bool GetSphereIntersectionFromTo(float radius, const C3DVector& from, const C3DVector& to,
                                     CMeshIntersectionData& data) const;   // 0x00A120D0
};
struct CThing {
    unsigned char _pad00[0x10];
    unsigned char Type;                                 // +0x10 (2 = player)
};
struct CTCPhysicsBase {
    virtual void v000();
    CThing*   ParentThing;                              // +0x04 (CTCBase)
    bool      ToBeRemoved;                              // +0x08
    bool      IsInGlobalUpdateTCs;                      // +0x09
    unsigned char _pad0a[2];
    C3DVector Position;                                 // +0x0c
    C3DVector OldPosition;                              // +0x18
    long      OldPositionLastFrameSet;                  // +0x24
    C3DVector Velocity;                                 // +0x28
    float     Radius;                                   // +0x34
    CPhysicsMeshObject GetPhysicsMeshObject() const;    // 0x006B0850
    bool HasSphereMeshCollidedFromTo(const CTCPhysicsBase& other, const C3DVector& from, const C3DVector& to,
                                     CPhysicsCollideResponse& response) const;
};

bool CTCPhysicsBase::HasSphereMeshCollidedFromTo(const CTCPhysicsBase& other, const C3DVector& from, const C3DVector& to,
                                                 CPhysicsCollideResponse& response) const
{
    float radius;
    if (ParentThing->Type == 2)
        radius = 0.4f;
    else
        radius = Radius;
    CPhysicsMeshObject mesh = other.GetPhysicsMeshObject();
    C3DVector start(from.X, from.Y, from.Z + radius);
    C3DVector end(to.X, to.Y, to.Z + radius);
    CMeshIntersectionData data;
    if (mesh.GetSphereIntersectionFromTo(radius, start, end, data)) {
        response.CollideNormal = data.Normal;
        response.CollidePos = data.IntersectionPosition;
        response.CollidePos.Z -= radius;
        return true;
    }
    return false;
}