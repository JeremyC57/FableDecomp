// CCreatureActionBase::StatRegisterClass() -- register the creature-action event types, then one prototype of every registered action
//   class in the RegisteredActions map, keyed by its EActionRegisteredType (a null prototype for ACTION_NULL)
// Leaf: ?StatRegisterClass@CCreatureActionBase@@SIXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006F5C10, 8717 bytes (static __fastcall, no arguments; one caller CWorld::Init 0x004A7601).
// Identity (proven): Ego_r.pdb (0x0071BCF0) and FableWin.pdb (0x023ED150) ?StatRegisterClass@CCreatureActionBase@@SIXXZ; the body calls
//   ?RegisterActionEventTypes@CCreatureActionBase@@SIXXZ 0x006932B0 first and fills the map 0x013BA8C8 (Ego_r / FableWin
//   `dynamic initializer for 'CCreatureActionBase::RegisteredActions'`) through insert_unique 0x006F5400. The catalog's
//   `_global::StatRegisterClass` lacks the class.
// Inferred: 1 dead release assert (FABLE_RETAIL_ASSERT(RegisteredActions.empty()), fable_assert.h dead arm, emits no bytes): device d of
//   the owner's acceptance test. Measured: the row's inline budget (c2 caller-size penalty pen = (sz/2)*(a+g)/9000 once a+g > 9000)
//   puts four call-v-expand transitions on retail's blocks (value_type ~CCountedPointer called from block 21, temp ~CCountedPointer
//   called from block 23, CCountedPointer(T*) called from block 21, the pair destructors called from block 45) only with the dead
//   arm's front-end tuples in the caller: without it block 21 expands one destructor too many (a+g 8995 < 9000), with a ~pair(){}
//   instead block 44 calls the pair destructors one block early. Any one assert on the map (empty() / size() == 0, before or after
//   RegisterActionEventTypes) matches; `ASSERT(true)` does not.
// EVIDENCED: the 55 insert blocks (keys, new sizes, ctor calls, vtables), FABLE_RETAIL_STLP_MAP_ORIGINAL_INSERT_BODY (the visible
//   STLport _M_insert body: c2 then sees the value_type temporary is not captured -- no Info reload, no dead zero stores, frame 0x2C),
//   FABLE_RETAIL_CCOUNTEDPOINTER_RELEASEDATA (the out-of-line ~CCountedPointer 0x0048C5C0 / pair dtors 0x00695D90 / 0x006F5260).
// Shared headers: the 41 action classes with an engine/<Class>.h are modelled there under new FABLE_RETAIL_<CLASS>_PROTOTYPE
//   opt-ins (hdr/README.md); the 14 classes without a shared header stay local (ctor addresses / inline ctors from the call sites).
// NF259 (2026-11-22, q12731, nf257 port debt): the row-local CCreatureAction_SummonCreatures model (a class that has engine/
//   CCreatureAction_SummonCreatures.h) moved onto that header with the nf257 SUMMONCREATURES_OVERRIDES opt-in, which declares the default
//   constructor 0x008B83C0 this row calls (??0CCreatureAction_SummonCreatures@@QAE@XZ, unchanged); its ctor stays out of line, so no vtable.
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCOUNTEDPOINTER_RELEASEDATA
#define FABLE_RETAIL_STLP_MAP
#define FABLE_RETAIL_STLP_MAP_EMPTY
#define FABLE_RETAIL_STLP_SET_EMPTY
#define FABLE_RETAIL_STLP_MAP_ORIGINAL_INSERT_BODY
#define FABLE_RETAIL_CCREATUREACTIONBASE
#define FABLE_RETAIL_CCREATUREACTIONBASE_REGISTEREDACTIONS
#define FABLE_RETAIL_CCOMBATACTIONBASE_PRIORITYCTOR
#define FABLE_RETAIL_CCOMBATACTION_GENERICSTRIKERESPONSEBASE_DEFAULTCTOR
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_ASSERT_DEAD_ARM
#include "fable_assert.h"
#define FABLE_RETAIL_CCREATUREACTION_UNSHEATHEWEAPONS_PROTOTYPE
#define FABLE_RETAIL_CCREATUREACTION_GENERICAIMPROJECTILEWEAPON_PROTOTYPE
#define FABLE_RETAIL_CCREATUREACTION_GENERICFIREPROJECTILEWEAPON_PROTOTYPE
#define FABLE_RETAIL_CCREATUREACTION_STRAFEFORWARD_PROTOTYPE
#define FABLE_RETAIL_CCREATUREACTION_STRAFEBACKWARD_PROTOTYPE
#define FABLE_RETAIL_CCOMBATACTION_STRIKERESPONSEJACKOFBLADESINTORUSH_PROTOTYPE
#define FABLE_RETAIL_CCOMBATACTION_STRIKERESPONSESCORPIONKING_PROTOTYPE
#define FABLE_RETAIL_CCREATUREACTION_SCORPIONKINGDEATH_PROTOTYPE
#define FABLE_RETAIL_CCREATUREACTION_BANDITKINGGETHIT_PROTOTYPE
#define FABLE_RETAIL_CCREATUREACTION_TENTACLESTRIKEGETHIT_PROTOTYPE
#define FABLE_RETAIL_CCREATUREACTION_NYMPHGETHIT_PROTOTYPE
#define FABLE_RETAIL_CCREATUREACTION_NYMPHGETHITDIE_PROTOTYPE
#define FABLE_RETAIL_CCREATUREACTION_GENERICLOADPROJECTILEWEAPON_PROTOTYPE
#define FABLE_RETAIL_CCREATUREACTION_GENERICLEADERCOMMAND_PROTOTYPE
#define FABLE_RETAIL_CCREATUREACTION_COMBATBOAST_PROTOTYPE
#define FABLE_RETAIL_CCREATUREACTION_COMBATIDLE_PROTOTYPE
#define FABLE_RETAIL_CCREATUREACTION_LEAPBACK_PROTOTYPE
#define FABLE_RETAIL_CCREATUREACTION_GENERICSTRAFELEFT_PROTOTYPE
#define FABLE_RETAIL_CCREATUREACTION_GENERICSTRAFERIGHT_PROTOTYPE
#define FABLE_RETAIL_CCREATUREACTION_DIAGONALSTRAFEBL_PROTOTYPE
#define FABLE_RETAIL_CCREATUREACTION_DIAGONALSTRAFEBR_PROTOTYPE
#define FABLE_RETAIL_CCREATUREACTION_CHARGE_PROTOTYPE
#define FABLE_RETAIL_CCREATUREACTION_UNBLOCKABLEMIDATTACK_PROTOTYPE
#define FABLE_RETAIL_CCREATUREACTION_INTERRUPTABLEFARATTACK_PROTOTYPE
#define FABLE_RETAIL_CCREATUREACTION_INTERRUPTABLEMIDATTACK_PROTOTYPE
#define FABLE_RETAIL_CCREATUREACTION_MIDATTACKSIDE_PROTOTYPE
#define FABLE_RETAIL_CCREATUREACTION_BODGESIDESTRIKE_PROTOTYPE
#define FABLE_RETAIL_CCREATUREACTION_CASTBATTLECHARGE_PROTOTYPE
#define FABLE_RETAIL_CCREATUREACTION_BALVERINELUNGEATTACK_PROTOTYPE
#define FABLE_RETAIL_CCREATUREACTION_BALVERINEBREAKOFFFROMCOMBAT_PROTOTYPE
#define FABLE_RETAIL_CCREATUREACTION_BALVERINEBREAKOFFFROMCOMBATLONG_PROTOTYPE
#define FABLE_RETAIL_CCREATUREACTION_HOBBELUNGEATTACK_PROTOTYPE
#define FABLE_RETAIL_CCREATUREACTION_SCREAMERDRAINATTACK_PROTOTYPE
#define FABLE_RETAIL_CCREATUREACTION_SCREAMERDRAINOUTOF_PROTOTYPE
#define FABLE_RETAIL_CCREATUREACTION_SCREAMERADVANCE_PROTOTYPE
#define FABLE_RETAIL_CCREATUREACTION_SCREAMERBACKOFF_PROTOTYPE
#define FABLE_RETAIL_CCREATUREACTION_SCREAMERIDLE_PROTOTYPE
#define FABLE_RETAIL_CCREATUREACTION_SUMMONERFLAMESLICE_PROTOTYPE
#define FABLE_RETAIL_CCREATUREACTION_SUMMONERUNSHEATHESTRIKE_PROTOTYPE
#define FABLE_RETAIL_CCREATUREACTION_SUMMONERSTRIKE_PROTOTYPE
#define FABLE_RETAIL_CCREATUREACTION_SUMMONERDIE_PROTOTYPE
#define FABLE_RETAIL_CCREATUREACTION_SUMMONCREATURES_OVERRIDES
#include "fable_string.h"
#include "stlport/fable_containers.h"
#include "stlport/fable_map.h"
#include "engine/C3DVector.h"
#include "engine/CCountedPointer.h"
#include "engine/CIntelligentPointer.h"
#include "engine/CCreatureActionBase.h"
#include "engine/CCombatActionBase.h"
#include "engine/CCombatAction_GenericStrikeResponseBase.h"

class CThing;

class CCreatureAction_CastWillDrainLifeSpell : public CCombatActionBase {
public:
    CCreatureAction_CastWillDrainLifeSpell();                                   // 0x008BBB10
    long Level;
    long DrainDefIndex;
};
#include "engine/CCreatureAction_UnsheatheWeapons.h"
#include "engine/CCreatureAction_GenericAimProjectileWeapon.h"
#include "engine/CCreatureAction_GenericFireProjectileWeapon.h"
#include "engine/CCreatureAction_StrafeForward.h"
#include "engine/CCreatureAction_StrafeBackward.h"
#include "engine/CCombatAction_StrikeResponseJackOfBladesIntoRush.h"
#include "engine/CCombatAction_StrikeResponseScorpionKing.h"
class CCreatureAction_RockTrollFallToDeath : public CCombatActionBase {
public:
    CCreatureAction_RockTrollFallToDeath() : CCombatActionBase(250) { }
};
#include "engine/CCreatureAction_ScorpionKingDeath.h"
class CCreatureAction_WaspQueenDeath : public CCombatActionBase {
public:
    CCreatureAction_WaspQueenDeath() : CCombatActionBase(250) { }
    CIntelligentPointer<CThing> PEmitter;
};
class CCombatAction_StrikeResponseMazeDoForcePush : public CCombatAction_GenericStrikeResponseBase {
public:
    CCombatAction_StrikeResponseMazeDoForcePush() { }
};
#include "engine/CCreatureAction_BanditKingGetHit.h"
class CCreatureAction_BanditKingStuckGetHit : public CCombatActionBase {
public:
    CCreatureAction_BanditKingStuckGetHit();                                   // 0x008C91B0
    CIntelligentPointer<CThing> PStuckEffect;
};
#include "engine/CCreatureAction_TentacleStrikeGetHit.h"
#include "engine/CCreatureAction_NymphGetHit.h"
#include "engine/CCreatureAction_NymphGetHitDie.h"
#include "engine/CCreatureAction_GenericLoadProjectileWeapon.h"
#include "engine/CCreatureAction_GenericLeaderCommand.h"
#include "engine/CCreatureAction_CombatBoast.h"
#include "engine/CCreatureAction_CombatIdle.h"
class CCreatureAction_BreakIntoMelee : public CCombatActionBase {
public:
    CCreatureAction_BreakIntoMelee() : CCombatActionBase(50) { }
};
#include "engine/CCreatureAction_SummonCreatures.h"
#include "engine/CCreatureAction_LeapBack.h"
#include "engine/CCreatureAction_GenericStrafeLeft.h"
#include "engine/CCreatureAction_GenericStrafeRight.h"
#include "engine/CCreatureAction_DiagonalStrafeBL.h"
#include "engine/CCreatureAction_DiagonalStrafeBR.h"
#include "engine/CCreatureAction_Charge.h"
#include "engine/CCreatureAction_UnblockableMidAttack.h"
#include "engine/CCreatureAction_InterruptableFarAttack.h"
#include "engine/CCreatureAction_InterruptableMidAttack.h"
#include "engine/CCreatureAction_MidAttackSide.h"
class CCreatureAction_MidAttackKnockdown : public CCombatActionBase {
public:
    CCreatureAction_MidAttackKnockdown() : CCombatActionBase(150) { }
};
class CCreatureAction_InterruptableNearAttack : public CCombatActionBase {
public:
    CCreatureAction_InterruptableNearAttack() : CCombatActionBase(150) { }
};
#include "engine/CCreatureAction_BodgeSideStrike.h"
class CCreatureAction_TurnStrikeLeft : public CCombatActionBase {
public:
    CCreatureAction_TurnStrikeLeft() : CCombatActionBase(150) { }
};
class CCreatureAction_TurnStrikeRight : public CCombatActionBase {
public:
    CCreatureAction_TurnStrikeRight() : CCombatActionBase(150) { }
};
class CCreatureAction_ChargeStrike : public CCombatActionBase {
public:
    CCreatureAction_ChargeStrike();                                   // 0x008B8540
};
#include "engine/CCreatureAction_CastBattleCharge.h"
#include "engine/CCreatureAction_BalverineLungeAttack.h"
#include "engine/CCreatureAction_BalverineBreakOffFromCombat.h"
#include "engine/CCreatureAction_BalverineBreakOffFromCombatLong.h"
class CCreatureAction_HobbeSpellcasterAim : public CCreatureAction_GenericAimProjectileWeapon {
public:
    CCreatureAction_HobbeSpellcasterAim() { }
    CIntelligentPointer<CThing> PEffect;
};
class CCreatureAction_HobbeSpellcasterFire : public CCreatureAction_GenericFireProjectileWeapon {
public:
    CCreatureAction_HobbeSpellcasterFire() { }
};
#include "engine/CCreatureAction_HobbeLungeAttack.h"
class CCreatureAction_ScreamerDie : public CCombatActionBase {
public:
    CCreatureAction_ScreamerDie();                                   // 0x008B9110
    CIntelligentPointer<CThing> PDieEffect;
};
#include "engine/CCreatureAction_ScreamerDrainAttack.h"
#include "engine/CCreatureAction_ScreamerDrainOutOf.h"
#include "engine/CCreatureAction_ScreamerAdvance.h"
#include "engine/CCreatureAction_ScreamerBackOff.h"
#include "engine/CCreatureAction_ScreamerIdle.h"
#include "engine/CCreatureAction_SummonerFlameSlice.h"
#include "engine/CCreatureAction_SummonerUnsheatheStrike.h"
#include "engine/CCreatureAction_SummonerStrike.h"
#include "engine/CCreatureAction_SummonerDie.h"

void CCreatureActionBase::StatRegisterClass()
{
    FABLE_RETAIL_ASSERT(RegisteredActions.empty());
    RegisterActionEventTypes();
    RegisteredActions.insert(std::make_pair(ACTION_NULL, CCountedPointer<CCreatureActionBase>()));
    RegisteredActions.insert(std::make_pair(ACTION_JACK_OF_BLADES_HIT_RESPONSE, CCountedPointer<CCreatureActionBase>(new CCombatAction_StrikeResponseJackOfBladesIntoRush)));
    RegisteredActions.insert(std::make_pair(ACTION_SCORPION_KING_HIT_RESPONSE, CCountedPointer<CCreatureActionBase>(new CCombatAction_StrikeResponseScorpionKing)));
    RegisteredActions.insert(std::make_pair(ACTION_ROCK_TROLL_DEATH, CCountedPointer<CCreatureActionBase>(new CCreatureAction_RockTrollFallToDeath)));
    RegisteredActions.insert(std::make_pair(ACTION_KING_SCORPION_DEATH, CCountedPointer<CCreatureActionBase>(new CCreatureAction_ScorpionKingDeath)));
    RegisteredActions.insert(std::make_pair(ACTION_WASP_QUEEN_DEATH, CCountedPointer<CCreatureActionBase>(new CCreatureAction_WaspQueenDeath)));
    RegisteredActions.insert(std::make_pair(ACTION_MAZE_CAUSE_FORCE_PUSH_HIT_RESPONSE, CCountedPointer<CCreatureActionBase>(new CCombatAction_StrikeResponseMazeDoForcePush)));
    RegisteredActions.insert(std::make_pair(ACTION_BANDIT_KING_HIT_RESPONSE, CCountedPointer<CCreatureActionBase>(new CCreatureAction_BanditKingGetHit)));
    RegisteredActions.insert(std::make_pair(ACTION_BANDIT_KING_STUCK_HIT_RESPONSE, CCountedPointer<CCreatureActionBase>(new CCreatureAction_BanditKingStuckGetHit)));
    RegisteredActions.insert(std::make_pair(ACTION_TENTACLE_HIT_RESPONSE, CCountedPointer<CCreatureActionBase>(new CCreatureAction_TentacleStrikeGetHit)));
    RegisteredActions.insert(std::make_pair(ACTION_NYMPH_GET_HIT, CCountedPointer<CCreatureActionBase>(new CCreatureAction_NymphGetHit)));
    RegisteredActions.insert(std::make_pair(ACTION_NYMPH_GET_HIT_DIE, CCountedPointer<CCreatureActionBase>(new CCreatureAction_NymphGetHitDie)));
    RegisteredActions.insert(std::make_pair(ACTION_COMBAT_GENERIC_PROJECTILE_WEAPON_AIM, CCountedPointer<CCreatureActionBase>(new CCreatureAction_GenericAimProjectileWeapon)));
    RegisteredActions.insert(std::make_pair(ACTION_COMBAT_GENERIC_PROJECTILE_WEAPON_FIRE, CCountedPointer<CCreatureActionBase>(new CCreatureAction_GenericFireProjectileWeapon)));
    RegisteredActions.insert(std::make_pair(ACTION_COMBAT_GENERIC_PROJECTILE_WEAPON_LOAD, CCountedPointer<CCreatureActionBase>(new CCreatureAction_GenericLoadProjectileWeapon)));
    RegisteredActions.insert(std::make_pair(ACTION_COMBAT_GENERIC_LEADER_COMMAND, CCountedPointer<CCreatureActionBase>(new CCreatureAction_GenericLeaderCommand)));
    RegisteredActions.insert(std::make_pair(ACTION_COMBAT_GENERIC_BOAST, CCountedPointer<CCreatureActionBase>(new CCreatureAction_CombatBoast)));
    RegisteredActions.insert(std::make_pair(ACTION_COMBAT_IDLE, CCountedPointer<CCreatureActionBase>(new CCreatureAction_CombatIdle)));
    RegisteredActions.insert(std::make_pair(ACTION_BREAK_INTO_MELEE, CCountedPointer<CCreatureActionBase>(new CCreatureAction_BreakIntoMelee)));
    RegisteredActions.insert(std::make_pair(ACTION_COMBAT_SUMMON_CREATURES, CCountedPointer<CCreatureActionBase>(new CCreatureAction_SummonCreatures)));
    RegisteredActions.insert(std::make_pair(ACTION_COMBAT_HOP_BACK, CCountedPointer<CCreatureActionBase>(new CCreatureAction_LeapBack)));
    RegisteredActions.insert(std::make_pair(ACTION_COMBAT_STRAFE_FORWARD, CCountedPointer<CCreatureActionBase>(new CCreatureAction_StrafeForward)));
    RegisteredActions.insert(std::make_pair(ACTION_COMBAT_STRAFE_BACKWARD, CCountedPointer<CCreatureActionBase>(new CCreatureAction_StrafeBackward)));
    RegisteredActions.insert(std::make_pair(ACTION_COMBAT_STRAFE_LEFT, CCountedPointer<CCreatureActionBase>(new CCreatureAction_GenericStrafeLeft)));
    RegisteredActions.insert(std::make_pair(ACTION_COMBAT_STRAFE_RIGHT, CCountedPointer<CCreatureActionBase>(new CCreatureAction_GenericStrafeRight)));
    RegisteredActions.insert(std::make_pair(ACTION_COMBAT_STRAFE_BACK_LEFT, CCountedPointer<CCreatureActionBase>(new CCreatureAction_DiagonalStrafeBL)));
    RegisteredActions.insert(std::make_pair(ACTION_COMBAT_STRAFE_BACK_RIGHT, CCountedPointer<CCreatureActionBase>(new CCreatureAction_DiagonalStrafeBR)));
    RegisteredActions.insert(std::make_pair(ACTION_COMBAT_CHARGE, CCountedPointer<CCreatureActionBase>(new CCreatureAction_Charge)));
    RegisteredActions.insert(std::make_pair(ACTION_COMBAT_UNBLOCKABLE_ATTACK, CCountedPointer<CCreatureActionBase>(new CCreatureAction_UnblockableMidAttack)));
    RegisteredActions.insert(std::make_pair(ACTION_COMBAT_ATTACK_LUNGE, CCountedPointer<CCreatureActionBase>(new CCreatureAction_InterruptableFarAttack)));
    RegisteredActions.insert(std::make_pair(ACTION_COMBAT_ATTACK_MAIN, CCountedPointer<CCreatureActionBase>(new CCreatureAction_InterruptableMidAttack)));
    RegisteredActions.insert(std::make_pair(ACTION_COMBAT_ATTACK_SIDE, CCountedPointer<CCreatureActionBase>(new CCreatureAction_MidAttackSide)));
    RegisteredActions.insert(std::make_pair(ACTION_COMBAT_ATTACK_KNOCKDOWN, CCountedPointer<CCreatureActionBase>(new CCreatureAction_MidAttackKnockdown)));
    RegisteredActions.insert(std::make_pair(ACTION_COMBAT_ATTACK_SHORT_RANGE, CCountedPointer<CCreatureActionBase>(new CCreatureAction_InterruptableNearAttack)));
    RegisteredActions.insert(std::make_pair(ACTION_COMBAT_BODGE_SIDE_ATTACK, CCountedPointer<CCreatureActionBase>(new CCreatureAction_BodgeSideStrike)));
    RegisteredActions.insert(std::make_pair(ACTION_COMBAT_TURN_STRIKE_LEFT, CCountedPointer<CCreatureActionBase>(new CCreatureAction_TurnStrikeLeft)));
    RegisteredActions.insert(std::make_pair(ACTION_COMBAT_TURN_STRIKE_RIGHT, CCountedPointer<CCreatureActionBase>(new CCreatureAction_TurnStrikeRight)));
    RegisteredActions.insert(std::make_pair(ACTION_COMBAT_CHARGE_STRIKE, CCountedPointer<CCreatureActionBase>(new CCreatureAction_ChargeStrike)));
    RegisteredActions.insert(std::make_pair(ACTION_BATTLE_CHARGE, CCountedPointer<CCreatureActionBase>(new CCreatureAction_CastBattleCharge)));
    RegisteredActions.insert(std::make_pair(ACTION_BALVERINE_LUNGE_ATTACK, CCountedPointer<CCreatureActionBase>(new CCreatureAction_BalverineLungeAttack)));
    RegisteredActions.insert(std::make_pair(ACTION_BALVERINE_BREAK_OFF_FROM_COMBAT, CCountedPointer<CCreatureActionBase>(new CCreatureAction_BalverineBreakOffFromCombat)));
    RegisteredActions.insert(std::make_pair(ACTION_BALVERINE_BREAK_OFF_FROM_COMBAT_LONG, CCountedPointer<CCreatureActionBase>(new CCreatureAction_BalverineBreakOffFromCombatLong)));
    RegisteredActions.insert(std::make_pair(ACTION_HOBBE_SPELLCASTER_AIM, CCountedPointer<CCreatureActionBase>(new CCreatureAction_HobbeSpellcasterAim)));
    RegisteredActions.insert(std::make_pair(ACTION_HOBBE_SPELLCASTER_FIRE, CCountedPointer<CCreatureActionBase>(new CCreatureAction_HobbeSpellcasterFire)));
    RegisteredActions.insert(std::make_pair(ACTION_HOBBE_LUNGE, CCountedPointer<CCreatureActionBase>(new CCreatureAction_HobbeLungeAttack)));
    RegisteredActions.insert(std::make_pair(ACTION_SCREAMER_DIE, CCountedPointer<CCreatureActionBase>(new CCreatureAction_ScreamerDie)));
    RegisteredActions.insert(std::make_pair(ACTION_SCREAMER_DRAIN_ATTACK, CCountedPointer<CCreatureActionBase>(new CCreatureAction_ScreamerDrainAttack)));
    RegisteredActions.insert(std::make_pair(ACTION_SCREAMER_DRAIN_OUT_OF, CCountedPointer<CCreatureActionBase>(new CCreatureAction_ScreamerDrainOutOf)));
    RegisteredActions.insert(std::make_pair(ACTION_SCREAMER_ADVANCE, CCountedPointer<CCreatureActionBase>(new CCreatureAction_ScreamerAdvance)));
    RegisteredActions.insert(std::make_pair(ACTION_SCREAMER_BACK_OFF, CCountedPointer<CCreatureActionBase>(new CCreatureAction_ScreamerBackOff)));
    RegisteredActions.insert(std::make_pair(ACTION_SCREAMER_IDLE, CCountedPointer<CCreatureActionBase>(new CCreatureAction_ScreamerIdle)));
    RegisteredActions.insert(std::make_pair(ACTION_SUMMONER_FLAME_SLICE, CCountedPointer<CCreatureActionBase>(new CCreatureAction_SummonerFlameSlice)));
    RegisteredActions.insert(std::make_pair(ACTION_SUMMONER_UNSHEATHE_STRIKE, CCountedPointer<CCreatureActionBase>(new CCreatureAction_SummonerUnsheatheStrike)));
    RegisteredActions.insert(std::make_pair(ACTION_SUMMONER_STRIKE, CCountedPointer<CCreatureActionBase>(new CCreatureAction_SummonerStrike)));
    RegisteredActions.insert(std::make_pair(ACTION_SUMMONER_DIE, CCountedPointer<CCreatureActionBase>(new CCreatureAction_SummonerDie)));
}