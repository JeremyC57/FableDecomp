// Grade: DIFFER(874v917) -- x87 scheduling and frame: retail keeps the helper-minus-position components on the x87 stack through the inline fast normalise (exponent ((0x17c-e)<<22)&0xff800000, r spilled to [esp+4]) and computes the level test as (ny+nx)*0.0f+nz from the spilled nx; its helper-name calls test each result in sequence with the graphic TC reloaded through eax, and it keeps the obstacle loop's dead 'clear' byte; ours schedules the normalise products differently, drops the dead flag and has a 0xb8 frame; the flow (three helper lookups, level window, world-map ground clamp, obstacle mesh probe with the 0.007f lift, AngleYZBetweenPositions, the +-0.5 wrap, angle window, axis-side rotate, creature re-placement flag) is modelled
// CThingBodyReorienter::ReorientBodyYZForSlopes() -- tilt the body to follow the slope: take a foot helper point (or
// dummy), and when it is roughly level with the body's position, probe the ground (and the nearby obstacles' meshes)
// below it, measure the YZ angle from the body to the probed point and, when it is small, rotate the body by it
// towards the side the point lies on; flags a near-stationary creature for re-placement
// Leaf: ?ReorientBodyYZForSlopes@CThingBodyReorienter
// Retail: Fable.exe (TLC, VC7.1) @ 0x008633B0, plain `ret`.
// Callees: CTCGraphicAppearance::GetHelperPoint 0x004C0560 / GetDummyObject 0x004C08C0, CThing::GetPosition 0x004C73D0,
//   the world's virtual +0x30 (world map) and CWorldMap::GetGroundSizeZAt 0x0063A300, CBaseIntelligentPointer::GetPItem
//   0x00A01B50, CTCPhysicsBase::GetPhysicsMeshObject 0x006B0850, CPhysicsMeshObject::GetPointIntersectionFromTo
//   0x00A12450 / ~CPhysicsMeshObject 0x00A127D0, AngleYZBetweenPositions 0x00754E70 (__fastcall), the physics
//   component's virtuals +0x118 (an axis), +0x10c (rotate) and +0x100 (speed).
// Globals: enable flag 0x0137573D, helper names 0x013BAC5C / 0x013BAC54 / 0x013BAC48, the inverse-sqrt mantissa table
//   0x013CE558, 3.0f 0x0122DED4, 0.5f 0x0122F59C, 0.0f 0x0122DEDC, 1.0f 0x0122DED8, the level threshold 0x01385AE8,
//   0.007f 0x0125A164, the foot lift 0x01265DF0, the angle limits 0x01276DF4 / 0x01276DF0, the speed epsilon 0x0129BA3C,
//   the main game component 0x013B86A0 (+0x24 world).
// Layout: PDB CThingBodyReorienter has Body +0x28; retail has PossibleIntersectingThings +0x18 (12-byte STLport
//   vector) and Body +0x24. Retail CThing: physics +0x60, graphic appearance +0x64, type flags +0x6c (bit 3 creature),
//   creature flag byte +0x15d.
#include <math.h>
struct C3DVector {
    float X, Y, Z;
    C3DVector() {}
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
};
struct CCharString { void* Data; };
struct CRightHandedSet { C3DVector Up, Forward; };
struct CMeshIntersectionData { unsigned char _pad[8]; float Z; unsigned char _rest[0x10]; };
struct CPhysicsMeshObject {
    unsigned char _body[0x40];
    ~CPhysicsMeshObject();                                             // 0x00A127D0
    bool GetPointIntersectionFromTo(const C3DVector& from, const C3DVector& to, CMeshIntersectionData* out); // 0x00A12450
};
struct CTCPhysicsBase {
#define VF(n) virtual void v##n();
#define VF10(p) VF(p##0) VF(p##1) VF(p##2) VF(p##3) VF(p##4) VF(p##5) VF(p##6) VF(p##7) VF(p##8) VF(p##9)
    VF10(0) VF10(1) VF10(2) VF10(3) VF10(4) VF10(5) VF(60) VF(61) VF(62) VF(63)
    virtual float GetSpeed();                                          // slot 64 (+0x100)
    VF(65) VF(66)
    virtual void RotateYZ(float angle);                                // slot 67 (+0x10c)
    VF(68) VF(69)
    virtual C3DVector GetRightAxis();                                  // slot 70 (+0x118)
    CPhysicsMeshObject GetPhysicsMeshObject();                         // 0x006B0850
};
struct CTCGraphicAppearance {
    bool GetHelperPoint(const CCharString& name, long index, unsigned long flags, C3DVector* position, float time); // 0x004C0560
    bool GetDummyObject(const CCharString& name, long index, unsigned long flags, C3DVector* pos, CRightHandedSet* set,
                        float scale);                                  // 0x004C08C0
};
struct CThing {
    unsigned char         _pad00[0x60];
    CTCPhysicsBase*       PhysicsTC;                                   // +0x60
    CTCGraphicAppearance* GraphicTC;                                   // +0x64
    unsigned char         _pad68[4];
    unsigned long         ThingTypeFlags;                              // +0x6c
    unsigned char         _pad70[0x15d - 0x70];
    unsigned char         CreatureFlags;                               // +0x15d
    const C3DVector& GetPosition() const;                              // 0x004C73D0
};
struct CBaseIntelligentPointer {
    void* _p[2];
    CThing* GetPItem() const;                                          // 0x00A01B50
};
struct CWorldMap { float GetGroundSizeZAt(const C3DVector& pos) const; };   // 0x0063A300
struct CWorld {
    VF10(0) VF(10) VF(11)
    virtual CWorldMap* GetWorldMap();                                  // slot 12 (+0x30)
};
struct CMainGameComponent { unsigned char _pad[0x24]; CWorld* World; };
extern CMainGameComponent* g_pMainGameComponent;                      // 0x013B86A0
extern bool g_ReorientBodiesForSlopes;                                 // 0x0137573D
extern CCharString g_FootHelperName;                                   // 0x013BAC5C
extern CCharString g_EyeSetName;                                       // 0x013BAC54
extern CCharString g_FocusEyeName;                                     // 0x013BAC48
extern unsigned long g_InvSqrtMantissaTable[128];                      // 0x013CE558
extern const float g_SlopeLevelThreshold;                              // 0x01385AE8
extern const float g_SlopeFootLift;                                    // 0x01265DF0
extern const float g_SlopeMaxAngle;                                    // 0x01276DF4
extern const float g_SlopeMinAngle;                                    // 0x01276DF0
extern const float g_SlopeSpeedEpsilon;                                // 0x0129BA3C
float __fastcall AngleYZBetweenPositions(const C3DVector& from, const C3DVector& to);   // 0x00754E70

struct CThingBodyReorienter {
    bool  BodyReorientationDone;                                       // +0x00
    bool  ReorientingBody;                                             // +0x01
    long  ReorientationCount;                                          // +0x04
    bool  StillCheckingForNewBodyReorientationAngle;                   // +0x08
    bool  HaveFinalReorientationAngle;                                 // +0x09
    float FinalReorientationAngleXY;                                   // +0x0c
    long  RotationDirection;                                           // +0x10
    bool  HasPossibleIntersectingThings;                               // +0x14
    CBaseIntelligentPointer* ThingsBegin;                              // +0x18
    CBaseIntelligentPointer* ThingsEnd;                                // +0x1c
    CBaseIntelligentPointer* ThingsCap;                                // +0x20
    CThing* Body;                                                      // +0x24
    void ReorientBodyYZForSlopes();
};

void CThingBodyReorienter::ReorientBodyYZForSlopes()
{
    if (!g_ReorientBodiesForSlopes)
        return;
    C3DVector helper;
    CRightHandedSet set;
    if (!Body->GraphicTC->GetHelperPoint(g_FootHelperName, 0, 0, &helper, 1.0f)
        && !Body->GraphicTC->GetDummyObject(g_EyeSetName, 0, 0, &helper, &set, 1.0f)
        && !Body->GraphicTC->GetHelperPoint(g_FocusEyeName, 0, 0, &helper, 1.0f))
        return;
    C3DVector pos = Body->GetPosition();
    float dx = helper.X - pos.X;
    float dy = helper.Y - pos.Y;
    float dz = helper.Z - pos.Z;
    float lengthSquared = dz * dz + dy * dy + dx * dx;
    unsigned long bits = *(unsigned long*)&lengthSquared;
    unsigned long estimate = (((0x17c - ((bits >> 23) & 0xff)) << 22) & 0xff800000) |
                             (g_InvSqrtMantissaTable[(bits >> 17) & 0x7f] << 15);
    float r = *(float*)&estimate;
    r = (3.0f - r * r * lengthSquared) * r * 0.5f;
    float nx = dx * r;
    float ny = dy * r;
    float nz = dz * r;
    float level = (ny + nx) * 0.0f + nz;
    if (!(level < g_SlopeLevelThreshold && level > -g_SlopeLevelThreshold))
        return;
    C3DVector from(helper.X, helper.Y, helper.Z + 3.0f);
    float ground = g_pMainGameComponent->World->GetWorldMap()->GetGroundSizeZAt(from);
    C3DVector foot(helper.X, helper.Y, helper.Z - 3.0f);
    if (foot.Z < ground)
        foot.Z = ground;
    bool clear = true;
    for (CBaseIntelligentPointer* it = ThingsBegin; it != ThingsEnd; ++it) {
        CThing* thing = it->GetPItem();
        if (thing) {
            CPhysicsMeshObject mesh = thing->PhysicsTC->GetPhysicsMeshObject();
            CMeshIntersectionData hit;
            if (mesh.GetPointIntersectionFromTo(from, foot, &hit)) {
                float z = hit.Z + 0.007f;
                if (z > foot.Z) {
                    foot.Z = z;
                    clear = false;
                }
            }
        }
    }
    foot.Z += g_SlopeFootLift;
    float angle = AngleYZBetweenPositions(pos, foot);
    if (angle > 0.5f)
        angle -= 1.0f;
    if ((angle > 0.0f && angle < g_SlopeMaxAngle) || (angle < 0.0f && angle > g_SlopeMinAngle)) {
        C3DVector axis = Body->PhysicsTC->GetRightAxis();
        float side = (foot.Z - pos.Z) * axis.Z + (foot.Y - pos.Y) * axis.Y + (foot.X - pos.X) * axis.X;
        if (side > 0.0f)
            Body->PhysicsTC->RotateYZ(angle);
        else
            Body->PhysicsTC->RotateYZ(-angle);
        CThing* body = Body;
        if ((body->ThingTypeFlags & 8) && fabs(body->PhysicsTC->GetSpeed()) > g_SlopeSpeedEpsilon)
            body->CreatureFlags |= 0x20;
    }
}
