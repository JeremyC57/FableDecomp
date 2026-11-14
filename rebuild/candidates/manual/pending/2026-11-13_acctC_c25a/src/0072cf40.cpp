// CTCSpecialAbilities::UseSpecialAbility(EHeroAbility ability) -- start a hero ability: refuse an absent / unavailable one; tell the
// creature's entity-events TC the ability was performed; run the ability's Setup member; when it succeeded post the ability's opinion
// deed (battle charge etc. 0x11; heal life 0x3D; berserk none) through the hero-opinion deed log at the creature's position.
// Retail: Fable.exe (TLC, VC7.1) @ 0x0072CF40, 488 bytes of code + 76-byte jump table (564), thiscall `ret 4`.
// Callees: IsAbilityAvailable 0x0072C430, CVectorMap::LowerBound 0x0040F020 (GetTC<CTCEntityEvents> 0x42 / <CTCHeroOpinionDeedLog> 0x51),
//   operator new 0x00BFEA1A, NEntityEvents::CEventPerformedSpecialAbility ctor 0x008111F0, CTCEntityEvents::AddEvent 0x008113C0, the
//   eighteen Setup<Ability> members (landed rows, SetupMultiStrike 0x0072AAF0), CThing::GetPos 0x004C73D0, CTCHeroOpinionDeedLog::PostDeed 0x006C6990.
// Shape: jump-table switch with the case bodies in retail's layout order; HERO_ABILITY_RAISE_DEAD_SPELL assigns `success = false` and breaks
//   (VC7.1 threads the known-false deed test to a duplicated epilogue: `xor bl,bl ; ... mov al,bl`); the entity-events pointer is
//   zero-initialised and used unchecked; the deed log out pointer is not initialised (retail reads the reused slot on a miss).
//   Deed values 0x11 / 0x3D are written as casts: the repo's EOpinionDeedType.h names them EXPRESSION_BATTLE_CRY / RECEIVE_GIFT_FRIENDLY,
//   which do not fit an ability (enum provenance unverified for TLC) -- coordinator decision.
// Leaf: ?UseSpecialAbility@CTCSpecialAbilities@@QAE_NW4EHeroAbility@@@Z
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTCSPECIALABILITIES_FULL
#define FABLE_RETAIL_CTCSPECIALABILITIES_USE
#define FABLE_RETAIL_CTCENTITYEVENTS
#define FABLE_RETAIL_CTCENTITYEVENTS_ADDEVENT
#define FABLE_RETAIL_CTCHEROOPINIONDEEDLOG
#include "engine/CThing.h"
#include "engine/CTCSpecialAbilities.h"
#include "engine/CTCEntityEvents.h"
#include "engine/CTCHeroOpinionDeedLog.h"
#include "engine/NEntityEvents_CEventPerformedSpecialAbility.h"

bool CTCSpecialAbilities::UseSpecialAbility(EHeroAbility ability)
{
    if (ability == HERO_ABILITY_NULL || !IsAbilityAvailable(ability))
        return false;
    CTCEntityEvents* entity_events = 0;
    ParentThing->GetTC(&entity_events);
    entity_events->AddEvent(new NEntityEvents::CEventPerformedSpecialAbility(ability));
    EOpinionDeedType deed = (EOpinionDeedType)0x11;
    bool success = false;
    switch (ability) {
    case HERO_ABILITY_TIME_SPELL: success = SetupBulletTime(); break;
    case HERO_ABILITY_FORCE_PUSH: success = SetupForceMove(); break;
    case HERO_ABILITY_ENFLAME_SPELL: success = SetupEnflameSpell(); break;
    case HERO_ABILITY_PHYSICAL_SHIELD_SPELL: success = SetupPhysicalShieldSpell(); break;
    case HERO_ABILITY_TURNCOAT_SPELL: success = SetupTurncoatSpell(); break;
    case HERO_ABILITY_DRAIN_LIFE_SPELL: success = SetupDrainLifeSpell(); break;
    case HERO_ABILITY_RAISE_DEAD_SPELL: success = false; break;
    case HERO_ABILITY_SUMMON_SPELL: success = SetupSummonSpell(); break;
    case HERO_ABILITY_LIGHTNING_SPELL: success = SetupLightningSpell(); break;
    case HERO_ABILITY_BERSERK: deed = (EOpinionDeedType)0; success = SetupBerserk(); break;
    case HERO_ABILITY_DOUBLE_STRIKE: success = SetupMultiStrike(); break;
    case HERO_ABILITY_MULTI_ARROW: success = SetupMultiArrow(); break;
    case HERO_ABILITY_BATTLE_CHARGE: success = SetupBattleCharge(); break;
    case HERO_ABILITY_ASSASSIN_RUSH: success = SetupAssassinRush(); break;
    case HERO_ABILITY_HEAL_LIFE_SPELL: deed = (EOpinionDeedType)0x3D; success = SetupHealLifeSpell(); break;
    case HERO_ABILITY_GHOST_SWORD_SPELL: success = SetupGhostSwordSpell(); break;
    case HERO_ABILITY_FIREBALL_SPELL: success = SetupFireballSpell(); break;
    case HERO_ABILITY_DIVINE_WRATH_SPELL: success = SetupDivineWrath(); break;
    case HERO_ABILITY_UNHOLY_POWER_SPELL: success = SetupUnholyPower(); break;
    }
    if (success && deed != 0 && ParentThing->HasTC(TCI_HERO_OPINION_DEED_LOG)) {
        CTCHeroOpinionDeedLog* deed_log;
        ParentThing->GetTC(&deed_log);
        deed_log->PostDeed(deed, ParentThing->GetPos(), 0, 0, (EOpinionPostDeedType)7, false, false, 0, false, 0);
    }
    return success;
}
