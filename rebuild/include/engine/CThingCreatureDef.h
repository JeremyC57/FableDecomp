#ifndef FABLE_ENGINE_CTHINGCREATUREDEF_H
#define FABLE_ENGINE_CTHINGCREATUREDEF_H
#include "engine/CThingBaseDef.h"
#ifdef FABLE_RETAIL_CTHINGCREATUREDEF_MEMBERS
#include <stddef.h>
#include "engine/CEngineGraphic.h"
#endif
// Original TPI0x14ae8; complete native14c, original flags begin148.
class CThingCreatureDef : public CThingBaseDef {
public:
#if defined(FABLE_RETAIL_CTHINGCREATUREDEF_MEMBERS)
    // Opt-in (agentNF12, 2026-11-11, row 0x008338D0 CThingAICreature::InitialiseFromDef): the Ego_r.pdb members between the base and the
    // flag block, in order and spelling, at the retail offsets (PDB - 4 up to NavigatorTypes, a 12-byte STLport vector; PDB - 8 after).
    // Evidence: InitialiseFromDef reads MaxWalkingSpeed / MaxJoggingSpeed / MaxRunningSpeed / MaxFlyingSpeed at +0x90 / +0x94 / +0x98 /
    // +0xA0 and BrainDef at +0xE8 (0x008338DA..0x00833963); the landed branches below agree (CombatTypeDef +0x124, LeaderTypeDef +0x13C,
    // flags +0x148). Enum and vector members stay opaque (their real type in the comment). Same size and flag block as the default.
    float StunTimeMin;                                      // +0x70 (PDB +116)
    float StunTimeMax;                                      // +0x74 (PDB +120)
    float StunHealthThreshold;                              // +0x78 (PDB +124)
    float DazedTimeMin;                                     // +0x7C (PDB +128)
    float DazedTimeMax;                                     // +0x80 (PDB +132)
    long DazedHealthThreshold;                              // +0x84 (PDB +136)
    float NumSecondsToPersistOnDeath;                       // +0x88 (PDB +140)
    float MaxSlowWalkingSpeed;                              // +0x8C (PDB +144)
    float MaxWalkingSpeed;                                  // +0x90 (PDB +148)
    float MaxJoggingSpeed;                                  // +0x94 (PDB +152)
    float MaxRunningSpeed;                                  // +0x98 (PDB +156)
    float MaxPushingSpeed;                                  // +0x9C (PDB +160)
    float MaxFlyingSpeed;                                   // +0xA0 (PDB +164)
    float MaxHoverSpeedSlow;                                // +0xA4 (PDB +168)
    float MaxHoverSpeedMedium;                              // +0xA8 (PDB +172)
    float MaxHoverSpeedFast;                                // +0xAC (PDB +176)
    float SpeedRandomisationFactor;                         // +0xB0 (PDB +180)
    float MaxAcceleration;                                  // +0xB4 (PDB +184)
    float HearingRadius;                                    // +0xB8 (PDB +188)
    float FOV;                                              // +0xBC (PDB +192)
    unsigned char Sex[0x4];                                 // +0xC0 ESex (PDB +196)
    unsigned char CreatureType[0x4];                        // +0xC4 ECreatureType (PDB +200)
    unsigned char NavigatorTypes[0xc];                      // +0xC8 vector<ENavigatorType> (PDB +204)
    long InitialWeaponDef;                                  // +0xD4 (PDB +220)
    long PrimaryMeleeWeaponDef;                             // +0xD8 (PDB +224)
    long SecondaryMeleeWeaponDef;                           // +0xDC (PDB +228)
    long RangedWeaponDef;                                   // +0xE0 (PDB +232)
    long InitialSheathedWeapon;                             // +0xE4 (PDB +236)
    long BrainDef;                                          // +0xE8 (PDB +240)
    unsigned char CreatureInteractionType[0x4];             // +0xEC ECreatureInteractionType (PDB +244)
    unsigned char Property[0x4];                            // +0xF0 EThingCreatureProperty (PDB +248)
    float Damage;                                           // +0xF4 (PDB +252)
    long DamageEffectDef;                                   // +0xF8 (PDB +256)
    long StunnedParticle;                                   // +0xFC (PDB +260)
    CEngineGraphic Graphic;                                 // +0x100 (PDB +264)
    CEngineGraphic EyeGraphic;                              // +0x10E (PDB +278)
    long DefaultOwner;                                      // +0x11C (PDB +292)
    unsigned char ControlledMovementType[0x4];              // +0x120 EControlledMovementType (PDB +296)
    long CombatTypeDef;                                     // +0x124 (PDB +300)
    bool ResetCombatTypeOnUnsheatheDefaultWeapons;          // +0x128 (PDB +304)
    unsigned char _pad_0x129[3];                              // +0x129
    long HeroCombatDef;                                     // +0x12C (PDB +308)
    long HeroCombatPCDef;                                   // +0x130 (PDB +312)
    float CombatDiameter;                                   // +0x134 (PDB +316)
    bool DeferHitsIfBehindHitter:1;                         // +0x138 (PDB +320)
    unsigned char _pad_0x139[3];                              // +0x139
    long LeaderTypeDef;                                     // +0x13C (PDB +324)
    float RecoverEvadeHitsPercent;                          // +0x140 (PDB +328)
    long DeadBodyDecayEffect;                               // +0x144 (PDB +332)
#else
#ifdef FABLE_RETAIL_CTHINGCREATUREDEF_LEADER
    unsigned char _pad_0x70[0x13c-0x70];
    long LeaderTypeDef;
    unsigned char _pad_0x140[8];
#elif defined(FABLE_RETAIL_CTHINGCREATUREBASE_COMBATTYPE)
    unsigned char _pad_0x70[0xb4];
    long CombatTypeDef;
    unsigned char _pad_0x128[0x20];
#else
    unsigned char _pad_0x70[0xd8];
#endif
#endif // FABLE_RETAIL_CTHINGCREATUREDEF_MEMBERS
    bool LeaveDeadCreature:1;
    bool AbleToStrafe:1;
    bool CreateDummyOnDeath:1;
    bool ImmuneToAttack:1;
    bool UseMeleeWeaponAsDefault:1;
    bool UnsheatheWeaponsAutomatically:1;
    bool IsWillable:1;
    bool IsIndestructible:1;
    bool IsInvulnerableDuringGenericResponse:1;
    bool IsPushedByExplosions:1;
    unsigned char _pad_0x14a[2];
};
FABLE_STATIC_ASSERT(sizeof(CThingCreatureDef)==0x14c);
#ifdef FABLE_RETAIL_CTHINGCREATUREDEF_MEMBERS
FABLE_STATIC_ASSERT(offsetof(CThingCreatureDef,MaxWalkingSpeed)==0x90);
FABLE_STATIC_ASSERT(offsetof(CThingCreatureDef,BrainDef)==0xe8);
FABLE_STATIC_ASSERT(offsetof(CThingCreatureDef,CombatTypeDef)==0x124);
FABLE_STATIC_ASSERT(offsetof(CThingCreatureDef,LeaderTypeDef)==0x13c);
#endif
// CThingCreatureBase::PeekDefinition (inline, FableWin-only): defined here when engine/CThingCreatureBase.h came first.
#if defined(FABLE_ENGINE_CTHINGCREATUREBASE_H) && defined(FABLE_RETAIL_CTHINGCREATUREBASE)
inline const CThingCreatureDef& CThingCreatureBase::PeekDefinition() const { return *PDef; }
#endif
#endif
