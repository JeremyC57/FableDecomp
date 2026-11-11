#ifndef FABLE_ENGINE_CPLAYERMOVEMENTDEF_H
#define FABLE_ENGINE_CPLAYERMOVEMENTDEF_H
// New header (chunk Q9, row 0x006AB430 CThingPlayerCreature::GetMovementBandTypeFromJoytickMagnitude): the hero's movement def.
// RTTI .?AVCPlayerMovementDef@@ : CParentDefClassBase (ghidra_out/rtti_bases.tsv). Members: Ego_r.pdb CPlayerMovementDef
// (struct_layouts_egor.tsv, 128 bytes, all scalar) after the retail CParentDefClassBase (0x39, padded to 0x3C): Ego_r +0x40 .. +0x7C
// sit 4 lower in retail. Checked against retail 0x006AB430: JoystickMovementStationaryBand `fcomp [ecx+3Ch]` (0x006AB4B1),
// SlowBand [esi+40h], WalkingBand [ecx+44h], JoggingBand [esi+48h].
#include <stddef.h>
#include "rebuild_abi.h"
#include "engine/CParentDefClassBase.h"
class CPlayerMovementDef : public CParentDefClassBase {
public:
    float JoystickMovementStationaryBand;                                // +0x3c
    float JoystickMovementSlowBand;                                      // +0x40
    float JoystickMovementWalkingBand;                                   // +0x44
    float JoystickMovementJoggingBand;                                   // +0x48
    float MinRequiredAngleFor180;                                        // +0x4c
    float SecondsToStoreLastMovementFor;                                 // +0x50
    long  NoMovementFramesToCheckFor180;                                 // +0x54
    long  NoMovementFramesToCheckFor180PriorStraightLine;                // +0x58
    float MinNoRunSecondsForSkidHaltMove;                                // +0x5c
    long  NoRunningFramesToPreventSkidHalt;                              // +0x60
    float MaxAllowedAngleChangeForSkidHalt;                              // +0x64
    float MaxTurnSpeed;                                                  // +0x68
    float MaxTurnSpeedHeavyWeapon;                                       // +0x6c
    float LeanAngleTurnFactor;                                           // +0x70
    float LeanAngleLimitAngle;                                           // +0x74
    float LeanAngleStepPerFrame;                                         // +0x78
};
FABLE_STATIC_ASSERT(offsetof(CPlayerMovementDef, JoystickMovementStationaryBand) == 0x3c);
FABLE_STATIC_ASSERT(offsetof(CPlayerMovementDef, JoystickMovementJoggingBand) == 0x48);
FABLE_STATIC_ASSERT(sizeof(CPlayerMovementDef) == 0x7c);
#endif
