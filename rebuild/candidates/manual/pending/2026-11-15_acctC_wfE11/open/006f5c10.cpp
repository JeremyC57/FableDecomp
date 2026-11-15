// CCreatureActionBase::StatRegisterClass() -- register the creature-action event types, then one prototype of every registered action
// Grade: DIFFER(9096v8717) -- inline-budget transition at block 20 and blocks 21..54 equal in instruction count; blocks 1..19 carry +5 instructions each: after insert_unique ours reloads pair.second.Info after 'dec [Info]' and keeps the dying pair's Data/Info zero stores, and caches 0 in ebx (retail: 'dec [edi]; jne', no reload, test reg,reg, frame 0x2c v ours 0x30)
//   class in the RegisteredActions map, keyed by its EActionRegisteredType (a null prototype for ACTION_NULL)
// Leaf: ?StatRegisterClass@CCreatureActionBase@@SIXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006F5C10, 8717 bytes (static __fastcall, no arguments; one caller CWorld::Init 0x004A7601).
// Identity (proven): Ego_r.pdb (0x0071BCF0) and FableWin.pdb (0x023ED150) ?StatRegisterClass@CCreatureActionBase@@SIXXZ; the body calls
//   ?RegisterActionEventTypes@CCreatureActionBase@@SIXXZ 0x006932B0 first and fills the map 0x013BA8C8 (Ego_r / FableWin
//   `dynamic initializer for 'CCreatureActionBase::RegisteredActions'`) through insert_unique 0x006F5400. The catalog's
//   `_global::StatRegisterClass` lacks the class.
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCOUNTEDPOINTER_RELEASEDATA
#define FABLE_RETAIL_STLP_MAP
#define FABLE_RETAIL_CCREATUREACTIONBASE
#define FABLE_RETAIL_CCREATUREACTIONBASE_REGISTEREDACTIONS
#define FABLE_RETAIL_CCOMBATACTIONBASE_PRIORITYCTOR
#define FABLE_RETAIL_CCOMBATACTION_GENERICSTRIKERESPONSEBASE_DEFAULTCTOR
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CCHARSTRING
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
class CCreatureAction_UnsheatheWeapons : public CCombatActionBase {
public:
    CCreatureAction_UnsheatheWeapons();
    long WeaponType;
};
class CCreatureAction_GenericAimProjectileWeapon : public CCombatActionBase {
public:
    CCreatureAction_GenericAimProjectileWeapon();                                   // 0x008BFB00
};
class CCreatureAction_GenericFireProjectileWeapon : public CCombatActionBase {
public:
    CCreatureAction_GenericFireProjectileWeapon();                                   // 0x008BFA40
};
class CCreatureAction_StrafeForward : public CCombatActionBase {
public:
    CCreatureAction_StrafeForward();                                   // 0x008BFCE0
};
class CCreatureAction_StrafeBackward : public CCombatActionBase {
public:
    CCreatureAction_StrafeBackward();                                   // 0x008BFD40
};
class CCombatAction_StrikeResponseJackOfBladesIntoRush : public CCombatAction_GenericStrikeResponseBase {
public:
    CCombatAction_StrikeResponseJackOfBladesIntoRush();                                   // 0x008C2530
    bool CauseRush;
};
class CCombatAction_StrikeResponseScorpionKing : public CCombatAction_GenericStrikeResponseBase {
public:
    CCombatAction_StrikeResponseScorpionKing() { }
};
class CCreatureAction_RockTrollFallToDeath : public CCombatActionBase {
public:
    CCreatureAction_RockTrollFallToDeath() : CCombatActionBase(250) { }
};
class CCreatureAction_ScorpionKingDeath : public CCombatActionBase {
public:
    CCreatureAction_ScorpionKingDeath() : CCombatActionBase(250) { }
};
class CCreatureAction_WaspQueenDeath : public CCombatActionBase {
public:
    CCreatureAction_WaspQueenDeath() : CCombatActionBase(250) { }
    CIntelligentPointer<CThing> PEmitter;
};
class CCombatAction_StrikeResponseMazeDoForcePush : public CCombatAction_GenericStrikeResponseBase {
public:
    CCombatAction_StrikeResponseMazeDoForcePush() { }
};
class CCreatureAction_BanditKingGetHit : public CCombatAction_GenericStrikeResponseBase {
public:
    CCreatureAction_BanditKingGetHit() { }
};
class CCreatureAction_BanditKingStuckGetHit : public CCombatActionBase {
public:
    CCreatureAction_BanditKingStuckGetHit();                                   // 0x008C91B0
    CIntelligentPointer<CThing> PStuckEffect;
};
class CCreatureAction_TentacleStrikeGetHit : public CCombatActionBase {
public:
    CCreatureAction_TentacleStrikeGetHit();                                   // 0x008CF8F0
};
class CCreatureAction_NymphGetHit : public CCombatAction_GenericStrikeResponseBase {
public:
    CCreatureAction_NymphGetHit() { }
    C3DVector InitDummyPos;
};
class CCreatureAction_NymphGetHitDie : public CCombatAction_GenericStrikeResponseBase {
public:
    CCreatureAction_NymphGetHitDie() { }
    float HeightOnInit;
};
class CCreatureAction_GenericLoadProjectileWeapon : public CCombatActionBase {
public:
    CCreatureAction_GenericLoadProjectileWeapon();                                   // 0x008BFAA0
};
class CCreatureAction_GenericLeaderCommand : public CCombatActionBase {
public:
    CCreatureAction_GenericLeaderCommand();                                   // 0x008BFB60
};
class CCreatureAction_CombatBoast : public CCombatActionBase {
public:
    CCreatureAction_CombatBoast() : CCombatActionBase(50) { }
};
class CCreatureAction_CombatIdle : public CCombatActionBase {
public:
    CCreatureAction_CombatIdle() : CCombatActionBase(50) { }
};
class CCreatureAction_BreakIntoMelee : public CCombatActionBase {
public:
    CCreatureAction_BreakIntoMelee() : CCombatActionBase(50) { }
};
class CCreatureAction_SummonCreatures : public CCombatActionBase {
public:
    CCreatureAction_SummonCreatures();                                   // 0x008B83C0
    bool SpawnOnTarget;
};
class CCreatureAction_LeapBack : public CCombatActionBase {
public:
    CCreatureAction_LeapBack() : CCombatActionBase(50) { }
};
class CCreatureAction_GenericStrafeLeft : public CCombatActionBase {
public:
    CCreatureAction_GenericStrafeLeft() : CCombatActionBase(50) { }
};
class CCreatureAction_GenericStrafeRight : public CCombatActionBase {
public:
    CCreatureAction_GenericStrafeRight() : CCombatActionBase(50) { }
};
class CCreatureAction_DiagonalStrafeBL : public CCombatActionBase {
public:
    CCreatureAction_DiagonalStrafeBL();                                   // 0x008BFC20
};
class CCreatureAction_DiagonalStrafeBR : public CCombatActionBase {
public:
    CCreatureAction_DiagonalStrafeBR();                                   // 0x008BFC80
};
class CCreatureAction_Charge : public CCombatActionBase {
public:
    CCreatureAction_Charge();                                   // 0x008B84E0
};
class CCreatureAction_UnblockableMidAttack : public CCombatActionBase {
public:
    CCreatureAction_UnblockableMidAttack();                                   // 0x008BFBC0
};
class CCreatureAction_InterruptableFarAttack : public CCombatActionBase {
public:
    CCreatureAction_InterruptableFarAttack() : CCombatActionBase(150) { }
};
class CCreatureAction_InterruptableMidAttack : public CCombatActionBase {
public:
    CCreatureAction_InterruptableMidAttack() : CCombatActionBase(150) { }
    long InterruptTime;
};
class CCreatureAction_MidAttackSide : public CCombatActionBase {
public:
    CCreatureAction_MidAttackSide();                                   // 0x008B81E0
};
class CCreatureAction_MidAttackKnockdown : public CCombatActionBase {
public:
    CCreatureAction_MidAttackKnockdown() : CCombatActionBase(150) { }
};
class CCreatureAction_InterruptableNearAttack : public CCombatActionBase {
public:
    CCreatureAction_InterruptableNearAttack() : CCombatActionBase(150) { }
};
class CCreatureAction_BodgeSideStrike : public CCombatActionBase {
public:
    CCreatureAction_BodgeSideStrike();                                   // 0x008BFE00
};
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
class CCreatureAction_CastBattleCharge : public CCombatActionBase {
public:
    CCreatureAction_CastBattleCharge();                                   // 0x008CE3C0
    bool Override;
    C3DVector Dir;
    float Distance;
    long Level;
    bool IsZTargetting;
};
class CCreatureAction_BalverineLungeAttack : public CCombatActionBase {
public:
    CCreatureAction_BalverineLungeAttack();                                   // 0x008CCC50
};
class CCreatureAction_BalverineBreakOffFromCombat : public CCombatActionBase {
public:
    CCreatureAction_BalverineBreakOffFromCombat();                                   // 0x008CCCB0
};
class CCreatureAction_BalverineBreakOffFromCombatLong : public CCombatActionBase {
public:
    CCreatureAction_BalverineBreakOffFromCombatLong();                                   // 0x008CCD10
};
class CCreatureAction_HobbeSpellcasterAim : public CCreatureAction_GenericAimProjectileWeapon {
public:
    CCreatureAction_HobbeSpellcasterAim() { }
    CIntelligentPointer<CThing> PEffect;
};
class CCreatureAction_HobbeSpellcasterFire : public CCreatureAction_GenericFireProjectileWeapon {
public:
    CCreatureAction_HobbeSpellcasterFire() { }
};
class CCreatureAction_HobbeLungeAttack : public CCombatActionBase {
public:
    CCreatureAction_HobbeLungeAttack();                                   // 0x008C9D20
};
class CCreatureAction_ScreamerDie : public CCombatActionBase {
public:
    CCreatureAction_ScreamerDie();                                   // 0x008B9110
    CIntelligentPointer<CThing> PDieEffect;
};
class CCreatureAction_ScreamerDrainAttack : public CCreatureAction_CastWillDrainLifeSpell {
public:
    CCreatureAction_ScreamerDrainAttack() { }
};
class CCreatureAction_ScreamerDrainOutOf : public CCombatActionBase {
public:
    CCreatureAction_ScreamerDrainOutOf() : CCombatActionBase(150) { }
};
class CCreatureAction_ScreamerAdvance : public CCreatureAction_StrafeForward {
public:
    CCreatureAction_ScreamerAdvance() { }
};
class CCreatureAction_ScreamerBackOff : public CCreatureAction_StrafeBackward {
public:
    CCreatureAction_ScreamerBackOff() { }
};
class CCreatureAction_ScreamerIdle : public CCombatActionBase {
public:
    CCreatureAction_ScreamerIdle() : CCombatActionBase(50) { }
};
class CCreatureAction_SummonerFlameSlice : public CCombatActionBase {
public:
    CCreatureAction_SummonerFlameSlice() : CCombatActionBase(150) { }
    CIntelligentPointer<CThing> PObstruction;
};
class CCreatureAction_SummonerUnsheatheStrike : public CCreatureAction_UnsheatheWeapons {
public:
    CCreatureAction_SummonerUnsheatheStrike();                                   // 0x008CBEE0
};
class CCreatureAction_SummonerStrike : public CCombatActionBase {
public:
    CCreatureAction_SummonerStrike() : CCombatActionBase(150) { }
};
class CCreatureAction_SummonerDie : public CCombatActionBase {
public:
    CCreatureAction_SummonerDie() : CCombatActionBase(250) { }
};

void CCreatureActionBase::StatRegisterClass()
{
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
