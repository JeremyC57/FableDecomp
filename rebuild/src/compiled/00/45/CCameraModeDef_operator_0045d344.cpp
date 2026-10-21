// CCameraModeDef::operator=(const CCameraModeDef&) -- memberwise copy: CParentDefClassBase part, then the camera-mode tuning scalars (+0x3c..+0xd4); returns *this
#pragma optimize("s", on)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0045D344, length 405 bytes.
// Pragma: #pragma optimize("s", on) -- the def module is size-optimised (every scalar copy goes through eax/al).
// Callees: CParentDefClassBase::operator= @ 0x00431ED0.
// Identity: the catalog label "CParentDefClassBase::operator=" is propagated. Slot 19 (Copy) of CCameraModeDef's
// vtable 0x01233B7C is the 5-byte `jmp 0x0045D344` right before this body; the PDB members (+0x40..) sit at
// retail +0x3c.. (retail CParentDefClassBase is 0x39 bytes and its vectors are 12 bytes, not the PDB 16).
// Shape: the compiler-style memberwise assignment written out -- base part first (out of line), then each
// member in declaration order: containers through their out-of-line operator=, scalars as eax/al moves.
// The engine header CParentDefClassBase.h is the PDB layout (0x3d bytes, 16-byte SubDefInfoMap); the retail
// base is modelled locally as a packed polymorphic base exactly as the landed sibling 0x00460256 does.
#pragma pack(push, 1)
struct CParentDefClassBase {
    virtual void v00();
    unsigned char _defClassBase[0x35];      // +0x04 CDefClassBase part, InstantiationName (+0x25), SubDefInfoMap (+0x29)
    CParentDefClassBase& operator=(const CParentDefClassBase& other);   // 0x00431ED0
};
#pragma pack(pop)

struct CCameraModeDef : CParentDefClassBase {          // vtable 0x01233B7C
    long CameraMode;
    float StringLengthXY;
    float CageRadius;
    float HeightOffsetZ;
    float LookOffsetZ;
    float FOV;
    float RollAngle;
    long TransitionInFrames;
    float LazyLookAtPosWeight;
    bool RaiseUpWhenClose;
    bool StretchAroundHero;
    float ZTargetBaseOffset;
    float ZTargetLookAtTargetOffset;
    float ZTargetStringLengthToObservedThing;
    float ZTargetStringLengthPerpendicularToObservedThing;
    bool PerformExtendedOcclusionTests;
    float XAxisDisplacementCoupling;
    float XAxisVelocityDamping;
    float XAxisAttractorCoupling;
    float XAxisAttractorFallOff;
    float YAxisDisplacementCoupling;
    float YAxisVelocityDamping;
    float YAxisAttractorCoupling;
    float YAxisAttractorFallOff;
    bool ProjectileCameraFOVZoom;
    bool ProjectileCameraZoomToggle;
    float ProjectileCameraDefaultTranslationalZoomOffset;
    float ProjectileCameraInitialPhiOffsetForCamera;
    float ProjectileCameraInitialHeightParamForCamera;
    float PhiRotationCouple;
    float ThetaRotationCouple;
    float DollyLength;
    long NumberOfFramesToAverageInputsOver;
    float DollyLengthBlendResistance;
    float ThetaAngleLimitUp;
    float ThetaAngleLimitDown;
    float SecondsToReset;
    float ViewHeroXYOffsetMultiplier;
    float ViewHeroXYRotationMaxVelocity;
    float ViewHeroHeightParamMaxVelocity;
    float ViewHeroGroundHeightOffset;
    CCameraModeDef& operator=(const CCameraModeDef& other);
};

CCameraModeDef& CCameraModeDef::operator=(const CCameraModeDef& other)
{
    CParentDefClassBase::operator=(other);
    CameraMode = other.CameraMode;
    StringLengthXY = other.StringLengthXY;
    CageRadius = other.CageRadius;
    HeightOffsetZ = other.HeightOffsetZ;
    LookOffsetZ = other.LookOffsetZ;
    FOV = other.FOV;
    RollAngle = other.RollAngle;
    TransitionInFrames = other.TransitionInFrames;
    LazyLookAtPosWeight = other.LazyLookAtPosWeight;
    RaiseUpWhenClose = other.RaiseUpWhenClose;
    StretchAroundHero = other.StretchAroundHero;
    ZTargetBaseOffset = other.ZTargetBaseOffset;
    ZTargetLookAtTargetOffset = other.ZTargetLookAtTargetOffset;
    ZTargetStringLengthToObservedThing = other.ZTargetStringLengthToObservedThing;
    ZTargetStringLengthPerpendicularToObservedThing = other.ZTargetStringLengthPerpendicularToObservedThing;
    PerformExtendedOcclusionTests = other.PerformExtendedOcclusionTests;
    XAxisDisplacementCoupling = other.XAxisDisplacementCoupling;
    XAxisVelocityDamping = other.XAxisVelocityDamping;
    XAxisAttractorCoupling = other.XAxisAttractorCoupling;
    XAxisAttractorFallOff = other.XAxisAttractorFallOff;
    YAxisDisplacementCoupling = other.YAxisDisplacementCoupling;
    YAxisVelocityDamping = other.YAxisVelocityDamping;
    YAxisAttractorCoupling = other.YAxisAttractorCoupling;
    YAxisAttractorFallOff = other.YAxisAttractorFallOff;
    ProjectileCameraFOVZoom = other.ProjectileCameraFOVZoom;
    ProjectileCameraZoomToggle = other.ProjectileCameraZoomToggle;
    ProjectileCameraDefaultTranslationalZoomOffset = other.ProjectileCameraDefaultTranslationalZoomOffset;
    ProjectileCameraInitialPhiOffsetForCamera = other.ProjectileCameraInitialPhiOffsetForCamera;
    ProjectileCameraInitialHeightParamForCamera = other.ProjectileCameraInitialHeightParamForCamera;
    PhiRotationCouple = other.PhiRotationCouple;
    ThetaRotationCouple = other.ThetaRotationCouple;
    DollyLength = other.DollyLength;
    NumberOfFramesToAverageInputsOver = other.NumberOfFramesToAverageInputsOver;
    DollyLengthBlendResistance = other.DollyLengthBlendResistance;
    ThetaAngleLimitUp = other.ThetaAngleLimitUp;
    ThetaAngleLimitDown = other.ThetaAngleLimitDown;
    SecondsToReset = other.SecondsToReset;
    ViewHeroXYOffsetMultiplier = other.ViewHeroXYOffsetMultiplier;
    ViewHeroXYRotationMaxVelocity = other.ViewHeroXYRotationMaxVelocity;
    ViewHeroHeightParamMaxVelocity = other.ViewHeroHeightParamMaxVelocity;
    ViewHeroGroundHeightOffset = other.ViewHeroGroundHeightOffset;
    return *this;
}