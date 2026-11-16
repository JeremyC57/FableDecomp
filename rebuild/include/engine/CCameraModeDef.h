#ifndef FABLE_ENGINE_CCAMERAMODEDEF_H
#define FABLE_ENGINE_CCAMERAMODEDEF_H
#ifdef FABLE_RETAIL_CCAMERAMODEDEF_CYLINDER
// Retail opt-in (Codex pass23): both original TPI records inherit CParentDefClassBase.
// Ego_r full fields shifted-4 by retail parent STLport layout; native0085c5c3 reads ViewHeroXYOffsetMultiplier+c8.
#include "engine/CParentDefClassBase.h"
class CCameraModeDef: public CParentDefClassBase {
public:
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
};
FABLE_STATIC_ASSERT(sizeof(CCameraModeDef)==0xd8);
#endif
#endif
