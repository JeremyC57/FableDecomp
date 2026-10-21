// CTCPhysicsControlled::UpdateOnSurface(const vector<CThing*>& things, C3DVector& pos) -- snap the position onto the highest physics mesh under it among the nearby things (sweeping the body sphere down through each mesh) and onto the ground, then update OnMesh / OnGround, the thing stood on and the render Z offset
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00725290, length 481 bytes (+ int3 padding), esp frame, `ret 8`.
// Callees: CTCPhysicsBase virtual slots 57 (+0xe4) / 58 (+0xe8) (two surface capability tests; names descriptive), 90 (+0x168,
// the sphere-through-mesh sweep (other, from, to, response)) and 93 (+0x174, GetPhysicsMeshInfo); CTCPhysicsBase::
// ModifyResponseForNearVerticalSlopes @ 0x006B0610, GetWorldMap @ 0x006AF620 (label "CThingTrackNode::DrawIsDrawable" is
// propagated: it is `GetWorld()->slot 12`), CWorldMap::GetGroundSizeZAt @ 0x0063A300, SetThingStandingOn @ 0x006AF6D0.
// Constants: 0.5f @ 0x0122F59C (and the 0.5f immediate), 1e-4f g_epsilon @ 0x0129BA3C, 0.199f @ 0x012662FC, 0.05f @
// 0x0125A15C, -0.055f / 0.0f immediates.
// Shape: the radius comes through an inline by-value getter (dword copy into the dead `pos` argument slot); the sweep end
// points are `pos + down` with a named `C3DVector down(0, 0, -radius)` (only its Z is kept, hence the 3-word frame hole) and
// `pos + C3DVector(0, 0, radius)`: VC7.1 folds the `+ 0.0f` X/Y terms, so pos.X/pos.Y are loaded once and stored twice; the
// collide response's ctor sets Friction = 1.0f once before the loop; OnGround is a bool bit-field assignment
// (shl/xor/and/xor).
// Layout: Ego_r.pdb CTCPhysicsBase Radius +0x34; CTCPhysicsStandard bit byte +0x84 (OnGround bit 4, OnMesh bit 5);
// CTCPhysicsControlled RenderZOffset +0xa4, LastRenderZOffset +0xa8; CThing PhysicsTC +0x60; CPhysicsCollideResponse
// {CollidePos, CollideNormal, Friction}.
#include <math.h>
#pragma intrinsic(fabs)

struct C3DVector {
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) { }
    C3DVector operator+(const C3DVector& o) const { return C3DVector(X + o.X, Y + o.Y, Z + o.Z); }
};
struct CPhysicsCollideResponse {
    C3DVector CollidePos;                               // +0x00
    C3DVector CollideNormal;                            // +0x0c
    float     Friction;                                 // +0x18
    CPhysicsCollideResponse() : Friction(1.0f) { }
};
struct CThingPhysicsMeshInfo;
struct CWorldMap {
    float GetGroundSizeZAt(const C3DVector& pos);       // 0x0063A300
};
struct CTCPhysicsBase;
class CThing {
public:
    unsigned char   _pad00[0x60];
    CTCPhysicsBase* PhysicsTC;                          // +0x60
};
template <class T> struct vector {                      // STLport vector, 12 bytes
    T* _M_start; T* _M_finish; T* _M_end_of_storage;
    T* begin() const { return _M_start; }
    T* end() const { return _M_finish; }
};
extern float g_epsilon_0129ba3c;                        // 0x0129BA3C (1e-4f)

#define FILL10(n) virtual void v##n##0(); virtual void v##n##1(); virtual void v##n##2(); virtual void v##n##3(); \
    virtual void v##n##4(); virtual void v##n##5(); virtual void v##n##6(); virtual void v##n##7(); \
    virtual void v##n##8(); virtual void v##n##9();
struct CTCPhysicsBase {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09();
    FILL10(1) FILL10(2) FILL10(3) FILL10(4)
    virtual void v50(); virtual void v51(); virtual void v52(); virtual void v53(); virtual void v54();
    virtual void v55(); virtual void v56();
    virtual bool IsSolidSurface();                      // slot 57 (+0xe4) (name descriptive)
    virtual bool IsSurfaceFollower();                   // slot 58 (+0xe8) (name descriptive)
    virtual void v59();
    FILL10(6) FILL10(7) FILL10(8)
    virtual bool HasSphereMeshCollidedFromTo(const CTCPhysicsBase& other, const C3DVector& from, const C3DVector& to,
                                             CPhysicsCollideResponse& response);   // slot 90 (+0x168)
    virtual void v91(); virtual void v92();
    virtual const CThingPhysicsMeshInfo* GetPhysicsMeshInfo();   // slot 93 (+0x174)
    unsigned char _pad04[0x0c - 0x04];
    C3DVector     Position;                             // +0x0c
    C3DVector     OldPosition;                          // +0x18
    long          OldPositionLastFrameSet;              // +0x24
    C3DVector     Velocity;                             // +0x28
    float         Radius;                               // +0x34
    float GetRadius() const { return Radius; }
    CWorldMap* GetWorldMap();                           // 0x006AF620
    void ModifyResponseForNearVerticalSlopes(CPhysicsCollideResponse* response);   // 0x006B0610
    void SetThingStandingOn(CTCPhysicsBase* other);     // 0x006AF6D0
};
struct CTCPhysicsStandard : CTCPhysicsBase {
    unsigned char _pad38[0x84 - 0x38];
    bool AlwaysFacingMovementDirection : 1;             // +0x84 bit 0
    bool AngledWithGround : 1;                          // bit 1
    bool FeelsGravity : 1;                              // bit 2
    bool FeelsAirResistance : 1;                        // bit 3
    bool OnGround : 1;                                  // bit 4
    bool OnMesh : 1;                                    // bit 5
    bool InteractsWithThings : 1;                       // bit 6
    bool CollidesWithThings : 1;                        // bit 7
    unsigned char _pad85[3];
};
struct CTCPhysicsControlled : CTCPhysicsStandard {
    C3DVector ImpulseVelocity;                          // +0x88
    C3DVector ForcedMoveToPos;                          // +0x94
    bool      ForcedMoveToPosSet : 1;                   // +0xa0 bit 0
    bool      ForcedMoveToPosUsePhysics : 1;            // +0xa0 bit 1
    float     RenderZOffset;                            // +0xa4
    float     LastRenderZOffset;                        // +0xa8
    void UpdateOnSurface(const vector<CThing*>& things, C3DVector& pos);
};

void CTCPhysicsControlled::UpdateOnSurface(const vector<CThing*>& things, C3DVector& pos)
{
    LastRenderZOffset = RenderZOffset;
    CTCPhysicsBase* standingOn = 0;
    if (IsSolidSurface() || IsSurfaceFollower()) {
        float radius = GetRadius();
        OnMesh = false;
        if (radius < 0.5f)
            radius = 0.5f;
        C3DVector down(0.0f, 0.0f, -radius);
        CPhysicsCollideResponse response;
        for (CThing** it = things.begin(); it != things.end(); ++it) {
            CTCPhysicsBase* other = (*it)->PhysicsTC;
            if (other->IsSolidSurface() && other->GetPhysicsMeshInfo()) {
                C3DVector to = pos + down;
                C3DVector from = pos + C3DVector(0.0f, 0.0f, radius);
                if (HasSphereMeshCollidedFromTo(*other, from, to, response)) {
                    ModifyResponseForNearVerticalSlopes(&response);
                    if ((float)fabs(response.CollideNormal.Z) > g_epsilon_0129ba3c) {
                        if (response.CollidePos.Z > pos.Z)
                            pos.Z = response.CollidePos.Z - 0.199f;
                        OnMesh = true;
                        standingOn = other;
                    }
                }
            }
        }
    }
    float groundZ = GetWorldMap()->GetGroundSizeZAt(pos);
    if (groundZ > pos.Z)
        pos.Z = groundZ;
    OnGround = (float)fabs(pos.Z - groundZ) < 0.05f;
    SetThingStandingOn(standingOn);
    if (OnMesh)
        RenderZOffset = -0.055f;
    else
        RenderZOffset = 0.0f;
}
