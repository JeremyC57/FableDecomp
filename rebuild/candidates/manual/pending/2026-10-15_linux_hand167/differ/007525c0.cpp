// CTCLook::GetTurningAnglesAndCheckForTurningBody(float* angleYZ, float* angleXY) -- compute the turning angles; when they exist, report whether the body must turn: twice the XY head angle exceeds the threshold (the def's XY head range, or 1e-4 for the "turn body at once" look flags)
// Grade: DIFFER(115v115) -- the negation tests the long helper result as `test eax,eax` where retail tests `al` after a full-register `mov eax,1 / xor eax,eax`
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007525C0, length 115 bytes (`ret 8`).
// Callees: CTCLook::GetTurningAngles(float*, float*) @ 0x00751E80 (both pointers forwarded).
// Layout (retail, local model): PDef +0x14 (CLookDef MaxHeadAngleXY +0x30, degrees; 1/360 @ 0x01238E00), LookFlags +0x64
// (bit 0 with neither bit 1 nor 2, or bit 7, selects the 1e-4 @ 0x0129BA3C threshold).
// Shape: LookFlags read once into eax and tested by byte (`test al,1 / test al,6 / test al,al ; js`); the threshold is computed
// before the angle (`fld [edi] ; fabs ; fadd st,st ; fcomp st(1)`) with the def arm as the fall-through; the in-range test is an
// inline long 1/0 helper (retail `mov eax,1 / xor eax,eax`) and the result its negation (retail `test al,al ; sete dl`).
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
    static long IsWithinRange(float angle, float threshold)
    {
        if ((float)fabs(angle) * 2.0f <= threshold)
            return 1;
        return 0;
    }
    bool GetTurningAngles(float* angleYZ, float* angleXY);   // 0x00751E80
    bool GetTurningAnglesAndCheckForTurningBody(float* angleYZ, float* angleXY);
};

bool CTCLook::GetTurningAnglesAndCheckForTurningBody(float* angleYZ, float* angleXY)
{
    if (!GetTurningAngles(angleYZ, angleXY))
        return false;
    float threshold;
    long flags = LookFlags;
    if (!((flags & 1) && !(flags & 6)) && !(flags & 0x80))
        threshold = PDef->MaxHeadAngleXY * 0.0027777778f;
    else
        threshold = 1e-4f;
    return !(bool)IsWithinRange(*angleXY, threshold);
}
