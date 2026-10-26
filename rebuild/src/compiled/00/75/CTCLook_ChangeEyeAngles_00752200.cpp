// CTCLook::ChangeEyeAngles(float xy, float yz) -- set the eye angles, each clamped to half the def's eye turn range (degrees / 360); flag the change and poke the graphic appearance when either angle moved
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00752200, length 242 bytes (+ int3 padding), `ret 8`.
// Callees: CThing::UpdateGraphicAppearance(long) @ 0x004C7590 (the parent's appearance forwarder; manifest
// "CRenderTarget::DetachTarget" is propagated). Constants: 1/360 0x01238E00 (`/ 360.0f` becomes a reciprocal multiply),
// 0.0f 0x0122DEDC, 0.5f 0x0122F59C.
// Shape: the range stays on the x87 stack; `(float)fabs(x) * 2.0f <= range` gives `fadd st,st ; fcomp st(1) ; test ah,41h ;
// jp`; the clamp is `Sign(x) * range * 0.5f` with an int Sign (-1 stored, 1 on `x >= 0`) homed in the dead `xy` argument
// slot for both axes; the change test is written new-first (`Eye == old`) for retail's fucompp operand order.
// Layout: Ego_r.pdb CTCLook (the landed CentreHead 0x007528A0 model): PDef +0x14, EyeAnglesRelToHead XY/YZ +0x34/+0x38,
// EyeAnglesChanged +0x80; CLookDef EyesTurnRangeXY/YZ +0x28/+0x2c. No engine header for CTCLook.
#include <math.h>
class CLookDef { public:
    unsigned char _pad00[0x28];
    float EyesTurnRangeXY;                      // +0x28 (degrees)
    float EyesTurnRangeYZ;                      // +0x2c
    float HeadTurnRangeXY;                      // +0x30
    float HeadTurnRangeYZUp;                    // +0x34
    float HeadTurnRangeYZDown;                  // +0x38
    float MaxTurnSpeed;                         // +0x3c
};
template<class T> class CDefPointer { public:
    T* Object;
    T* operator->() const { return Object; }
};
class CThing { public:
    void UpdateGraphicAppearance(long reason);  // 0x004C7590
};
class CThingCreatureBase;
class C3DVector { public: float X, Y, Z; };
class CTCLook { public:
    virtual void v00();
    CThing* ParentThing;                        // +0x04 (CTCBase)
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
    C3DVector PosToLookAt;                      // +0x68
    C3DVector DirectionToLookIn;                // +0x74
    bool  EyeAnglesChanged;                     // +0x80
    void ChangeEyeAngles(float xy, float yz);
};

inline int Sign(float x) { return x < 0.0f ? -1 : 1; }

void CTCLook::ChangeEyeAngles(float xy, float yz)
{
    float oldXY = EyeAnglesRelToHeadXY;
    float oldYZ = EyeAnglesRelToHeadYZ;
    float rangeXY = PDef->EyesTurnRangeXY / 360.0f;
    if ((float)fabs(xy) * 2.0f <= rangeXY)
        EyeAnglesRelToHeadXY = xy;
    else
        EyeAnglesRelToHeadXY = Sign(xy) * rangeXY * 0.5f;
    float rangeYZ = PDef->EyesTurnRangeYZ / 360.0f;
    if ((float)fabs(yz) * 2.0f <= rangeYZ)
        EyeAnglesRelToHeadYZ = yz;
    else
        EyeAnglesRelToHeadYZ = Sign(yz) * rangeYZ * 0.5f;
    if (EyeAnglesRelToHeadXY == oldXY && EyeAnglesRelToHeadYZ == oldYZ) {
        EyeAnglesChanged = false;
    } else {
        EyeAnglesChanged = true;
        ParentThing->UpdateGraphicAppearance(4);
    }
}