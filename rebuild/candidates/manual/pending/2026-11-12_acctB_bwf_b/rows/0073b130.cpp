// CTCCreatureModeManager::RegisterModes() -- fill the creature-mode registry: map every NCreatureMode::EMode to its mode class's allocator (85 registrations, MODE_STAND three times), then sort the registry
// Retail: Fable.exe (TLC, VC7.1) @ 0x0073B130, 6673 bytes; public static __fastcall, no arguments, plain `ret`. Ego_r.pdb / FableWin.pdb
//   ?RegisterModes@CTCCreatureModeManager@@SIXXZ; only caller CTCBase::RegisterTCs (0x004F9129).
// Leaf: ?RegisterModes@CTCCreatureModeManager@@SIXXZ
// Callees: vector<pair<unsigned long,CRegisteredModeInfo> >::_M_insert_overflow 0x00743270, vector::push_back 0x00743B30,
//   std::sort<pair*,CKeyPairCompareLess> 0x007441D0 (__fastcall); the 85 NCreatureMode::CModeX::Alloc allocators (FableWin.pdb / Ego_r.pdb
//   ?Alloc@CModeX@NCreatureMode@@SIPAVCModeBase@CTCCreatureModeManager@@AAVCThing@@@Z) are referenced by address only.
// Shape: RegisteredModes (0x013BAD44) gets 85 CVectorMap::Insert(key, CRegisteredModeInfo(Alloc)) calls, then Update(). Insert is the
//   forced-inline (key, value) overload with make_pair (FABLE_RETAIL_CVECTORMAP_INSERT_KEYVALUE): the pair temporary shares one 8-byte
//   slot; its push_back is a plain STLport inline that VC7.1's caller budget expands at the first 70 sites (placement-new null test,
//   _M_insert_overflow when full) and calls (0x00743B30) at the last 15, as in retail; the value_type spelling
//   (_INSERT_VALUE_TYPE) expands 75 and does not match. Update (FABLE_RETAIL_CVECTORMAP_INSERT) folds its Dirty test (Dirty is known
//   true) into std::sort + Dirty=false. No pragma, no flag (aligned `and esp,-8` frame is VC7.1's own).
// Models: engine/CTCCreatureModeManager.h (FABLE_RETAIL_CTCCREATUREMODEMANAGER_REGISTRY: CRegisteredModeInfo, RegisterModes,
//   RegisteredModes), engine/CVectorMap.h, the six NCreatureMode_CMode*.h headers (FABLE_RETAIL_NCREATUREMODE_ALLOC; Thunder's
//   header already declares Alloc). The other 78 mode classes have no engine header and are declared by their registration interface.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CMODEBASE_VIRTUALS
#define FABLE_RETAIL_CMODEBASE_PARENTTHING
#define FABLE_RETAIL_CMODESTANDBASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCCREATUREMODEMANAGER_REGISTRY
#define FABLE_RETAIL_CVECTORMAP_INSERT_KEYVALUE
#define FABLE_RETAIL_CVECTORMAP_INSERT
#define FABLE_RETAIL_STLP_SORT_FASTCALL
#include "fable_string.h"
#include "engine/ETCInterfaceType.h"
#include "engine/CThing.h"
#include "engine/CAnimateThing.h"
#include "engine/CTCCreatureModeManager.h"
#define FABLE_RETAIL_NCREATUREMODE_ALLOC
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "engine/NCreatureMode.h"
#include "engine/NCreatureMode_CModeSitFloor.h"
#include "engine/NCreatureMode_CModeThunderLightningStorm.h"
#include "engine/NCreatureMode_CModeScorpionKingVulnerable.h"
#include "engine/NCreatureMode_CModeGuildmasterPowerUpShield.h"
#include "engine/NCreatureMode_CModeBriarRoseConcentrate.h"
#include "engine/NCreatureMode_CModeDrained.h"

class CThing;
// The registration interface of a creature-mode class (FableWin.pdb ?Alloc@CModeX@NCreatureMode@@SIPAVCModeBase@CTCCreatureModeManager@@AAVCThing@@@Z).
#define MODE_REGISTRATION_INTERFACE(ModeClass) \
    class ModeClass { public: static CTCCreatureModeManager::CModeBase* __fastcall Alloc(CThing& owner); };
namespace NCreatureMode {
MODE_REGISTRATION_INTERFACE(CModeNull)   // Alloc 0x00742430
MODE_REGISTRATION_INTERFACE(CModeCarryingWeaponStand)   // Alloc 0x0073A0A0
MODE_REGISTRATION_INTERFACE(CModeCarryingPlateStand)   // Alloc 0x007426F0
MODE_REGISTRATION_INTERFACE(CModeCarryingGoodsStand)   // Alloc 0x00742710
MODE_REGISTRATION_INTERFACE(CModeStandFront)   // Alloc 0x0073EBE0
MODE_REGISTRATION_INTERFACE(CModeStandLeft)   // Alloc 0x0073EC10
MODE_REGISTRATION_INTERFACE(CModeStandRight)   // Alloc 0x0073EC40
MODE_REGISTRATION_INTERFACE(CModeCrouch)   // Alloc 0x00742730
MODE_REGISTRATION_INTERFACE(CModeSitChair)   // Alloc 0x00742750
MODE_REGISTRATION_INTERFACE(CModeSitBench)   // Alloc 0x00742790
MODE_REGISTRATION_INTERFACE(CModeSleepBed)   // Alloc 0x0073A130
MODE_REGISTRATION_INTERFACE(CModeBlock)   // Alloc 0x007427B0
MODE_REGISTRATION_INTERFACE(CModeCook)   // Alloc 0x00742630
MODE_REGISTRATION_INTERFACE(CModeHandsBound)   // Alloc 0x00742650
MODE_REGISTRATION_INTERFACE(CModeScared)   // Alloc 0x007424F0
MODE_REGISTRATION_INTERFACE(CModeCreeping)   // Alloc 0x00742570
MODE_REGISTRATION_INTERFACE(CModeCheering)   // Alloc 0x00742590
MODE_REGISTRATION_INTERFACE(CModeHeckling)   // Alloc 0x007425B0
MODE_REGISTRATION_INTERFACE(CModeOutraged)   // Alloc 0x00742510
MODE_REGISTRATION_INTERFACE(CModeSad)   // Alloc 0x00742530
MODE_REGISTRATION_INTERFACE(CModeAttracted)   // Alloc 0x00742550
MODE_REGISTRATION_INTERFACE(CModeHeroImitation)   // Alloc 0x007425D0
MODE_REGISTRATION_INTERFACE(CModeCurious)   // Alloc 0x007425F0
MODE_REGISTRATION_INTERFACE(CModeBully)   // Alloc 0x00742610
MODE_REGISTRATION_INTERFACE(CModeStrafe)   // Alloc 0x007431C0
MODE_REGISTRATION_INTERFACE(CModeHoverNormal)   // Alloc 0x00744250
MODE_REGISTRATION_INTERFACE(CModeConversation)   // Alloc 0x0073A350
MODE_REGISTRATION_INTERFACE(CModeWalkInAllDirections)   // Alloc 0x00744160
MODE_REGISTRATION_INTERFACE(CModeStunned)   // Alloc 0x007427F0
MODE_REGISTRATION_INTERFACE(CModeUnderground)   // Alloc 0x00742A20
MODE_REGISTRATION_INTERFACE(CModeStandBored)   // Alloc 0x007424B0
MODE_REGISTRATION_INTERFACE(CModeStandHappy)   // Alloc 0x00742490
MODE_REGISTRATION_INTERFACE(CModeKnockedDown)   // Alloc 0x00742A40
MODE_REGISTRATION_INTERFACE(CModeForcePushed)   // Alloc 0x00742A60
MODE_REGISTRATION_INTERFACE(CModeCarryingGenericBox)   // Alloc 0x007426D0
MODE_REGISTRATION_INTERFACE(CModeBattleCharge)   // Alloc 0x00742AC0
MODE_REGISTRATION_INTERFACE(CModeBerserk)   // Alloc 0x00742A80
MODE_REGISTRATION_INTERFACE(CModeAssassinRush)   // Alloc 0x00742AE0
MODE_REGISTRATION_INTERFACE(CModeWading)   // Alloc 0x00742AA0
MODE_REGISTRATION_INTERFACE(CModeDrinkingFromTankard)   // Alloc 0x00742B00
MODE_REGISTRATION_INTERFACE(CModeEatingAtTable)   // Alloc 0x00742B20
MODE_REGISTRATION_INTERFACE(CModeHoldJug)   // Alloc 0x00742B40
MODE_REGISTRATION_INTERFACE(CModeServeCustomer)   // Alloc 0x00742B60
MODE_REGISTRATION_INTERFACE(CModeFillJug)   // Alloc 0x00742B80
MODE_REGISTRATION_INTERFACE(CModeDrunk)   // Alloc 0x00742BA0
MODE_REGISTRATION_INTERFACE(CModeArmsOffTable)   // Alloc 0x00742BC0
MODE_REGISTRATION_INTERFACE(CModeRockTrollVulnerable)   // Alloc 0x0073A1D0
MODE_REGISTRATION_INTERFACE(CModeJackOfBladesFloat)   // Alloc 0x00742850
MODE_REGISTRATION_INTERFACE(CModeJackOfBladesScream)   // Alloc 0x00742880
MODE_REGISTRATION_INTERFACE(CModeJackOfBladesFlash)   // Alloc 0x0073A2E0
MODE_REGISTRATION_INTERFACE(CModeJackOfBladesVulnerable)   // Alloc 0x00742670
MODE_REGISTRATION_INTERFACE(CModeJackOfBladesDeath)   // Alloc 0x007428B0
MODE_REGISTRATION_INTERFACE(CModeScorpionKingTailPlunge)   // Alloc 0x0073A250
MODE_REGISTRATION_INTERFACE(CModeCreatureIsProjectile)   // Alloc 0x0073A190
MODE_REGISTRATION_INTERFACE(CModeScorpionKingExhausted)   // Alloc 0x0073A290
MODE_REGISTRATION_INTERFACE(CModeGuildmasterConcentrate)   // Alloc 0x00742CC0
MODE_REGISTRATION_INTERFACE(CModeGuildmasterPowerUpHeal)   // Alloc 0x007434E0
MODE_REGISTRATION_INTERFACE(CModeDazed)   // Alloc 0x00742820
MODE_REGISTRATION_INTERFACE(CModeVulnerable)   // Alloc 0x00742690
MODE_REGISTRATION_INTERFACE(CModeUnderwater)   // Alloc 0x007426B0
MODE_REGISTRATION_INTERFACE(CModeScreamerAggressive)   // Alloc 0x00742BE0
MODE_REGISTRATION_INTERFACE(CModeDragonRearUp)   // Alloc 0x00742F80
MODE_REGISTRATION_INTERFACE(CModeDragonFireBeam)   // Alloc 0x0073A540
MODE_REGISTRATION_INTERFACE(CModeDragonStrafe)   // Alloc 0x00743CF0
MODE_REGISTRATION_INTERFACE(CModeDragonStrafeBurninate)   // Alloc 0x00743D10
MODE_REGISTRATION_INTERFACE(CModeDragonLanded)   // Alloc 0x00742F20
MODE_REGISTRATION_INTERFACE(CModeDragonGetHit)   // Alloc 0x00742F50
MODE_REGISTRATION_INTERFACE(CModeDragonFlying)   // Alloc 0x00743CC0
MODE_REGISTRATION_INTERFACE(CModeDragonHover)   // Alloc 0x00742FA0
MODE_REGISTRATION_INTERFACE(CModeIntangible)   // Alloc 0x007430A0
MODE_REGISTRATION_INTERFACE(CModeCaged)   // Alloc 0x00742C60
MODE_REGISTRATION_INTERFACE(CModeScripted)   // Alloc 0x007424D0
MODE_REGISTRATION_INTERFACE(CModeBasic)   // Alloc 0x00742460
MODE_REGISTRATION_INTERFACE(CModeChargeForcePush)   // Alloc 0x00742CA0
MODE_REGISTRATION_INTERFACE(CModeSummonerDie)   // Alloc 0x00742C80
MODE_REGISTRATION_INTERFACE(CModeIceTrollGroundSmashIcicleAssault)   // Alloc 0x00743F90
MODE_REGISTRATION_INTERFACE(CModeRollAround)   // Alloc 0x00742C40
MODE_REGISTRATION_INTERFACE(CModeHitByEpicSpell)   // Alloc 0x00743080
}

void __fastcall CTCCreatureModeManager::RegisterModes()
{
    RegisteredModes.Insert(NCreatureMode::MODE_NULL, CRegisteredModeInfo(NCreatureMode::CModeNull::Alloc));
    RegisteredModes.Insert(NCreatureMode::MODE_CARRYING_WEAPON_STAND, CRegisteredModeInfo(NCreatureMode::CModeCarryingWeaponStand::Alloc));
    RegisteredModes.Insert(NCreatureMode::MODE_CARRYING_PLATE_STAND, CRegisteredModeInfo(NCreatureMode::CModeCarryingPlateStand::Alloc));
    RegisteredModes.Insert(NCreatureMode::MODE_CARRYING_GOODS_STAND, CRegisteredModeInfo(NCreatureMode::CModeCarryingGoodsStand::Alloc));
    RegisteredModes.Insert(NCreatureMode::MODE_STAND, CRegisteredModeInfo(NCreatureMode::CModeStandFront::Alloc));
    RegisteredModes.Insert(NCreatureMode::MODE_STAND, CRegisteredModeInfo(NCreatureMode::CModeStandLeft::Alloc));
    RegisteredModes.Insert(NCreatureMode::MODE_STAND, CRegisteredModeInfo(NCreatureMode::CModeStandRight::Alloc));
    RegisteredModes.Insert(NCreatureMode::MODE_SNEAK, CRegisteredModeInfo(NCreatureMode::CModeCrouch::Alloc));
    RegisteredModes.Insert(NCreatureMode::MODE_SIT_CHAIR, CRegisteredModeInfo(NCreatureMode::CModeSitChair::Alloc));
    RegisteredModes.Insert(NCreatureMode::MODE_SIT_BENCH, CRegisteredModeInfo(NCreatureMode::CModeSitBench::Alloc));
    RegisteredModes.Insert(NCreatureMode::MODE_SIT_FLOOR, CRegisteredModeInfo(NCreatureMode::CModeSitFloor::Alloc));
    RegisteredModes.Insert(NCreatureMode::MODE_SLEEP_BED, CRegisteredModeInfo(NCreatureMode::CModeSleepBed::Alloc));
    RegisteredModes.Insert(NCreatureMode::MODE_BLOCK, CRegisteredModeInfo(NCreatureMode::CModeBlock::Alloc));
    RegisteredModes.Insert(NCreatureMode::MODE_COOK, CRegisteredModeInfo(NCreatureMode::CModeCook::Alloc));
    RegisteredModes.Insert(NCreatureMode::MODE_HANDS_BOUND, CRegisteredModeInfo(NCreatureMode::CModeHandsBound::Alloc));
    RegisteredModes.Insert(NCreatureMode::MODE_SCARED, CRegisteredModeInfo(NCreatureMode::CModeScared::Alloc));
    RegisteredModes.Insert(NCreatureMode::MODE_CREEPING, CRegisteredModeInfo(NCreatureMode::CModeCreeping::Alloc));
    RegisteredModes.Insert(NCreatureMode::MODE_CHEERING, CRegisteredModeInfo(NCreatureMode::CModeCheering::Alloc));
    RegisteredModes.Insert(NCreatureMode::MODE_HECKLING, CRegisteredModeInfo(NCreatureMode::CModeHeckling::Alloc));
    RegisteredModes.Insert(NCreatureMode::MODE_OUTRAGED, CRegisteredModeInfo(NCreatureMode::CModeOutraged::Alloc));
    RegisteredModes.Insert(NCreatureMode::MODE_SAD, CRegisteredModeInfo(NCreatureMode::CModeSad::Alloc));
    RegisteredModes.Insert(NCreatureMode::MODE_ATTRACTED, CRegisteredModeInfo(NCreatureMode::CModeAttracted::Alloc));
    RegisteredModes.Insert(NCreatureMode::MODE_HERO_IMITATION, CRegisteredModeInfo(NCreatureMode::CModeHeroImitation::Alloc));
    RegisteredModes.Insert(NCreatureMode::MODE_CURIOUS, CRegisteredModeInfo(NCreatureMode::CModeCurious::Alloc));
    RegisteredModes.Insert(NCreatureMode::MODE_BULLY, CRegisteredModeInfo(NCreatureMode::CModeBully::Alloc));
    RegisteredModes.Insert(NCreatureMode::MODE_STRAFE, CRegisteredModeInfo(NCreatureMode::CModeStrafe::Alloc));
    RegisteredModes.Insert(NCreatureMode::MODE_HOVER, CRegisteredModeInfo(NCreatureMode::CModeHoverNormal::Alloc));
    RegisteredModes.Insert(NCreatureMode::MODE_CONVERSATION, CRegisteredModeInfo(NCreatureMode::CModeConversation::Alloc));
    RegisteredModes.Insert(NCreatureMode::MODE_STAND_IN_ALL_DIRECTIONS, CRegisteredModeInfo(NCreatureMode::CModeWalkInAllDirections::Alloc));
    RegisteredModes.Insert(NCreatureMode::MODE_STUNNED, CRegisteredModeInfo(NCreatureMode::CModeStunned::Alloc));
    RegisteredModes.Insert(NCreatureMode::MODE_UNDERGROUND, CRegisteredModeInfo(NCreatureMode::CModeUnderground::Alloc));
    RegisteredModes.Insert(NCreatureMode::MODE_STAND_BORED, CRegisteredModeInfo(NCreatureMode::CModeStandBored::Alloc));
    RegisteredModes.Insert(NCreatureMode::MODE_STAND_HAPPY, CRegisteredModeInfo(NCreatureMode::CModeStandHappy::Alloc));
    RegisteredModes.Insert(NCreatureMode::MODE_KNOCKED_DOWN, CRegisteredModeInfo(NCreatureMode::CModeKnockedDown::Alloc));
    RegisteredModes.Insert(NCreatureMode::MODE_FORCE_PUSHED, CRegisteredModeInfo(NCreatureMode::CModeForcePushed::Alloc));
    RegisteredModes.Insert(NCreatureMode::MODE_CARRYING_GENERIC_BOX, CRegisteredModeInfo(NCreatureMode::CModeCarryingGenericBox::Alloc));
    RegisteredModes.Insert(NCreatureMode::MODE_BATTLE_CHARGE, CRegisteredModeInfo(NCreatureMode::CModeBattleCharge::Alloc));
    RegisteredModes.Insert(NCreatureMode::MODE_BERSERK, CRegisteredModeInfo(NCreatureMode::CModeBerserk::Alloc));
    RegisteredModes.Insert(NCreatureMode::MODE_ASSASSIN_RUSH, CRegisteredModeInfo(NCreatureMode::CModeAssassinRush::Alloc));
    RegisteredModes.Insert(NCreatureMode::MODE_WADING, CRegisteredModeInfo(NCreatureMode::CModeWading::Alloc));
    RegisteredModes.Insert(NCreatureMode::MODE_DRINKING_FROM_TANKARD, CRegisteredModeInfo(NCreatureMode::CModeDrinkingFromTankard::Alloc));
    RegisteredModes.Insert(NCreatureMode::MODE_EATING_AT_TABLE, CRegisteredModeInfo(NCreatureMode::CModeEatingAtTable::Alloc));
    RegisteredModes.Insert(NCreatureMode::MODE_HOLD_JUG, CRegisteredModeInfo(NCreatureMode::CModeHoldJug::Alloc));
    RegisteredModes.Insert(NCreatureMode::MODE_SERVE_CUSTOMER, CRegisteredModeInfo(NCreatureMode::CModeServeCustomer::Alloc));
    RegisteredModes.Insert(NCreatureMode::MODE_FILL_JUG, CRegisteredModeInfo(NCreatureMode::CModeFillJug::Alloc));
    RegisteredModes.Insert(NCreatureMode::MODE_DRUNK, CRegisteredModeInfo(NCreatureMode::CModeDrunk::Alloc));
    RegisteredModes.Insert(NCreatureMode::MODE_ARMS_OFF_TABLE, CRegisteredModeInfo(NCreatureMode::CModeArmsOffTable::Alloc));
    RegisteredModes.Insert(NCreatureMode::MODE_ROCK_TROLL_VULNERABLE, CRegisteredModeInfo(NCreatureMode::CModeRockTrollVulnerable::Alloc));
    RegisteredModes.Insert(NCreatureMode::MODE_JACK_OF_BLADES_FLOAT, CRegisteredModeInfo(NCreatureMode::CModeJackOfBladesFloat::Alloc));
    RegisteredModes.Insert(NCreatureMode::MODE_JACK_OF_BLADES_SCREAM, CRegisteredModeInfo(NCreatureMode::CModeJackOfBladesScream::Alloc));
    RegisteredModes.Insert(NCreatureMode::MODE_JACK_OF_BLADES_FLASH, CRegisteredModeInfo(NCreatureMode::CModeJackOfBladesFlash::Alloc));
    RegisteredModes.Insert(NCreatureMode::MODE_JACK_OF_BLADES_VULNERABLE, CRegisteredModeInfo(NCreatureMode::CModeJackOfBladesVulnerable::Alloc));
    RegisteredModes.Insert(NCreatureMode::MODE_JACK_OF_BLADES_DEATH, CRegisteredModeInfo(NCreatureMode::CModeJackOfBladesDeath::Alloc));
    RegisteredModes.Insert(NCreatureMode::MODE_THUNDER_LIGHTNING_STORM, CRegisteredModeInfo(NCreatureMode::CModeThunderLightningStorm::Alloc));
    RegisteredModes.Insert(NCreatureMode::MODE_SCORPION_KING_VULNERABLE, CRegisteredModeInfo(NCreatureMode::CModeScorpionKingVulnerable::Alloc));
    RegisteredModes.Insert(NCreatureMode::MODE_SCORPION_KING_TAIL_PLUNGE, CRegisteredModeInfo(NCreatureMode::CModeScorpionKingTailPlunge::Alloc));
    RegisteredModes.Insert(NCreatureMode::MODE_CREATURE_IS_PROJECTILE, CRegisteredModeInfo(NCreatureMode::CModeCreatureIsProjectile::Alloc));
    RegisteredModes.Insert(NCreatureMode::MODE_SCORPION_KING_EXHAUSTED, CRegisteredModeInfo(NCreatureMode::CModeScorpionKingExhausted::Alloc));
    RegisteredModes.Insert(NCreatureMode::MODE_GUILDMASTER_CONCENTRATE, CRegisteredModeInfo(NCreatureMode::CModeGuildmasterConcentrate::Alloc));
    RegisteredModes.Insert(NCreatureMode::MODE_GUILDMASTER_POWERUP_HEAL, CRegisteredModeInfo(NCreatureMode::CModeGuildmasterPowerUpHeal::Alloc));
    RegisteredModes.Insert(NCreatureMode::MODE_GUILDMASTER_POWERUP_SHIELD, CRegisteredModeInfo(NCreatureMode::CModeGuildmasterPowerUpShield::Alloc));
    RegisteredModes.Insert(NCreatureMode::MODE_GUILDMASTER_POWERUP_SPEED, CRegisteredModeInfo(NCreatureMode::CModeGuildmasterPowerUpSpeed::Alloc));
    RegisteredModes.Insert(NCreatureMode::MODE_BRIAR_ROSE_CONCENTRATE, CRegisteredModeInfo(NCreatureMode::CModeBriarRoseConcentrate::Alloc));
    RegisteredModes.Insert(NCreatureMode::MODE_DAZED, CRegisteredModeInfo(NCreatureMode::CModeDazed::Alloc));
    RegisteredModes.Insert(NCreatureMode::MODE_VULNERABLE, CRegisteredModeInfo(NCreatureMode::CModeVulnerable::Alloc));
    RegisteredModes.Insert(NCreatureMode::MODE_UNDERWATER, CRegisteredModeInfo(NCreatureMode::CModeUnderwater::Alloc));
    RegisteredModes.Insert(NCreatureMode::MODE_DRAINED, CRegisteredModeInfo(NCreatureMode::CModeDrained::Alloc));
    RegisteredModes.Insert(NCreatureMode::MODE_SCREAMER_AGGRESSIVE, CRegisteredModeInfo(NCreatureMode::CModeScreamerAggressive::Alloc));
    RegisteredModes.Insert(NCreatureMode::MODE_DRAGON_REAR_UP, CRegisteredModeInfo(NCreatureMode::CModeDragonRearUp::Alloc));
    RegisteredModes.Insert(NCreatureMode::MODE_DRAGON_FIRE_BEAM, CRegisteredModeInfo(NCreatureMode::CModeDragonFireBeam::Alloc));
    RegisteredModes.Insert(NCreatureMode::MODE_DRAGON_STRAFE, CRegisteredModeInfo(NCreatureMode::CModeDragonStrafe::Alloc));
    RegisteredModes.Insert(NCreatureMode::MODE_DRAGON_STRAFE_BURNINATE, CRegisteredModeInfo(NCreatureMode::CModeDragonStrafeBurninate::Alloc));
    RegisteredModes.Insert(NCreatureMode::MODE_DRAGON_LANDED, CRegisteredModeInfo(NCreatureMode::CModeDragonLanded::Alloc));
    RegisteredModes.Insert(NCreatureMode::MODE_DRAGON_GET_HIT, CRegisteredModeInfo(NCreatureMode::CModeDragonGetHit::Alloc));
    RegisteredModes.Insert(NCreatureMode::MODE_DRAGON_FLYING, CRegisteredModeInfo(NCreatureMode::CModeDragonFlying::Alloc));
    RegisteredModes.Insert(NCreatureMode::MODE_DRAGON_HOVER, CRegisteredModeInfo(NCreatureMode::CModeDragonHover::Alloc));
    RegisteredModes.Insert(NCreatureMode::MODE_INTANGIBLE, CRegisteredModeInfo(NCreatureMode::CModeIntangible::Alloc));
    RegisteredModes.Insert(NCreatureMode::MODE_CAGED, CRegisteredModeInfo(NCreatureMode::CModeCaged::Alloc));
    RegisteredModes.Insert(NCreatureMode::MODE_SCRIPTED, CRegisteredModeInfo(NCreatureMode::CModeScripted::Alloc));
    RegisteredModes.Insert(NCreatureMode::MODE_BASIC, CRegisteredModeInfo(NCreatureMode::CModeBasic::Alloc));
    RegisteredModes.Insert(NCreatureMode::MODE_CHARGE_FORCE_PUSH, CRegisteredModeInfo(NCreatureMode::CModeChargeForcePush::Alloc));
    RegisteredModes.Insert(NCreatureMode::MODE_SUMMONER_DIE, CRegisteredModeInfo(NCreatureMode::CModeSummonerDie::Alloc));
    RegisteredModes.Insert(NCreatureMode::MODE_ICE_TROLL_ICICLE_ASSAULT, CRegisteredModeInfo(NCreatureMode::CModeIceTrollGroundSmashIcicleAssault::Alloc));
    RegisteredModes.Insert(NCreatureMode::MODE_ROLL, CRegisteredModeInfo(NCreatureMode::CModeRollAround::Alloc));
    RegisteredModes.Insert(NCreatureMode::MODE_HIT_BY_EPIC_SPELL, CRegisteredModeInfo(NCreatureMode::CModeHitByEpicSpell::Alloc));
    RegisteredModes.Update();
}
