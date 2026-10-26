// CCameraModeDef::Transfer(CPersistContext&) -- serialize the camera-mode def fields in retail order (the
//   ViewHeroXYOffsetMultiplier field goes after the two max velocities, as in the retail persist order)
// Leaf: ?Transfer@CCameraModeDef@@UAEXAAVCPersistContext@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00456564, 531 bytes; `ret 4`.
// Callees: CPersistContext transfer overloads long 0x00431102, float 0x00431061, bool 0x0043314A.
// Layout: Ego_r.pdb CCameraModeDef (220 bytes) shifted down by 4 in retail (retail CDefBase prefix is 0x3c, the PDB
//   view's 0x40): CameraMode PDB +0x40 = retail +0x3c ... ViewHeroGroundHeightOffset PDB +0xd8 = retail +0xd4. No
//   engine header exists for this class; the fields are modelled locally with their PDB names.
#pragma optimize("s", on)
class CPersistContext { public:
    template <class T> void Transfer(T& value);                    // 0x00431102
};
class CCameraModeDef { public:
    unsigned char prefix[0x38];                        // CDefBase body after the vptr (retail)
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
    virtual void Transfer(CPersistContext& context);
};
void CCameraModeDef::Transfer(CPersistContext& context)
{
    context.Transfer(*(long*)(&CameraMode));
    context.Transfer(*(float*)(&StringLengthXY));
    context.Transfer(*(float*)(&CageRadius));
    context.Transfer(*(float*)(&HeightOffsetZ));
    context.Transfer(*(float*)(&LookOffsetZ));
    context.Transfer(*(float*)(&FOV));
    context.Transfer(*(float*)(&RollAngle));
    context.Transfer(*(long*)(&TransitionInFrames));
    context.Transfer(*(float*)(&LazyLookAtPosWeight));
    context.Transfer(*(bool*)(&RaiseUpWhenClose));
    context.Transfer(*(bool*)(&StretchAroundHero));
    context.Transfer(*(float*)(&ZTargetBaseOffset));
    context.Transfer(*(float*)(&ZTargetLookAtTargetOffset));
    context.Transfer(*(float*)(&ZTargetStringLengthToObservedThing));
    context.Transfer(*(float*)(&ZTargetStringLengthPerpendicularToObservedThing));
    context.Transfer(*(bool*)(&PerformExtendedOcclusionTests));
    context.Transfer(*(float*)(&XAxisDisplacementCoupling));
    context.Transfer(*(float*)(&XAxisVelocityDamping));
    context.Transfer(*(float*)(&XAxisAttractorCoupling));
    context.Transfer(*(float*)(&XAxisAttractorFallOff));
    context.Transfer(*(float*)(&YAxisDisplacementCoupling));
    context.Transfer(*(float*)(&YAxisVelocityDamping));
    context.Transfer(*(float*)(&YAxisAttractorCoupling));
    context.Transfer(*(float*)(&YAxisAttractorFallOff));
    context.Transfer(*(bool*)(&ProjectileCameraFOVZoom));
    context.Transfer(*(bool*)(&ProjectileCameraZoomToggle));
    context.Transfer(*(float*)(&ProjectileCameraDefaultTranslationalZoomOffset));
    context.Transfer(*(float*)(&ProjectileCameraInitialPhiOffsetForCamera));
    context.Transfer(*(float*)(&ProjectileCameraInitialHeightParamForCamera));
    context.Transfer(*(float*)(&PhiRotationCouple));
    context.Transfer(*(float*)(&ThetaRotationCouple));
    context.Transfer(*(float*)(&DollyLength));
    context.Transfer(*(long*)(&NumberOfFramesToAverageInputsOver));
    context.Transfer(*(float*)(&DollyLengthBlendResistance));
    context.Transfer(*(float*)(&ThetaAngleLimitUp));
    context.Transfer(*(float*)(&ThetaAngleLimitDown));
    context.Transfer(*(float*)(&SecondsToReset));
    context.Transfer(*(float*)(&ViewHeroXYRotationMaxVelocity));
    context.Transfer(*(float*)(&ViewHeroHeightParamMaxVelocity));
    context.Transfer(*(float*)(&ViewHeroXYOffsetMultiplier));
    context.Transfer(*(float*)(&ViewHeroGroundHeightOffset));
}