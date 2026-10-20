// Behaviour test for CTCLook::ChangeEyeAngles (0x00752200).
#include <cstdio>
#include <math.h>
struct CLookDef {
    unsigned char _pad00[0x28];
    float EyesTurnRangeXY;                      // +0x28 (degrees)
    float EyesTurnRangeYZ;                      // +0x2c
    float HeadTurnRangeXY;                      // +0x30
    float HeadTurnRangeYZUp;                    // +0x34
    float HeadTurnRangeYZDown;                  // +0x38
    float MaxTurnSpeed;                         // +0x3c
};
template<class T> struct CDefPointer {
    T* Object;
    T* operator->() const { return Object; }
};
struct CThing {
    int Pokes; long LastReason;
    void UpdateGraphicAppearance(long reason) { ++Pokes; LastReason = reason; }
};
struct CThingCreatureBase;
struct C3DVector { float X, Y, Z; };
struct CTCLook {
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

int main()
{
    CLookDef def; def.EyesTurnRangeXY = 72.0f; def.EyesTurnRangeYZ = 36.0f;   // half ranges 0.1 / 0.05
    CThing thing; thing.Pokes = 0; thing.LastReason = 0;
    CTCLook look;
    look.ParentThing = &thing;
    look.PDef.Object = &def;
    look.EyeAnglesRelToHeadXY = 0.0f; look.EyeAnglesRelToHeadYZ = 0.0f; look.EyeAnglesChanged = false;
    bool ok = true;
    look.ChangeEyeAngles(0.05f, -0.02f);                 // inside both ranges
    ok = ok && look.EyeAnglesRelToHeadXY == 0.05f && look.EyeAnglesRelToHeadYZ == -0.02f && look.EyeAnglesChanged
            && thing.Pokes == 1 && thing.LastReason == 4;
    look.ChangeEyeAngles(0.5f, -0.5f);                   // clamped to +0.1 / -0.05
    ok = ok && fabs(look.EyeAnglesRelToHeadXY - 0.1f) < 1e-6f && fabs(look.EyeAnglesRelToHeadYZ + 0.05f) < 1e-6f;
    float x = look.EyeAnglesRelToHeadXY, y = look.EyeAnglesRelToHeadYZ;
    look.ChangeEyeAngles(x, y);                          // no change
    ok = ok && !look.EyeAnglesChanged && thing.Pokes == 2;
    if (!ok) { std::printf("00752200_TEST FAIL\n"); return 1; }
    std::printf("00752200_TEST PASS\n");
    return 0;
}
