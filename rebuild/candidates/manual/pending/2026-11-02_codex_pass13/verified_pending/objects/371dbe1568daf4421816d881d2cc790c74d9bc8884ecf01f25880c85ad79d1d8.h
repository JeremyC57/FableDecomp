#ifdef FABLE_RETAIL_REGION_CONNECTIVITY_TYPES
#ifndef FABLE_ENGINE_CTHINGPLAYERCREATURE_H
#define FABLE_ENGINE_CTHINGPLAYERCREATURE_H
#include "rebuild_abi.h"
#include "engine/CThingCreatureBase.h"
#include "engine/EControlledMovementType.h"
#include "engine/EMovementBandType.h"
#include "engine/EGameAction.h"
#include "engine/CDefPointer.h"
#include "stlport/fable_map.h"
class CHeroCombatDef;class CPlayerMovementDef;
// Original complete data members: both PDB receipts evidence/00501450_*_types.json.
class CThingPlayerCreature : public CThingCreatureBase {
private: C3DVector ControlPos;
private: C3DVector ControlMoveByVector;
private: unsigned char MovedByPlayer:1;
private: unsigned char Jumping:1;
private: unsigned char Pad1:1;
private: EControlledMovementType ControlledMovementType;
private: C3DVector MovementAcceleration;
private: float MaxSlowWalkingSpeed;
private: float MaxWalkingSpeed;
private: float MaxJoggingSpeed;
private: float MaxRunningSpeed;
private: float MaxSprintingSpeed;
private: float MaxFlyingSpeed;
private: float MaxStrafingSpeed;
private: std::map<enum EGameAction,std::pair<C3DVector,long>,std::less<enum EGameAction>,std::allocator<std::pair<enum EGameAction const ,std::pair<C3DVector,long> > > > RelativeMovementAccelerationComponents;
private: C3DVector RelativeMovementAcceleration;
private: C3DVector LastRelativeMovementAcceleration;
private: bool CollidedWithThing;
private: C3DVector LastThingCollisionNormal;
private: C3DVector LastThingCollisionPosition;
private: CDefPointer<CHeroCombatDef const > PHeroCombatDef;
private: CDefPointer<CPlayerMovementDef const > PPlayerMovementDef;
protected: C3DVector ImpulseVelocity;
protected: float LeanAngle;
protected: EMovementBandType MovementBandType;
public:CThingPlayerCreature();
};
FABLE_STATIC_ASSERT(sizeof(CThingPlayerCreature)==520);
#endif
#else
#ifndef FABLE_ENGINE_CTHINGPLAYERCREATURE_H
#define FABLE_ENGINE_CTHINGPLAYERCREATURE_H
#include "engine/CThingCreatureBase.h"
// Original sole base CThingCreatureBase; native Create006ac91c allocates208.
// Complete original tail ControlPos through MovementBandType is unused here.
class CThingPlayerCreature:public CThingCreatureBase {
private: unsigned char _pad_0x160[0xa8];
public: CThingPlayerCreature();
};
FABLE_STATIC_ASSERT(sizeof(CThingPlayerCreature)==0x208);
#endif
#endif
