// CTCInventoryAbilities::IsAbleToUseAbility(EHeroAbility ability, bool checkCost) -- whether the hero may use an ability: it must be available (level >= 0) and, when asked, the hero's stamina must cover its cost (the fireball's first-level stamina cost, the unholy power / divine wrath stamina-per-second times their env-change seconds plus 20, the summon spell's reset-creature cost while a summon is active, otherwise the def's stamina cost at the current level)
// Leaf: ?IsAbleToUseAbility@CTCInventoryAbilities@@QBE_NW4EHeroAbility@@_N@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005EE49A, 549 bytes, thiscall `ret 8`; frame-pointer size TU (optimize "s").
//   Ego_r.pdb / FableWin.pdb ?IsAbleToUseAbility@CTCInventoryAbilities@@QBE_NW4EHeroAbility@@_N@Z.
// Callees: std::bitset<274>::test 0x00410DE0 + const CVectorMap::LowerBound 0x004365B0 (the inline CThing::PeekTC, interfaces 4 and
//   0x91), CCharString 0x0099EBF0 / ~ 0x0099EAE0, CGameDefinitionManager::Get 0x0044C6B0, CDefinitionManager::GetGlobalIndexFromClassIndex
//   0x009AD390, CDefinitionManager::GetDef<CSpecialAbilitiesFireballSpellDef> 0x005F8A57 / <CSpecialAbilitiesUnholyPowerDef> 0x005F8AB9 /
//   <CSpecialAbilitiesDivineWrathDef> 0x005F8B1B (this TU's instantiations), CTCSummonSpell::GetResetCreatureManaCost 0x0079A610.
// Layout: Abilities +0x160 (pair<long level, CDefPointer<const CHeroAbilityDef> >), CHeroAbilityDef::StaminaCost +0x70,
//   CTCHeroStats::Stamina +0x58, fireball StaminaCost +0x44, unholy power / divine wrath EnvChangeSecs +0xB8 / StaminaCostPerSec +0xDC.
#ifdef _MSC_VER
#pragma optimize("s", on)
#endif
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CGAMEDEFINITIONMANAGER
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_STLP_BITSET_TEST_DECL
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_CTCINVENTORYBASE
#define FABLE_RETAIL_CTCINVENTORYABILITIES
#define FABLE_RETAIL_CTCINVENTORYABILITIES_ASSIGN
#define FABLE_RETAIL_CTCHEROSTATS
#define FABLE_RETAIL_CTCHEROSTATS_SERIALISE
#define FABLE_RETAIL_CTCSUMMONSPELL
#include "engine/ETCInterfaceType.h"
#include "engine/CThing.h"
#include "engine/CTCHeroStats.h"
#include "engine/CTCSummonSpell.h"
#include "engine/CHeroAbilityDef.h"
#include "engine/CGameDefinitionManager.h"
#include "engine/CSpecialAbilitiesFireballSpellDef.h"
#include "engine/CSpecialAbilitiesUnholyPowerDef.h"
#include "engine/CSpecialAbilitiesDivineWrathDef.h"
#include "engine/CTCInventoryAbilities.h"

// Same-TU body of the def lookup (retail instantiates CDefinitionManager::GetDef<T> in this TU and calls it out of line,
// 0x005F8A57 / 0x005F8AB9 / 0x005F8B1B); the body is the one the landed rows of this TU carry (0x005F4C8F).
#pragma auto_inline(off)
template <class T> bool CDefinitionManager::GetDef(long index, CDefPointer<const T>* out) const
{
    if (index <= 0)
        return false;
    CDefPointer<const CDefClassBase> def = GetPDefFromGlobalIndex(index);
    if (def.IsNull())
        return false;
    out->SetDynamicCast(def);
    return true;
}
#pragma auto_inline(on)

bool CTCInventoryAbilities::IsAbleToUseAbility(EHeroAbility ability, bool checkCost) const
{
    if (ability == HERO_ABILITY_NULL || Abilities[ability].first < 0)
        return false;
    if (checkCost) {
        const CTCHeroStats* stats;
        ParentThing->PeekTC(&stats);
        if (ability == HERO_ABILITY_FIREBALL_SPELL) {
            long index = CGameDefinitionManager::Get().GetGlobalIndexFromClassIndex("SPECIAL_ABILITIES_FIREBALL_SPELL_DEF", 1);
            CDefPointer<const CSpecialAbilitiesFireballSpellDef> def;
            CGameDefinitionManager::Get().GetDef(index, &def);
            return stats->Stamina >= def->StaminaCost[0];
        }
        if (ability == HERO_ABILITY_UNHOLY_POWER_SPELL) {
            long index = CGameDefinitionManager::Get().GetGlobalIndexFromClassIndex("SPECIAL_ABILITIES_UNHOLY_POWER_DEF", 1);
            CDefPointer<const CSpecialAbilitiesUnholyPowerDef> def;
            CGameDefinitionManager::Get().GetDef(index, &def);
            long level = Abilities[ability].first;
            return stats->Stamina >= def->StaminaCostPerSec[level] * def->EnvChangeSecs[level] + 20.0f;
        }
        if (ability == HERO_ABILITY_DIVINE_WRATH_SPELL) {
            long index = CGameDefinitionManager::Get().GetGlobalIndexFromClassIndex("SPECIAL_ABILITIES_DIVINE_WRATH_DEF", 1);
            CDefPointer<const CSpecialAbilitiesDivineWrathDef> def;
            CGameDefinitionManager::Get().GetDef(index, &def);
            long level = Abilities[ability].first;
            return stats->Stamina >= def->StaminaCostPerSec[level] * def->EnvChangeSecs[level] + 20.0f;
        }
        long cost = Abilities[ability].second->StaminaCost[Abilities[ability].first];
        if (ability == HERO_ABILITY_SUMMON_SPELL) {
            const CTCSummonSpell* summon;
            if (ParentThing->PeekTC(&summon))
                cost = summon->GetResetCreatureManaCost();
        }
        return stats->Stamina >= cost;
    }
    return true;
}