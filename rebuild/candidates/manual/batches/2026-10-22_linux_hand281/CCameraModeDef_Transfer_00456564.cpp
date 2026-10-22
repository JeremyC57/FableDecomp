// CCameraModeDef::Transfer(CPersistContext&) -- serialize the camera-mode def fields in retail order (the
//   ViewHeroXYOffsetMultiplier field goes after the two max velocities, as in the retail persist order)
// Leaf: ?Transfer@CCameraModeDef@@UAEXAAVCPersistContext@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00456564, 531 bytes; `ret 4`.
// Callees: CPersistContext transfer overloads long 0x00431102, float 0x00431061, bool 0x0043314A.
// Layout: Ego_r.pdb CCameraModeDef (220 bytes) shifted down by 4 in retail (retail CDefBase prefix is 0x3c, the PDB
//   view's 0x40): CameraMode PDB +0x40 = retail +0x3c ... ViewHeroGroundHeightOffset PDB +0xd8 = retail +0xd4. No
//   engine header exists for this class; the fields are modelled locally with their PDB names.
#pragma optimize("s", on)
struct CPersistContext {
    void TransferLong(long* value);                    // 0x00431102
    void TransferFloat(float* value);                  // 0x00431061
    void TransferBool(bool* value);                    // 0x0043314A
};
struct CCameraModeDef {
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
