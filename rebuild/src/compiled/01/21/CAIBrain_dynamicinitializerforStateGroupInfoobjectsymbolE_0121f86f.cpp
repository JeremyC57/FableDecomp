// CAIBrain::`dynamic initializer for 'StateGroupInfo'' -- build the 203-entry state-group registration table: per state group its name (the class's GetName()) and its allocator (the class's Alloc), closed by an empty name with a null allocator; then register the table's destructor with atexit
// Leaf: _$E1
// Retail: Fable.exe (TLC, VC7.1) @ 0x0121F86F, 8142 bytes (/Os: `push ebp ; mov ebp,esp`, packed at an odd address after the
//   LSocket::GlobalList initializer 0x0121F850): 202 x (push Alloc ; GetName into the argument slot ; CAIStateGroupInfo::
//   CAIStateGroupInfo(CCharString, alloc) 0x008FD21F into the [ebp-8] temporary ; copy constructor 0x008FE940 into the entry ;
//   ~CCharString 0x0099EAE0), the sentinel built from CCharString("", -1) and 0, then `push 0x0122A20A ; call _atexit` and
//   `leave ; ret` at 0x0122183C (extent proven: the next byte starts the following function).
// Identity (proven): Ego_r.pdb 0x0101368D and FableWin.pdb 0x03FFAA4B name `dynamic initializer for 'CAIBrain::StateGroupInfo''
//   (its atexit partner 0x0122A20A is the landed CAIBrain::DestroyStateGroupInfo `_$E2`); the 8-byte entries at 0x013BB008 are
//   read by CAIBrain::InitStateGroups 0x0088BF30. Each entry's GetName / Alloc pair belongs to one class: the Alloc body's vtable
//   store gives the class's RTTI name (Fable.exe), which equals the class of the landed GetName row at the same slot for 191 of 202
//   entries; every ?Alloc@<Class>@@SIPAVCAIStateGroupBase@@XZ is in Ego_r.pdb and FableWin.pdb, every ?GetName@<Class>@@SI?AV
//   CCharString@@XZ in FableWin.pdb (Ego_r inlines GetName). Full list in LABELS.tsv (11 landed GetName rows misnamed: NAME_FIXES).
// Shape: CAIStateGroupInfo's constructor takes the name by value, so each GetName() result is built straight into the argument
//   slot; VC7.1 copies each temporary into the array element through the implicit copy constructor. The TU is size-optimised (the
//   landed same-TU `_$E2` 0x0122A20A carries the same pragma; under /O2 this initializer is 10723 bytes).
// Headers: the 131 state groups that have engine/<Class>.h are included (they compose in one TU once the CAIStateGroupBase
//   retail model is on: FABLE_RETAIL_CAISTATEGROUPBASE + _MEMBERS give the 0x14-byte base the derived headers' size asserts need
//   and declare EScriptAIPriority, FABLE_RETAIL_CINTELLIGENTPOINTER gives CIntelligentPointer<T>); their static GetName / Alloc
//   come from the additive overlay opt-in FABLE_RETAIL_CAISTATEGROUP_GETNAME_ALLOC (hdr/README.md; 5 headers already declare
//   both). The 71 state groups with no shared header are declared below with their two static members only (no layout).
// Devices (not chosen by grading; none changes the bytes, the grade was RELOCATION_MATCH before and after):
//   - `class CThingCreatureBase;` forward declaration: CAIStateGroup_KidReactToBlockage.h / _SellToBuyer.h name
//     CIntelligentPointer<CThingCreatureBase> without declaring the class (header gap, not a steering declaration).
//   - the three base opt-in #defines (CAISTATEGROUPBASE, _MEMBERS, CINTELLIGENTPOINTER) exist only to make the included headers
//     compile. FITTED spellings: none.
#pragma optimize("s", on)
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CAISTATEGROUPBASE
#define FABLE_RETAIL_CAISTATEGROUPBASE_MEMBERS
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CAISTATEGROUP_GETNAME_ALLOC
#include "engine/CAIStateGroupInfo.h"
#define FABLE_RETAIL_CAIBRAIN_STATEGROUPINFO
#include "engine/CAIBrain.h"
class CThingCreatureBase;
#include "engine/CAIStateGroup_MinionWander.h"
#include "engine/CAIStateGroup_MinionSleep.h"
#include "engine/CAIStateGroup_GoToBed.h"
#include "engine/CAIStateGroup_MinionWorkInInn.h"
#include "engine/CAIStateGroup_MinionReactToPlayerInteraction.h"
#include "engine/CAIStateGroup_MinionGuardHeardYellForHelp.h"
#include "engine/CAIStateGroup_MinionReactToCriminal.h"
#include "engine/CAIStateGroup_MinionGuardReactToCriminal.h"
#include "engine/CAIStateGroup_MinionGuardReactToBribe.h"
#include "engine/CAIStateGroup_PerformAction.h"
#include "engine/CAIStateGroup_MinionWatchFires.h"
#include "engine/CAIStateGroup_MinionHasInformation.h"
#include "engine/CAIStateGroup_MinionSitOnSeat.h"
#include "engine/CAIStateGroup_MinionSitAtInn.h"
#include "engine/CAIStateGroup_MinionSitOutside.h"
#include "engine/CAIStateGroup_MinionListenToStory.h"
#include "engine/CAIStateGroup_SitRoundCampfire.h"
#include "engine/CAIStateGroup_MinionGossip.h"
#include "engine/CAIStateGroup_BanditGossip.h"
#include "engine/CAIStateGroup_MinionKidAnnoy.h"
#include "engine/CAIStateGroup_MinionKidGroupTag.h"
#include "engine/CAIStateGroup_MinionKidCalledToMother.h"
#include "engine/CAIStateGroup_CreatureFindCover.h"
#include "engine/CAIStateGroup_ChickenBeg.h"
#include "engine/CAIStateGroup_BirdGroundWander.h"
#include "engine/CAIStateGroup_MeleeCombatNearby.h"
#include "engine/CAIStateGroup_MeleeCombatNearby2.h"
#include "engine/CAIStateGroup_MeleeCombatNearbyAttackWhenVeryClose.h"
#include "engine/CAIStateGroup_SeekEnemy.h"
#include "engine/CAIStateGroup_SeekScriptTarget.h"
#include "engine/CAIStateGroup_StealthSeekEnemy.h"
#include "engine/CAIStateGroup_RangedCombatUsePreferredFirePoint.h"
#include "engine/CAIStateGroup_CreatureRangedCombatUseWillShield.h"
#include "engine/CAIStateGroup_VillageTaskTorch.h"
#include "engine/CAIStateGroup_VillageTaskPickCrops.h"
#include "engine/CAIStateGroup_VillageTaskHorn.h"
#include "engine/CAIStateGroup_KidWanderNearKids.h"
#include "engine/CAIStateGroup_KidHurtSelfRunToMother.h"
#include "engine/CAIStateGroup_KidReactToBlockage.h"
#include "engine/CAIStateGroup_IdleAtHome.h"
#include "engine/CAIStateGroup_WorkAtSchool.h"
#include "engine/CAIStateGroup_WorkSawingWood.h"
#include "engine/CAIStateGroup_WorkHammerAnvil.h"
#include "engine/CAIStateGroup_ThrowLogsOnFire.h"
#include "engine/CAIStateGroup_CleanHouse.h"
#include "engine/CAIStateGroup_PrepareMeal.h"
#include "engine/CAIStateGroup_GazeAtHome.h"
#include "engine/CAIStateGroup_GazeAtWork.h"
#include "engine/CAIStateGroup_GazeOutside.h"
#include "engine/CAIStateGroup_WanderAroundShops.h"
#include "engine/CAIStateGroup_GoToPub.h"
#include "engine/CAIStateGroup_SitInPub.h"
#include "engine/CAIStateGroup_LightHomeCandle.h"
#include "engine/CAIStateGroup_LightWorkCandle.h"
#include "engine/CAIStateGroup_SeekPerceivedThing.h"
#include "engine/CAIStateGroup_FollowPlayer.h"
#include "engine/CAIStateGroup_BuyFromSeller.h"
#include "engine/CAIStateGroup_SellToBuyer.h"
#include "engine/CAIStateGroup_HandyMan.h"
#include "engine/CAIStateGroup_LookAtInterestingThings.h"
#include "engine/CAIStateGroup_StandStill.h"
#include "engine/CAIStateGroup_BalverineCornered.h"
#include "engine/CAIStateGroup_BalverineJumpAtFleeingEnemy.h"
#include "engine/CAIStateGroup_HoverWander.h"
#include "engine/CAIStateGroup_HoverWanderAroundThing.h"
#include "engine/CAIStateGroup_HoverAttack.h"
#include "engine/CAIStateGroup_HoverAttackUnlessTargetIsFleeing.h"
#include "engine/CAIStateGroup_HoverKeepInArea.h"
#include "engine/CAIStateGroup_HoverAttackArea.h"
#include "engine/CAIStateGroup_HoverHornetQueenTakeDamage.h"
#include "engine/CAIStateGroup_HoverHornetQueen.h"
#include "engine/CAIStateGroup_EarthTroll.h"
#include "engine/CAIStateGroup_RockTroll.h"
#include "engine/CAIStateGroup_IceTrollIcicleAttack.h"
#include "engine/CAIStateGroup_IceTrollPoundGroundAttack.h"
#include "engine/CAIStateGroup_IceTrollIcicleAssault.h"
#include "engine/CAIStateGroup_ScorpionKing.h"
#include "engine/CAIStateGroup_JackOfBladesStage1.h"
#include "engine/CAIStateGroup_JackOfBladesStage2.h"
#include "engine/CAIStateGroup_MazeTeleport.h"
#include "engine/CAIStateGroup_MazeRangedCombat.h"
#include "engine/CAIStateGroup_MazeDefensiveMeleeCombat.h"
#include "engine/CAIStateGroup_MazeShieldedChase.h"
#include "engine/CAIStateGroup_MazeTutorialBattle.h"
#include "engine/CAIStateGroup_MazeTutorialBattleTeleport.h"
#include "engine/CAIStateGroup_MazeTutorialBattleRespondToLightning.h"
#include "engine/CAIStateGroup_MazeTutorialBattleUseShield.h"
#include "engine/CAIStateGroup_Guildmaster.h"
#include "engine/CAIStateGroup_BriarRose.h"
#include "engine/CAIStateGroup_BriarRoseWillUse.h"
#include "engine/CAIStateGroup_VersusScorpionKing.h"
#include "engine/CAIStateGroup_CreatureRangedCombat2.h"
#include "engine/CAIStateGroup_CreatureTakeCover.h"
#include "engine/CAIStateGroup_CreatureAdvanceInCover.h"
#include "engine/CAIStateGroup_CreatureRangedCombatFireAtSpecificTargets.h"
#include "engine/CAIStateGroup_TestTestTest.h"
#include "engine/CAIStateGroup_BeetleToggleHover.h"
#include "engine/CAIStateGroup_MinionReactToTrespassing.h"
#include "engine/CAIStateGroup_MinionReactToNoise.h"
#include "engine/CAIStateGroup_MinionReactToCrime.h"
#include "engine/CAIStateGroup_MinionLeaveRegion.h"
#include "engine/CAIStateGroup_ReactToDeadLeader.h"
#include "engine/CAIStateGroup_OpinionReactionRoot.h"
#include "engine/CAIStateGroup_DeedReactionRoot.h"
#include "engine/CAIStateGroup_BanditKingCharge.h"
#include "engine/CAIStateGroup_BanditKingLunge.h"
#include "engine/CAIStateGroup_BanditKingThrowWeapons.h"
#include "engine/CAIStateGroup_JackDragon.h"
#include "engine/CAIStateGroup_MinionFollowThing.h"
#include "engine/CAIStateGroup_TavernServer.h"
#include "engine/CAIStateGroup_TavernKeeper.h"
#include "engine/CAIStateGroup_StealStealableItem.h"
#include "engine/CAIStateGroup_RecoverStealableItem.h"
#include "engine/CAIStateGroup_CarryCrateAndFollow.h"
#include "engine/CAIStateGroup_WhisperRetreat.h"
#include "engine/CAIStateGroup_WhisperPotionCombat.h"
#include "engine/CAIStateGroup_Thunder.h"
#include "engine/CAIStateGroup_Drunk.h"
#include "engine/CAIStateGroup_ServeFoodAtHome.h"
#include "engine/CAIStateGroup_ServeFoodAtTavern.h"
#include "engine/CAIStateGroup_EatAtHome.h"
#include "engine/CAIStateGroup_WifeGiveYouGift.h"
#include "engine/CAIStateGroup_BanditPlayDice.h"
#include "engine/CAIStateGroup_BanditPlot.h"
#include "engine/CAIStateGroup_MinionGuardGreetPlayer.h"
#include "engine/CAIStateGroup_ApprenticeHero.h"
#include "engine/CAIStateGroup_OnHeroPush.h"
#include "engine/CAIStateGroup_AvoidRegionExit.h"
#include "engine/CAIStateGroup_SummonerLightningAttack.h"
#include "engine/CAIStateGroup_SummonerSummon.h"
#include "engine/CAIStateGroup_SummonerCharge.h"
// The 71 state groups without a shared header: declared with their two static members only.

class CAIStateGroup_MinionFindPersonalSpace { public: static CCharString FABLE_FASTCALL GetName(); static CAIStateGroupBase* FABLE_FASTCALL Alloc(); };
class CAIStateGroup_MinionGuardReactToPlayerWithWeapon { public: static CCharString FABLE_FASTCALL GetName(); static CAIStateGroupBase* FABLE_FASTCALL Alloc(); };
class CAIStateGroup_MinionGuardReactToBodyFoundMessage { public: static CCharString FABLE_FASTCALL GetName(); static CAIStateGroupBase* FABLE_FASTCALL Alloc(); };
class CAIStateGroup_BanditSitRoundCampfire { public: static CCharString FABLE_FASTCALL GetName(); static CAIStateGroupBase* FABLE_FASTCALL Alloc(); };
class CAIStateGroup_MinionFaceHero { public: static CCharString FABLE_FASTCALL GetName(); static CAIStateGroupBase* FABLE_FASTCALL Alloc(); };
class CAIStateGroup_MinionPutOutFires { public: static CCharString FABLE_FASTCALL GetName(); static CAIStateGroupBase* FABLE_FASTCALL Alloc(); };
class CAIStateGroup_MinionReactToAlert { public: static CCharString FABLE_FASTCALL GetName(); static CAIStateGroupBase* FABLE_FASTCALL Alloc(); };
class CAIStateGroup_MinionReactToEnemy { public: static CCharString FABLE_FASTCALL GetName(); static CAIStateGroupBase* FABLE_FASTCALL Alloc(); };
class CAIStateGroup_MinionReactToCorpse { public: static CCharString FABLE_FASTCALL GetName(); static CAIStateGroupBase* FABLE_FASTCALL Alloc(); };
class CAIStateGroup_MinionReactToUnconsciousPerson { public: static CCharString FABLE_FASTCALL GetName(); static CAIStateGroupBase* FABLE_FASTCALL Alloc(); };
class CAIStateGroup_MinionSitAtWork { public: static CCharString FABLE_FASTCALL GetName(); static CAIStateGroupBase* FABLE_FASTCALL Alloc(); };
class CAIStateGroup_MinionSitAtHome { public: static CCharString FABLE_FASTCALL GetName(); static CAIStateGroupBase* FABLE_FASTCALL Alloc(); };
class CAIStateGroup_MinionSitAtSchool { public: static CCharString FABLE_FASTCALL GetName(); static CAIStateGroupBase* FABLE_FASTCALL Alloc(); };
class CAIStateGroup_MinionCuriosity { public: static CCharString FABLE_FASTCALL GetName(); static CAIStateGroupBase* FABLE_FASTCALL Alloc(); };
class CAIStateGroup_MinionMotherProtectFamily { public: static CCharString FABLE_FASTCALL GetName(); static CAIStateGroupBase* FABLE_FASTCALL Alloc(); };
class CAIStateGroup_ChickenPanic { public: static CCharString FABLE_FASTCALL GetName(); static CAIStateGroupBase* FABLE_FASTCALL Alloc(); };
class CAIStateGroup_FireflyWander { public: static CCharString FABLE_FASTCALL GetName(); static CAIStateGroupBase* FABLE_FASTCALL Alloc(); };
class CAIStateGroup_ButterflyWander { public: static CCharString FABLE_FASTCALL GetName(); static CAIStateGroupBase* FABLE_FASTCALL Alloc(); };
class CAIStateGroup_SmallBirdWander { public: static CCharString FABLE_FASTCALL GetName(); static CAIStateGroupBase* FABLE_FASTCALL Alloc(); };
class CAIStateGroup_BirdFlyToFood { public: static CCharString FABLE_FASTCALL GetName(); static CAIStateGroupBase* FABLE_FASTCALL Alloc(); };
class CAIStateGroup_BirdStartled { public: static CCharString FABLE_FASTCALL GetName(); static CAIStateGroupBase* FABLE_FASTCALL Alloc(); };
class CAIStateGroup_Guard { public: static CCharString FABLE_FASTCALL GetName(); static CAIStateGroupBase* FABLE_FASTCALL Alloc(); };
class CAIStateGroup_Wander { public: static CCharString FABLE_FASTCALL GetName(); static CAIStateGroupBase* FABLE_FASTCALL Alloc(); };
class CAIStateGroup_ChickenWander { public: static CCharString FABLE_FASTCALL GetName(); static CAIStateGroupBase* FABLE_FASTCALL Alloc(); };
class CAIStateGroup_WanderAroundInitialPos { public: static CCharString FABLE_FASTCALL GetName(); static CAIStateGroupBase* FABLE_FASTCALL Alloc(); };
class CAIStateGroup_NonVillageGuardPatrol { public: static CCharString FABLE_FASTCALL GetName(); static CAIStateGroupBase* FABLE_FASTCALL Alloc(); };
class CAIStateGroup_VillageTaskPatrol { public: static CCharString FABLE_FASTCALL GetName(); static CAIStateGroupBase* FABLE_FASTCALL Alloc(); };
class CAIStateGroup_VillageTaskSecuritySweep { public: static CCharString FABLE_FASTCALL GetName(); static CAIStateGroupBase* FABLE_FASTCALL Alloc(); };
class CAIStateGroup_KidWatchHopscotch { public: static CCharString FABLE_FASTCALL GetName(); static CAIStateGroupBase* FABLE_FASTCALL Alloc(); };
class CAIStateGroup_KidPlayHopscotch { public: static CCharString FABLE_FASTCALL GetName(); static CAIStateGroupBase* FABLE_FASTCALL Alloc(); };
class CAIStateGroup_TellStory { public: static CCharString FABLE_FASTCALL GetName(); static CAIStateGroupBase* FABLE_FASTCALL Alloc(); };
class CAIStateGroup_WorkInWarehouse { public: static CCharString FABLE_FASTCALL GetName(); static CAIStateGroupBase* FABLE_FASTCALL Alloc(); };
class CAIStateGroup_StayNearSchool { public: static CCharString FABLE_FASTCALL GetName(); static CAIStateGroupBase* FABLE_FASTCALL Alloc(); };
class CAIStateGroup_CleanWork { public: static CCharString FABLE_FASTCALL GetName(); static CAIStateGroupBase* FABLE_FASTCALL Alloc(); };
class CAIStateGroup_SitForMeal { public: static CCharString FABLE_FASTCALL GetName(); static CAIStateGroupBase* FABLE_FASTCALL Alloc(); };
class CAIStateGroup_StayNearFoodHall { public: static CCharString FABLE_FASTCALL GetName(); static CAIStateGroupBase* FABLE_FASTCALL Alloc(); };
class CAIStateGroup_MinionHawkingFromStall { public: static CCharString FABLE_FASTCALL GetName(); static CAIStateGroupBase* FABLE_FASTCALL Alloc(); };
class CAIStateGroup_GameMaster { public: static CCharString FABLE_FASTCALL GetName(); static CAIStateGroupBase* FABLE_FASTCALL Alloc(); };
class CAIStateGroup_BalverineWander { public: static CCharString FABLE_FASTCALL GetName(); static CAIStateGroupBase* FABLE_FASTCALL Alloc(); };
class CAIStateGroup_NymphFloatAround { public: static CCharString FABLE_FASTCALL GetName(); static CAIStateGroupBase* FABLE_FASTCALL Alloc(); };
class CAIStateGroup_NymphAttack { public: static CCharString FABLE_FASTCALL GetName(); static CAIStateGroupBase* FABLE_FASTCALL Alloc(); };
class CAIStateGroup_HoverAllAroundTest { public: static CCharString FABLE_FASTCALL GetName(); static CAIStateGroupBase* FABLE_FASTCALL Alloc(); };
class CAIStateGroup_JackOfBladesStage3 { public: static CCharString FABLE_FASTCALL GetName(); static CAIStateGroupBase* FABLE_FASTCALL Alloc(); };
class CAIStateGroup_VersusRockTroll { public: static CCharString FABLE_FASTCALL GetName(); static CAIStateGroupBase* FABLE_FASTCALL Alloc(); };
class CAIStateGroup_CreatureGoUnderground { public: static CCharString FABLE_FASTCALL GetName(); static CAIStateGroupBase* FABLE_FASTCALL Alloc(); };
class CAIStateGroup_CreatureIdleUnderground { public: static CCharString FABLE_FASTCALL GetName(); static CAIStateGroupBase* FABLE_FASTCALL Alloc(); };
class CAIStateGroup_CreatureSeekEnemyUnderground { public: static CCharString FABLE_FASTCALL GetName(); static CAIStateGroupBase* FABLE_FASTCALL Alloc(); };
class CAIStateGroup_MinionBehaviourGreetCreature { public: static CCharString FABLE_FASTCALL GetName(); static CAIStateGroupBase* FABLE_FASTCALL Alloc(); };
class CAIStateGroup_MinionBehaviourSad { public: static CCharString FABLE_FASTCALL GetName(); static CAIStateGroupBase* FABLE_FASTCALL Alloc(); };
class CAIStateGroup_MinionBehaviourPodiumEmoteNeutral { public: static CCharString FABLE_FASTCALL GetName(); static CAIStateGroupBase* FABLE_FASTCALL Alloc(); };
class CAIStateGroup_MinionMotherConsoleChild { public: static CCharString FABLE_FASTCALL GetName(); static CAIStateGroupBase* FABLE_FASTCALL Alloc(); };
class CAIStateGroup_MinionReactToBurglary { public: static CCharString FABLE_FASTCALL GetName(); static CAIStateGroupBase* FABLE_FASTCALL Alloc(); };
class CAIStateGroup_MinionReactToApology { public: static CCharString FABLE_FASTCALL GetName(); static CAIStateGroupBase* FABLE_FASTCALL Alloc(); };
class CAIStateGroup_MinionReactToCrimeFromSleep { public: static CCharString FABLE_FASTCALL GetName(); static CAIStateGroupBase* FABLE_FASTCALL Alloc(); };
class CAIStateGroup_BoastingCrowdMember { public: static CCharString FABLE_FASTCALL GetName(); static CAIStateGroupBase* FABLE_FASTCALL Alloc(); };
class CAIStateGroup_DeedReactionRootFromSleep { public: static CCharString FABLE_FASTCALL GetName(); static CAIStateGroupBase* FABLE_FASTCALL Alloc(); };
class CAIStateGroup_MinionWalkToMarker { public: static CCharString FABLE_FASTCALL GetName(); static CAIStateGroupBase* FABLE_FASTCALL Alloc(); };
class CAIStateGroup_SleepOnFloor { public: static CCharString FABLE_FASTCALL GetName(); static CAIStateGroupBase* FABLE_FASTCALL Alloc(); };
class CAIStateGroup_Freeze { public: static CCharString FABLE_FASTCALL GetName(); static CAIStateGroupBase* FABLE_FASTCALL Alloc(); };
class CAIStateGroup_EatAtTavern { public: static CCharString FABLE_FASTCALL GetName(); static CAIStateGroupBase* FABLE_FASTCALL Alloc(); };
class CAIStateGroup_ScreamerPursueTarget { public: static CCharString FABLE_FASTCALL GetName(); static CAIStateGroupBase* FABLE_FASTCALL Alloc(); };
class CAIStateGroup_KrakenAttack { public: static CCharString FABLE_FASTCALL GetName(); static CAIStateGroupBase* FABLE_FASTCALL Alloc(); };
class CAIStateGroup_KrakenTentacleIdle { public: static CCharString FABLE_FASTCALL GetName(); static CAIStateGroupBase* FABLE_FASTCALL Alloc(); };
class CAIStateGroup_KrakenTentacleRise { public: static CCharString FABLE_FASTCALL GetName(); static CAIStateGroupBase* FABLE_FASTCALL Alloc(); };
class CAIStateGroup_KrakenTentacleAttack { public: static CCharString FABLE_FASTCALL GetName(); static CAIStateGroupBase* FABLE_FASTCALL Alloc(); };
class CAIStateGroup_OpenAndCloseShop { public: static CCharString FABLE_FASTCALL GetName(); static CAIStateGroupBase* FABLE_FASTCALL Alloc(); };
class CAIStateGroup_ShopkeeperWatchPlayer { public: static CCharString FABLE_FASTCALL GetName(); static CAIStateGroupBase* FABLE_FASTCALL Alloc(); };
class CAIStateGroup_SetupWares { public: static CCharString FABLE_FASTCALL GetName(); static CAIStateGroupBase* FABLE_FASTCALL Alloc(); };
class CAIStateGroup_MinionGuardFollowPlayer { public: static CCharString FABLE_FASTCALL GetName(); static CAIStateGroupBase* FABLE_FASTCALL Alloc(); };
class CAIStateGroup_MinionGuardBribed { public: static CCharString FABLE_FASTCALL GetName(); static CAIStateGroupBase* FABLE_FASTCALL Alloc(); };
class CAIStateGroup_SkillPractice { public: static CCharString FABLE_FASTCALL GetName(); static CAIStateGroupBase* FABLE_FASTCALL Alloc(); };

const CAIStateGroupInfo CAIBrain::StateGroupInfo[203] = {
    CAIStateGroupInfo(CAIStateGroup_MinionWander::GetName(), CAIStateGroup_MinionWander::Alloc),
    CAIStateGroupInfo(CAIStateGroup_MinionSleep::GetName(), CAIStateGroup_MinionSleep::Alloc),
    CAIStateGroupInfo(CAIStateGroup_GoToBed::GetName(), CAIStateGroup_GoToBed::Alloc),
    CAIStateGroupInfo(CAIStateGroup_MinionWorkInInn::GetName(), CAIStateGroup_MinionWorkInInn::Alloc),
    CAIStateGroupInfo(CAIStateGroup_MinionFindPersonalSpace::GetName(), CAIStateGroup_MinionFindPersonalSpace::Alloc),
    CAIStateGroupInfo(CAIStateGroup_MinionReactToPlayerInteraction::GetName(), CAIStateGroup_MinionReactToPlayerInteraction::Alloc),
    CAIStateGroupInfo(CAIStateGroup_MinionGuardHeardYellForHelp::GetName(), CAIStateGroup_MinionGuardHeardYellForHelp::Alloc),
    CAIStateGroupInfo(CAIStateGroup_MinionGuardReactToPlayerWithWeapon::GetName(), CAIStateGroup_MinionGuardReactToPlayerWithWeapon::Alloc),
    CAIStateGroupInfo(CAIStateGroup_MinionGuardReactToBodyFoundMessage::GetName(), CAIStateGroup_MinionGuardReactToBodyFoundMessage::Alloc),
    CAIStateGroupInfo(CAIStateGroup_BanditSitRoundCampfire::GetName(), CAIStateGroup_BanditSitRoundCampfire::Alloc),
    CAIStateGroupInfo(CAIStateGroup_MinionReactToCriminal::GetName(), CAIStateGroup_MinionReactToCriminal::Alloc),
    CAIStateGroupInfo(CAIStateGroup_MinionGuardReactToCriminal::GetName(), CAIStateGroup_MinionGuardReactToCriminal::Alloc),
    CAIStateGroupInfo(CAIStateGroup_MinionGuardReactToBribe::GetName(), CAIStateGroup_MinionGuardReactToBribe::Alloc),
    CAIStateGroupInfo(CAIStateGroup_PerformAction::GetName(), CAIStateGroup_PerformAction::Alloc),
    CAIStateGroupInfo(CAIStateGroup_MinionFaceHero::GetName(), CAIStateGroup_MinionFaceHero::Alloc),
    CAIStateGroupInfo(CAIStateGroup_MinionPutOutFires::GetName(), CAIStateGroup_MinionPutOutFires::Alloc),
    CAIStateGroupInfo(CAIStateGroup_MinionWatchFires::GetName(), CAIStateGroup_MinionWatchFires::Alloc),
    CAIStateGroupInfo(CAIStateGroup_MinionReactToAlert::GetName(), CAIStateGroup_MinionReactToAlert::Alloc),
    CAIStateGroupInfo(CAIStateGroup_MinionReactToEnemy::GetName(), CAIStateGroup_MinionReactToEnemy::Alloc),
    CAIStateGroupInfo(CAIStateGroup_MinionReactToCorpse::GetName(), CAIStateGroup_MinionReactToCorpse::Alloc),
    CAIStateGroupInfo(CAIStateGroup_MinionReactToUnconsciousPerson::GetName(), CAIStateGroup_MinionReactToUnconsciousPerson::Alloc),
    CAIStateGroupInfo(CAIStateGroup_MinionHasInformation::GetName(), CAIStateGroup_MinionHasInformation::Alloc),
    CAIStateGroupInfo(CAIStateGroup_MinionSitOnSeat::GetName(), CAIStateGroup_MinionSitOnSeat::Alloc),
    CAIStateGroupInfo(CAIStateGroup_MinionSitAtWork::GetName(), CAIStateGroup_MinionSitAtWork::Alloc),
    CAIStateGroupInfo(CAIStateGroup_MinionSitAtHome::GetName(), CAIStateGroup_MinionSitAtHome::Alloc),
    CAIStateGroupInfo(CAIStateGroup_MinionSitAtInn::GetName(), CAIStateGroup_MinionSitAtInn::Alloc),
    CAIStateGroupInfo(CAIStateGroup_MinionSitAtSchool::GetName(), CAIStateGroup_MinionSitAtSchool::Alloc),
    CAIStateGroupInfo(CAIStateGroup_MinionSitOutside::GetName(), CAIStateGroup_MinionSitOutside::Alloc),
    CAIStateGroupInfo(CAIStateGroup_MinionListenToStory::GetName(), CAIStateGroup_MinionListenToStory::Alloc),
    CAIStateGroupInfo(CAIStateGroup_SitRoundCampfire::GetName(), CAIStateGroup_SitRoundCampfire::Alloc),
    CAIStateGroupInfo(CAIStateGroup_MinionCuriosity::GetName(), CAIStateGroup_MinionCuriosity::Alloc),
    CAIStateGroupInfo(CAIStateGroup_MinionGossip::GetName(), CAIStateGroup_MinionGossip::Alloc),
    CAIStateGroupInfo(CAIStateGroup_BanditGossip::GetName(), CAIStateGroup_BanditGossip::Alloc),
    CAIStateGroupInfo(CAIStateGroup_MinionMotherProtectFamily::GetName(), CAIStateGroup_MinionMotherProtectFamily::Alloc),
    CAIStateGroupInfo(CAIStateGroup_MinionKidAnnoy::GetName(), CAIStateGroup_MinionKidAnnoy::Alloc),
    CAIStateGroupInfo(CAIStateGroup_MinionKidGroupTag::GetName(), CAIStateGroup_MinionKidGroupTag::Alloc),
    CAIStateGroupInfo(CAIStateGroup_MinionKidCalledToMother::GetName(), CAIStateGroup_MinionKidCalledToMother::Alloc),
    CAIStateGroupInfo(CAIStateGroup_CreatureFindCover::GetName(), CAIStateGroup_CreatureFindCover::Alloc),
    CAIStateGroupInfo(CAIStateGroup_ChickenPanic::GetName(), CAIStateGroup_ChickenPanic::Alloc),
    CAIStateGroupInfo(CAIStateGroup_ChickenBeg::GetName(), CAIStateGroup_ChickenBeg::Alloc),
    CAIStateGroupInfo(CAIStateGroup_FireflyWander::GetName(), CAIStateGroup_FireflyWander::Alloc),
    CAIStateGroupInfo(CAIStateGroup_ButterflyWander::GetName(), CAIStateGroup_ButterflyWander::Alloc),
    CAIStateGroupInfo(CAIStateGroup_SmallBirdWander::GetName(), CAIStateGroup_SmallBirdWander::Alloc),
    CAIStateGroupInfo(CAIStateGroup_BirdFlyToFood::GetName(), CAIStateGroup_BirdFlyToFood::Alloc),
    CAIStateGroupInfo(CAIStateGroup_BirdGroundWander::GetName(), CAIStateGroup_BirdGroundWander::Alloc),
    CAIStateGroupInfo(CAIStateGroup_BirdStartled::GetName(), CAIStateGroup_BirdStartled::Alloc),
    CAIStateGroupInfo(CAIStateGroup_Guard::GetName(), CAIStateGroup_Guard::Alloc),
    CAIStateGroupInfo(CAIStateGroup_Wander::GetName(), CAIStateGroup_Wander::Alloc),
    CAIStateGroupInfo(CAIStateGroup_ChickenWander::GetName(), CAIStateGroup_ChickenWander::Alloc),
    CAIStateGroupInfo(CAIStateGroup_WanderAroundInitialPos::GetName(), CAIStateGroup_WanderAroundInitialPos::Alloc),
    CAIStateGroupInfo(CAIStateGroup_MeleeCombatNearby::GetName(), CAIStateGroup_MeleeCombatNearby::Alloc),
    CAIStateGroupInfo(CAIStateGroup_MeleeCombatNearby2::GetName(), CAIStateGroup_MeleeCombatNearby2::Alloc),
    CAIStateGroupInfo(CAIStateGroup_MeleeCombatNearbyAttackWhenVeryClose::GetName(), CAIStateGroup_MeleeCombatNearbyAttackWhenVeryClose::Alloc),
    CAIStateGroupInfo(CAIStateGroup_NonVillageGuardPatrol::GetName(), CAIStateGroup_NonVillageGuardPatrol::Alloc),
    CAIStateGroupInfo(CAIStateGroup_SeekEnemy::GetName(), CAIStateGroup_SeekEnemy::Alloc),
    CAIStateGroupInfo(CAIStateGroup_SeekScriptTarget::GetName(), CAIStateGroup_SeekScriptTarget::Alloc),
    CAIStateGroupInfo(CAIStateGroup_StealthSeekEnemy::GetName(), CAIStateGroup_StealthSeekEnemy::Alloc),
    CAIStateGroupInfo(CAIStateGroup_RangedCombatUsePreferredFirePoint::GetName(), CAIStateGroup_RangedCombatUsePreferredFirePoint::Alloc),
    CAIStateGroupInfo(CAIStateGroup_CreatureRangedCombatUseWillShield::GetName(), CAIStateGroup_CreatureRangedCombatUseWillShield::Alloc),
    CAIStateGroupInfo(CAIStateGroup_VillageTaskTorch::GetName(), CAIStateGroup_VillageTaskTorch::Alloc),
    CAIStateGroupInfo(CAIStateGroup_VillageTaskPatrol::GetName(), CAIStateGroup_VillageTaskPatrol::Alloc),
    CAIStateGroupInfo(CAIStateGroup_VillageTaskSecuritySweep::GetName(), CAIStateGroup_VillageTaskSecuritySweep::Alloc),
    CAIStateGroupInfo(CAIStateGroup_VillageTaskPickCrops::GetName(), CAIStateGroup_VillageTaskPickCrops::Alloc),
    CAIStateGroupInfo(CAIStateGroup_VillageTaskHorn::GetName(), CAIStateGroup_VillageTaskHorn::Alloc),
    CAIStateGroupInfo(CAIStateGroup_KidWanderNearKids::GetName(), CAIStateGroup_KidWanderNearKids::Alloc),
    CAIStateGroupInfo(CAIStateGroup_KidWatchHopscotch::GetName(), CAIStateGroup_KidWatchHopscotch::Alloc),
    CAIStateGroupInfo(CAIStateGroup_KidPlayHopscotch::GetName(), CAIStateGroup_KidPlayHopscotch::Alloc),
    CAIStateGroupInfo(CAIStateGroup_KidHurtSelfRunToMother::GetName(), CAIStateGroup_KidHurtSelfRunToMother::Alloc),
    CAIStateGroupInfo(CAIStateGroup_KidReactToBlockage::GetName(), CAIStateGroup_KidReactToBlockage::Alloc),
    CAIStateGroupInfo(CAIStateGroup_IdleAtHome::GetName(), CAIStateGroup_IdleAtHome::Alloc),
    CAIStateGroupInfo(CAIStateGroup_WorkAtSchool::GetName(), CAIStateGroup_WorkAtSchool::Alloc),
    CAIStateGroupInfo(CAIStateGroup_TellStory::GetName(), CAIStateGroup_TellStory::Alloc),
    CAIStateGroupInfo(CAIStateGroup_WorkInWarehouse::GetName(), CAIStateGroup_WorkInWarehouse::Alloc),
    CAIStateGroupInfo(CAIStateGroup_WorkSawingWood::GetName(), CAIStateGroup_WorkSawingWood::Alloc),
    CAIStateGroupInfo(CAIStateGroup_WorkHammerAnvil::GetName(), CAIStateGroup_WorkHammerAnvil::Alloc),
    CAIStateGroupInfo(CAIStateGroup_ThrowLogsOnFire::GetName(), CAIStateGroup_ThrowLogsOnFire::Alloc),
    CAIStateGroupInfo(CAIStateGroup_StayNearSchool::GetName(), CAIStateGroup_StayNearSchool::Alloc),
    CAIStateGroupInfo(CAIStateGroup_CleanHouse::GetName(), CAIStateGroup_CleanHouse::Alloc),
    CAIStateGroupInfo(CAIStateGroup_CleanWork::GetName(), CAIStateGroup_CleanWork::Alloc),
    CAIStateGroupInfo(CAIStateGroup_SitForMeal::GetName(), CAIStateGroup_SitForMeal::Alloc),
    CAIStateGroupInfo(CAIStateGroup_PrepareMeal::GetName(), CAIStateGroup_PrepareMeal::Alloc),
    CAIStateGroupInfo(CAIStateGroup_GazeAtHome::GetName(), CAIStateGroup_GazeAtHome::Alloc),
    CAIStateGroupInfo(CAIStateGroup_GazeAtWork::GetName(), CAIStateGroup_GazeAtWork::Alloc),
    CAIStateGroupInfo(CAIStateGroup_GazeOutside::GetName(), CAIStateGroup_GazeOutside::Alloc),
    CAIStateGroupInfo(CAIStateGroup_WanderAroundShops::GetName(), CAIStateGroup_WanderAroundShops::Alloc),
    CAIStateGroupInfo(CAIStateGroup_StayNearFoodHall::GetName(), CAIStateGroup_StayNearFoodHall::Alloc),
    CAIStateGroupInfo(CAIStateGroup_GoToPub::GetName(), CAIStateGroup_GoToPub::Alloc),
    CAIStateGroupInfo(CAIStateGroup_SitInPub::GetName(), CAIStateGroup_SitInPub::Alloc),
    CAIStateGroupInfo(CAIStateGroup_LightHomeCandle::GetName(), CAIStateGroup_LightHomeCandle::Alloc),
    CAIStateGroupInfo(CAIStateGroup_LightWorkCandle::GetName(), CAIStateGroup_LightWorkCandle::Alloc),
    CAIStateGroupInfo(CAIStateGroup_SeekPerceivedThing::GetName(), CAIStateGroup_SeekPerceivedThing::Alloc),
    CAIStateGroupInfo(CAIStateGroup_FollowPlayer::GetName(), CAIStateGroup_FollowPlayer::Alloc),
    CAIStateGroupInfo(CAIStateGroup_MinionHawkingFromStall::GetName(), CAIStateGroup_MinionHawkingFromStall::Alloc),
    CAIStateGroupInfo(CAIStateGroup_BuyFromSeller::GetName(), CAIStateGroup_BuyFromSeller::Alloc),
    CAIStateGroupInfo(CAIStateGroup_SellToBuyer::GetName(), CAIStateGroup_SellToBuyer::Alloc),
    CAIStateGroupInfo(CAIStateGroup_HandyMan::GetName(), CAIStateGroup_HandyMan::Alloc),
    CAIStateGroupInfo(CAIStateGroup_LookAtInterestingThings::GetName(), CAIStateGroup_LookAtInterestingThings::Alloc),
    CAIStateGroupInfo(CAIStateGroup_GameMaster::GetName(), CAIStateGroup_GameMaster::Alloc),
    CAIStateGroupInfo(CAIStateGroup_StandStill::GetName(), CAIStateGroup_StandStill::Alloc),
    CAIStateGroupInfo(CAIStateGroup_BalverineWander::GetName(), CAIStateGroup_BalverineWander::Alloc),
    CAIStateGroupInfo(CAIStateGroup_BalverineCornered::GetName(), CAIStateGroup_BalverineCornered::Alloc),
    CAIStateGroupInfo(CAIStateGroup_BalverineJumpAtFleeingEnemy::GetName(), CAIStateGroup_BalverineJumpAtFleeingEnemy::Alloc),
    CAIStateGroupInfo(CAIStateGroup_NymphFloatAround::GetName(), CAIStateGroup_NymphFloatAround::Alloc),
    CAIStateGroupInfo(CAIStateGroup_NymphAttack::GetName(), CAIStateGroup_NymphAttack::Alloc),
    CAIStateGroupInfo(CAIStateGroup_HoverWander::GetName(), CAIStateGroup_HoverWander::Alloc),
    CAIStateGroupInfo(CAIStateGroup_HoverWanderAroundThing::GetName(), CAIStateGroup_HoverWanderAroundThing::Alloc),
    CAIStateGroupInfo(CAIStateGroup_HoverAttack::GetName(), CAIStateGroup_HoverAttack::Alloc),
    CAIStateGroupInfo(CAIStateGroup_HoverAttackUnlessTargetIsFleeing::GetName(), CAIStateGroup_HoverAttackUnlessTargetIsFleeing::Alloc),
    CAIStateGroupInfo(CAIStateGroup_HoverKeepInArea::GetName(), CAIStateGroup_HoverKeepInArea::Alloc),
    CAIStateGroupInfo(CAIStateGroup_HoverAttackArea::GetName(), CAIStateGroup_HoverAttackArea::Alloc),
    CAIStateGroupInfo(CAIStateGroup_HoverHornetQueenTakeDamage::GetName(), CAIStateGroup_HoverHornetQueenTakeDamage::Alloc),
    CAIStateGroupInfo(CAIStateGroup_HoverHornetQueen::GetName(), CAIStateGroup_HoverHornetQueen::Alloc),
    CAIStateGroupInfo(CAIStateGroup_HoverAllAroundTest::GetName(), CAIStateGroup_HoverAllAroundTest::Alloc),
    CAIStateGroupInfo(CAIStateGroup_EarthTroll::GetName(), CAIStateGroup_EarthTroll::Alloc),
    CAIStateGroupInfo(CAIStateGroup_RockTroll::GetName(), CAIStateGroup_RockTroll::Alloc),
    CAIStateGroupInfo(CAIStateGroup_IceTrollIcicleAttack::GetName(), CAIStateGroup_IceTrollIcicleAttack::Alloc),
    CAIStateGroupInfo(CAIStateGroup_IceTrollPoundGroundAttack::GetName(), CAIStateGroup_IceTrollPoundGroundAttack::Alloc),
    CAIStateGroupInfo(CAIStateGroup_IceTrollIcicleAssault::GetName(), CAIStateGroup_IceTrollIcicleAssault::Alloc),
    CAIStateGroupInfo(CAIStateGroup_ScorpionKing::GetName(), CAIStateGroup_ScorpionKing::Alloc),
    CAIStateGroupInfo(CAIStateGroup_JackOfBladesStage1::GetName(), CAIStateGroup_JackOfBladesStage1::Alloc),
    CAIStateGroupInfo(CAIStateGroup_JackOfBladesStage2::GetName(), CAIStateGroup_JackOfBladesStage2::Alloc),
    CAIStateGroupInfo(CAIStateGroup_JackOfBladesStage3::GetName(), CAIStateGroup_JackOfBladesStage3::Alloc),
    CAIStateGroupInfo(CAIStateGroup_MazeTeleport::GetName(), CAIStateGroup_MazeTeleport::Alloc),
    CAIStateGroupInfo(CAIStateGroup_MazeRangedCombat::GetName(), CAIStateGroup_MazeRangedCombat::Alloc),
    CAIStateGroupInfo(CAIStateGroup_MazeDefensiveMeleeCombat::GetName(), CAIStateGroup_MazeDefensiveMeleeCombat::Alloc),
    CAIStateGroupInfo(CAIStateGroup_MazeShieldedChase::GetName(), CAIStateGroup_MazeShieldedChase::Alloc),
    CAIStateGroupInfo(CAIStateGroup_MazeTutorialBattle::GetName(), CAIStateGroup_MazeTutorialBattle::Alloc),
    CAIStateGroupInfo(CAIStateGroup_MazeTutorialBattleTeleport::GetName(), CAIStateGroup_MazeTutorialBattleTeleport::Alloc),
    CAIStateGroupInfo(CAIStateGroup_MazeTutorialBattleRespondToLightning::GetName(), CAIStateGroup_MazeTutorialBattleRespondToLightning::Alloc),
    CAIStateGroupInfo(CAIStateGroup_MazeTutorialBattleUseShield::GetName(), CAIStateGroup_MazeTutorialBattleUseShield::Alloc),
    CAIStateGroupInfo(CAIStateGroup_Guildmaster::GetName(), CAIStateGroup_Guildmaster::Alloc),
    CAIStateGroupInfo(CAIStateGroup_BriarRose::GetName(), CAIStateGroup_BriarRose::Alloc),
    CAIStateGroupInfo(CAIStateGroup_BriarRoseWillUse::GetName(), CAIStateGroup_BriarRoseWillUse::Alloc),
    CAIStateGroupInfo(CAIStateGroup_VersusRockTroll::GetName(), CAIStateGroup_VersusRockTroll::Alloc),
    CAIStateGroupInfo(CAIStateGroup_VersusScorpionKing::GetName(), CAIStateGroup_VersusScorpionKing::Alloc),
    CAIStateGroupInfo(CAIStateGroup_CreatureRangedCombat2::GetName(), CAIStateGroup_CreatureRangedCombat2::Alloc),
    CAIStateGroupInfo(CAIStateGroup_CreatureTakeCover::GetName(), CAIStateGroup_CreatureTakeCover::Alloc),
    CAIStateGroupInfo(CAIStateGroup_CreatureAdvanceInCover::GetName(), CAIStateGroup_CreatureAdvanceInCover::Alloc),
    CAIStateGroupInfo(CAIStateGroup_CreatureRangedCombatFireAtSpecificTargets::GetName(), CAIStateGroup_CreatureRangedCombatFireAtSpecificTargets::Alloc),
    CAIStateGroupInfo(CAIStateGroup_TestTestTest::GetName(), CAIStateGroup_TestTestTest::Alloc),
    CAIStateGroupInfo(CAIStateGroup_CreatureGoUnderground::GetName(), CAIStateGroup_CreatureGoUnderground::Alloc),
    CAIStateGroupInfo(CAIStateGroup_CreatureIdleUnderground::GetName(), CAIStateGroup_CreatureIdleUnderground::Alloc),
    CAIStateGroupInfo(CAIStateGroup_CreatureSeekEnemyUnderground::GetName(), CAIStateGroup_CreatureSeekEnemyUnderground::Alloc),
    CAIStateGroupInfo(CAIStateGroup_BeetleToggleHover::GetName(), CAIStateGroup_BeetleToggleHover::Alloc),
    CAIStateGroupInfo(CAIStateGroup_MinionBehaviourGreetCreature::GetName(), CAIStateGroup_MinionBehaviourGreetCreature::Alloc),
    CAIStateGroupInfo(CAIStateGroup_MinionBehaviourSad::GetName(), CAIStateGroup_MinionBehaviourSad::Alloc),
    CAIStateGroupInfo(CAIStateGroup_MinionBehaviourPodiumEmoteNeutral::GetName(), CAIStateGroup_MinionBehaviourPodiumEmoteNeutral::Alloc),
    CAIStateGroupInfo(CAIStateGroup_MinionMotherConsoleChild::GetName(), CAIStateGroup_MinionMotherConsoleChild::Alloc),
    CAIStateGroupInfo(CAIStateGroup_MinionReactToBurglary::GetName(), CAIStateGroup_MinionReactToBurglary::Alloc),
    CAIStateGroupInfo(CAIStateGroup_MinionReactToTrespassing::GetName(), CAIStateGroup_MinionReactToTrespassing::Alloc),
    CAIStateGroupInfo(CAIStateGroup_MinionReactToNoise::GetName(), CAIStateGroup_MinionReactToNoise::Alloc),
    CAIStateGroupInfo(CAIStateGroup_MinionReactToApology::GetName(), CAIStateGroup_MinionReactToApology::Alloc),
    CAIStateGroupInfo(CAIStateGroup_MinionReactToCrime::GetName(), CAIStateGroup_MinionReactToCrime::Alloc),
    CAIStateGroupInfo(CAIStateGroup_MinionReactToCrimeFromSleep::GetName(), CAIStateGroup_MinionReactToCrimeFromSleep::Alloc),
    CAIStateGroupInfo(CAIStateGroup_MinionLeaveRegion::GetName(), CAIStateGroup_MinionLeaveRegion::Alloc),
    CAIStateGroupInfo(CAIStateGroup_ReactToDeadLeader::GetName(), CAIStateGroup_ReactToDeadLeader::Alloc),
    CAIStateGroupInfo(CAIStateGroup_BoastingCrowdMember::GetName(), CAIStateGroup_BoastingCrowdMember::Alloc),
    CAIStateGroupInfo(CAIStateGroup_OpinionReactionRoot::GetName(), CAIStateGroup_OpinionReactionRoot::Alloc),
    CAIStateGroupInfo(CAIStateGroup_DeedReactionRoot::GetName(), CAIStateGroup_DeedReactionRoot::Alloc),
    CAIStateGroupInfo(CAIStateGroup_DeedReactionRootFromSleep::GetName(), CAIStateGroup_DeedReactionRootFromSleep::Alloc),
    CAIStateGroupInfo(CAIStateGroup_BanditKingCharge::GetName(), CAIStateGroup_BanditKingCharge::Alloc),
    CAIStateGroupInfo(CAIStateGroup_BanditKingLunge::GetName(), CAIStateGroup_BanditKingLunge::Alloc),
    CAIStateGroupInfo(CAIStateGroup_BanditKingThrowWeapons::GetName(), CAIStateGroup_BanditKingThrowWeapons::Alloc),
    CAIStateGroupInfo(CAIStateGroup_JackDragon::GetName(), CAIStateGroup_JackDragon::Alloc),
    CAIStateGroupInfo(CAIStateGroup_MinionFollowThing::GetName(), CAIStateGroup_MinionFollowThing::Alloc),
    CAIStateGroupInfo(CAIStateGroup_MinionWalkToMarker::GetName(), CAIStateGroup_MinionWalkToMarker::Alloc),
    CAIStateGroupInfo(CAIStateGroup_TavernServer::GetName(), CAIStateGroup_TavernServer::Alloc),
    CAIStateGroupInfo(CAIStateGroup_TavernKeeper::GetName(), CAIStateGroup_TavernKeeper::Alloc),
    CAIStateGroupInfo(CAIStateGroup_StealStealableItem::GetName(), CAIStateGroup_StealStealableItem::Alloc),
    CAIStateGroupInfo(CAIStateGroup_RecoverStealableItem::GetName(), CAIStateGroup_RecoverStealableItem::Alloc),
    CAIStateGroupInfo(CAIStateGroup_CarryCrateAndFollow::GetName(), CAIStateGroup_CarryCrateAndFollow::Alloc),
    CAIStateGroupInfo(CAIStateGroup_SleepOnFloor::GetName(), CAIStateGroup_SleepOnFloor::Alloc),
    CAIStateGroupInfo(CAIStateGroup_WhisperRetreat::GetName(), CAIStateGroup_WhisperRetreat::Alloc),
    CAIStateGroupInfo(CAIStateGroup_WhisperPotionCombat::GetName(), CAIStateGroup_WhisperPotionCombat::Alloc),
    CAIStateGroupInfo(CAIStateGroup_Thunder::GetName(), CAIStateGroup_Thunder::Alloc),
    CAIStateGroupInfo(CAIStateGroup_Freeze::GetName(), CAIStateGroup_Freeze::Alloc),
    CAIStateGroupInfo(CAIStateGroup_Drunk::GetName(), CAIStateGroup_Drunk::Alloc),
    CAIStateGroupInfo(CAIStateGroup_ServeFoodAtHome::GetName(), CAIStateGroup_ServeFoodAtHome::Alloc),
    CAIStateGroupInfo(CAIStateGroup_ServeFoodAtTavern::GetName(), CAIStateGroup_ServeFoodAtTavern::Alloc),
    CAIStateGroupInfo(CAIStateGroup_EatAtHome::GetName(), CAIStateGroup_EatAtHome::Alloc),
    CAIStateGroupInfo(CAIStateGroup_EatAtTavern::GetName(), CAIStateGroup_EatAtTavern::Alloc),
    CAIStateGroupInfo(CAIStateGroup_ScreamerPursueTarget::GetName(), CAIStateGroup_ScreamerPursueTarget::Alloc),
    CAIStateGroupInfo(CAIStateGroup_KrakenAttack::GetName(), CAIStateGroup_KrakenAttack::Alloc),
    CAIStateGroupInfo(CAIStateGroup_KrakenTentacleIdle::GetName(), CAIStateGroup_KrakenTentacleIdle::Alloc),
    CAIStateGroupInfo(CAIStateGroup_KrakenTentacleRise::GetName(), CAIStateGroup_KrakenTentacleRise::Alloc),
    CAIStateGroupInfo(CAIStateGroup_KrakenTentacleAttack::GetName(), CAIStateGroup_KrakenTentacleAttack::Alloc),
    CAIStateGroupInfo(CAIStateGroup_WifeGiveYouGift::GetName(), CAIStateGroup_WifeGiveYouGift::Alloc),
    CAIStateGroupInfo(CAIStateGroup_OpenAndCloseShop::GetName(), CAIStateGroup_OpenAndCloseShop::Alloc),
    CAIStateGroupInfo(CAIStateGroup_ShopkeeperWatchPlayer::GetName(), CAIStateGroup_ShopkeeperWatchPlayer::Alloc),
    CAIStateGroupInfo(CAIStateGroup_SetupWares::GetName(), CAIStateGroup_SetupWares::Alloc),
    CAIStateGroupInfo(CAIStateGroup_BanditPlayDice::GetName(), CAIStateGroup_BanditPlayDice::Alloc),
    CAIStateGroupInfo(CAIStateGroup_BanditPlot::GetName(), CAIStateGroup_BanditPlot::Alloc),
    CAIStateGroupInfo(CAIStateGroup_MinionGuardGreetPlayer::GetName(), CAIStateGroup_MinionGuardGreetPlayer::Alloc),
    CAIStateGroupInfo(CAIStateGroup_MinionGuardFollowPlayer::GetName(), CAIStateGroup_MinionGuardFollowPlayer::Alloc),
    CAIStateGroupInfo(CAIStateGroup_MinionGuardBribed::GetName(), CAIStateGroup_MinionGuardBribed::Alloc),
    CAIStateGroupInfo(CAIStateGroup_ApprenticeHero::GetName(), CAIStateGroup_ApprenticeHero::Alloc),
    CAIStateGroupInfo(CAIStateGroup_SkillPractice::GetName(), CAIStateGroup_SkillPractice::Alloc),
    CAIStateGroupInfo(CAIStateGroup_OnHeroPush::GetName(), CAIStateGroup_OnHeroPush::Alloc),
    CAIStateGroupInfo(CAIStateGroup_AvoidRegionExit::GetName(), CAIStateGroup_AvoidRegionExit::Alloc),
    CAIStateGroupInfo(CAIStateGroup_SummonerLightningAttack::GetName(), CAIStateGroup_SummonerLightningAttack::Alloc),
    CAIStateGroupInfo(CAIStateGroup_SummonerSummon::GetName(), CAIStateGroup_SummonerSummon::Alloc),
    CAIStateGroupInfo(CAIStateGroup_SummonerCharge::GetName(), CAIStateGroup_SummonerCharge::Alloc),
    CAIStateGroupInfo("", NULL)
};