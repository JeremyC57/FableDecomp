// Behaviour test for 0x0044C72B CGameDefinitionManager::InitAndCompile: the model below is the row body statement for statement
// (path lists, the 138 class registrations in order, compile, then the four cached defs); the expected tables are the strings
// retail 0x0044C72B builds, in call order (read from Fable.exe at the pushed addresses).
#include <cstdio>
#include <cstring>
#include <string>
#include <vector>
static std::vector<std::string> g_log;
struct Mgr { std::vector<std::wstring> sym, comp; std::vector<std::string> classes; int allocs; bool compiled; int defs[4];
  void AddDefClass(const char* name, int alloc) { classes.push_back(name); allocs += alloc; g_log.push_back(std::string("add:") + name); }
  void Compile() { compiled = true; g_log.push_back("compile"); }
  void GetDef(const char* name, long index, int* p) { g_log.push_back(std::string("get:") + name); *p = (int)index; }
  void InitAndCompile(); };
static void Progress(const char* t, float f, bool a, bool b) { if (f == -1.0f && !a && !b) g_log.push_back(std::string("progress:") + t); }
void Mgr::InitAndCompile()
{
    std::vector<std::wstring> headers, defs; std::wstring platform(L"pc\\");
    std::wstring hdr(L"H\\"), def(L"D\\");
    headers.push_back(hdr + L"*.h"); headers.push_back(hdr + platform + L"*.h");
    headers.push_back(def + L"*.h"); headers.push_back(def + L"*.tpl"); headers.push_back(def + L"*.def");
    defs.push_back(def + L"*.tpl"); defs.push_back(def + L"*.def");
    AddDefClass("CHeroPostcardGeneratorDef", 1);
    AddDefClass("GLOBAL", 1);
    AddDefClass("CREATURE", 1);
    AddDefClass("CCreatureDef", 1);
    AddDefClass("CPhysicsDef", 1);
    AddDefClass("CSmashableDef", 1);
    AddDefClass("CDegradableDef", 1);
    AddDefClass("CSpecialEffectsDef", 1);
    AddDefClass("CTextureReplacementDef", 1);
    AddDefClass("CTargetingDef", 1);
    AddDefClass("CActionUseDef", 1);
    AddDefClass("CAppearanceDef", 1);
    AddDefClass("SHOT", 1);
    AddDefClass("CThingMultiArrowShotDef", 1);
    AddDefClass("OBJECT", 1);
    AddDefClass("BUILDING", 1);
    AddDefClass("VILLAGE", 1);
    AddDefClass("CREATURE_ABILITY", 1);
    AddDefClass("SWITCH", 1);
    AddDefClass("MARKER", 1);
    AddDefClass("PHYSICAL_SWITCH", 1);
    AddDefClass("HOLY_SITE", 1);
    AddDefClass("PLAYER", 1);
    AddDefClass("PLAYER_MOVEMENT", 1);
    AddDefClass("SIM_BUILDING", 1);
    AddDefClass("THING_GROUP", 1);
    AddDefClass("ENGINE_THEME_GROUP", 1);
    AddDefClass("ENGINE_THEME", 1);
    AddDefClass("BRAIN", 1);
    AddDefClass("PLAYER_INVENTORY", 1);
    AddDefClass("PLAYER_GUI", 1);
    AddDefClass("INVENTORY_CATEGORY", 1);
    AddDefClass("INVENTORY_ITEM", 1);
    AddDefClass("INVENTORY_TYPE", 1);
    AddDefClass("MESSAGE_EVENT", 1);
    AddDefClass("SKY", 1);
    AddDefClass("ENVIRONMENT", 1);
    AddDefClass("ENVIRONMENT_THEME_DAY", 1);
    AddDefClass("LOCAL_DETAIL_GENERATOR", 1);
    AddDefClass("CARRY_SLOT", 1);
    AddDefClass("COMBAT_TYPE", 1);
    AddDefClass("MATERIAL", 1);
    AddDefClass("ENGINE", 1);
    AddDefClass("ENGINE_VIDEO_OPTIONS", 1);
    AddDefClass("SIM_VOICES", 1);
    AddDefClass("SOUND_THEME", 1);
    AddDefClass("NOISE", 1);
    AddDefClass("THING", 1);
    AddDefClass("CREATURE_GENERATION_FAMILY", 1);
    AddDefClass("OBJECT_FAMILY", 1);
    AddDefClass("HERO_ABILITY", 1);
    AddDefClass("HERO_STATS", 1);
    AddDefClass("REGION", 1);
    AddDefClass("FACTION", 1);
    AddDefClass("CONTROL_SCHEME", 1);
    AddDefClass("CAppearanceModifierDef", 1);
    AddDefClass("CEntitySoundDef", 1);
    AddDefClass("CAMERA_MODE", 1);
    AddDefClass("CAMERA_MANAGER", 1);
    AddDefClass("CAMERA_MANAGER_SET", 1);
    AddDefClass("EXPRESSION", 1);
    AddDefClass("OPINION_DEED_EFFECTS", 1);
    AddDefClass("OPINION_PERSONALITY", 1);
    AddDefClass("OPINION_REACTION_MANAGER", 1);
    AddDefClass("OPINION_REACTION_MASK", 1);
    AddDefClass("OPINION_DEED_MASK", 1);
    AddDefClass("OPINION_SOURCE", 1);
    AddDefClass("SPECIAL_ABILITIES_ASSASSIN_RUSH_DEF", 1);
    AddDefClass("SPECIAL_ABILITIES_BATTLE_CHARGE_DEF", 1);
    AddDefClass("SPECIAL_ABILITIES_BERSERK_DEF", 1);
    AddDefClass("SPECIAL_ABILITIES_BULLET_TIME_DEF", 1);
    AddDefClass("SPECIAL_ABILITIES_FORCE_PUSH_DEF", 1);
    AddDefClass("SPECIAL_ABILITIES_SUMMON_SPELL_DEF", 1);
    AddDefClass("SPECIAL_ABILITIES_ENFLAME_DEF", 1);
    AddDefClass("SPECIAL_ABILITIES_PHYSICAL_SHIELD_DEF", 1);
    AddDefClass("SPECIAL_ABILITIES_DRAIN_LIFE_DEF", 1);
    AddDefClass("SPECIAL_ABILITIES_HEAL_LIFE_DEF", 1);
    AddDefClass("SPECIAL_ABILITIES_GHOST_SWORD_DEF", 1);
    AddDefClass("SPECIAL_ABILITIES_TURNCOAT_SPELL_DEF", 1);
    AddDefClass("SPECIAL_ABILITIES_FIREBALL_SPELL_DEF", 1);
    AddDefClass("SPECIAL_ABILITIES_LIGHTNING_SPELL_DEF", 1);
    AddDefClass("SPECIAL_ABILITIES_THUNDER_LIGHTNING_STORM_DEF", 1);
    AddDefClass("SPECIAL_ABILITIES_MULTI_ARROW_DEF", 1);
    AddDefClass("SPECIAL_ABILITIES_MULTI_STRIKE_DEF", 1);
    AddDefClass("SPECIAL_ABILITIES_BURNT_EFFECT_DEF", 1);
    AddDefClass("SPECIAL_ABILITIES_CREATURE_TINT_DEF", 1);
    AddDefClass("SPECIAL_ABILITIES_DRUNKENNESS_DEF", 1);
    AddDefClass("SPECIAL_ABILITIES_ELECTROCUTED_EFFECT_DEF", 1);
    AddDefClass("SPECIAL_ABILITIES_DIVINE_WRATH_DEF", 1);
    AddDefClass("SPECIAL_ABILITIES_UNHOLY_POWER_DEF", 1);
    AddDefClass("COMBAT_DIALOGUE_DEF", 1);
    AddDefClass("SOUND_SETUP", 1);
    AddDefClass("CVillagePeopleDef", 1);
    AddDefClass("VILLAGER_INTERACTION", 1);
    AddDefClass("HERO_COMBAT", 1);
    AddDefClass("HERO_MELEE_COMBAT_ABILITY", 1);
    AddDefClass("ATTACK_PATTERN", 1);
    AddDefClass("CCreatureStatsDef", 1);
    AddDefClass("ARMOUR", 1);
    AddDefClass("HIT_LOCATION", 1);
    AddDefClass("CHitLocationsDef", 1);
    AddDefClass("CCoopSpiritDef", 1);
    AddDefClass("CContextSensitiveItemDef", 1);
    AddDefClass("MELEE_COMBAT_KNOCKDOWN_EFFECTS", 1);
    AddDefClass("CAbilityDef", 1);
    AddDefClass("CBonusItemDef", 1);
    AddDefClass("CHeroMarriageDef", 1);
    AddDefClass("CHeroTitleDef", 1);
    AddDefClass("CTattooDef", 1);
    AddDefClass("CHairCardDef", 1);
    AddDefClass("UI", 1);
    AddDefClass("CCombatAbilityBlockUnarmedAttackDef", 1);
    AddDefClass("CCombatAbilityBlockLightWeaponAttackDef", 1);
    AddDefClass("CCombatAbilityBlockHeavyWeaponAttackDef", 1);
    AddDefClass("CCombatAbilityBlockProjectileWeaponAttackDef", 1);
    AddDefClass("CCombatAbilityStrafeDef", 1);
    AddDefClass("CCombatAbilityBlockCounterAttackDef", 1);
    AddDefClass("CCombatAbilityFlourishCounterAttackDef", 1);
    AddDefClass("CCombatAbilityUseProjectileWeaponDef", 1);
    AddDefClass("CCombatAbilityGetHitCounterAttackDef", 1);
    AddDefClass("COMBAT_SEQUENCE", 1);
    AddDefClass("UI_MISC_THINGS_DEF", 1);
    AddDefClass("CDragonActionSwoopDef", 1);
    AddDefClass("CDragonActionNapalmDef", 1);
    AddDefClass("CDragonActionHoverDef", 1);
    AddDefClass("UI_LOCALE_GRAPHICS_DEF", 1);
    AddDefClass("UI_ICONS_DEF", 1);
    AddDefClass("LIGHTNING", 1);
    AddDefClass("CAugmentationDef", 1);
    AddDefClass("CSummonerDef", 1);
    AddDefClass("CSnowTrollDef", 1);
    AddDefClass("CGuildMasterDef", 1);
    AddDefClass("CJackDragonDef", 1);
    AddDefClass("CBriarRoseDef", 1);
    AddDefClass("CExplosiveTrailDef", 1);
    AddDefClass("CSpecialAbilitiesDrainLifeDataDef", 1);
    AddDefClass("CSpecialAbilitiesForcePushDataDef", 1);
    AddDefClass("CONFIG_OPTIONS_DEFAULTS_DEF", 1);
    g_log.push_back("stat:anim"); g_log.push_back("stat:physprim");
    sym = headers; comp = defs;
    Progress("Game Definition Manager: Compile", -1.0f, false, false);
    Compile();
    Progress("Game Definition Manager: GetDefs", -1.0f, false, false);
    GetDef("GLOBAL", 1, &this->defs[0]); GetDef("ENGINE", 1, &this->defs[1]); GetDef("ENVIRONMENT", 1, &this->defs[2]); GetDef("HERO_STATS", 1, &this->defs[3]);
}
static const char* const kRetail[] = {"CHeroPostcardGeneratorDef", "GLOBAL", "CREATURE", "CCreatureDef", "CPhysicsDef", "CSmashableDef", "CDegradableDef", "CSpecialEffectsDef", "CTextureReplacementDef", "CTargetingDef", "CActionUseDef", "CAppearanceDef", "SHOT", "CThingMultiArrowShotDef", "OBJECT", "BUILDING", "VILLAGE", "CREATURE_ABILITY", "SWITCH", "MARKER", "PHYSICAL_SWITCH", "HOLY_SITE", "PLAYER", "PLAYER_MOVEMENT", "SIM_BUILDING", "THING_GROUP", "ENGINE_THEME_GROUP", "ENGINE_THEME", "BRAIN", "PLAYER_INVENTORY", "PLAYER_GUI", "INVENTORY_CATEGORY", "INVENTORY_ITEM", "INVENTORY_TYPE", "MESSAGE_EVENT", "SKY", "ENVIRONMENT", "ENVIRONMENT_THEME_DAY", "LOCAL_DETAIL_GENERATOR", "CARRY_SLOT", "COMBAT_TYPE", "MATERIAL", "ENGINE", "ENGINE_VIDEO_OPTIONS", "SIM_VOICES", "SOUND_THEME", "NOISE", "THING", "CREATURE_GENERATION_FAMILY", "OBJECT_FAMILY", "HERO_ABILITY", "HERO_STATS", "REGION", "FACTION", "CONTROL_SCHEME", "CAppearanceModifierDef", "CEntitySoundDef", "CAMERA_MODE", "CAMERA_MANAGER", "CAMERA_MANAGER_SET", "EXPRESSION", "OPINION_DEED_EFFECTS", "OPINION_PERSONALITY", "OPINION_REACTION_MANAGER", "OPINION_REACTION_MASK", "OPINION_DEED_MASK", "OPINION_SOURCE", "SPECIAL_ABILITIES_ASSASSIN_RUSH_DEF", "SPECIAL_ABILITIES_BATTLE_CHARGE_DEF", "SPECIAL_ABILITIES_BERSERK_DEF", "SPECIAL_ABILITIES_BULLET_TIME_DEF", "SPECIAL_ABILITIES_FORCE_PUSH_DEF", "SPECIAL_ABILITIES_SUMMON_SPELL_DEF", "SPECIAL_ABILITIES_ENFLAME_DEF", "SPECIAL_ABILITIES_PHYSICAL_SHIELD_DEF", "SPECIAL_ABILITIES_DRAIN_LIFE_DEF", "SPECIAL_ABILITIES_HEAL_LIFE_DEF", "SPECIAL_ABILITIES_GHOST_SWORD_DEF", "SPECIAL_ABILITIES_TURNCOAT_SPELL_DEF", "SPECIAL_ABILITIES_FIREBALL_SPELL_DEF", "SPECIAL_ABILITIES_LIGHTNING_SPELL_DEF", "SPECIAL_ABILITIES_THUNDER_LIGHTNING_STORM_DEF", "SPECIAL_ABILITIES_MULTI_ARROW_DEF", "SPECIAL_ABILITIES_MULTI_STRIKE_DEF", "SPECIAL_ABILITIES_BURNT_EFFECT_DEF", "SPECIAL_ABILITIES_CREATURE_TINT_DEF", "SPECIAL_ABILITIES_DRUNKENNESS_DEF", "SPECIAL_ABILITIES_ELECTROCUTED_EFFECT_DEF", "SPECIAL_ABILITIES_DIVINE_WRATH_DEF", "SPECIAL_ABILITIES_UNHOLY_POWER_DEF", "COMBAT_DIALOGUE_DEF", "SOUND_SETUP", "CVillagePeopleDef", "VILLAGER_INTERACTION", "HERO_COMBAT", "HERO_MELEE_COMBAT_ABILITY", "ATTACK_PATTERN", "CCreatureStatsDef", "ARMOUR", "HIT_LOCATION", "CHitLocationsDef", "CCoopSpiritDef", "CContextSensitiveItemDef", "MELEE_COMBAT_KNOCKDOWN_EFFECTS", "CAbilityDef", "CBonusItemDef", "CHeroMarriageDef", "CHeroTitleDef", "CTattooDef", "CHairCardDef", "UI", "CCombatAbilityBlockUnarmedAttackDef", "CCombatAbilityBlockLightWeaponAttackDef", "CCombatAbilityBlockHeavyWeaponAttackDef", "CCombatAbilityBlockProjectileWeaponAttackDef", "CCombatAbilityStrafeDef", "CCombatAbilityBlockCounterAttackDef", "CCombatAbilityFlourishCounterAttackDef", "CCombatAbilityUseProjectileWeaponDef", "CCombatAbilityGetHitCounterAttackDef", "COMBAT_SEQUENCE", "UI_MISC_THINGS_DEF", "CDragonActionSwoopDef", "CDragonActionNapalmDef", "CDragonActionHoverDef", "UI_LOCALE_GRAPHICS_DEF", "UI_ICONS_DEF", "LIGHTNING", "CAugmentationDef", "CSummonerDef", "CSnowTrollDef", "CGuildMasterDef", "CJackDragonDef", "CBriarRoseDef", "CExplosiveTrailDef", "CSpecialAbilitiesDrainLifeDataDef", "CSpecialAbilitiesForcePushDataDef", "CONFIG_OPTIONS_DEFAULTS_DEF", "Game Definition Manager: Compile", "Game Definition Manager: GetDefs", "GLOBAL", "ENGINE", "ENVIRONMENT", "HERO_STATS"};
int main()
{
    Mgr m; m.allocs = 0; m.compiled = false; m.InitAndCompile();
    // retail CCharString order = every add, then the two progress texts and the four def names
    std::vector<std::string> got;
    for (size_t i = 0; i < g_log.size(); ++i) { const std::string& e = g_log[i]; size_t c = e.find(':');
        if (c != std::string::npos && e.compare(0, 4, "stat") != 0) got.push_back(e.substr(c + 1)); }
    bool ok = got.size() == sizeof(kRetail) / sizeof(kRetail[0]) && m.classes.size() == 138 && m.allocs == 138 && m.compiled;
    for (size_t i = 0; ok && i < got.size(); ++i) ok = got[i] == kRetail[i];
    ok = ok && m.sym.size() == 5 && m.comp.size() == 2 && m.sym[1] == L"H\\pc\\*.h" && m.sym[3] == L"D\\*.tpl" && m.comp[1] == L"D\\*.def";
    ok = ok && m.defs[0] == 1 && m.defs[3] == 1;
    // compile happens after every registration and before the def lookups
    size_t ic = 0, ia = 0, ig = 0; for (size_t i = 0; i < g_log.size(); ++i) { if (g_log[i] == "compile") ic = i;
        if (g_log[i].compare(0, 4, "add:") == 0) ia = i; if (g_log[i] == "get:GLOBAL") ig = i; }
    ok = ok && ia < ic && ic < ig;
    if (!ok) { std::printf("0044c72b_TEST FAIL\n"); return 1; }
    std::printf("0044c72b_TEST PASS\n");
    return 0;
}