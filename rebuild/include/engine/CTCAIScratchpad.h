#ifndef FABLE_ENGINE_CTCAISCRATCHPAD_H
#define FABLE_ENGINE_CTCAISCRATCHPAD_H
#include "engine/CTCBase.h"
#include "engine/C3DVector.h"
#include "engine/CDefPointer.h"
#include "engine/EScriptingStateGroups.h"
#include "fable_string.h"
class CAIScratchpadDef;
class CTCAIScratchpad:public CTCBase {
public:
 static ETCInterfaceType FABLE_FASTCALL StatGetInterfaceType(){return TCI_AI_SCRATCHPAD;}
 long GetActionFrequency()const{return ActionFrequency;}
 float GetActionFrequencyVariation()const{return (ActionFrequencyVariation);}
public: unsigned long ThankingPhrase;
public: unsigned long IgnoringPhrase;
public: C3DVector WanderCentrePoint;
public: float WanderMinDistance;
public: float WanderMaxDistance;
public: long GossipCounter;
public: long MaxGossipPhrase;
public: unsigned long WarningPhrase;
public: unsigned long BeerRequestPhrase;
public: EScriptingStateGroups ScriptingStateGroup;
public: float MaxHeroReactionDistance;
public: long ActionFrequency;
public: float ActionFrequencyVariation;
public: CCharString Action;
public: bool FaceHeroForAction;
public: CCharString TargetName;
public: float FollowDistance;
public: bool AttackHeroOnSight;
public: long TimeToSpendHarassingHero;
public: float CombatNearbyEnemyFleeingBreakOffRange;
public: float CombatNearbyBreakOffRange;
public: bool StealStealableItems;
public: bool RecoverStealableItems;
public: bool TakeStealableItemToRandomDestination;
public: bool KillSelfAndStealableItemAfterReachingDestination;
public: bool AllowedToFollow;
public: CCharString TableName;
public: CCharString SeatName;
public: bool DisableHeadLooking;
public: bool IsPushableByHero;
public: bool LookForFiniteTime;
public: bool AvoidRegionExits;
public: float TargetingDistanceOffset;
private: CDefPointer<const CAIScratchpadDef> PDef;
};
FABLE_STATIC_ASSERT(sizeof(CTCAIScratchpad)==0x84);
#endif
