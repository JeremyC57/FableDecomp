// Grade: DIFFER(768v757) -- LTCG lane gap: retail loads PDef into edx once at entry and keeps using it after the call to IsWithinYZHeadRange 0x00751FC0 (that callee touches only eax/ecx, which only LTCG can know); the lane must reload the def after the call (`mov ecx,[esi+14h]` x4). Also open: retail hoists `fabs(angleXY)` above the includeEyes branch (try an explicit abs local), tests the callee's bool in al, and multiplies `range * sign` by fimul. Tried: the first full model (this file); parked after one grade as an LTCG gap.
// CTCLook::SetHeadLooking(float speed, bool includeEyes, bool turnBody, float angleXY, float angleYZ) -- pick the head's target angles for a look (the eyes take up what lies inside their range), flag a body turn or centre the head when the target is out of reach, then turn the head or settle it
// Leaf: ?SetHeadLooking@CTCLook@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00752B00, 757 bytes, __thiscall, `ret 14h`.
// PDB: ?SetHeadLooking@CTCLook@@AAEXM_N0MM@Z (float, bool, bool, float, float); catalog "CTCLook_UpdateHeadDirection" and the
//   landed caller FrameUpdateHeadLookAtPos 0x007530D0 (call at 0x00753157) name it UpdateHeadDirection -- rename at landing.
// Callees: CTCLook::IsWithinYZHeadRange 0x00751FC0, CTCLook::CentreHead 0x007528A0, AngleDifference 0x00A441E0 (x2),
//   CTCLook::ChangeHeadAngleXY 0x00752120 (catalogued "ChangeHeadAngleYZ"), CTCLook::ChangeHeadAngleYZ 0x00752190.
// Layout: as the landed CentreHead 0x007528A0 (retail CTCLook, 4 below the PDB past the 12-byte TurningAnims vector);
//   CLookDef EyesTurnRangeXY/YZ +0x28/+0x2C, HeadTurnRangeXY +0x30, HeadTurnRangeYZUp/Down +0x34/+0x38 (degrees, x 1/360).
// Shape: with the eyes, the eye range absorbs what it can (target = angle - sign * eyeRange / 2), beyond the head range
//   flag a body turn / the YZ limit; without, the head takes the angle when in range. A body turn without turnBody
//   centres the head unless |angleXY| < 0.55 range and the head is already past 0.45 range; an out-of-range YZ clamps to
//   up or -down; then both axes turn by AngleDifference (2|d| <= 1e-4 counts as there), the settled arm writing out
//   SetHeadTurnState(state | 8) with the -1 store after the change test and zeroing the head velocities.
#include <math.h>

float AngleDifference(float a, float b);           // 0x00A441E0

struct CLookDef {                                   // Ego_r.pdb
    unsigned char _pad00[0x28];
    float EyesTurnRangeXY;                          // +0x28 (degrees)
    float EyesTurnRangeYZ;                          // +0x2c
    float HeadTurnRangeXY;                          // +0x30
    float HeadTurnRangeYZUp;                        // +0x34
    float HeadTurnRangeYZDown;                      // +0x38
};
template<class T> struct CDefPointer {
    T* Object;
    T* operator->() const { return Object; }
};
struct CThingCreatureBase;

inline bool IsAngleWithinRange(float angle, float range) { return fabs(angle) * 2.0f <= range; }
inline long GetSign(float value) { return value < 0.0f ? -1 : 1; }

struct CTCLook {
    virtual void v00();
    void* ParentThing;                              // +0x04 (CTCBase)
    unsigned char _base08[0x10 - 0x08];
    CThingCreatureBase* PParentCreature;            // +0x10
    CDefPointer<const CLookDef> PDef;               // +0x14
    void* PAppearanceDef;                           // +0x18
    float RecoveryAngleToLookAtXY;                  // +0x1c
    float RecoveryAngleToLookAtYZ;                  // +0x20
    float BodyAngleXY;                              // +0x24
    float BodyAngleYZ;                              // +0x28
    float HeadAngleRelToBodyXY;                     // +0x2c
    float HeadAngleRelToBodyYZ;                     // +0x30
    float EyeAnglesRelToHeadXY;                     // +0x34
    float EyeAnglesRelToHeadYZ;                     // +0x38
    long  LookTimer;                                // +0x3c
    bool  UsingOldStyleTurning;                     // +0x40
    bool  TurningViaAnimationDisable;               // +0x41
    float FractionOfTurningToAcceptEachFrame;       // +0x44
    long  HeadTurnState;                            // +0x48
    long  BodyTurnState;                            // +0x4c
    bool  CanTurnBody;                              // +0x50
    float BodyVelocityXY;                           // +0x54
    float BodyVelocityYZ;                           // +0x58
    float HeadVelocityXY;                           // +0x5c
    float HeadVelocityYZ;                           // +0x60
    long  LookFlags;                                // +0x64
    unsigned char _pad68[0xa0 - 0x68];
    bool  TargetAcquired;                           // +0xa0 (PDB +0xa4)
    unsigned char _padA1[0xb4 - 0xa1];
    long  TimeToStopLooking;                        // +0xb4 (PDB +0xb8)
    bool  OneFrameDelay;                            // +0xb8 (PDB +0xbc)

    float GetEyesTurnRangeXY() const { return PDef->EyesTurnRangeXY * 0.0027777778f; }
    float GetEyesTurnRangeYZ() const { return PDef->EyesTurnRangeYZ * 0.0027777778f; }
    float GetHeadTurnRangeXY() const { return PDef->HeadTurnRangeXY * 0.0027777778f; }
    float GetHeadTurnRangeYZUp() const { return PDef->HeadTurnRangeYZUp * 0.0027777778f; }
    float GetHeadTurnRangeYZDown() const { return PDef->HeadTurnRangeYZDown * 0.0027777778f; }
    long IsWithinYZHeadRange(float angleYZ);        // 0x00751FC0
    void CentreHead();                              // 0x007528A0
    void ChangeHeadAngleXY(float targetAngle, float time);   // 0x00752120
    void ChangeHeadAngleYZ(float targetAngle, float time);   // 0x00752190
    void SetHeadLooking(float speed, bool includeEyes, bool turnBody, float angleXY, float angleYZ);
};

void CTCLook::SetHeadLooking(float speed, bool includeEyes, bool turnBody, float angleXY, float angleYZ)
{
    bool bodyNeedsTurning = false;
    float targetXY = 0.0f;
    float targetYZ = 0.0f;
    bool headOutOfRange = false;
    if (includeEyes) {
        float eyesRangeXY = GetEyesTurnRangeXY();
        if (IsAngleWithinRange(angleXY, eyesRangeXY))
            targetXY = 0.0f;
        else if (IsAngleWithinRange(angleXY, GetHeadTurnRangeXY()))
            targetXY = angleXY - GetSign(angleXY) * eyesRangeXY * 0.5f;
        else
            bodyNeedsTurning = true;
        float eyesRangeYZ = GetEyesTurnRangeYZ();
        if (IsAngleWithinRange(angleYZ, eyesRangeYZ))
            targetYZ = 0.0f;
        else if (IsWithinYZHeadRange(angleYZ))
            targetYZ = angleYZ - GetSign(angleYZ) * eyesRangeYZ * 0.5f;
        else
            headOutOfRange = true;
    }
    else {
        if (IsAngleWithinRange(angleXY, GetHeadTurnRangeXY()))
            targetXY = angleXY;
        else
            bodyNeedsTurning = true;
        if (angleYZ <= GetHeadTurnRangeYZUp() && angleYZ >= -GetHeadTurnRangeYZDown())
            targetYZ = angleYZ;
        else
            headOutOfRange = true;
    }
    if (bodyNeedsTurning) {
        if (!turnBody) {
            float headRangeXY = GetHeadTurnRangeXY();
            if (!(fabs(angleXY) < 0.55f * headRangeXY && HeadAngleRelToBodyXY > 0.45f * headRangeXY)) {
                CentreHead();
                return;
            }
        }
        targetXY = GetHeadTurnRangeXY() * GetSign(angleXY) * 0.5f;
    }
    if (headOutOfRange)
        targetYZ = angleYZ > 0.0f ? GetHeadTurnRangeYZUp() : -GetHeadTurnRangeYZDown();
    bool headSettled = true;
    float differenceXY = AngleDifference(targetXY, HeadAngleRelToBodyXY);
    float differenceYZ = AngleDifference(targetYZ, HeadAngleRelToBodyYZ);
    if (!IsAngleWithinRange(differenceXY, 0.0001f)) {
        ChangeHeadAngleXY(targetXY, speed);
        headSettled = false;
    }
    if (!IsAngleWithinRange(differenceYZ, 0.0001f))
        ChangeHeadAngleYZ(targetYZ, speed);
    else if (headSettled) {
        TargetAcquired = true;
        long state = HeadTurnState | 8;
        if (HeadTurnState != state) {
            HeadTurnState = state;
            OneFrameDelay = true;
        }
        TimeToStopLooking = -1;
        HeadVelocityXY = 0.0f;
        HeadVelocityYZ = 0.0f;
    }
}
