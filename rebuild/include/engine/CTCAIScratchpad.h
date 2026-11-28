// Retail opt-in (Codex pass21): FABLE_RETAIL_CTCTARGETINGPLAYER_UPDATE_LIST exposes original targeting helpers; row00804020 retains both-PDB/donor/native proof.
#ifndef FABLE_ENGINE_CTCAISCRATCHPAD_H
#define FABLE_ENGINE_CTCAISCRATCHPAD_H
#include "engine/CTCBase.h"
#include "engine/C3DVector.h"
#include "engine/CDefPointer.h"
#include "engine/EScriptingStateGroups.h"
#include "fable_string.h"
#if defined(FABLE_RETAIL_CTCAISCRATCHPAD_GOSSIP_MAPS) || defined(FABLE_RETAIL_CTCAISCRATCHPAD_GOSSIP_BOOL_MAPS)
#include "stlport/fable_map.h"
#endif
class CAIScratchpadDef;
class CTCAIScratchpad:public CTCBase {
public:
#ifdef FABLE_RETAIL_MINION_FOLLOW_UPDATE_PASS29
    CCharString GetTargetName() const { return TargetName; }
    EScriptingStateGroups GetScriptingStateGroup() const { return ScriptingStateGroup; }
    float GetFollowDistance() const { return FollowDistance; }
#endif
#ifdef FABLE_RETAIL_DRINK_FIXTURE_PASS27
public:
    CTCAIScratchpad(CThing&);
#endif
#ifdef FABLE_RETAIL_CTCAISCRATCHPAD_GOSSIP_MAPS
    // Opt-in (NF48 round 7, rows 0x008AA010 / 0x008AA270 / 0x008AA4D0 CGameScriptInterface::ClearGossip*): the static gossip maps,
    // Ego_r.pdb `dynamic initializer for 'CTCAIScratchpad::Gossip'` / GossipVillages / GossipFactions (0x0100CD00..0x0100CE00 in that
    // build); retail 0x013BAE44 / 0x013BAE50 / 0x013BAE5C, 12-byte STLport maps keyed by name, each value the vector<CCharString>
    // CGameScriptInterface::GetGossip / GetGossipVillages / GetGossipFactions return. Declarations only.
    static std::map<CCharString, std::vector<CCharString> > Gossip;
    static std::map<CCharString, std::vector<CCharString> > GossipVillages;
    static std::map<CCharString, std::vector<CCharString> > GossipFactions;
#endif
#ifdef FABLE_RETAIL_CTCAISCRATCHPAD_ADDGOSSIP
    // NF208 opt-in (2026-11-20, FP-VECTORINSERT, row 0x008AE5E0): FableWin.pdb ?AddGossip@CTCAIScratchpad@@SIXVCCharString@@0@Z
    // (public static __fastcall, two by-value strings; retail 0x008AE5E0 `ret 8`, `Gossip[name].push_back(value)`). Declaration only.
    static void FABLE_FASTCALL AddGossip(CCharString name, CCharString value);
#endif
#ifdef FABLE_RETAIL_CTCAISCRATCHPAD_ADDGOSSIPVILLAGES
    // NF208 opt-in (2026-11-20, row 0x008AE810): FableWin.pdb ?AddGossipVillages@CTCAIScratchpad@@SIXVCCharString@@0@Z. Declaration only.
    static void FABLE_FASTCALL AddGossipVillages(CCharString name, CCharString value);
#endif
#ifdef FABLE_RETAIL_CTCAISCRATCHPAD_ADDGOSSIPFACTIONS
    // NF208 opt-in (2026-11-20, row 0x008AEA40): FableWin.pdb ?AddGossipFactions@CTCAIScratchpad@@SIXVCCharString@@0@Z. Declaration only.
    static void FABLE_FASTCALL AddGossipFactions(CCharString name, CCharString value);
#endif
#ifdef FABLE_RETAIL_CTCAISCRATCHPAD_GOSSIP_BOOL_MAPS
    // NF87 opt-in (rows 0x008A96C0 CGameScriptInterface::SetCategoryActivity / 0x008A9720 SetActiveGossipCategories): the two static
    // flag maps, FableWin.pdb `dynamic initializer for 'CTCAIScratchpad::ActiveGossipCategories'` / `..IsGossipForPlayer'` and Ego_d.pdb
    // ?ActiveGossipCategories@CTCAIScratchpad@@2V?$map@VCCharString@@_N..@@A / ?IsGossipForPlayer@..; retail 0x013BAE2C / 0x013BAE38
    // (12-byte STLport maps). Declarations only.
    static std::map<CCharString, bool> ActiveGossipCategories;
    static std::map<CCharString, bool> IsGossipForPlayer;
#endif
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
#ifdef FABLE_RETAIL_AVOID_REGION_FRAME_PASS29
 void SetTargetingDistanceOffset(float distance){TargetingDistanceOffset=distance;}
#endif
 static ETCInterfaceType FABLE_FASTCALL StatGetInterfaceType(){return TCI_AI_SCRATCHPAD;}
#ifdef FABLE_RETAIL_CTCAISCRATCHPAD_TARGETINGOFFSET
 // Retail opt-in (chunk 1025, row 0x00802B60 GetTargetingRangeForTarget): FableWin.pdb ?GetTargetingDistanceOffset@CTCAIScratchpad@@QBEMXZ,
 // inline in retail (`fadd dword ptr [eax+7Ch]`, 0x00802BFF). No layout change.
 float GetTargetingDistanceOffset()const{return TargetingDistanceOffset;}
#endif
 long GetActionFrequency()const{return ActionFrequency;}
#ifdef FABLE_RETAIL_CTCAISCRATCHPAD_TABLESEAT
 // Opt-in (chunk 2020, row 0x00949AB0 CAIStateGroup_GoToPub::IsRunnable): FableWin.pdb ?GetTableName@CTCAIScratchpad@@QBE?AVCCharString@@XZ /
 // ?GetSeatName@CTCAIScratchpad@@QBE?AVCCharString@@XZ (by value), inline in retail: the copy constructor 0x0099EC30 on &TableName (+0x70) /
 // &SeatName (+0x74) straight into the caller's temporary (0x00949B1B / 0x00949B3F / 0x00949B9A). No layout change.
 CCharString GetTableName()const{return TableName;}
 CCharString GetSeatName()const{return SeatName;}
#endif
 float GetActionFrequencyVariation()const{return (ActionFrequencyVariation);}
public: unsigned long ThankingPhrase;
#ifdef FABLE_RETAIL_CTCAISCRATCHPAD_GETTHANKINGPHRASE
    // FableWin.pdb ?GetThankingPhrase@CTCAIScratchpad@@QBEKXZ (inline in retail; row 0x00949380 CAIStateGroup_GoToPub::ThankBarmaid).
    unsigned long GetThankingPhrase() const { return ThankingPhrase; }
#endif
public: unsigned long IgnoringPhrase;
#ifdef FABLE_RETAIL_BUY_FROM_SELLER_UPDATE_PASS30
    unsigned long GetIgnoringPhrase() const { return IgnoringPhrase; }
#endif
public: C3DVector WanderCentrePoint;
public: float WanderMinDistance;
public: float WanderMaxDistance;
public: long GossipCounter;
public: long MaxGossipPhrase;
public: unsigned long WarningPhrase;
public: unsigned long BeerRequestPhrase;
#ifdef FABLE_RETAIL_GOTOPUB_DRINK_PASS27
    unsigned long GetBeerRequestPhrase() const { return BeerRequestPhrase; }
#endif
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
#ifdef FABLE_RETAIL_CTCAISCRATCHPAD_SETISPUSHABLEBYHERO
    // agentC29 (row 0x00945060 CAIStateGroup_WorkHammerAnvil::Update): Ego_r.pdb / FableWin.pdb ?SetIsPushableByHero@CTCAIScratchpad@@QAEX_N@Z,
    // inline in retail (`mov byte ptr [eax+79h],0` 0x00945223 / `,1` 0x0094533D on the looked-up TCI_AI_SCRATCHPAD component). No layout change.
public: void SetIsPushableByHero(bool pushable) { IsPushableByHero = pushable; }
#endif
};
FABLE_STATIC_ASSERT_AT(83, sizeof(CTCAIScratchpad)==0x84);
#endif
