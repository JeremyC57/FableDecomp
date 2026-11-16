#ifndef FABLE_ENGINE_CCAMERAMODEDEF_H
#define FABLE_ENGINE_CCAMERAMODEDEF_H
// NEW header (wfE9 overlay, 2026-11-15; no repo header existed). Needs FABLE_RETAIL_DEF_CLASS_BASE (polymorphic CParentDefClassBase).
// Members: Ego_r.pdb CCameraModeDef (ghidra_out/struct_layouts_egor.tsv, sizeof 220, members in PDB order and spelling, after a
// 64-byte base). Retail: the CParentDefClassBase base is 0x3C bytes, so every member sits 4 bytes before its Ego_r offset (the same
// shift as engine/CAbilityTrackDef.h). Retail reads in NCameraModes::CProjectileWeapon::Update 0x00879900: ProjectileCameraFOVZoom
// +0x98 (0x0087990E), XAxisDisplacementCoupling +0x78 (0x00879A33), XAxisVelocityDamping +0x7C (0x00879A22),
// YAxisDisplacementCoupling +0x88 (0x00879A3A), YAxisVelocityDamping +0x8C (0x00879A40). Class methods (Ego_r.pdb): GetSizeofClass
// 0x0045CA6D, Transfer 0x0045CA73, operator= 0x00476A9D, Copy 0x0047C289 (not declared: no row needs them yet).
#include "engine/CParentDefClassBase.h"
class CCameraModeDef : public CParentDefClassBase {
public:
    long  CameraMode;                                        // +0x3C
    float StringLengthXY;                                    // +0x40
    float CageRadius;                                        // +0x44
    float HeightOffsetZ;                                     // +0x48
    float LookOffsetZ;                                       // +0x4C
    float FOV;                                               // +0x50
    float RollAngle;                                         // +0x54
    long  TransitionInFrames;                                // +0x58
    float LazyLookAtPosWeight;                               // +0x5C
    bool  RaiseUpWhenClose;                                  // +0x60
    bool  StretchAroundHero;                                 // +0x61
    float ZTargetBaseOffset;                                 // +0x64
    float ZTargetLookAtTargetOffset;                         // +0x68
    float ZTargetStringLengthToObservedThing;                // +0x6C
    float ZTargetStringLengthPerpendicularToObservedThing;   // +0x70
    bool  PerformExtendedOcclusionTests;                     // +0x74
    float XAxisDisplacementCoupling;                         // +0x78
    float XAxisVelocityDamping;                              // +0x7C
    float XAxisAttractorCoupling;                            // +0x80
    float XAxisAttractorFallOff;                             // +0x84
    float YAxisDisplacementCoupling;                         // +0x88
    float YAxisVelocityDamping;                              // +0x8C
    float YAxisAttractorCoupling;                            // +0x90
    float YAxisAttractorFallOff;                             // +0x94
    bool  ProjectileCameraFOVZoom;                           // +0x98
    bool  ProjectileCameraZoomToggle;                        // +0x99
    float ProjectileCameraDefaultTranslationalZoomOffset;    // +0x9C
    float ProjectileCameraInitialPhiOffsetForCamera;         // +0xA0
    float ProjectileCameraInitialHeightParamForCamera;       // +0xA4
    float PhiRotationCouple;                                 // +0xA8
    float ThetaRotationCouple;                               // +0xAC
    float DollyLength;                                       // +0xB0
    long  NumberOfFramesToAverageInputsOver;                 // +0xB4
    float DollyLengthBlendResistance;                        // +0xB8
    float ThetaAngleLimitUp;                                 // +0xBC
    float ThetaAngleLimitDown;                               // +0xC0
    float SecondsToReset;                                    // +0xC4
    float ViewHeroXYOffsetMultiplier;                        // +0xC8
    float ViewHeroXYRotationMaxVelocity;                     // +0xCC
    float ViewHeroHeightParamMaxVelocity;                    // +0xD0
    float ViewHeroGroundHeightOffset;                        // +0xD4
};
FABLE_STATIC_ASSERT(sizeof(CCameraModeDef)==0xd8);
FABLE_STATIC_ASSERT(offsetof(CCameraModeDef,CameraMode)==0x3c);
FABLE_STATIC_ASSERT(offsetof(CCameraModeDef,XAxisDisplacementCoupling)==0x78);
FABLE_STATIC_ASSERT(offsetof(CCameraModeDef,ProjectileCameraFOVZoom)==0x98);
FABLE_STATIC_ASSERT(offsetof(CCameraModeDef,ProjectileCameraInitialPhiOffsetForCamera)==0xa0);
#endif
