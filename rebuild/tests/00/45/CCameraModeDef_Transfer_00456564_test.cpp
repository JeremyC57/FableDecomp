#include <cstdio>
#include <cstring>
#include <cstddef>
// behaviour test: CCameraModeDef::Transfer visits every field once, in the retail order, with the right overload
static char g_log[8192];
class CPersistContext { public:
    char* Base;
    void Note(const char* kind, void* p) { char b[32]; std::sprintf(b, "%s%d;", kind, (int)((char*)p - Base)); std::strcat(g_log, b); }
    void TransferLong(long* value) { Note("L", value); *value += 1; }
    void TransferFloat(float* value) { Note("F", value); *value += 0.5f; }
    void TransferBool(bool* value) { Note("B", value); *value = !*value; }
};
class CCameraModeDef { public:
    long   CameraMode;                                      // +0x3c
    float  StringLengthXY;                                  // +0x40
    float  CageRadius;                                      // +0x44
    float  HeightOffsetZ;                                   // +0x48
    float  LookOffsetZ;                                     // +0x4c
    float  FOV;                                             // +0x50
    float  RollAngle;                                       // +0x54
    long   TransitionInFrames;                              // +0x58
    float  LazyLookAtPosWeight;                             // +0x5c
    bool   RaiseUpWhenClose;                                // +0x60
    bool   StretchAroundHero;                               // +0x61
    unsigned char pad62[2];
    float  ZTargetBaseOffset;                               // +0x64
    float  ZTargetLookAtTargetOffset;                       // +0x68
    float  ZTargetStringLengthToObservedThing;              // +0x6c
    float  ZTargetStringLengthPerpendicularToObservedThing; // +0x70
    bool   PerformExtendedOcclusionTests;                   // +0x74
    unsigned char pad75[3];
    float  XAxisDisplacementCoupling;                       // +0x78
    float  XAxisVelocityDamping;                            // +0x7c
    float  XAxisAttractorCoupling;                          // +0x80
    float  XAxisAttractorFallOff;                           // +0x84
    float  YAxisDisplacementCoupling;                       // +0x88
    float  YAxisVelocityDamping;                            // +0x8c
    float  YAxisAttractorCoupling;                          // +0x90
    float  YAxisAttractorFallOff;                           // +0x94
    bool   ProjectileCameraFOVZoom;                         // +0x98
    bool   ProjectileCameraZoomToggle;                      // +0x99
    unsigned char pad9a[2];
    float  ProjectileCameraDefaultTranslationalZoomOffset;  // +0x9c
    float  ProjectileCameraInitialPhiOffsetForCamera;       // +0xa0
    float  ProjectileCameraInitialHeightParamForCamera;     // +0xa4
    float  PhiRotationCouple;                               // +0xa8
    float  ThetaRotationCouple;                             // +0xac
    float  DollyLength;                                     // +0xb0
    long   NumberOfFramesToAverageInputsOver;               // +0xb4
    float  DollyLengthBlendResistance;                      // +0xb8
    float  ThetaAngleLimitUp;                               // +0xbc
    float  ThetaAngleLimitDown;                             // +0xc0
    float  SecondsToReset;                                  // +0xc4
    float  ViewHeroXYOffsetMultiplier;                      // +0xc8
    float  ViewHeroXYRotationMaxVelocity;                   // +0xcc
    float  ViewHeroHeightParamMaxVelocity;                  // +0xd0
    float  ViewHeroGroundHeightOffset;                      // +0xd4
    void Transfer(CPersistContext& context);
};
void CCameraModeDef::Transfer(CPersistContext& context)
{
    context.TransferLong(&CameraMode);
    context.TransferFloat(&StringLengthXY);
    context.TransferFloat(&CageRadius);
    context.TransferFloat(&HeightOffsetZ);
    context.TransferFloat(&LookOffsetZ);
    context.TransferFloat(&FOV);
    context.TransferFloat(&RollAngle);
    context.TransferLong(&TransitionInFrames);
    context.TransferFloat(&LazyLookAtPosWeight);
    context.TransferBool(&RaiseUpWhenClose);
    context.TransferBool(&StretchAroundHero);
    context.TransferFloat(&ZTargetBaseOffset);
    context.TransferFloat(&ZTargetLookAtTargetOffset);
    context.TransferFloat(&ZTargetStringLengthToObservedThing);
    context.TransferFloat(&ZTargetStringLengthPerpendicularToObservedThing);
    context.TransferBool(&PerformExtendedOcclusionTests);
    context.TransferFloat(&XAxisDisplacementCoupling);
    context.TransferFloat(&XAxisVelocityDamping);
    context.TransferFloat(&XAxisAttractorCoupling);
    context.TransferFloat(&XAxisAttractorFallOff);
    context.TransferFloat(&YAxisDisplacementCoupling);
    context.TransferFloat(&YAxisVelocityDamping);
    context.TransferFloat(&YAxisAttractorCoupling);
    context.TransferFloat(&YAxisAttractorFallOff);
    context.TransferBool(&ProjectileCameraFOVZoom);
    context.TransferBool(&ProjectileCameraZoomToggle);
    context.TransferFloat(&ProjectileCameraDefaultTranslationalZoomOffset);
    context.TransferFloat(&ProjectileCameraInitialPhiOffsetForCamera);
    context.TransferFloat(&ProjectileCameraInitialHeightParamForCamera);
    context.TransferFloat(&PhiRotationCouple);
    context.TransferFloat(&ThetaRotationCouple);
    context.TransferFloat(&DollyLength);
    context.TransferLong(&NumberOfFramesToAverageInputsOver);
    context.TransferFloat(&DollyLengthBlendResistance);
    context.TransferFloat(&ThetaAngleLimitUp);
    context.TransferFloat(&ThetaAngleLimitDown);
    context.TransferFloat(&SecondsToReset);
    context.TransferFloat(&ViewHeroXYRotationMaxVelocity);
    context.TransferFloat(&ViewHeroHeightParamMaxVelocity);
    context.TransferFloat(&ViewHeroXYOffsetMultiplier);
    context.TransferFloat(&ViewHeroGroundHeightOffset);
}

int main()
{
    CCameraModeDef def; std::memset(&def, 0, sizeof(def));
    CPersistContext ctx; ctx.Base = (char*)&def.CameraMode;
    def.Transfer(ctx);
    char want[8192]; want[0] = 0;
#define W(k, f) { char b[32]; std::sprintf(b, "%s%d;", k, (int)((char*)&def.f - (char*)&def.CameraMode)); std::strcat(want, b); }
    W("L", CameraMode)
    W("F", StringLengthXY)
    W("F", CageRadius)
    W("F", HeightOffsetZ)
    W("F", LookOffsetZ)
    W("F", FOV)
    W("F", RollAngle)
    W("L", TransitionInFrames)
    W("F", LazyLookAtPosWeight)
    W("B", RaiseUpWhenClose)
    W("B", StretchAroundHero)
    W("F", ZTargetBaseOffset)
    W("F", ZTargetLookAtTargetOffset)
    W("F", ZTargetStringLengthToObservedThing)
    W("F", ZTargetStringLengthPerpendicularToObservedThing)
    W("B", PerformExtendedOcclusionTests)
    W("F", XAxisDisplacementCoupling)
    W("F", XAxisVelocityDamping)
    W("F", XAxisAttractorCoupling)
    W("F", XAxisAttractorFallOff)
    W("F", YAxisDisplacementCoupling)
    W("F", YAxisVelocityDamping)
    W("F", YAxisAttractorCoupling)
    W("F", YAxisAttractorFallOff)
    W("B", ProjectileCameraFOVZoom)
    W("B", ProjectileCameraZoomToggle)
    W("F", ProjectileCameraDefaultTranslationalZoomOffset)
    W("F", ProjectileCameraInitialPhiOffsetForCamera)
    W("F", ProjectileCameraInitialHeightParamForCamera)
    W("F", PhiRotationCouple)
    W("F", ThetaRotationCouple)
    W("F", DollyLength)
    W("L", NumberOfFramesToAverageInputsOver)
    W("F", DollyLengthBlendResistance)
    W("F", ThetaAngleLimitUp)
    W("F", ThetaAngleLimitDown)
    W("F", SecondsToReset)
    W("F", ViewHeroXYRotationMaxVelocity)
    W("F", ViewHeroHeightParamMaxVelocity)
    W("F", ViewHeroXYOffsetMultiplier)
    W("F", ViewHeroGroundHeightOffset)
    if (std::strcmp(g_log, want) != 0 || def.CameraMode != 1 || def.FOV != 0.5f || !def.StretchAroundHero || def.ViewHeroGroundHeightOffset != 0.5f) {
        std::printf("00456564_TEST FAIL [%s] want [%s]\n", g_log, want);
        return 1;
    }
    std::printf("00456564_TEST PASS\n");
    return 0;
}