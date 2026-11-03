#ifndef FABLE_ENGINE_CCAMERAMODEDEF_H
#define FABLE_ENGINE_CCAMERAMODEDEF_H
#include "engine/CParentDefClassBase.h"
// New header (chunk 724b, Codex pass-12 model). Needs FABLE_RETAIL_DEF_CLASS_BASE.
// PDB CCameraModeDef: CParentDefClassBase (retail 0x39) then the Ego_r settings; native read
// ProjectileCameraInitialPhiOffsetForCamera+0xa0, four bytes before donor.
class CCameraModeDef : public CParentDefClassBase {
public:
 long CameraMode;float StringLengthXY,CageRadius,HeightOffsetZ,LookOffsetZ,FOV,RollAngle;
 long TransitionInFrames;float LazyLookAtPosWeight;bool RaiseUpWhenClose,StretchAroundHero;
 float ZTargetBaseOffset,ZTargetLookAtTargetOffset,ZTargetStringLengthToObservedThing,ZTargetStringLengthPerpendicularToObservedThing;
 bool PerformExtendedOcclusionTests;
 float XAxisDisplacementCoupling,XAxisVelocityDamping,XAxisAttractorCoupling,XAxisAttractorFallOff;
 float YAxisDisplacementCoupling,YAxisVelocityDamping,YAxisAttractorCoupling,YAxisAttractorFallOff;
 bool ProjectileCameraFOVZoom,ProjectileCameraZoomToggle;
 float ProjectileCameraDefaultTranslationalZoomOffset,ProjectileCameraInitialPhiOffsetForCamera,ProjectileCameraInitialHeightParamForCamera;
 float PhiRotationCouple,ThetaRotationCouple,DollyLength;long NumberOfFramesToAverageInputsOver;
 float DollyLengthBlendResistance,ThetaAngleLimitUp,ThetaAngleLimitDown,SecondsToReset,ViewHeroXYOffsetMultiplier,ViewHeroXYRotationMaxVelocity,ViewHeroHeightParamMaxVelocity,ViewHeroGroundHeightOffset;
};
FABLE_STATIC_ASSERT(sizeof(CCameraModeDef)==0xd8);
FABLE_STATIC_ASSERT(offsetof(CCameraModeDef,ProjectileCameraInitialPhiOffsetForCamera)==0xa0);
#endif
