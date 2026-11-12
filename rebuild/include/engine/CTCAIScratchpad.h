// Retail opt-in (Codex pass21): FABLE_RETAIL_CTCTARGETINGPLAYER_UPDATE_LIST exposes original targeting helpers; row00804020 retains both-PDB/donor/native proof.
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
#ifdef FABLE_RETAIL_CTCTARGETINGPLAYER_UPDATE_LIST
 static float GlobalTargetingDistanceOffset;
 static float __fastcall GetGlobalTargetingDistanceOffset(){return GlobalTargetingDistanceOffset;}
#endif
#ifdef FABLE_RETAIL_CTCAISCRATCHPAD_BETTING
 // Both PDB original static names and FableWin getters02ac3ed0/3f50/3f90; native013bae1d/20/24.
 static bool BettingActive;
 static long BettingAmount;
 static bool CountBetMoneyDown;
#endif
 static ETCInterfaceType FABLE_FASTCALL StatGetInterfaceType(){return TCI_AI_SCRATCHPAD;}
#ifdef FABLE_RETAIL_CTCAISCRATCHPAD_TARGETINGOFFSET
 // Retail opt-in (chunk 1025, row 0x00802B60 GetTargetingRangeForTarget): FableWin.pdb ?GetTargetingDistanceOffset@CTCAIScratchpad@@QBEMXZ,
 // inline in retail (`fadd dword ptr [eax+7Ch]`, 0x00802BFF). No layout change.
 float GetTargetingDistanceOffset()const{return TargetingDistanceOffset;}
#endif
 long GetActionFrequency()const{return ActionFrequency;}
 float GetActionFrequencyVariation()const{return (ActionFrequencyVariation);}
public: unsigned long ThankingPhrase;
#ifdef FABLE_RETAIL_CTCAISCRATCHPAD_GETTHANKINGPHRASE
    // FableWin.pdb ?GetThankingPhrase@CTCAIScratchpad@@QBEKXZ (inline in retail; row 0x00949380 CAIStateGroup_GoToPub::ThankBarmaid).
    unsigned long GetThankingPhrase() const { return ThankingPhrase; }
#endif
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
#ifdef FABLE_RETAIL_CTCAISCRATCHPAD_VSLOTS
    // agentFAM03 (2026-11-11): retail virtuals missing from this declaration, declaration only (no layout change).
    // Slots: the TLC vtable aligned with the Xbox 360 Test PDB order on TLC-named slots; spellings: Ego_r / FableWin name sets.
protected:
    virtual bool IsComponentSerialisable() const;  // slot 23, 0x007ACDD0
public:
#endif
};
FABLE_STATIC_ASSERT(sizeof(CTCAIScratchpad)==0x84);
#endif
