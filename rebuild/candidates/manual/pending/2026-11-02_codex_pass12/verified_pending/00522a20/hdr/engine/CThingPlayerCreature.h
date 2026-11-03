#ifndef FABLE_ENGINE_CTHINGPLAYERCREATURE_H
#define FABLE_ENGINE_CTHINGPLAYERCREATURE_H
#include "engine/CThingCreatureBase.h"
// Original sole base CThingCreatureBase; native Create006ac91c allocates208.
// Complete original tail ControlPos through MovementBandType is unused here.
class CThingPlayerCreature:public CThingCreatureBase {
#ifdef FABLE_RETAIL_INIT_THING_TYPES
private:
    C3DVector ControlPos; // +160
    C3DVector ControlMoveByVector; // +16c
    unsigned char MovedByPlayer:1,Jumping:1,Pad1:1; // +178
    unsigned char _pad_0x179[3];
    unsigned char ControlledMovementType[4]; // +17c EControlledMovementType
    C3DVector MovementAcceleration; // +180
    float MaxSlowWalkingSpeed; // +18c
    float MaxWalkingSpeed; // +190
    float MaxJoggingSpeed; // +194, native getter52abc0
    float MaxRunningSpeed; // +198
    float MaxSprintingSpeed; // +19c
    float MaxFlyingSpeed; // +1a0
    float MaxStrafingSpeed; // +1a4
    unsigned char RelativeMovementAccelerationComponents[12]; // +1a8 std::map<EGameAction,std::pair<C3DVector,long> >, ctor52ab35..52ab66
    C3DVector RelativeMovementAcceleration; // +1b4
    C3DVector LastRelativeMovementAcceleration; // +1c0
    bool CollidedWithThing; // +1cc
    unsigned char LastThingCollisionNormal[12]; // +1cd C3DVector, original unaligned member
    unsigned char LastThingCollisionPosition[12]; // +1d9 C3DVector, original unaligned member
    unsigned char _pad_0x1e5[3];
    unsigned char PHeroCombatDef[4]; // +1e8 CDefPointer<const CHeroCombatDef>, ctor52ab69
    unsigned char PPlayerMovementDef[4]; // +1ec CDefPointer<const CPlayerMovementDef>, ctor52ab72
protected:
    C3DVector ImpulseVelocity; // +1f0
    float LeanAngle; // +1fc
    unsigned char MovementBandType[4]; // +200 EMovementBandType
    unsigned char _pad_0x204[4];
#else
private: unsigned char _pad_0x160[0xa8];
#endif
public:
#ifdef FABLE_RETAIL_INIT_THING_TYPES
    static CClass* __fastcall Alloc();
#endif
    CThingPlayerCreature();
};
FABLE_STATIC_ASSERT(sizeof(CThingPlayerCreature)==0x208);
#endif
