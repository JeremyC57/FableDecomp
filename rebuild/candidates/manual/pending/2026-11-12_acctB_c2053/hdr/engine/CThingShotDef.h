#ifndef FABLE_ENGINE_CTHINGSHOTDEF_H
#define FABLE_ENGINE_CTHINGSHOTDEF_H
#include "engine/CParentDefClassBase.h"
#ifdef FABLE_RETAIL_CTHINGSHOTDEF_CLASSNAME
#include "engine/CDefString.h"
#endif
// Retail opt-in (Codex 2026-11-10): RTTI vtable01234e2c proves CParentDefClassBase;
// the PDB field sequence is four bytes earlier after its STLport vector shrinks.
// Parent size39h aligns the first float Speed at3ch, as GetInitialVelocity007befb0.
class CThingShotDef : public CParentDefClassBase {
public:
    float         Speed;                                     // +0x3c
    long          PrimaryEffect;                             // +0x40
    long          PrimaryEffectSound;                        // +0x44
    long          AttachedContinuousEffect;                  // +0x48
    long          AttachedContinuousSound;                   // +0x4c
    bool          UseEndTimeLineOnContinousEffect;           // +0x50
    unsigned char _pad_0x51[0x3];                            // +0x51
    float         HitThingDamage;                            // +0x54
    float         HitThingShieldDamage;                      // +0x58
    bool          CauseDamage;                               // +0x5c
    unsigned char _pad_0x5d[0x3];                            // +0x5d
#ifdef FABLE_RETAIL_CTHINGSHOTDEF_CLASSNAME
    // Opt-in (chunk 2053, row 0x0087F3A0): the Ego_r.pdb member ShotDefinitionClassName typed as CDefString (4 bytes, TablePos), read
    // through CDefString::GetString 0x00415DD0 (`lea ecx,[ebx+60h] ; call 00415DD0`, 0x0087F7F9). Same offset and size.
    CDefString    ShotDefinitionClassName;                   // +0x60
#else
    long          ShotDefinitionClassName_TablePos;          // +0x60 CDefString::TablePos
#endif
    float         ShotOffsetAngleRange_HighValue;            // +0x64 CFloatRange::HighValue
    float         ShotOffsetAngleRange_LowValue;             // +0x68 CFloatRange::LowValue
    float         ShotOffsetRadiusRange_HighValue;           // +0x6c CFloatRange::HighValue
    float         ShotOffsetRadiusRange_LowValue;            // +0x70 CFloatRange::LowValue
    long          ExplosionDef;                              // +0x74
    long          GroundExplosionDef;                        // +0x78
    long          ObjectDef;                                 // +0x7c
    bool          LeavesObjectBehind;                        // +0x80
    bool          IsHittableByMelee;                         // +0x81
    bool          IsDeflectedOnHitByMelee;                   // +0x82
    unsigned char _pad_0x83[0x1];                            // +0x83
    float         MeleeHitDeflectionDamageMultiplier;        // +0x84
    bool          FaceMovementDirection;                     // +0x88
    bool          AllowDeflections;                          // +0x89
    bool          BounceOnGround;                            // +0x8a
    unsigned char _pad_0x8b[0x1];                            // +0x8b
    float         Bounciness;                                // +0x8c
    bool          RotateOnDeflection;                        // +0x90
    bool          IsBlockable;                               // +0x91
    unsigned char _pad_0x92[0x2];                            // +0x92
    unsigned char DamageType[0x4];                           // +0x94 EDamageAttribute
    long          NumSubShots;                               // +0x98
    long          SubShotType;                               // +0x9c
    float         SubShotCreationTime;                       // +0xa0
    float         SubShotMaxAngle;                           // +0xa4
    bool          RemoveMainShotOnSubShotCreation;           // +0xa8
    bool          DoesExplosionDamageCaster;                 // +0xa9
    bool          CanHitRollingTargets;                      // +0xaa
    bool          PreventSlidingOnCollision;                 // +0xab
};
FABLE_STATIC_ASSERT(sizeof(CThingShotDef)==0xac);
#endif
