// CTCPhysicsBase::HasCollidedWithLandscapeAt(const C3DVector& position, CPhysicsCollideResponse& response) const [Ego_r.pdb UBE] -- when the landscape height at the position is above it, fill the response with the ground point (position raised to the ground height) and the ground face normal and return true
// Leaf: HasCollidedWithLandscapeAt
// Plan phase 3 (2026-11-04), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
// Retail: Fable.exe (TLC, VC7.1) @ 0x00880220, 116 bytes. CTCPhysicsBase vtable slot 92 (0x00880220).
// Callees: CTCPhysicsBase::GetWorldMap() const @ 0x006AF630 (Ego_r.pdb) (catalog "CThingTrackNode::DrawIsDrawable" is propagated), CWorldMap::GetGroundSizeZAt
//   (const C3DVector&) @ 0x0063A300, CWorldMap::GetGroundFaceNormalAt(const C3DVector&) @ 0x0063AB10 (C3DVector by value).
// Layout: CPhysicsCollideResponse (PDB): CollidePos +0x00, CollideNormal +0x0c, Friction +0x18 (not touched).
// Shape: the ground height is a named float local (retail keeps it in the dead position-argument slot); `!(z <= ground)`-style
//   compare as `ground > position.Z`; the normal is copied word by word from the by-value temporary.
#include "engine/C3DVector.h"
class CPhysicsCollideResponse { public:
    C3DVector CollidePos;                       // +0x00
    C3DVector CollideNormal;                    // +0x0c
    float     Friction;                         // +0x18
};
class CWorldMap { public:
public:
    C3DVector GetGroundFaceNormalAt(const C3DVector&) const;   // 0x0063AB10
public:
    float GetGroundSizeZAt(const C3DVector&) const;   // 0x0063A300
};
class CTCPhysicsBase { public:
    virtual void v00();
    void* ParentThing;                          // +0x04
protected:
    const CWorldMap& GetWorldMap() const;               // 0x006AF630 Ego_r.pdb ?GetWorldMap@CTCPhysicsBase@@IBEABVCWorldMap@@XZ (was GetWorldMap_006af630)
public:
    virtual bool HasCollidedWithLandscapeAt(const C3DVector& position, CPhysicsCollideResponse& response) const;   // Ego_r.pdb UBE (slot 92)
};

bool CTCPhysicsBase::HasCollidedWithLandscapeAt(const C3DVector& position, CPhysicsCollideResponse& response) const
{
    float ground = GetWorldMap().GetGroundSizeZAt(position);
    if (ground > position.Z) {
        response.CollideNormal = GetWorldMap().GetGroundFaceNormalAt(position);
        response.CollidePos.X = position.X;
        response.CollidePos.Y = position.Y;
        response.CollidePos.Z = ground;
        return true;
    }
    return false;
}