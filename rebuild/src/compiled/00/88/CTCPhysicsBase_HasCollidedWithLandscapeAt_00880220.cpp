// CTCPhysicsBase::HasCollidedWithLandscapeAt(const C3DVector& position, CPhysicsCollideResponse* response) -- when the landscape height at the position is above it, fill the response with the ground point (position raised to the ground height) and the ground face normal and return true
// Leaf: HasCollidedWithLandscapeAt
// Retail: Fable.exe (TLC, VC7.1) @ 0x00880220, 116 bytes. CTCPhysicsBase vtable slot 92 (0x00880220).
// Callees: CTCBase::GetWorldMap @ 0x006AF630 (catalog "CThingTrackNode::DrawIsDrawable" is propagated), CWorldMap::GetGroundSizeZAt
//   (const C3DVector&) @ 0x0063A300, CWorldMap::GetGroundFaceNormalAt(const C3DVector&) @ 0x0063AB10 (C3DVector by value).
// Layout: CPhysicsCollideResponse (PDB): CollidePos +0x00, CollideNormal +0x0c, Friction +0x18 (not touched).
// Shape: the ground height is a named float local (retail keeps it in the dead position-argument slot); `!(z <= ground)`-style
//   compare as `ground > position.Z`; the normal is copied word by word from the by-value temporary.
class C3DVector { public:
    float X, Y, Z;
    C3DVector() {}
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
};
class CPhysicsCollideResponse { public:
    C3DVector CollidePos;                       // +0x00
    C3DVector CollideNormal;                    // +0x0c
    float     Friction;                         // +0x18
};
class CWorldMap { public:
    float GetGroundSizeZAt(const C3DVector& position);        // 0x0063A300
    C3DVector GetGroundFaceNormalAt(const C3DVector& position); // 0x0063AB10
};
class CTCPhysicsBase { public:
    virtual void v00();
    void* ParentThing;                          // +0x04
    CWorldMap* GetWorldMap_006af630();                   // 0x006AF630 (CTCBase)
    bool HasCollidedWithLandscapeAt(const C3DVector& position, CPhysicsCollideResponse* response);
};

bool CTCPhysicsBase::HasCollidedWithLandscapeAt(const C3DVector& position, CPhysicsCollideResponse* response)
{
    float ground = GetWorldMap_006af630()->GetGroundSizeZAt(position);
    if (ground > position.Z) {
        response->CollideNormal = GetWorldMap_006af630()->GetGroundFaceNormalAt(position);
        response->CollidePos.X = position.X;
        response->CollidePos.Y = position.Y;
        response->CollidePos.Z = ground;
        return true;
    }
    return false;
}