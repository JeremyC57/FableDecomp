#ifndef FABLE_ENGINE_CPLAYERMOVEMENTDEF_H
#define FABLE_ENGINE_CPLAYERMOVEMENTDEF_H
// NEW header (chunk 2041, row 0x006AB820 CThingPlayerCreature::ResolveFacingDirection): the retail CPlayerMovementDef (Ego_r.pdb
// class key V, pdb_class_keys.tsv; vtable 0x0123505C) on the retail def chain CParentDefClassBase (FABLE_RETAIL_DEF_CLASS_BASE: 0x39
// bytes packed, so every Ego_r member sits 4 bytes lower: Ego_r +0x40..+0x7c -> retail +0x3c..+0x78). Member names and order are
// Ego_r's (struct_layouts_egor.tsv); the offsets are the landed rows': the memberwise operator= 0x004628D0 copies +0x3c..+0x78,
// GetMaxTurnSpeed 0x006A99D0 reads MaxTurnSpeed +0x68 / MaxTurnSpeedHeavyWeapon +0x6c, and ResolveFacingDirection 0x006AB820 reads
// LeanAngleTurnFactor +0x70 / LeanAngleLimitAngle +0x74 / LeanAngleStepPerFrame +0x78. Members only (no methods).
#include "fable_compiler.h"
#include "rebuild_abi.h"
#include <stddef.h>
#pragma pack(push, 1)
#include "engine/CParentDefClassBase.h"
#pragma pack(pop)
class CPlayerMovementDef : public CParentDefClassBase {
public:
    float JoystickMovementStationaryBand;                   // +0x3c
    float JoystickMovementSlowBand;                         // +0x40
    float JoystickMovementWalkingBand;                      // +0x44
    float JoystickMovementJoggingBand;                      // +0x48
    float MinRequiredAngleFor180;                           // +0x4c
    float SecondsToStoreLastMovementFor;                    // +0x50
    long  NoMovementFramesToCheckFor180;                    // +0x54
    long  NoMovementFramesToCheckFor180PriorStraightLine;   // +0x58
    float MinNoRunSecondsForSkidHaltMove;                   // +0x5c
    long  NoRunningFramesToPreventSkidHalt;                 // +0x60
    float MaxAllowedAngleChangeForSkidHalt;                 // +0x64
    float MaxTurnSpeed;                                     // +0x68
    float MaxTurnSpeedHeavyWeapon;                          // +0x6c
    float LeanAngleTurnFactor;                              // +0x70
    float LeanAngleLimitAngle;                              // +0x74
    float LeanAngleStepPerFrame;                            // +0x78
    static void CheckRetailLayout() {
        FABLE_STATIC_ASSERT(offsetof(CPlayerMovementDef, JoystickMovementStationaryBand) == 0x3c);
        FABLE_STATIC_ASSERT(offsetof(CPlayerMovementDef, MaxTurnSpeed) == 0x68);
        FABLE_STATIC_ASSERT(offsetof(CPlayerMovementDef, LeanAngleStepPerFrame) == 0x78);
    }
};
#endif
