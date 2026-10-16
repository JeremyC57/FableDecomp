// CTCLook::CentreHead() -- drop the target; once the head is centred (both relative angles within 1e-4) set head turn state 9 and stop the head, otherwise keep turning it back towards 0 on both axes
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007528A0, length 124 bytes.
// Callees: CTCLook::ChangeHeadAngleXY(float, float) @ 0x00752120 and CTCLook::ChangeHeadAngleYZ(float, float) @ 0x00752190 (both
// catalogued "ChangeHeadAngleYZ"; the first works on the XY members), both with (0, 1.0); the centred arm is SetHeadTurnState's work written out
// with the -1 store after the change test (an inlined SetHeadTurnState schedules it before the stores).
// Layout (retail, local model; see landed SetHeadTurnState 0x007517B0): HeadAngleRelToBody XY/YZ +0x2C/+0x30, HeadTurnState +0x48,
// HeadVelocity XY/YZ +0x5C/+0x60, TargetAcquired +0xA0, TimeToStopLooking +0xB4, OneFrameDelay +0xB8 (4 below the PDB past the
// 12-byte TurningAnims vector). 1e-4f @ 0x0129BA3C.
// Shape: `(float)fabs(x) < 1e-4f` gives `test ah,5 ; jp`;
// the velocity zeros are float immediates.
#include <math.h>
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
    void ChangeHeadAngleXY(float targetAngle, float time);   // 0x00752120
    void ChangeHeadAngleYZ(float targetAngle, float time);   // 0x00752190
    void CentreHead();
};

void CTCLook::CentreHead()
{
    TargetAcquired = false;
    if ((float)fabs(HeadAngleRelToBodyXY) < 1e-4f && (float)fabs(HeadAngleRelToBodyYZ) < 1e-4f) {
        if (HeadTurnState != 9) {
            HeadTurnState = 9;
            OneFrameDelay = true;
        }
        TimeToStopLooking = -1;
        HeadVelocityXY = 0.0f;
        HeadVelocityYZ = 0.0f;
        return;
    }
    ChangeHeadAngleXY(0.0f, 1.0f);
    ChangeHeadAngleYZ(0.0f, 1.0f);
}
