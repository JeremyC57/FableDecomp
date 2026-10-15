// CPlayerMovementDef::operator=(const CPlayerMovementDef&) -- memberwise copy: CParentDefClassBase part (out of line), then sixteen joystick/turn/lean tuning words; returns *this
#pragma optimize("s", on)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004628D0, length 117 bytes.
// Pragma: #pragma optimize("s", on) -- the def module is size-optimised (every copy goes through eax).
// Callees: CParentDefClassBase::operator= @ 0x00431ED0.
// Identity: the catalog label "CParentDefClassBase::operator=" is propagated. Slot 19 (Copy) of
// CPlayerMovementDef's vtable 0x0123505C is the 5-byte `jmp 0x004628D0` right before this body; the PDB
// members (+0x40..+0x7c) sit 4 bytes lower in retail (+0x3c..+0x78) because the retail base is 0x39 bytes.
// Shape: the compiler-style memberwise assignment written out -- base first, then every float/long member in
// declaration order as one dword move through eax.
// The engine header CParentDefClassBase.h is the PDB layout (0x3d bytes, 16-byte SubDefInfoMap); the retail
// base is modelled locally as a packed polymorphic base whose operator= is out of line.
#pragma pack(push, 1)
struct CParentDefClassBase {
    virtual void v00();
    unsigned char _defClassBase[0x35];      // +0x04 CDefClassBase part, InstantiationName (+0x25), SubDefInfoMap (+0x29)
    CParentDefClassBase& operator=(const CParentDefClassBase& other);   // 0x00431ED0
};
#pragma pack(pop)

struct CPlayerMovementDef : CParentDefClassBase {  // vtable 0x0123505C
    float JoystickMovementStationaryBand;          // +0x3c
    float JoystickMovementSlowBand;                // +0x40
    float JoystickMovementWalkingBand;             // +0x44
    float JoystickMovementJoggingBand;             // +0x48
    float MinRequiredAngleFor180;                  // +0x4c
    float SecondsToStoreLastMovementFor;           // +0x50
    long  NoMovementFramesToCheckFor180;           // +0x54
    long  NoMovementFramesToCheckFor180PriorStraightLine; // +0x58
    float MinNoRunSecondsForSkidHaltMove;          // +0x5c
    long  NoRunningFramesToPreventSkidHalt;        // +0x60
    float MaxAllowedAngleChangeForSkidHalt;        // +0x64
    float MaxTurnSpeed;                            // +0x68
    float MaxTurnSpeedHeavyWeapon;                 // +0x6c
    float LeanAngleTurnFactor;                     // +0x70
    float LeanAngleLimitAngle;                     // +0x74
    float LeanAngleStepPerFrame;                   // +0x78
    CPlayerMovementDef& operator=(const CPlayerMovementDef& other);
};

CPlayerMovementDef& CPlayerMovementDef::operator=(const CPlayerMovementDef& other)
{
    CParentDefClassBase::operator=(other);
    JoystickMovementStationaryBand = other.JoystickMovementStationaryBand;
    JoystickMovementSlowBand = other.JoystickMovementSlowBand;
    JoystickMovementWalkingBand = other.JoystickMovementWalkingBand;
    JoystickMovementJoggingBand = other.JoystickMovementJoggingBand;
    MinRequiredAngleFor180 = other.MinRequiredAngleFor180;
    SecondsToStoreLastMovementFor = other.SecondsToStoreLastMovementFor;
    NoMovementFramesToCheckFor180 = other.NoMovementFramesToCheckFor180;
    NoMovementFramesToCheckFor180PriorStraightLine = other.NoMovementFramesToCheckFor180PriorStraightLine;
    MinNoRunSecondsForSkidHaltMove = other.MinNoRunSecondsForSkidHaltMove;
    NoRunningFramesToPreventSkidHalt = other.NoRunningFramesToPreventSkidHalt;
    MaxAllowedAngleChangeForSkidHalt = other.MaxAllowedAngleChangeForSkidHalt;
    MaxTurnSpeed = other.MaxTurnSpeed;
    MaxTurnSpeedHeavyWeapon = other.MaxTurnSpeedHeavyWeapon;
    LeanAngleTurnFactor = other.LeanAngleTurnFactor;
    LeanAngleLimitAngle = other.LeanAngleLimitAngle;
    LeanAngleStepPerFrame = other.LeanAngleStepPerFrame;
    return *this;
}
