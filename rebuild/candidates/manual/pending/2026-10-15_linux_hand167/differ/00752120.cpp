// CTCLook::ChangeHeadAngleXY(float targetAngle, float time) -- drive the head's XY angle (relative to the body) towards the target through the slow-in/fast-out integrator, the turn speed depending on whether a target is acquired
// Leaf: ChangeHeadAngleXY
// Grade: DIFFER(91v112) -- ours folds K*0.5 into one constant and computes the halved XY range once (CSE) for both limits; retail multiplies by 1/360 then 0.5 separately, twice, and keeps the bool in dl
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00752120, length 112 bytes (`ret 8`).
// Label correction: catalog "CTCLook::ChangeHeadAngleYZ" is propagated; this body works on HeadAngleRelToBodyXY/HeadVelocityXY and CentreHead
// (0x007528A0) calls it first with the XY argument pair, then the real ChangeHeadAngleYZ @ 0x00752190.
// Callees: CTCLook::GetHeadSettingsSlowInFastOut(float*, float*, float, float, float, float, float) @ 0x00751820.
// Globals (.data float pairs {XY, YZ}): turn speed with a target 0x01382680, without 0x01382688, acceleration 0x01382690; 1/360
// (0.0027777778f) @ 0x01238E00 converts the def's degrees to turns; limits are the def's XY range (+0x30) halved for both limits.
// Layout (retail, local model): PDef +0x14, HeadAngleRelToBody XY/YZ +0x2C/+0x30, HeadVelocity XY/YZ +0x5C/+0x60 (PDB offsets),
// TargetAcquired +0xA0 (PDB +0xA4; the retail TurningAnims vector is 12 bytes).
// Shape: the speed select is a named float local taken first (it stays on the x87 stack); the call's arguments evaluate right to left.
struct SLookAnglePair { float XY; float YZ; };
extern SLookAnglePair g_HeadTurnSpeedTargetAcquired;   // 0x01382680 (.data tunables)
extern SLookAnglePair g_HeadTurnSpeedNoTarget;         // 0x01382688
extern SLookAnglePair g_HeadTurnAcceleration;          // 0x01382690
struct CLookDef {
    unsigned char _pad00[0x30];
    float MaxHeadAngleXY;                       // +0x30 (degrees)
    float MaxHeadAngleYZUp;                     // +0x34 (degrees)
    float MaxHeadAngleYZDown;                   // +0x38 (degrees)
};
template<class T> struct CDefPointer {
    T* Object;
    T* operator->() const { return Object; }
};
struct CThingCreatureBase;
struct CTCLook {
    virtual void v00();
    void* ParentThing;                          // +0x04 (CTCBase)
    unsigned char _base08[0x10 - 0x08];
    CThingCreatureBase* PParentCreature;        // +0x10
    CDefPointer<const CLookDef> PDef;           // +0x14
    void* PAppearanceDef;                       // +0x18
    float RecoveryAngleToLookAtXY;              // +0x1c
    float RecoveryAngleToLookAtYZ;              // +0x20
    float BodyAngleXY;                          // +0x24
    float BodyAngleYZ;                          // +0x28
    float HeadAngleRelToBodyXY;                 // +0x2c
    float HeadAngleRelToBodyYZ;                 // +0x30
    float EyeAnglesRelToHeadXY;                 // +0x34
    float EyeAnglesRelToHeadYZ;                 // +0x38
    long  LookTimer;                            // +0x3c
    bool  UsingOldStyleTurning;                 // +0x40
    bool  TurningViaAnimationDisable;           // +0x41
    float FractionOfTurningToAcceptEachFrame;   // +0x44
    long  HeadTurnState;                        // +0x48
    long  BodyTurnState;                        // +0x4c
    bool  CanTurnBody;                          // +0x50
    float BodyVelocityXY;                       // +0x54
    float BodyVelocityYZ;                       // +0x58
    float HeadVelocityXY;                       // +0x5c
    float HeadVelocityYZ;                       // +0x60
    long  LookFlags;                            // +0x64
    unsigned char _pad68[0xa0 - 0x68];          // PosToLookAt .. TurningViaAnimMinAngle, PCurrentTurningAnim (retail vector 12 bytes)
    bool  TargetAcquired;                       // +0xa0 (PDB +0xa4)
    unsigned char _padA1[0xb4 - 0xa1];          // CachedHeadPos, HeadCacheTimer
    long  TimeToStopLooking;                    // +0xb4 (PDB +0xb8)
    bool  OneFrameDelay;                        // +0xb8 (PDB +0xbc)
    void GetHeadSettingsSlowInFastOut(float* angle, float* velocity, float acceleration, float maxSpeed, float target, float minAngle, float maxAngle);   // 0x00751820
    void ChangeHeadAngleXY(float targetAngle, float time);
};

void CTCLook::ChangeHeadAngleXY(float targetAngle, float time)
{
    float speed = TargetAcquired ? g_HeadTurnSpeedTargetAcquired.XY : g_HeadTurnSpeedNoTarget.XY;
    GetHeadSettingsSlowInFastOut(&HeadAngleRelToBodyXY, &HeadVelocityXY, g_HeadTurnAcceleration.XY * time, speed * time, targetAngle,
                                 PDef->MaxHeadAngleXY * 0.0027777778f * 0.5f,
                                 PDef->MaxHeadAngleXY * 0.0027777778f * 0.5f);
}
