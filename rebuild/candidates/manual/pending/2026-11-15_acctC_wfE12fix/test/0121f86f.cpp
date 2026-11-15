// Behaviour test for 0x0121F86F CAIBrain::`dynamic initializer for 'StateGroupInfo'': the 203-entry state-group registration table.
// Each entry must carry its class's GetName() text and its class's allocator (calling it must create that class: the allocator
// records its slot), in the retail order (expected names listed independently of the table); the last entry is the empty name
// with a null allocator; each temporary is copied into the array (name copies counted) and destroyed.
#include <cstdio>
#include <cstring>
#include <cstdlib>
static int g_live = 0, g_copies = 0;
class CCharString { public: char* Text;
  CCharString(const char* text, long length = -1) { (void)length; Text = (char*)std::malloc(std::strlen(text) + 1); std::strcpy(Text, text); ++g_live; }
  CCharString(const CCharString& o) { Text = (char*)std::malloc(std::strlen(o.Text) + 1); std::strcpy(Text, o.Text); ++g_live; ++g_copies; }
  ~CCharString() { std::free(Text); --g_live; } };
class CAIStateGroupBase { public: int Slot; CAIStateGroupBase(int slot) : Slot(slot) {} };
class CAIStateGroupInfo { public:
  CAIStateGroupInfo(CCharString name, CAIStateGroupBase* (*allocFunc)()) : StateGroupName(name), AllocFunc(allocFunc) {}
  ~CAIStateGroupInfo() {}
  CCharString StateGroupName; CAIStateGroupBase* (*AllocFunc)(); };
class CAIStateGroup_MinionWander { public: static CCharString GetName() { return CCharString("SG_MINION_WANDER"); } static CAIStateGroupBase* Alloc() { return new CAIStateGroupBase(0); } };
class CAIStateGroup_MinionSleep { public: static CCharString GetName() { return CCharString("SG_MINION_SLEEP"); } static CAIStateGroupBase* Alloc() { return new CAIStateGroupBase(1); } };
class CAIStateGroup_GoToBed { public: static CCharString GetName() { return CCharString("SG_MINION_GO_TO_BED"); } static CAIStateGroupBase* Alloc() { return new CAIStateGroupBase(2); } };
class CAIStateGroup_MinionWorkInInn { public: static CCharString GetName() { return CCharString("SG_MINION_WORK_IN_INN"); } static CAIStateGroupBase* Alloc() { return new CAIStateGroupBase(3); } };
class CAIStateGroup_MinionFindPersonalSpace { public: static CCharString GetName() { return CCharString("SG_MINION_FIND_PERSONAL_SPACE"); } static CAIStateGroupBase* Alloc() { return new CAIStateGroupBase(4); } };
class CAIStateGroup_MinionReactToPlayerInteraction { public: static CCharString GetName() { return CCharString("SG_MINION_REACT_TO_PLAYER_INTERACTION"); } static CAIStateGroupBase* Alloc() { return new CAIStateGroupBase(5); } };
class CAIStateGroup_MinionGuardHeardYellForHelp { public: static CCharString GetName() { return CCharString("SG_MINION_GUARD_HEARD_YELL_FOR_HELP"); } static CAIStateGroupBase* Alloc() { return new CAIStateGroupBase(6); } };
class CAIStateGroup_MinionGuardReactToPlayerWithWeapon { public: static CCharString GetName() { return CCharString("SG_MINION_GUARD_REACT_TO_PLAYER_WEAPON"); } static CAIStateGroupBase* Alloc() { return new CAIStateGroupBase(7); } };
class CAIStateGroup_MinionGuardReactToBodyFoundMessage { public: static CCharString GetName() { return CCharString("SG_MINION_GUARD_REACT_TO_BODY_FOUND_MESSAGE"); } static CAIStateGroupBase* Alloc() { return new CAIStateGroupBase(8); } };
class CAIStateGroup_BanditSitRoundCampfire { public: static CCharString GetName() { return CCharString("SG_BANDIT_SIT_ROUND_CAMPFIRE"); } static CAIStateGroupBase* Alloc() { return new CAIStateGroupBase(9); } };
class CAIStateGroup_MinionReactToCriminal { public: static CCharString GetName() { return CCharString("SG_MINION_REACT_TO_CRIMINAL"); } static CAIStateGroupBase* Alloc() { return new CAIStateGroupBase(10); } };
class CAIStateGroup_MinionGuardReactToCriminal { public: static CCharString GetName() { return CCharString("SG_MINION_GUARD_REACT_TO_CRIMINAL"); } static CAIStateGroupBase* Alloc() { return new CAIStateGroupBase(11); } };
class CAIStateGroup_MinionGuardReactToBribe { public: static CCharString GetName() { return CCharString("SG_MINION_GUARD_REACT_TO_BRIBE"); } static CAIStateGroupBase* Alloc() { return new CAIStateGroupBase(12); } };
class CAIStateGroup_PerformAction { public: static CCharString GetName() { return CCharString("SG_PERFORM_ACTION"); } static CAIStateGroupBase* Alloc() { return new CAIStateGroupBase(13); } };
class CAIStateGroup_MinionFaceHero { public: static CCharString GetName() { return CCharString("SG_MINION_FACE_HERO_READY_FOR_INTERACTION"); } static CAIStateGroupBase* Alloc() { return new CAIStateGroupBase(14); } };
class CAIStateGroup_MinionPutOutFires { public: static CCharString GetName() { return CCharString("SG_MINION_PUT_OUT_FIRES"); } static CAIStateGroupBase* Alloc() { return new CAIStateGroupBase(15); } };
class CAIStateGroup_MinionWatchFires { public: static CCharString GetName() { return CCharString("SG_MINION_WATCH_FIRES"); } static CAIStateGroupBase* Alloc() { return new CAIStateGroupBase(16); } };
class CAIStateGroup_MinionReactToAlert { public: static CCharString GetName() { return CCharString("SG_MINION_REACT_TO_ALERT"); } static CAIStateGroupBase* Alloc() { return new CAIStateGroupBase(17); } };
class CAIStateGroup_MinionReactToEnemy { public: static CCharString GetName() { return CCharString("SG_MINION_REACT_TO_ENEMY"); } static CAIStateGroupBase* Alloc() { return new CAIStateGroupBase(18); } };
class CAIStateGroup_MinionReactToCorpse { public: static CCharString GetName() { return CCharString("SG_MINION_REACT_TO_CORPSE"); } static CAIStateGroupBase* Alloc() { return new CAIStateGroupBase(19); } };
class CAIStateGroup_MinionReactToUnconsciousPerson { public: static CCharString GetName() { return CCharString("SG_MINION_REACT_TO_UNCONSCIOUS_PERSON"); } static CAIStateGroupBase* Alloc() { return new CAIStateGroupBase(20); } };
class CAIStateGroup_MinionHasInformation { public: static CCharString GetName() { return CCharString("SG_MINION_HAS_INFORMATION"); } static CAIStateGroupBase* Alloc() { return new CAIStateGroupBase(21); } };
class CAIStateGroup_MinionSitOnSeat { public: static CCharString GetName() { return CCharString("SG_MINION_SIT_ON_SEAT"); } static CAIStateGroupBase* Alloc() { return new CAIStateGroupBase(22); } };
class CAIStateGroup_MinionSitAtWork { public: static CCharString GetName() { return CCharString("SG_MINION_SIT_AT_WORK"); } static CAIStateGroupBase* Alloc() { return new CAIStateGroupBase(23); } };
class CAIStateGroup_MinionSitAtHome { public: static CCharString GetName() { return CCharString("SG_MINION_SIT_AT_HOME"); } static CAIStateGroupBase* Alloc() { return new CAIStateGroupBase(24); } };
class CAIStateGroup_MinionSitAtInn { public: static CCharString GetName() { return CCharString("SG_MINION_SIT_AT_INN"); } static CAIStateGroupBase* Alloc() { return new CAIStateGroupBase(25); } };
class CAIStateGroup_MinionSitAtSchool { public: static CCharString GetName() { return CCharString("SG_MINION_SIT_AT_SCHOOL"); } static CAIStateGroupBase* Alloc() { return new CAIStateGroupBase(26); } };
class CAIStateGroup_MinionSitOutside { public: static CCharString GetName() { return CCharString("SG_MINION_SIT_OUTSIDE"); } static CAIStateGroupBase* Alloc() { return new CAIStateGroupBase(27); } };
class CAIStateGroup_MinionListenToStory { public: static CCharString GetName() { return CCharString("SG_MINION_LISTEN_TO_STORY"); } static CAIStateGroupBase* Alloc() { return new CAIStateGroupBase(28); } };
class CAIStateGroup_SitRoundCampfire { public: static CCharString GetName() { return CCharString("SG_SIT_ROUND_CAMPFIRE"); } static CAIStateGroupBase* Alloc() { return new CAIStateGroupBase(29); } };
class CAIStateGroup_MinionCuriosity { public: static CCharString GetName() { return CCharString("SG_MINION_CURIOSITY"); } static CAIStateGroupBase* Alloc() { return new CAIStateGroupBase(30); } };
class CAIStateGroup_MinionGossip { public: static CCharString GetName() { return CCharString("SG_MINION_GOSSIP"); } static CAIStateGroupBase* Alloc() { return new CAIStateGroupBase(31); } };
class CAIStateGroup_BanditGossip { public: static CCharString GetName() { return CCharString("SG_BANDIT_GOSSIP"); } static CAIStateGroupBase* Alloc() { return new CAIStateGroupBase(32); } };
class CAIStateGroup_MinionMotherProtectFamily { public: static CCharString GetName() { return CCharString("SG_MINION_MOTHER_PROTECT_FAMILY"); } static CAIStateGroupBase* Alloc() { return new CAIStateGroupBase(33); } };
class CAIStateGroup_MinionKidAnnoy { public: static CCharString GetName() { return CCharString("SG_MINION_KID_ANNOY"); } static CAIStateGroupBase* Alloc() { return new CAIStateGroupBase(34); } };
class CAIStateGroup_MinionKidGroupTag { public: static CCharString GetName() { return CCharString("SG_MINION_KID_GROUP_TAG"); } static CAIStateGroupBase* Alloc() { return new CAIStateGroupBase(35); } };
class CAIStateGroup_MinionKidCalledToMother { public: static CCharString GetName() { return CCharString("SG_MINION_KID_CALLED_TO_MOTHER"); } static CAIStateGroupBase* Alloc() { return new CAIStateGroupBase(36); } };
class CAIStateGroup_CreatureFindCover { public: static CCharString GetName() { return CCharString("SG_CREATURE_FIND_COVER"); } static CAIStateGroupBase* Alloc() { return new CAIStateGroupBase(37); } };
class CAIStateGroup_ChickenPanic { public: static CCharString GetName() { return CCharString("SG_CHICKEN_PANIC"); } static CAIStateGroupBase* Alloc() { return new CAIStateGroupBase(38); } };
class CAIStateGroup_ChickenBeg { public: static CCharString GetName() { return CCharString("SG_CHICKEN_BEG"); } static CAIStateGroupBase* Alloc() { return new CAIStateGroupBase(39); } };
class CAIStateGroup_FireflyWander { public: static CCharString GetName() { return CCharString("SG_FIREFLY_WANDER"); } static CAIStateGroupBase* Alloc() { return new CAIStateGroupBase(40); } };
class CAIStateGroup_ButterflyWander { public: static CCharString GetName() { return CCharString("SG_BUTTERFLY_WANDER"); } static CAIStateGroupBase* Alloc() { return new CAIStateGroupBase(41); } };
class CAIStateGroup_SmallBirdWander { public: static CCharString GetName() { return CCharString("SG_SMALL_BIRD_WANDER"); } static CAIStateGroupBase* Alloc() { return new CAIStateGroupBase(42); } };
class CAIStateGroup_BirdFlyToFood { public: static CCharString GetName() { return CCharString("SG_BIRD_FLY_TO_FOOD"); } static CAIStateGroupBase* Alloc() { return new CAIStateGroupBase(43); } };
class CAIStateGroup_BirdGroundWander { public: static CCharString GetName() { return CCharString("SG_BIRD_GROUND_WANDER"); } static CAIStateGroupBase* Alloc() { return new CAIStateGroupBase(44); } };
class CAIStateGroup_BirdStartled { public: static CCharString GetName() { return CCharString("SG_BIRD_STARTLED"); } static CAIStateGroupBase* Alloc() { return new CAIStateGroupBase(45); } };
class CAIStateGroup_Guard { public: static CCharString GetName() { return CCharString("SG_GUARD"); } static CAIStateGroupBase* Alloc() { return new CAIStateGroupBase(46); } };
class CAIStateGroup_Wander { public: static CCharString GetName() { return CCharString("SG_WANDER"); } static CAIStateGroupBase* Alloc() { return new CAIStateGroupBase(47); } };
class CAIStateGroup_ChickenWander { public: static CCharString GetName() { return CCharString("SG_CHICKEN_WANDER"); } static CAIStateGroupBase* Alloc() { return new CAIStateGroupBase(48); } };
class CAIStateGroup_WanderAroundInitialPos { public: static CCharString GetName() { return CCharString("SG_WANDER_AROUND_INITIAL_POS"); } static CAIStateGroupBase* Alloc() { return new CAIStateGroupBase(49); } };
class CAIStateGroup_MeleeCombatNearby { public: static CCharString GetName() { return CCharString("SG_MELEE_COMBAT_NEARBY"); } static CAIStateGroupBase* Alloc() { return new CAIStateGroupBase(50); } };
class CAIStateGroup_MeleeCombatNearby2 { public: static CCharString GetName() { return CCharString("SG_MELEE_COMBAT_NEARBY2"); } static CAIStateGroupBase* Alloc() { return new CAIStateGroupBase(51); } };
class CAIStateGroup_MeleeCombatNearbyAttackWhenVeryClose { public: static CCharString GetName() { return CCharString("SG_MELEE_COMBAT_NEARBY_ATTACK_WHEN_VERY_CLOSE"); } static CAIStateGroupBase* Alloc() { return new CAIStateGroupBase(52); } };
class CAIStateGroup_NonVillageGuardPatrol { public: static CCharString GetName() { return CCharString("SG_NON_VILLAGE_GUARD_PATROL"); } static CAIStateGroupBase* Alloc() { return new CAIStateGroupBase(53); } };
class CAIStateGroup_SeekEnemy { public: static CCharString GetName() { return CCharString("SG_SEEK_ENEMY"); } static CAIStateGroupBase* Alloc() { return new CAIStateGroupBase(54); } };
class CAIStateGroup_SeekScriptTarget { public: static CCharString GetName() { return CCharString("SG_SEEK_SCRIPT_TARGET"); } static CAIStateGroupBase* Alloc() { return new CAIStateGroupBase(55); } };
class CAIStateGroup_StealthSeekEnemy { public: static CCharString GetName() { return CCharString("SG_STEALTH_SEEK_ENEMY"); } static CAIStateGroupBase* Alloc() { return new CAIStateGroupBase(56); } };
class CAIStateGroup_RangedCombatUsePreferredFirePoint { public: static CCharString GetName() { return CCharString("SG_CREATURE_RANGED_COMBAT_USE_PREFERRED_FIRE_POINT"); } static CAIStateGroupBase* Alloc() { return new CAIStateGroupBase(57); } };
class CAIStateGroup_CreatureRangedCombatUseWillShield { public: static CCharString GetName() { return CCharString("SG_CREATURE_RANGED_COMBAT_USE_WILL_SHIELD"); } static CAIStateGroupBase* Alloc() { return new CAIStateGroupBase(58); } };
class CAIStateGroup_VillageTaskTorch { public: static CCharString GetName() { return CCharString("SG_VILLAGE_TASK_TORCH"); } static CAIStateGroupBase* Alloc() { return new CAIStateGroupBase(59); } };
class CAIStateGroup_VillageTaskPatrol { public: static CCharString GetName() { return CCharString("SG_VILLAGE_TASK_PATROL"); } static CAIStateGroupBase* Alloc() { return new CAIStateGroupBase(60); } };
class CAIStateGroup_VillageTaskSecuritySweep { public: static CCharString GetName() { return CCharString("SG_VILLAGE_TASK_SECURITY_SWEEP"); } static CAIStateGroupBase* Alloc() { return new CAIStateGroupBase(61); } };
class CAIStateGroup_VillageTaskPickCrops { public: static CCharString GetName() { return CCharString("SG_VILLAGE_TASK_PICK_CROPS"); } static CAIStateGroupBase* Alloc() { return new CAIStateGroupBase(62); } };
class CAIStateGroup_VillageTaskHorn { public: static CCharString GetName() { return CCharString("SG_VILLAGE_TASK_HORN"); } static CAIStateGroupBase* Alloc() { return new CAIStateGroupBase(63); } };
class CAIStateGroup_KidWanderNearKids { public: static CCharString GetName() { return CCharString("SG_KID_WANDER_NEAR_KIDS"); } static CAIStateGroupBase* Alloc() { return new CAIStateGroupBase(64); } };
class CAIStateGroup_KidWatchHopscotch { public: static CCharString GetName() { return CCharString("SG_KID_WATCH_HOPSCOTCH"); } static CAIStateGroupBase* Alloc() { return new CAIStateGroupBase(65); } };
class CAIStateGroup_KidPlayHopscotch { public: static CCharString GetName() { return CCharString("SG_KID_PLAY_HOPSCOTCH"); } static CAIStateGroupBase* Alloc() { return new CAIStateGroupBase(66); } };
class CAIStateGroup_KidHurtSelfRunToMother { public: static CCharString GetName() { return CCharString("SG_KID_HURT_SELF_RUN_TO_MOTHER"); } static CAIStateGroupBase* Alloc() { return new CAIStateGroupBase(67); } };
class CAIStateGroup_KidReactToBlockage { public: static CCharString GetName() { return CCharString("SG_KID_REACT_TO_BLOCKAGE"); } static CAIStateGroupBase* Alloc() { return new CAIStateGroupBase(68); } };
class CAIStateGroup_IdleAtHome { public: static CCharString GetName() { return CCharString("SG_IDLE_AT_HOME"); } static CAIStateGroupBase* Alloc() { return new CAIStateGroupBase(69); } };
class CAIStateGroup_WorkAtSchool { public: static CCharString GetName() { return CCharString("SG_WORK_AT_SCHOOL"); } static CAIStateGroupBase* Alloc() { return new CAIStateGroupBase(70); } };
class CAIStateGroup_TellStory { public: static CCharString GetName() { return CCharString("SG_TELL_STORY"); } static CAIStateGroupBase* Alloc() { return new CAIStateGroupBase(71); } };
class CAIStateGroup_WorkInWarehouse { public: static CCharString GetName() { return CCharString("SG_WORK_IN_WAREHOUSE"); } static CAIStateGroupBase* Alloc() { return new CAIStateGroupBase(72); } };
class CAIStateGroup_WorkSawingWood { public: static CCharString GetName() { return CCharString("SG_WORK_SAWING_WOOD"); } static CAIStateGroupBase* Alloc() { return new CAIStateGroupBase(73); } };
class CAIStateGroup_WorkHammerAnvil { public: static CCharString GetName() { return CCharString("SG_WORK_HAMMER_ANVIL"); } static CAIStateGroupBase* Alloc() { return new CAIStateGroupBase(74); } };
class CAIStateGroup_ThrowLogsOnFire { public: static CCharString GetName() { return CCharString("SG_THROW_LOGS_ON_FIRE"); } static CAIStateGroupBase* Alloc() { return new CAIStateGroupBase(75); } };
class CAIStateGroup_StayNearSchool { public: static CCharString GetName() { return CCharString("SG_STAY_NEAR_SCHOOL"); } static CAIStateGroupBase* Alloc() { return new CAIStateGroupBase(76); } };
class CAIStateGroup_CleanHouse { public: static CCharString GetName() { return CCharString("SG_CLEAN_HOUSE"); } static CAIStateGroupBase* Alloc() { return new CAIStateGroupBase(77); } };
class CAIStateGroup_CleanWork { public: static CCharString GetName() { return CCharString("SG_CLEAN_WORK"); } static CAIStateGroupBase* Alloc() { return new CAIStateGroupBase(78); } };
class CAIStateGroup_SitForMeal { public: static CCharString GetName() { return CCharString("SG_SIT_FOR_MEAL"); } static CAIStateGroupBase* Alloc() { return new CAIStateGroupBase(79); } };
class CAIStateGroup_PrepareMeal { public: static CCharString GetName() { return CCharString("SG_PREPARE_MEAL"); } static CAIStateGroupBase* Alloc() { return new CAIStateGroupBase(80); } };
class CAIStateGroup_GazeAtHome { public: static CCharString GetName() { return CCharString("SG_GAZE_AT_HOME"); } static CAIStateGroupBase* Alloc() { return new CAIStateGroupBase(81); } };
class CAIStateGroup_GazeAtWork { public: static CCharString GetName() { return CCharString("SG_GAZE_AT_WORK"); } static CAIStateGroupBase* Alloc() { return new CAIStateGroupBase(82); } };
class CAIStateGroup_GazeOutside { public: static CCharString GetName() { return CCharString("SG_GAZE_OUTSIDE"); } static CAIStateGroupBase* Alloc() { return new CAIStateGroupBase(83); } };
class CAIStateGroup_WanderAroundShops { public: static CCharString GetName() { return CCharString("SG_WANDER_AROUND_SHOPS"); } static CAIStateGroupBase* Alloc() { return new CAIStateGroupBase(84); } };
class CAIStateGroup_StayNearFoodHall { public: static CCharString GetName() { return CCharString("SG_STAY_NEAR_FOOD_HALL"); } static CAIStateGroupBase* Alloc() { return new CAIStateGroupBase(85); } };
class CAIStateGroup_GoToPub { public: static CCharString GetName() { return CCharString("SG_GO_TO_PUB"); } static CAIStateGroupBase* Alloc() { return new CAIStateGroupBase(86); } };
class CAIStateGroup_SitInPub { public: static CCharString GetName() { return CCharString("SG_SIT_IN_PUB"); } static CAIStateGroupBase* Alloc() { return new CAIStateGroupBase(87); } };
class CAIStateGroup_LightHomeCandle { public: static CCharString GetName() { return CCharString("SG_HOME_CANDLE"); } static CAIStateGroupBase* Alloc() { return new CAIStateGroupBase(88); } };
class CAIStateGroup_LightWorkCandle { public: static CCharString GetName() { return CCharString("SG_WORK_CANDLE"); } static CAIStateGroupBase* Alloc() { return new CAIStateGroupBase(89); } };
class CAIStateGroup_SeekPerceivedThing { public: static CCharString GetName() { return CCharString("SG_SEEK_PERCEIVED_THING"); } static CAIStateGroupBase* Alloc() { return new CAIStateGroupBase(90); } };
class CAIStateGroup_FollowPlayer { public: static CCharString GetName() { return CCharString("SG_MINION_FOLLOW_PLAYER"); } static CAIStateGroupBase* Alloc() { return new CAIStateGroupBase(91); } };
class CAIStateGroup_MinionHawkingFromStall { public: static CCharString GetName() { return CCharString("SG_MINION_HAWKING_FROM_STALL"); } static CAIStateGroupBase* Alloc() { return new CAIStateGroupBase(92); } };
class CAIStateGroup_BuyFromSeller { public: static CCharString GetName() { return CCharString("SG_BUY_FROM_SELLER"); } static CAIStateGroupBase* Alloc() { return new CAIStateGroupBase(93); } };
class CAIStateGroup_SellToBuyer { public: static CCharString GetName() { return CCharString("SG_SELL_TO_BUYER"); } static CAIStateGroupBase* Alloc() { return new CAIStateGroupBase(94); } };
class CAIStateGroup_HandyMan { public: static CCharString GetName() { return CCharString("SG_HANDY_MAN"); } static CAIStateGroupBase* Alloc() { return new CAIStateGroupBase(95); } };
class CAIStateGroup_LookAtInterestingThings { public: static CCharString GetName() { return CCharString("SG_LOOK_AT_INTERESTING_THINGS"); } static CAIStateGroupBase* Alloc() { return new CAIStateGroupBase(96); } };
class CAIStateGroup_GameMaster { public: static CCharString GetName() { return CCharString("SG_GAME_MASTER"); } static CAIStateGroupBase* Alloc() { return new CAIStateGroupBase(97); } };
class CAIStateGroup_StandStill { public: static CCharString GetName() { return CCharString("SG_STAND_STILL"); } static CAIStateGroupBase* Alloc() { return new CAIStateGroupBase(98); } };
class CAIStateGroup_BalverineWander { public: static CCharString GetName() { return CCharString("SG_BALVERINE_WANDER"); } static CAIStateGroupBase* Alloc() { return new CAIStateGroupBase(99); } };
class CAIStateGroup_BalverineCornered { public: static CCharString GetName() { return CCharString("SG_BALVERINE_CORNERED"); } static CAIStateGroupBase* Alloc() { return new CAIStateGroupBase(100); } };
class CAIStateGroup_BalverineJumpAtFleeingEnemy { public: static CCharString GetName() { return CCharString("SG_BALVERINE_JUMP_AT_FLEEING_ENEMY"); } static CAIStateGroupBase* Alloc() { return new CAIStateGroupBase(101); } };
class CAIStateGroup_NymphFloatAround { public: static CCharString GetName() { return CCharString("SG_NYMPH_FLOAT_AROUND"); } static CAIStateGroupBase* Alloc() { return new CAIStateGroupBase(102); } };
class CAIStateGroup_NymphAttack { public: static CCharString GetName() { return CCharString("SG_NYMPH_ATTACK"); } static CAIStateGroupBase* Alloc() { return new CAIStateGroupBase(103); } };
class CAIStateGroup_HoverWander { public: static CCharString GetName() { return CCharString("SG_HOVER_WANDER"); } static CAIStateGroupBase* Alloc() { return new CAIStateGroupBase(104); } };
class CAIStateGroup_HoverWanderAroundThing { public: static CCharString GetName() { return CCharString("SG_HOVER_WANDER_AROUND_THING"); } static CAIStateGroupBase* Alloc() { return new CAIStateGroupBase(105); } };
class CAIStateGroup_HoverAttack { public: static CCharString GetName() { return CCharString("SG_HOVER_ATTACK"); } static CAIStateGroupBase* Alloc() { return new CAIStateGroupBase(106); } };
class CAIStateGroup_HoverAttackUnlessTargetIsFleeing { public: static CCharString GetName() { return CCharString("SG_HOVER_ATTACK_UNLESS_TARGET_FLEEING"); } static CAIStateGroupBase* Alloc() { return new CAIStateGroupBase(107); } };
class CAIStateGroup_HoverKeepInArea { public: static CCharString GetName() { return CCharString("SG_HOVER_KEEP_IN_AREA"); } static CAIStateGroupBase* Alloc() { return new CAIStateGroupBase(108); } };
class CAIStateGroup_HoverAttackArea { public: static CCharString GetName() { return CCharString("SG_HOVER_ATTACK_AREA"); } static CAIStateGroupBase* Alloc() { return new CAIStateGroupBase(109); } };
class CAIStateGroup_HoverHornetQueenTakeDamage { public: static CCharString GetName() { return CCharString("SG_HOVER_HORNET_QUEEN_TAKE_DAMAGE"); } static CAIStateGroupBase* Alloc() { return new CAIStateGroupBase(110); } };
class CAIStateGroup_HoverHornetQueen { public: static CCharString GetName() { return CCharString("SG_HOVER_HORNET_QUEEN"); } static CAIStateGroupBase* Alloc() { return new CAIStateGroupBase(111); } };
class CAIStateGroup_HoverAllAroundTest { public: static CCharString GetName() { return CCharString("SG_HOVER_ALL_AROUND_TEST"); } static CAIStateGroupBase* Alloc() { return new CAIStateGroupBase(112); } };
class CAIStateGroup_EarthTroll { public: static CCharString GetName() { return CCharString("SG_EARTH_TROLL"); } static CAIStateGroupBase* Alloc() { return new CAIStateGroupBase(113); } };
class CAIStateGroup_RockTroll { public: static CCharString GetName() { return CCharString("SG_ROCK_TROLL"); } static CAIStateGroupBase* Alloc() { return new CAIStateGroupBase(114); } };
class CAIStateGroup_IceTrollIcicleAttack { public: static CCharString GetName() { return CCharString("SG_ICE_TROLL_ICICLE_ATTACK"); } static CAIStateGroupBase* Alloc() { return new CAIStateGroupBase(115); } };
class CAIStateGroup_IceTrollPoundGroundAttack { public: static CCharString GetName() { return CCharString("SG_ICE_TROLL_POUND_GROUND_ATTACK"); } static CAIStateGroupBase* Alloc() { return new CAIStateGroupBase(116); } };
class CAIStateGroup_IceTrollIcicleAssault { public: static CCharString GetName() { return CCharString("SG_ICE_TROLL_ICICLE_ASSAULT"); } static CAIStateGroupBase* Alloc() { return new CAIStateGroupBase(117); } };
class CAIStateGroup_ScorpionKing { public: static CCharString GetName() { return CCharString("SG_SCORPION_KING"); } static CAIStateGroupBase* Alloc() { return new CAIStateGroupBase(118); } };
class CAIStateGroup_JackOfBladesStage1 { public: static CCharString GetName() { return CCharString("SG_JACK_OF_BLADES_STAGE1"); } static CAIStateGroupBase* Alloc() { return new CAIStateGroupBase(119); } };
class CAIStateGroup_JackOfBladesStage2 { public: static CCharString GetName() { return CCharString("SG_JACK_OF_BLADES_STAGE2"); } static CAIStateGroupBase* Alloc() { return new CAIStateGroupBase(120); } };
class CAIStateGroup_JackOfBladesStage3 { public: static CCharString GetName() { return CCharString("SG_JACK_OF_BLADES_STAGE3"); } static CAIStateGroupBase* Alloc() { return new CAIStateGroupBase(121); } };
class CAIStateGroup_MazeTeleport { public: static CCharString GetName() { return CCharString("SG_MAZE_TELEPORT"); } static CAIStateGroupBase* Alloc() { return new CAIStateGroupBase(122); } };
class CAIStateGroup_MazeRangedCombat { public: static CCharString GetName() { return CCharString("SG_MAZE_RANGED_COMBAT"); } static CAIStateGroupBase* Alloc() { return new CAIStateGroupBase(123); } };
class CAIStateGroup_MazeDefensiveMeleeCombat { public: static CCharString GetName() { return CCharString("SG_MAZE_DEFENSIVE_MELEE_COMBAT"); } static CAIStateGroupBase* Alloc() { return new CAIStateGroupBase(124); } };
class CAIStateGroup_MazeShieldedChase { public: static CCharString GetName() { return CCharString("SG_MAZE_SHIELDED_CHASE"); } static CAIStateGroupBase* Alloc() { return new CAIStateGroupBase(125); } };
class CAIStateGroup_MazeTutorialBattle { public: static CCharString GetName() { return CCharString("SG_MAZE_TUTORIAL_BATTLE"); } static CAIStateGroupBase* Alloc() { return new CAIStateGroupBase(126); } };
class CAIStateGroup_MazeTutorialBattleTeleport { public: static CCharString GetName() { return CCharString("SG_MAZE_TUTORIAL_BATTLE_TELEPORT"); } static CAIStateGroupBase* Alloc() { return new CAIStateGroupBase(127); } };
class CAIStateGroup_MazeTutorialBattleRespondToLightning { public: static CCharString GetName() { return CCharString("SG_MAZE_TUTORIAL_BATTLE_RESPOND_TO_LIGHTNING"); } static CAIStateGroupBase* Alloc() { return new CAIStateGroupBase(128); } };
class CAIStateGroup_MazeTutorialBattleUseShield { public: static CCharString GetName() { return CCharString("SG_MAZE_TUTORIAL_BATTLE_USE_SHIELD"); } static CAIStateGroupBase* Alloc() { return new CAIStateGroupBase(129); } };
class CAIStateGroup_Guildmaster { public: static CCharString GetName() { return CCharString("SG_GUILDMASTER"); } static CAIStateGroupBase* Alloc() { return new CAIStateGroupBase(130); } };
class CAIStateGroup_BriarRose { public: static CCharString GetName() { return CCharString("SG_BRIAR_ROSE"); } static CAIStateGroupBase* Alloc() { return new CAIStateGroupBase(131); } };
class CAIStateGroup_BriarRoseWillUse { public: static CCharString GetName() { return CCharString("SG_BRIAR_ROSE_WILL_USE"); } static CAIStateGroupBase* Alloc() { return new CAIStateGroupBase(132); } };
class CAIStateGroup_VersusRockTroll { public: static CCharString GetName() { return CCharString("SG_VERSUS_ROCK_TROLL"); } static CAIStateGroupBase* Alloc() { return new CAIStateGroupBase(133); } };
class CAIStateGroup_VersusScorpionKing { public: static CCharString GetName() { return CCharString("SG_VERSUS_SCORPION_KING"); } static CAIStateGroupBase* Alloc() { return new CAIStateGroupBase(134); } };
class CAIStateGroup_CreatureRangedCombat2 { public: static CCharString GetName() { return CCharString("SG_CREATURE_RANGED_COMBAT_TWO"); } static CAIStateGroupBase* Alloc() { return new CAIStateGroupBase(135); } };
class CAIStateGroup_CreatureTakeCover { public: static CCharString GetName() { return CCharString("SG_CREATURE_RANGED_COMBAT_USE_COVER"); } static CAIStateGroupBase* Alloc() { return new CAIStateGroupBase(136); } };
class CAIStateGroup_CreatureAdvanceInCover { public: static CCharString GetName() { return CCharString("SG_CREATURE_ADVANCE_IN_COVER"); } static CAIStateGroupBase* Alloc() { return new CAIStateGroupBase(137); } };
class CAIStateGroup_CreatureRangedCombatFireAtSpecificTargets { public: static CCharString GetName() { return CCharString("SG_CREATURE_RANGED_COMBAT_FIRE_AT_SPECIFIC_TARGETS"); } static CAIStateGroupBase* Alloc() { return new CAIStateGroupBase(138); } };
class CAIStateGroup_TestTestTest { public: static CCharString GetName() { return CCharString("SG_CREATURE_TEST_TEST_TEST"); } static CAIStateGroupBase* Alloc() { return new CAIStateGroupBase(139); } };
class CAIStateGroup_CreatureGoUnderground { public: static CCharString GetName() { return CCharString("SG_CREATURE_GO_UNDERGROUND"); } static CAIStateGroupBase* Alloc() { return new CAIStateGroupBase(140); } };
class CAIStateGroup_CreatureIdleUnderground { public: static CCharString GetName() { return CCharString("SG_CREATURE_IDLE_UNDERGROUND"); } static CAIStateGroupBase* Alloc() { return new CAIStateGroupBase(141); } };
class CAIStateGroup_CreatureSeekEnemyUnderground { public: static CCharString GetName() { return CCharString("SG_CREATURE_SEEK_ENEMY_UNDERGROUND"); } static CAIStateGroupBase* Alloc() { return new CAIStateGroupBase(142); } };
class CAIStateGroup_BeetleToggleHover { public: static CCharString GetName() { return CCharString("SG_BEETLE_TOGGLE_HOVER"); } static CAIStateGroupBase* Alloc() { return new CAIStateGroupBase(143); } };
class CAIStateGroup_MinionBehaviourGreetCreature { public: static CCharString GetName() { return CCharString("SG_MINION_BEHAVIOUR_GREET_CREATURE"); } static CAIStateGroupBase* Alloc() { return new CAIStateGroupBase(144); } };
class CAIStateGroup_MinionBehaviourSad { public: static CCharString GetName() { return CCharString("SG_MINION_BEHAVIOUR_SAD"); } static CAIStateGroupBase* Alloc() { return new CAIStateGroupBase(145); } };
class CAIStateGroup_MinionBehaviourPodiumEmoteNeutral { public: static CCharString GetName() { return CCharString("SG_MINION_BEHAVIOUR_PODIUM_EMOTE_NEUTRAL"); } static CAIStateGroupBase* Alloc() { return new CAIStateGroupBase(146); } };
class CAIStateGroup_MinionMotherConsoleChild { public: static CCharString GetName() { return CCharString("SG_MINION_MOTHER_CONSOLE_CHILD"); } static CAIStateGroupBase* Alloc() { return new CAIStateGroupBase(147); } };
class CAIStateGroup_MinionReactToBurglary { public: static CCharString GetName() { return CCharString("SG_MINION_REACT_TO_BURGLARY"); } static CAIStateGroupBase* Alloc() { return new CAIStateGroupBase(148); } };
class CAIStateGroup_MinionReactToTrespassing { public: static CCharString GetName() { return CCharString("SG_MINION_REACT_TO_TRESPASSING"); } static CAIStateGroupBase* Alloc() { return new CAIStateGroupBase(149); } };
class CAIStateGroup_MinionReactToNoise { public: static CCharString GetName() { return CCharString("SG_MINION_REACT_TO_NOISE"); } static CAIStateGroupBase* Alloc() { return new CAIStateGroupBase(150); } };
class CAIStateGroup_MinionReactToApology { public: static CCharString GetName() { return CCharString("SG_MINION_REACT_TO_APOLOGY"); } static CAIStateGroupBase* Alloc() { return new CAIStateGroupBase(151); } };
class CAIStateGroup_MinionReactToCrime { public: static CCharString GetName() { return CCharString("SG_MINION_REACT_TO_CRIME"); } static CAIStateGroupBase* Alloc() { return new CAIStateGroupBase(152); } };
class CAIStateGroup_MinionReactToCrimeFromSleep { public: static CCharString GetName() { return CCharString("SG_MINION_REACT_TO_CRIME_FROM_SLEEP"); } static CAIStateGroupBase* Alloc() { return new CAIStateGroupBase(153); } };
class CAIStateGroup_MinionLeaveRegion { public: static CCharString GetName() { return CCharString("SG_MINION_LEAVE_REGION"); } static CAIStateGroupBase* Alloc() { return new CAIStateGroupBase(154); } };
class CAIStateGroup_ReactToDeadLeader { public: static CCharString GetName() { return CCharString("SG_REACT_TO_DEAD_LEADER"); } static CAIStateGroupBase* Alloc() { return new CAIStateGroupBase(155); } };
class CAIStateGroup_BoastingCrowdMember { public: static CCharString GetName() { return CCharString("SG_BOASTING_CROWD_MEMBER"); } static CAIStateGroupBase* Alloc() { return new CAIStateGroupBase(156); } };
class CAIStateGroup_OpinionReactionRoot { public: static CCharString GetName() { return CCharString("SG_OPINION_REACTION_ROOT"); } static CAIStateGroupBase* Alloc() { return new CAIStateGroupBase(157); } };
class CAIStateGroup_DeedReactionRoot { public: static CCharString GetName() { return CCharString("SG_DEED_REACTION_ROOT"); } static CAIStateGroupBase* Alloc() { return new CAIStateGroupBase(158); } };
class CAIStateGroup_DeedReactionRootFromSleep { public: static CCharString GetName() { return CCharString("SG_DEED_REACTION_ROOT_FROM_SLEEP"); } static CAIStateGroupBase* Alloc() { return new CAIStateGroupBase(159); } };
class CAIStateGroup_BanditKingCharge { public: static CCharString GetName() { return CCharString("SG_BANDIT_KING_CHARGE"); } static CAIStateGroupBase* Alloc() { return new CAIStateGroupBase(160); } };
class CAIStateGroup_BanditKingLunge { public: static CCharString GetName() { return CCharString("SG_BANDIT_KING_LUNGE"); } static CAIStateGroupBase* Alloc() { return new CAIStateGroupBase(161); } };
class CAIStateGroup_BanditKingThrowWeapons { public: static CCharString GetName() { return CCharString("SG_BANDIT_KING_THROW_WEAPONS"); } static CAIStateGroupBase* Alloc() { return new CAIStateGroupBase(162); } };
class CAIStateGroup_JackDragon { public: static CCharString GetName() { return CCharString("SG_JACK_DRAGON"); } static CAIStateGroupBase* Alloc() { return new CAIStateGroupBase(163); } };
class CAIStateGroup_MinionFollowThing { public: static CCharString GetName() { return CCharString("SG_MINION_FOLLOW_THING"); } static CAIStateGroupBase* Alloc() { return new CAIStateGroupBase(164); } };
class CAIStateGroup_MinionWalkToMarker { public: static CCharString GetName() { return CCharString("SG_MINION_WALK_TO_MARKER"); } static CAIStateGroupBase* Alloc() { return new CAIStateGroupBase(165); } };
class CAIStateGroup_TavernServer { public: static CCharString GetName() { return CCharString("SG_TAVERN_SERVER"); } static CAIStateGroupBase* Alloc() { return new CAIStateGroupBase(166); } };
class CAIStateGroup_TavernKeeper { public: static CCharString GetName() { return CCharString("SG_TAVERN_KEEPER"); } static CAIStateGroupBase* Alloc() { return new CAIStateGroupBase(167); } };
class CAIStateGroup_StealStealableItem { public: static CCharString GetName() { return CCharString("SG_STEAL_STEALABLE_ITEM"); } static CAIStateGroupBase* Alloc() { return new CAIStateGroupBase(168); } };
class CAIStateGroup_RecoverStealableItem { public: static CCharString GetName() { return CCharString("SG_RECOVER_STEALABLE_ITEM"); } static CAIStateGroupBase* Alloc() { return new CAIStateGroupBase(169); } };
class CAIStateGroup_CarryCrateAndFollow { public: static CCharString GetName() { return CCharString("SG_CARRY_CRATE_AND_FOLLOW"); } static CAIStateGroupBase* Alloc() { return new CAIStateGroupBase(170); } };
class CAIStateGroup_SleepOnFloor { public: static CCharString GetName() { return CCharString("SG_SLEEP_ON_FLOOR"); } static CAIStateGroupBase* Alloc() { return new CAIStateGroupBase(171); } };
class CAIStateGroup_WhisperRetreat { public: static CCharString GetName() { return CCharString("SG_WHISPER_RETREAT"); } static CAIStateGroupBase* Alloc() { return new CAIStateGroupBase(172); } };
class CAIStateGroup_WhisperPotionCombat { public: static CCharString GetName() { return CCharString("SG_WHISPER_POTION_COMBAT"); } static CAIStateGroupBase* Alloc() { return new CAIStateGroupBase(173); } };
class CAIStateGroup_Thunder { public: static CCharString GetName() { return CCharString("SG_THUNDER"); } static CAIStateGroupBase* Alloc() { return new CAIStateGroupBase(174); } };
class CAIStateGroup_Freeze { public: static CCharString GetName() { return CCharString("SG_FREEZE"); } static CAIStateGroupBase* Alloc() { return new CAIStateGroupBase(175); } };
class CAIStateGroup_Drunk { public: static CCharString GetName() { return CCharString("SG_DRUNK"); } static CAIStateGroupBase* Alloc() { return new CAIStateGroupBase(176); } };
class CAIStateGroup_ServeFoodAtHome { public: static CCharString GetName() { return CCharString("SG_SERVE_FOOD_AT_HOME"); } static CAIStateGroupBase* Alloc() { return new CAIStateGroupBase(177); } };
class CAIStateGroup_ServeFoodAtTavern { public: static CCharString GetName() { return CCharString("SG_SERVE_FOOD_AT_TAVERN"); } static CAIStateGroupBase* Alloc() { return new CAIStateGroupBase(178); } };
class CAIStateGroup_EatAtHome { public: static CCharString GetName() { return CCharString("SG_EAT_AT_HOME"); } static CAIStateGroupBase* Alloc() { return new CAIStateGroupBase(179); } };
class CAIStateGroup_EatAtTavern { public: static CCharString GetName() { return CCharString("SG_EAT_AT_TAVERN"); } static CAIStateGroupBase* Alloc() { return new CAIStateGroupBase(180); } };
class CAIStateGroup_ScreamerPursueTarget { public: static CCharString GetName() { return CCharString("SG_SCREAMER_PURSUE_TARGET"); } static CAIStateGroupBase* Alloc() { return new CAIStateGroupBase(181); } };
class CAIStateGroup_KrakenAttack { public: static CCharString GetName() { return CCharString("SG_KRAKEN_ATTACK"); } static CAIStateGroupBase* Alloc() { return new CAIStateGroupBase(182); } };
class CAIStateGroup_KrakenTentacleIdle { public: static CCharString GetName() { return CCharString("SG_KRAKEN_TENTACLE_IDLE"); } static CAIStateGroupBase* Alloc() { return new CAIStateGroupBase(183); } };
class CAIStateGroup_KrakenTentacleRise { public: static CCharString GetName() { return CCharString("SG_KRAKEN_TENTACLE_RISE"); } static CAIStateGroupBase* Alloc() { return new CAIStateGroupBase(184); } };
class CAIStateGroup_KrakenTentacleAttack { public: static CCharString GetName() { return CCharString("SG_KRAKEN_TENTACLE_ATTACK"); } static CAIStateGroupBase* Alloc() { return new CAIStateGroupBase(185); } };
class CAIStateGroup_WifeGiveYouGift { public: static CCharString GetName() { return CCharString("SG_WIFE_GIVE_YOU_GIFT"); } static CAIStateGroupBase* Alloc() { return new CAIStateGroupBase(186); } };
class CAIStateGroup_OpenAndCloseShop { public: static CCharString GetName() { return CCharString("SG_OPEN_AND_CLOSE_SHOP"); } static CAIStateGroupBase* Alloc() { return new CAIStateGroupBase(187); } };
class CAIStateGroup_ShopkeeperWatchPlayer { public: static CCharString GetName() { return CCharString("SG_SHOPKEEPER_WATCH_PLAYER"); } static CAIStateGroupBase* Alloc() { return new CAIStateGroupBase(188); } };
class CAIStateGroup_SetupWares { public: static CCharString GetName() { return CCharString("SG_SETUP_WARES"); } static CAIStateGroupBase* Alloc() { return new CAIStateGroupBase(189); } };
class CAIStateGroup_BanditPlayDice { public: static CCharString GetName() { return CCharString("SG_BANDIT_PLAY_DICE"); } static CAIStateGroupBase* Alloc() { return new CAIStateGroupBase(190); } };
class CAIStateGroup_BanditPlot { public: static CCharString GetName() { return CCharString("SG_BANDIT_PLOT"); } static CAIStateGroupBase* Alloc() { return new CAIStateGroupBase(191); } };
class CAIStateGroup_MinionGuardGreetPlayer { public: static CCharString GetName() { return CCharString("SG_MINION_GUARD_GREET_PLAYER"); } static CAIStateGroupBase* Alloc() { return new CAIStateGroupBase(192); } };
class CAIStateGroup_MinionGuardFollowPlayer { public: static CCharString GetName() { return CCharString("SG_MINION_GUARD_FOLLOW_PLAYER"); } static CAIStateGroupBase* Alloc() { return new CAIStateGroupBase(193); } };
class CAIStateGroup_MinionGuardBribed { public: static CCharString GetName() { return CCharString("SG_MINION_GUARD_BRIBED"); } static CAIStateGroupBase* Alloc() { return new CAIStateGroupBase(194); } };
class CAIStateGroup_ApprenticeHero { public: static CCharString GetName() { return CCharString("SG_APPRENTICE_HERO"); } static CAIStateGroupBase* Alloc() { return new CAIStateGroupBase(195); } };
class CAIStateGroup_SkillPractice { public: static CCharString GetName() { return CCharString("SG_SKILL_PRACTICE"); } static CAIStateGroupBase* Alloc() { return new CAIStateGroupBase(196); } };
class CAIStateGroup_OnHeroPush { public: static CCharString GetName() { return CCharString("SG_ON_HERO_PUSH"); } static CAIStateGroupBase* Alloc() { return new CAIStateGroupBase(197); } };
class CAIStateGroup_AvoidRegionExit { public: static CCharString GetName() { return CCharString("SG_AVOID_REGION_EXIT"); } static CAIStateGroupBase* Alloc() { return new CAIStateGroupBase(198); } };
class CAIStateGroup_SummonerLightningAttack { public: static CCharString GetName() { return CCharString("SG_SUMMONER_LIGHTNING_ATTACK"); } static CAIStateGroupBase* Alloc() { return new CAIStateGroupBase(199); } };
class CAIStateGroup_SummonerSummon { public: static CCharString GetName() { return CCharString("SG_SUMMONER_SUMMON"); } static CAIStateGroupBase* Alloc() { return new CAIStateGroupBase(200); } };
class CAIStateGroup_SummonerCharge { public: static CCharString GetName() { return CCharString("SG_SUMMONER_CHARGE"); } static CAIStateGroupBase* Alloc() { return new CAIStateGroupBase(201); } };
class CAIBrain { friend int main(); static const CAIStateGroupInfo StateGroupInfo[]; };
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
    CAIStateGroupInfo("", 0)
};
static const char* const ExpectedName[202] = {
    "SG_MINION_WANDER",
    "SG_MINION_SLEEP",
    "SG_MINION_GO_TO_BED",
    "SG_MINION_WORK_IN_INN",
    "SG_MINION_FIND_PERSONAL_SPACE",
    "SG_MINION_REACT_TO_PLAYER_INTERACTION",
    "SG_MINION_GUARD_HEARD_YELL_FOR_HELP",
    "SG_MINION_GUARD_REACT_TO_PLAYER_WEAPON",
    "SG_MINION_GUARD_REACT_TO_BODY_FOUND_MESSAGE",
    "SG_BANDIT_SIT_ROUND_CAMPFIRE",
    "SG_MINION_REACT_TO_CRIMINAL",
    "SG_MINION_GUARD_REACT_TO_CRIMINAL",
    "SG_MINION_GUARD_REACT_TO_BRIBE",
    "SG_PERFORM_ACTION",
    "SG_MINION_FACE_HERO_READY_FOR_INTERACTION",
    "SG_MINION_PUT_OUT_FIRES",
    "SG_MINION_WATCH_FIRES",
    "SG_MINION_REACT_TO_ALERT",
    "SG_MINION_REACT_TO_ENEMY",
    "SG_MINION_REACT_TO_CORPSE",
    "SG_MINION_REACT_TO_UNCONSCIOUS_PERSON",
    "SG_MINION_HAS_INFORMATION",
    "SG_MINION_SIT_ON_SEAT",
    "SG_MINION_SIT_AT_WORK",
    "SG_MINION_SIT_AT_HOME",
    "SG_MINION_SIT_AT_INN",
    "SG_MINION_SIT_AT_SCHOOL",
    "SG_MINION_SIT_OUTSIDE",
    "SG_MINION_LISTEN_TO_STORY",
    "SG_SIT_ROUND_CAMPFIRE",
    "SG_MINION_CURIOSITY",
    "SG_MINION_GOSSIP",
    "SG_BANDIT_GOSSIP",
    "SG_MINION_MOTHER_PROTECT_FAMILY",
    "SG_MINION_KID_ANNOY",
    "SG_MINION_KID_GROUP_TAG",
    "SG_MINION_KID_CALLED_TO_MOTHER",
    "SG_CREATURE_FIND_COVER",
    "SG_CHICKEN_PANIC",
    "SG_CHICKEN_BEG",
    "SG_FIREFLY_WANDER",
    "SG_BUTTERFLY_WANDER",
    "SG_SMALL_BIRD_WANDER",
    "SG_BIRD_FLY_TO_FOOD",
    "SG_BIRD_GROUND_WANDER",
    "SG_BIRD_STARTLED",
    "SG_GUARD",
    "SG_WANDER",
    "SG_CHICKEN_WANDER",
    "SG_WANDER_AROUND_INITIAL_POS",
    "SG_MELEE_COMBAT_NEARBY",
    "SG_MELEE_COMBAT_NEARBY2",
    "SG_MELEE_COMBAT_NEARBY_ATTACK_WHEN_VERY_CLOSE",
    "SG_NON_VILLAGE_GUARD_PATROL",
    "SG_SEEK_ENEMY",
    "SG_SEEK_SCRIPT_TARGET",
    "SG_STEALTH_SEEK_ENEMY",
    "SG_CREATURE_RANGED_COMBAT_USE_PREFERRED_FIRE_POINT",
    "SG_CREATURE_RANGED_COMBAT_USE_WILL_SHIELD",
    "SG_VILLAGE_TASK_TORCH",
    "SG_VILLAGE_TASK_PATROL",
    "SG_VILLAGE_TASK_SECURITY_SWEEP",
    "SG_VILLAGE_TASK_PICK_CROPS",
    "SG_VILLAGE_TASK_HORN",
    "SG_KID_WANDER_NEAR_KIDS",
    "SG_KID_WATCH_HOPSCOTCH",
    "SG_KID_PLAY_HOPSCOTCH",
    "SG_KID_HURT_SELF_RUN_TO_MOTHER",
    "SG_KID_REACT_TO_BLOCKAGE",
    "SG_IDLE_AT_HOME",
    "SG_WORK_AT_SCHOOL",
    "SG_TELL_STORY",
    "SG_WORK_IN_WAREHOUSE",
    "SG_WORK_SAWING_WOOD",
    "SG_WORK_HAMMER_ANVIL",
    "SG_THROW_LOGS_ON_FIRE",
    "SG_STAY_NEAR_SCHOOL",
    "SG_CLEAN_HOUSE",
    "SG_CLEAN_WORK",
    "SG_SIT_FOR_MEAL",
    "SG_PREPARE_MEAL",
    "SG_GAZE_AT_HOME",
    "SG_GAZE_AT_WORK",
    "SG_GAZE_OUTSIDE",
    "SG_WANDER_AROUND_SHOPS",
    "SG_STAY_NEAR_FOOD_HALL",
    "SG_GO_TO_PUB",
    "SG_SIT_IN_PUB",
    "SG_HOME_CANDLE",
    "SG_WORK_CANDLE",
    "SG_SEEK_PERCEIVED_THING",
    "SG_MINION_FOLLOW_PLAYER",
    "SG_MINION_HAWKING_FROM_STALL",
    "SG_BUY_FROM_SELLER",
    "SG_SELL_TO_BUYER",
    "SG_HANDY_MAN",
    "SG_LOOK_AT_INTERESTING_THINGS",
    "SG_GAME_MASTER",
    "SG_STAND_STILL",
    "SG_BALVERINE_WANDER",
    "SG_BALVERINE_CORNERED",
    "SG_BALVERINE_JUMP_AT_FLEEING_ENEMY",
    "SG_NYMPH_FLOAT_AROUND",
    "SG_NYMPH_ATTACK",
    "SG_HOVER_WANDER",
    "SG_HOVER_WANDER_AROUND_THING",
    "SG_HOVER_ATTACK",
    "SG_HOVER_ATTACK_UNLESS_TARGET_FLEEING",
    "SG_HOVER_KEEP_IN_AREA",
    "SG_HOVER_ATTACK_AREA",
    "SG_HOVER_HORNET_QUEEN_TAKE_DAMAGE",
    "SG_HOVER_HORNET_QUEEN",
    "SG_HOVER_ALL_AROUND_TEST",
    "SG_EARTH_TROLL",
    "SG_ROCK_TROLL",
    "SG_ICE_TROLL_ICICLE_ATTACK",
    "SG_ICE_TROLL_POUND_GROUND_ATTACK",
    "SG_ICE_TROLL_ICICLE_ASSAULT",
    "SG_SCORPION_KING",
    "SG_JACK_OF_BLADES_STAGE1",
    "SG_JACK_OF_BLADES_STAGE2",
    "SG_JACK_OF_BLADES_STAGE3",
    "SG_MAZE_TELEPORT",
    "SG_MAZE_RANGED_COMBAT",
    "SG_MAZE_DEFENSIVE_MELEE_COMBAT",
    "SG_MAZE_SHIELDED_CHASE",
    "SG_MAZE_TUTORIAL_BATTLE",
    "SG_MAZE_TUTORIAL_BATTLE_TELEPORT",
    "SG_MAZE_TUTORIAL_BATTLE_RESPOND_TO_LIGHTNING",
    "SG_MAZE_TUTORIAL_BATTLE_USE_SHIELD",
    "SG_GUILDMASTER",
    "SG_BRIAR_ROSE",
    "SG_BRIAR_ROSE_WILL_USE",
    "SG_VERSUS_ROCK_TROLL",
    "SG_VERSUS_SCORPION_KING",
    "SG_CREATURE_RANGED_COMBAT_TWO",
    "SG_CREATURE_RANGED_COMBAT_USE_COVER",
    "SG_CREATURE_ADVANCE_IN_COVER",
    "SG_CREATURE_RANGED_COMBAT_FIRE_AT_SPECIFIC_TARGETS",
    "SG_CREATURE_TEST_TEST_TEST",
    "SG_CREATURE_GO_UNDERGROUND",
    "SG_CREATURE_IDLE_UNDERGROUND",
    "SG_CREATURE_SEEK_ENEMY_UNDERGROUND",
    "SG_BEETLE_TOGGLE_HOVER",
    "SG_MINION_BEHAVIOUR_GREET_CREATURE",
    "SG_MINION_BEHAVIOUR_SAD",
    "SG_MINION_BEHAVIOUR_PODIUM_EMOTE_NEUTRAL",
    "SG_MINION_MOTHER_CONSOLE_CHILD",
    "SG_MINION_REACT_TO_BURGLARY",
    "SG_MINION_REACT_TO_TRESPASSING",
    "SG_MINION_REACT_TO_NOISE",
    "SG_MINION_REACT_TO_APOLOGY",
    "SG_MINION_REACT_TO_CRIME",
    "SG_MINION_REACT_TO_CRIME_FROM_SLEEP",
    "SG_MINION_LEAVE_REGION",
    "SG_REACT_TO_DEAD_LEADER",
    "SG_BOASTING_CROWD_MEMBER",
    "SG_OPINION_REACTION_ROOT",
    "SG_DEED_REACTION_ROOT",
    "SG_DEED_REACTION_ROOT_FROM_SLEEP",
    "SG_BANDIT_KING_CHARGE",
    "SG_BANDIT_KING_LUNGE",
    "SG_BANDIT_KING_THROW_WEAPONS",
    "SG_JACK_DRAGON",
    "SG_MINION_FOLLOW_THING",
    "SG_MINION_WALK_TO_MARKER",
    "SG_TAVERN_SERVER",
    "SG_TAVERN_KEEPER",
    "SG_STEAL_STEALABLE_ITEM",
    "SG_RECOVER_STEALABLE_ITEM",
    "SG_CARRY_CRATE_AND_FOLLOW",
    "SG_SLEEP_ON_FLOOR",
    "SG_WHISPER_RETREAT",
    "SG_WHISPER_POTION_COMBAT",
    "SG_THUNDER",
    "SG_FREEZE",
    "SG_DRUNK",
    "SG_SERVE_FOOD_AT_HOME",
    "SG_SERVE_FOOD_AT_TAVERN",
    "SG_EAT_AT_HOME",
    "SG_EAT_AT_TAVERN",
    "SG_SCREAMER_PURSUE_TARGET",
    "SG_KRAKEN_ATTACK",
    "SG_KRAKEN_TENTACLE_IDLE",
    "SG_KRAKEN_TENTACLE_RISE",
    "SG_KRAKEN_TENTACLE_ATTACK",
    "SG_WIFE_GIVE_YOU_GIFT",
    "SG_OPEN_AND_CLOSE_SHOP",
    "SG_SHOPKEEPER_WATCH_PLAYER",
    "SG_SETUP_WARES",
    "SG_BANDIT_PLAY_DICE",
    "SG_BANDIT_PLOT",
    "SG_MINION_GUARD_GREET_PLAYER",
    "SG_MINION_GUARD_FOLLOW_PLAYER",
    "SG_MINION_GUARD_BRIBED",
    "SG_APPRENTICE_HERO",
    "SG_SKILL_PRACTICE",
    "SG_ON_HERO_PUSH",
    "SG_AVOID_REGION_EXIT",
    "SG_SUMMONER_LIGHTNING_ATTACK",
    "SG_SUMMONER_SUMMON",
    "SG_SUMMONER_CHARGE",
};
int main() {
    if (sizeof(CAIStateGroupInfo) != 8 && sizeof(void*) == 4) { std::puts("0121f86f_TEST FAIL size"); return 1; }
    if (g_live != 203 || g_copies < 203) { std::printf("0121f86f_TEST FAIL temporaries %d %d\n", g_live, g_copies); return 1; }
    for (int i = 0; i < 202; ++i) {
        const CAIStateGroupInfo& e = CAIBrain::StateGroupInfo[i];
        if (std::strcmp(e.StateGroupName.Text, ExpectedName[i])) { std::printf("0121f86f_TEST FAIL name %d\n", i); return 1; }
        if (!e.AllocFunc) { std::printf("0121f86f_TEST FAIL alloc %d\n", i); return 1; }
        CAIStateGroupBase* g = e.AllocFunc();
        if (!g || g->Slot != i) { std::printf("0121f86f_TEST FAIL created %d\n", i); return 1; }
        delete g;
        for (int j = 0; j < i; ++j)
            if (CAIBrain::StateGroupInfo[j].AllocFunc == e.AllocFunc) { std::printf("0121f86f_TEST FAIL dup %d\n", i); return 1; }
    }
    if (CAIBrain::StateGroupInfo[202].AllocFunc || CAIBrain::StateGroupInfo[202].StateGroupName.Text[0]) { std::puts("0121f86f_TEST FAIL sentinel"); return 1; }
    std::puts("0121f86f_TEST PASS");
    return 0;
}
