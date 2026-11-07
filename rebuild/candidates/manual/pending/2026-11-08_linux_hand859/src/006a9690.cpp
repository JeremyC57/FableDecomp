// CThingPlayerCreature::ResolveRelativeMovementAcceleration() -- turn the frame's requested relative acceleration into a world velocity change: normalise it (kept as the direction), rotate it by the control angle (the render camera's angle in spell mode, else the current game camera's control angle), scale it by the maximum speed; a blocking creature with a pending turn snaps its facing and stops; a non-negligible acceleration is added to the velocity and the accelerating flag set; the request is cleared
// Leaf: ?ResolveRelativeMovementAcceleration@CThingPlayerCreature@@IAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006A9690, 450 bytes, thiscall `ret`. Frame 0x6C + ebx/esi/edi.
// Identity: Ego_r.pdb ?ResolveRelativeMovementAcceleration@CThingPlayerCreature@@IAEXXZ (protected). The Ego_r CThingPlayerCreature
//   layout (560 bytes) is not the retail one (the retail members here lie past 0x178), so the member names are descriptive.
// Callees: C3DVector::AccurateNormalise @ 0x00A14440 (?AccurateNormalise@C3DVector@@QAEAAV1@XZ, landed), CThing::PeekWorld @ 0x004C7AB0
//   (Ego_r ?PeekWorld@CThing@@IBEABVCWorld@@XZ; 0x004C7AA0 is the byte-identical non-const GetWorld, naming wave 1 DECISIONS), CWorld::PeekMyPlayer @ 0x0049D9D0 (Ego_r ?PeekMyPlayer@CWorld@@QBEPBVCPlayer@@XZ),
//   CWorld slot 6 (+0x18) GetCurrentRenderCamera (Ego_r ?GetCurrentRenderCamera@CWorld@@UBE?AVCCamera@@XZ, a 0x38-byte CCamera by
//   hidden pointer), CCamera::GetAngleXY @ 0x00A0C380 (Ego_r ?GetAngleXY@CCamera@@QBEMXZ), CAIGameCameraBase slot 7 (+0x1c)
//   GetControlAngleXY (Ego_r ?GetControlAngleXY@CAIGameCameraBase@@UBEMXZ) on CWorld::PCurrentGameCamera (+0x34, Ego_r layout),
//   C3DVector::RotateXY @ 0x00A14040 (Ego_r ?RotateXY@C3DVector@@QAEXM@Z, landed), own virtual slot 87 (+0x15c, the maximum speed),
//   CThingCreatureBase::IsBlocking @ 0x00664D00 (Ego_r ?IsBlocking@CThingCreatureBase@@QBE_NXZ), the physics component's slots 63
//   (+0xfc, the facing angle) and 74 (+0x128, Ego_r ?SetRHSet@CTCPhysicsBase@@UAEXABVCRightHandedSet@@@Z), CRightHandedSet(const
//   C3DVector&) @ 0x00A229E0. Global: DELTA_NOTIONAL_ZERO @ 0x0129BA3C (engine/C3DVector.h). CPlayer +0x220 = SpellMode (Ego_r layout).
// Shape (chunk 859): the control angle is a float local assigned in both arms of the camera test and handed to RotateXY through a
//   `float&` reference. With a joined float local pushed directly VC7.1 materialises RotateXY's `this` (`lea ecx,[esp+10h]`) before the
//   local's reload and push; retail has the lea after the push (`lea ecx,[esp+14h]`). The reference binding (GOTCHAS reference lever)
//   gives the retail order (micro-probe: a single-call result pushes first too, a ternary / `+ 0.0f` / a second named copy do not);
//   170 earlier spellings and flag sets kept the early lea (chunks 757/840/844/847). The flag clear through a named `unsigned char&`
//   gives the read-modify-write `and byte ptr [esi+178h],0FEh`.
#define FABLE_RETAIL_C3DVECTOR
#include "engine/C3DVector.h"
class CRightHandedSet {
public:
    C3DVector Up;
    C3DVector Forward;
    CRightHandedSet(const C3DVector& eulerAngles);   // 0x00A229E0
};
class CCamera {                                 // 0x38 bytes
public:
    unsigned char _body[0x38];
    float GetAngleXY() const;                   // 0x00A0C380
};
class CAIGameCameraBase {
public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06();
    virtual float GetControlAngleXY() const;    // slot 7 (+0x1c)
};
class CPlayer {
public:
    unsigned char _pad00[0x220];
    bool SpellMode;                             // +0x220
};
class CWorld {
public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual CCamera GetCurrentRenderCamera() const;   // slot 6 (+0x18)
    unsigned char _pad04[0x34 - 0x04];
    CAIGameCameraBase* PCurrentGameCamera;      // +0x34
    const CPlayer* PeekMyPlayer() const;        // 0x0049D9D0
};
#define FILL10(n) virtual void v##n##0(); virtual void v##n##1(); virtual void v##n##2(); virtual void v##n##3(); \
    virtual void v##n##4(); virtual void v##n##5(); virtual void v##n##6(); virtual void v##n##7(); \
    virtual void v##n##8(); virtual void v##n##9();
class CTCPhysicsBase {
public:
    FILL10(0) FILL10(1) FILL10(2) FILL10(3) FILL10(4) FILL10(5)
    virtual void v60(); virtual void v61(); virtual void v62();
    virtual float GetFacingAngleXY() const;     // slot 63 (+0xfc)
    virtual void v64(); virtual void v65(); virtual void v66(); virtual void v67(); virtual void v68(); virtual void v69();
    virtual void v70(); virtual void v71(); virtual void v72(); virtual void v73();
    virtual void SetRHSet(const CRightHandedSet& set);   // slot 74 (+0x128)
};
class CThing {
public:
    FILL10(0) FILL10(1) FILL10(2) FILL10(3) FILL10(4) FILL10(5) FILL10(6) FILL10(7)
    virtual void v80(); virtual void v81(); virtual void v82(); virtual void v83(); virtual void v84(); virtual void v85();
    virtual void v86();
    unsigned char _pad04[0x60 - 0x04];
    CTCPhysicsBase* PhysicsTC;                  // +0x60
protected:
    const CWorld& PeekWorld() const;            // 0x004C7AB0
};
class CThingCreatureBase : public CThing {
public:
    bool IsBlocking() const;                    // 0x00664D00
};
class CThingPlayerCreature : public CThingCreatureBase {
public:
    virtual float GetMaxSpeed();                // slot 87 (+0x15c)
    unsigned char _pad64[0x178 - 0x64];
    unsigned char Flags178;                     // +0x178 (bit 0 accelerating)
    unsigned char _pad179[0x180 - 0x179];
    C3DVector Velocity;                         // +0x180
    unsigned char _pad18c[0x1b4 - 0x18c];
    C3DVector RelativeAcceleration;             // +0x1b4
    C3DVector RelativeAccelerationDirection;    // +0x1c0
    unsigned char _pad1cc[0x1fc - 0x1cc];
    float PendingTurn;                          // +0x1fc
protected:
    void ResolveRelativeMovementAcceleration();
};
void CThingPlayerCreature::ResolveRelativeMovementAcceleration()
{
    unsigned char& flags = Flags178;
    flags &= ~1;
    C3DVector acceleration(RelativeAcceleration.X, RelativeAcceleration.Y, RelativeAcceleration.Z);
    acceleration.AccurateNormalise();
    RelativeAccelerationDirection = acceleration;
    const CWorld& world = PeekWorld();
    float angle;
    if (world.PeekMyPlayer()->SpellMode)
        angle = world.GetCurrentRenderCamera().GetAngleXY();
    else
        angle = world.PCurrentGameCamera->GetControlAngleXY();
    float& turn = angle;
    acceleration.RotateXY(turn);
    acceleration *= GetMaxSpeed();
    if (IsBlocking()) {
        if (fabs(PendingTurn) > DELTA_NOTIONAL_ZERO) {
            PendingTurn = 0.0f;
            C3DVector angles(PhysicsTC->GetFacingAngleXY(), 0.0f, 0.0f);
            CRightHandedSet set(angles);
            PhysicsTC->SetRHSet(set);
        }
        Velocity.Z = 0.0f;
        Velocity.Y = 0.0f;
        Velocity.X = 0.0f;
    }
    if (acceleration.GetSquaredMagnitude() > DELTA_NOTIONAL_ZERO * DELTA_NOTIONAL_ZERO) {
        Velocity += acceleration;
        Flags178 |= 1;
    }
    RelativeAcceleration.Z = 0.0f;
    RelativeAcceleration.Y = 0.0f;
    RelativeAcceleration.X = 0.0f;
}
