# OUTLINE 0x0044C72B CGameDefinitionManager::InitAndCompile -- straight-line, no branches

## Frame facts
- `push ebp ; mov ebp,esp ; sub esp,2Ch ; push ebx ; push esi ; push edi` (saved order ebx, esi, edi); epilogue `pop edi ; pop esi ; pop ebx ; leave ; ret`.
- No EH (no /EHsc), no /GS cookie, `ret` (0 args). `#pragma optimize("s", on)` + `#pragma optimize("y", off)`.
- esi = this throughout. Register-cached constants (compiler's choice, follow from the source): edi = L"*.h" (0x44C757), ebx = L"*.tpl"
  (0x44C7F6), edi = L"*.def" (0x44C827), then `or edi,-1` (0x44C8B0, the CCharString length argument -1 for every later CCharString(const char*,long));
  ebx = "HERO_STATS" 0x1235E08 (0x44D4A8, reused at 0x44EA0C).

## Frame map (ebp-relative)
- [-0x20..-0x18] std::vector<CWideString> headers (declared FIRST; zeroed first at 0x44C740; ~vector 0x412130 last at 0x44EA44) -> SymbolPathList
- [-0x2c..-0x24] std::vector<CWideString> defs (declared second; zeroed 0x44C749; ~vector 0x412130 at 0x44EA3C) -> CompilePathList
- [-0x08] CWideString platform(L"pc\\") (0x99B6B0 at 0x44C752; ~CWideString 0x99B510 at 0x44EA34, before the two vectors)
- [-0x04], [-0x0c], [-0x10]: CWideString temporaries of the path expressions (see per-block); later [-0x04] = the CCharString temporary of every
  AddDefClass / DisplayProgress / GetDef statement
- [-0x14..-0x10] CDefClassInit temporary {Name (CCharString copy 0x99EC30) at -0x14, AllocFunc at -0x10}

## Body in retail order
```cpp
void CGameDefinitionManager::InitAndCompile()
{
    std::vector<CWideString> headers;                 // [-0x20]
    std::vector<CWideString> defs;                    // [-0x2c]
    CWideString platform(L"pc\\");                    // [-0x08], 0x44C752
    // 0x44C75C: GetHeaderDir->[-0xc], op+(lit) 0x99BF30 ->[-4], push_back 0x4128A0; dtors [-4],[-0xc]
    headers.push_back(NBankFileManager::GetHeaderDir() + L"*.h");
    // 0x44C788: GetHeaderDir->[-0x10], op+(CWideString) 0x99BE70 ->[-4], op+(lit)->[-0xc]; dtors [-0xc],[-4],[-0x10]
    headers.push_back(NBankFileManager::GetHeaderDir() + platform + L"*.h");
    // 0x44C7CA / 0x44C7F6 / 0x44C827: GetDefDir 0x41A080 ->[-0xc], op+(lit)->[-0x10]; dtors [-0x10],[-0xc]
    headers.push_back(NResourceDirectoryNames::GetDefDir() + L"*.h");
    headers.push_back(NResourceDirectoryNames::GetDefDir() + L"*.tpl");
    headers.push_back(NResourceDirectoryNames::GetDefDir() + L"*.def");
    // 0x44C858 / 0x44C884
    defs.push_back(NResourceDirectoryNames::GetDefDir() + L"*.tpl");
    defs.push_back(NResourceDirectoryNames::GetDefDir() + L"*.def");

    // 0x44C8B3 .. 0x44E907: 138 identical blocks, each:
    //   CCharString(str,-1)->[-4] (0x99EBF0); CDefClassInit [-0x14]: Name copy (0x99EC30), [-0x10]=Alloc;
    //   AddDefClass(&init) 0x9B0AC0 (ecx=esi); ~CCharString [-0x14] then [-4] (0x99EAE0)
    AddDefClass(CDefClassInit("CHeroPostcardGeneratorDef", CHeroPostcardGeneratorDef::Alloc));   // #1 call 0044c8da, Alloc 0x0045d613
    AddDefClass(CDefClassInit("GLOBAL", CGlobalDef::Alloc));   // #2 call 0044c916, Alloc 0x00461f5e
    AddDefClass(CDefClassInit("CREATURE", CThingCreatureDef::Alloc));   // #3 call 0044c952, Alloc 0x0044c065
    AddDefClass(CDefClassInit("CCreatureDef", CCreatureDef::Alloc));   // #4 call 0044c98e, Alloc 0x0044c08a
    AddDefClass(CDefClassInit("CPhysicsDef", CPhysicsDef::Alloc));   // #5 call 0044c9ca, Alloc 0x0044f67c
    AddDefClass(CDefClassInit("CSmashableDef", CSmashableDef::Alloc));   // #6 call 0044ca06, Alloc 0x0044f775
    AddDefClass(CDefClassInit("CDegradableDef", CDegradableDef::Alloc));   // #7 call 0044ca42, Alloc 0x004545d8
    AddDefClass(CDefClassInit("CSpecialEffectsDef", CSpecialEffectsDef::Alloc));   // #8 call 0044ca7e, Alloc 0x00456037
    AddDefClass(CDefClassInit("CTextureReplacementDef", CTextureReplacementDef::Alloc));   // #9 call 0044caba, Alloc 0x00456397
    AddDefClass(CDefClassInit("CTargetingDef", CTargetingDef::Alloc));   // #10 call 0044caf6, Alloc 0x004546f5
    AddDefClass(CDefClassInit("CActionUseDef", CActionUseDef::Alloc));   // #11 call 0044cb32, Alloc 0x0044f7ad
    AddDefClass(CDefClassInit("CAppearanceDef", CAppearanceDef::Alloc));   // #12 call 0044cb6e, Alloc 0x0046cde6
    AddDefClass(CDefClassInit("SHOT", CThingShotDef::Alloc));   // #13 call 0044cbaa, Alloc 0x004627dc
    AddDefClass(CDefClassInit("CThingMultiArrowShotDef", CThingMultiArrowShotDef::Alloc));   // #14 call 0044cbe6, Alloc 0x00450b55
    AddDefClass(CDefClassInit("OBJECT", CThingObjectDef::Alloc));   // #15 call 0044cc22, Alloc 0x004627f5
    AddDefClass(CDefClassInit("BUILDING", CThingBuildingDef::Alloc));   // #16 call 0044cc5e, Alloc 0x00459308
    AddDefClass(CDefClassInit("VILLAGE", CThingVillageDef::Alloc));   // #17 call 0044cc9a, Alloc 0x004594cd
    AddDefClass(CDefClassInit("CREATURE_ABILITY", CCreatureAbilityDef::Alloc));   // #18 call 0044ccd6, Alloc 0x00462827
    AddDefClass(CDefClassInit("SWITCH", CThingSwitchDef::Alloc));   // #19 call 0044cd12, Alloc 0x0045948d
    AddDefClass(CDefClassInit("MARKER", CThingMarkerDef::Alloc));   // #20 call 0044cd4e, Alloc 0x00459458
    AddDefClass(CDefClassInit("PHYSICAL_SWITCH", CThingPhysicalSwitchDef::Alloc));   // #21 call 0044cd8a, Alloc 0x00459502
    AddDefClass(CDefClassInit("HOLY_SITE", CThingHolySiteDef::Alloc));   // #22 call 0044cdc6, Alloc 0x00459423
    AddDefClass(CDefClassInit("PLAYER", CPlayerDef::Alloc));   // #23 call 0044ce02, Alloc 0x004628a7
    AddDefClass(CDefClassInit("PLAYER_MOVEMENT", CPlayerMovementDef::Alloc));   // #24 call 0044ce3e, Alloc 0x00462945
    AddDefClass(CDefClassInit("SIM_BUILDING", CSimBuildingDef::Alloc));   // #25 call 0044ce7a, Alloc 0x00462995
    AddDefClass(CDefClassInit("THING_GROUP", CThingGroupDef::Alloc));   // #26 call 0044ceb6, Alloc 0x00462864
    AddDefClass(CDefClassInit("ENGINE_THEME_GROUP", CEngineThemeGroupDef::Alloc));   // #27 call 0044cef2, Alloc 0x004629d2
    AddDefClass(CDefClassInit("ENGINE_THEME", CEngineThemeDef::Alloc));   // #28 call 0044cf2e, Alloc 0x00462ae9
    AddDefClass(CDefClassInit("BRAIN", CBrainDef::Alloc));   // #29 call 0044cf6a, Alloc 0x00462e12
    AddDefClass(CDefClassInit("PLAYER_INVENTORY", CPlayerInventoryDef::Alloc));   // #30 call 0044cfa6, Alloc 0x00462f7d
    AddDefClass(CDefClassInit("PLAYER_GUI", CPlayerGuiDef::Alloc));   // #31 call 0044cfe2, Alloc 0x00462f93
    AddDefClass(CDefClassInit("INVENTORY_CATEGORY", CInventoryCategoryDef::Alloc));   // #32 call 0044d01e, Alloc 0x0045bd8d
    AddDefClass(CDefClassInit("INVENTORY_ITEM", CInventoryItemDef::Alloc));   // #33 call 0044d05a, Alloc 0x0044f644
    AddDefClass(CDefClassInit("INVENTORY_TYPE", CInventoryDef::Alloc));   // #34 call 0044d096, Alloc 0x0045bd24
    AddDefClass(CDefClassInit("MESSAGE_EVENT", CMessageEventDef::Alloc));   // #35 call 0044d0d2, Alloc 0x0045bde9
    AddDefClass(CDefClassInit("SKY", CSkyDef::Alloc));   // #36 call 0044d10e, Alloc 0x0045be0d
    AddDefClass(CDefClassInit("ENVIRONMENT", CEnvironmentDef::Alloc));   // #37 call 0044d14a, Alloc 0x0045c0c8
    AddDefClass(CDefClassInit("ENVIRONMENT_THEME_DAY", CEnvironmentThemeDaySetDef::Alloc));   // #38 call 0044d186, Alloc 0x0045c0e1
    AddDefClass(CDefClassInit("LOCAL_DETAIL_GENERATOR", CEngineLocalDetailGeneratorDef::Alloc));   // #39 call 0044d1c2, Alloc 0x0045c0f7
    AddDefClass(CDefClassInit("CARRY_SLOT", CCarrySlotDef::Alloc));   // #40 call 0044d1fe, Alloc 0x0044f73d
    AddDefClass(CDefClassInit("COMBAT_TYPE", CCombatTypeDef::Alloc));   // #41 call 0044d23a, Alloc 0x0045c4f4
    AddDefClass(CDefClassInit("MATERIAL", CMaterialDef::Alloc));   // #42 call 0044d276, Alloc 0x0045c2ca
    AddDefClass(CDefClassInit("ENGINE", CEngineDef::Alloc));   // #43 call 0044d2b2, Alloc 0x004343f0
    AddDefClass(CDefClassInit("ENGINE_VIDEO_OPTIONS", CEngineVideoOptionsDef::Alloc));   // #44 call 0044d2ee, Alloc 0x00434540
    AddDefClass(CDefClassInit("SIM_VOICES", CSimVoicesDef::Alloc));   // #45 call 0044d32a, Alloc 0x0045c50d
    AddDefClass(CDefClassInit("SOUND_THEME", CSoundThemeDef::Alloc));   // #46 call 0044d366, Alloc 0x0045c5ab
    AddDefClass(CDefClassInit("NOISE", CThingNoiseDef::Alloc));   // #47 call 0044d3a2, Alloc 0x0045c5c1
    AddDefClass(CDefClassInit("THING", CThingBaseDef::Alloc));   // #48 call 0044d3de, Alloc 0x0044c0f0
    AddDefClass(CDefClassInit("CREATURE_GENERATION_FAMILY", CCreatureGenerationFamilyDef::Alloc));   // #49 call 0044d41a, Alloc 0x0045c769
    AddDefClass(CDefClassInit("OBJECT_FAMILY", CObjectFamilyDef::Alloc));   // #50 call 0044d456, Alloc 0x00461329
    AddDefClass(CDefClassInit("HERO_ABILITY", CHeroAbilityDef::Alloc));   // #51 call 0044d492, Alloc 0x0045cbcd
    AddDefClass(CDefClassInit("HERO_STATS", CHeroStatsDef::Alloc));   // #52 call 0044d4cf, Alloc 0x0045cbe6
    AddDefClass(CDefClassInit("REGION", CRegionDef::Alloc));   // #53 call 0044d50b, Alloc 0x0045d2b8
    AddDefClass(CDefClassInit("FACTION", CFactionDef::Alloc));   // #54 call 0044d547, Alloc 0x0045d329
    AddDefClass(CDefClassInit("CONTROL_SCHEME", CControlsDef::Alloc));   // #55 call 0044d583, Alloc 0x00432180
    AddDefClass(CDefClassInit("CAppearanceModifierDef", CAppearanceModifierDef::Alloc));   // #56 call 0044d5bf, Alloc 0x004563e8
    AddDefClass(CDefClassInit("CEntitySoundDef", CEntitySoundDef::Alloc));   // #57 call 0044d5fb, Alloc 0x0045cdf0
    AddDefClass(CDefClassInit("CAMERA_MODE", CCameraModeDef::Alloc));   // #58 call 0044d637, Alloc 0x0045d4d9
    AddDefClass(CDefClassInit("CAMERA_MANAGER", CCameraManagerDef::Alloc));   // #59 call 0044d673, Alloc 0x0045d562
    AddDefClass(CDefClassInit("CAMERA_MANAGER_SET", CCameraManagerSetDef::Alloc));   // #60 call 0044d6af, Alloc 0x0045d5aa
    AddDefClass(CDefClassInit("EXPRESSION", CExpressionDef::Alloc));   // #61 call 0044d6eb, Alloc 0x0045d70b
    AddDefClass(CDefClassInit("OPINION_DEED_EFFECTS", COpinionDeedEffectsDef::Alloc));   // #62 call 0044d727, Alloc 0x0045e53c
    AddDefClass(CDefClassInit("OPINION_PERSONALITY", COpinionPersonalityDef::Alloc));   // #63 call 0044d763, Alloc 0x0045e7b9
    AddDefClass(CDefClassInit("OPINION_REACTION_MANAGER", COpinionReactionManagerDef::Alloc));   // #64 call 0044d79f, Alloc 0x0044c2e8
    AddDefClass(CDefClassInit("OPINION_REACTION_MASK", COpinionReactionMaskDef::Alloc));   // #65 call 0044d7db, Alloc 0x0045de51
    AddDefClass(CDefClassInit("OPINION_DEED_MASK", COpinionDeedMaskDef::Alloc));   // #66 call 0044d817, Alloc 0x0045e2d3
    AddDefClass(CDefClassInit("OPINION_SOURCE", COpinionSourceDef::Alloc));   // #67 call 0044d853, Alloc 0x0045f15f
    AddDefClass(CDefClassInit("SPECIAL_ABILITIES_ASSASSIN_RUSH_DEF", CSpecialAbilitiesAssassinRushDef::Alloc));   // #68 call 0044d88f, Alloc 0x0045f617
    AddDefClass(CDefClassInit("SPECIAL_ABILITIES_BATTLE_CHARGE_DEF", CSpecialAbilitiesBattleChargeDef::Alloc));   // #69 call 0044d8cb, Alloc 0x0045f6f8
    AddDefClass(CDefClassInit("SPECIAL_ABILITIES_BERSERK_DEF", CSpecialAbilitiesBerserkDef::Alloc));   // #70 call 0044d907, Alloc 0x0045faa3
    AddDefClass(CDefClassInit("SPECIAL_ABILITIES_BULLET_TIME_DEF", CSpecialAbilitiesBulletTimeDef::Alloc));   // #71 call 0044d943, Alloc 0x0045fd35
    AddDefClass(CDefClassInit("SPECIAL_ABILITIES_FORCE_PUSH_DEF", CSpecialAbilitiesForcePushDef::Alloc));   // #72 call 0044d97f, Alloc 0x0045fe0d
    AddDefClass(CDefClassInit("SPECIAL_ABILITIES_SUMMON_SPELL_DEF", CSpecialAbilitiesSummonSpellDef::Alloc));   // #73 call 0044d9bb, Alloc 0x004604b4
    AddDefClass(CDefClassInit("SPECIAL_ABILITIES_ENFLAME_DEF", CSpecialAbilitiesEnflameDef::Alloc));   // #74 call 0044d9f7, Alloc 0x0045fec5
    AddDefClass(CDefClassInit("SPECIAL_ABILITIES_PHYSICAL_SHIELD_DEF", CSpecialAbilitiesPhysicalShieldDef::Alloc));   // #75 call 0044da33, Alloc 0x004600ad
    AddDefClass(CDefClassInit("SPECIAL_ABILITIES_DRAIN_LIFE_DEF", CSpecialAbilitiesDrainLifeDef::Alloc));   // #76 call 0044da6f, Alloc 0x00460116
    AddDefClass(CDefClassInit("SPECIAL_ABILITIES_HEAL_LIFE_DEF", CSpecialAbilitiesHealLifeDef::Alloc));   // #77 call 0044daab, Alloc 0x0046016a
    AddDefClass(CDefClassInit("SPECIAL_ABILITIES_GHOST_SWORD_DEF", CSpecialAbilitiesGhostSwordDef::Alloc));   // #78 call 0044dae7, Alloc 0x004601c1
    AddDefClass(CDefClassInit("SPECIAL_ABILITIES_TURNCOAT_SPELL_DEF", CSpecialAbilitiesTurncoatSpellDef::Alloc));   // #79 call 0044db23, Alloc 0x0046023b
    AddDefClass(CDefClassInit("SPECIAL_ABILITIES_FIREBALL_SPELL_DEF", CSpecialAbilitiesFireballSpellDef::Alloc));   // #80 call 0044db5f, Alloc 0x00460289
    AddDefClass(CDefClassInit("SPECIAL_ABILITIES_LIGHTNING_SPELL_DEF", CSpecialAbilitiesLightningSpellDef::Alloc));   // #81 call 0044db9b, Alloc 0x00460439
    AddDefClass(CDefClassInit("SPECIAL_ABILITIES_THUNDER_LIGHTNING_STORM_DEF", CSpecialAbilitiesThunderLightningStormDef::Alloc));   // #82 call 0044dbd7, Alloc 0x0046063c
    AddDefClass(CDefClassInit("SPECIAL_ABILITIES_MULTI_ARROW_DEF", CSpecialAbilitiesMultiArrowDef::Alloc));   // #83 call 0044dc13, Alloc 0x0045f836
    AddDefClass(CDefClassInit("SPECIAL_ABILITIES_MULTI_STRIKE_DEF", CSpecialAbilitiesMultiStrikeDef::Alloc));   // #84 call 0044dc4f, Alloc 0x0045f7a9
    AddDefClass(CDefClassInit("SPECIAL_ABILITIES_BURNT_EFFECT_DEF", CSpecialAbilitiesBurntEffectDef::Alloc));   // #85 call 0044dc8b, Alloc 0x00460502
    AddDefClass(CDefClassInit("SPECIAL_ABILITIES_CREATURE_TINT_DEF", CSpecialAbilitiesCreatureTintDef::Alloc));   // #86 call 0044dcc7, Alloc 0x00460a46
    AddDefClass(CDefClassInit("SPECIAL_ABILITIES_DRUNKENNESS_DEF", CSpecialAbilitiesDrunkennessDef::Alloc));   // #87 call 0044dd03, Alloc 0x004605bc
    AddDefClass(CDefClassInit("SPECIAL_ABILITIES_ELECTROCUTED_EFFECT_DEF", CSpecialAbilitiesElectrocutedEffectDef::Alloc));   // #88 call 0044dd3f, Alloc 0x0046056e
    AddDefClass(CDefClassInit("SPECIAL_ABILITIES_DIVINE_WRATH_DEF", CSpecialAbilitiesDivineWrathDef::Alloc));   // #89 call 0044dd7b, Alloc 0x004608a8
    AddDefClass(CDefClassInit("SPECIAL_ABILITIES_UNHOLY_POWER_DEF", CSpecialAbilitiesUnholyPowerDef::Alloc));   // #90 call 0044ddb7, Alloc 0x004609fb
    AddDefClass(CDefClassInit("COMBAT_DIALOGUE_DEF", CCombatDialogueDef::Alloc));   // #91 call 0044ddf3, Alloc 0x0045e5f9
    AddDefClass(CDefClassInit("SOUND_SETUP", CSoundDef::Alloc));   // #92 call 0044de2f, Alloc 0x00461188
    AddDefClass(CDefClassInit("CVillagePeopleDef", CVillagePeopleDef::Alloc));   // #93 call 0044de6b, Alloc 0x00453ae0
    AddDefClass(CDefClassInit("VILLAGER_INTERACTION", CVillagerInteractionsDef::Alloc));   // #94 call 0044dea7, Alloc 0x0045e645
    AddDefClass(CDefClassInit("HERO_COMBAT", CHeroCombatDef::Alloc));   // #95 call 0044dee3, Alloc 0x00462433
    AddDefClass(CDefClassInit("HERO_MELEE_COMBAT_ABILITY", CMeleeCombatAbilityDef::Alloc));   // #96 call 0044df1f, Alloc 0x004625e2
    AddDefClass(CDefClassInit("ATTACK_PATTERN", CAttackPatternDef::Alloc));   // #97 call 0044df5b, Alloc 0x004611c0
    AddDefClass(CDefClassInit("CCreatureStatsDef", CCreatureStatsDef::Alloc));   // #98 call 0044df97, Alloc 0x0044f8eb
    AddDefClass(CDefClassInit("ARMOUR", CArmourDef::Alloc));   // #99 call 0044dfd3, Alloc 0x004615e5
    AddDefClass(CDefClassInit("HIT_LOCATION", CHitLocationDef::Alloc));   // #100 call 0044e00f, Alloc 0x00461b82
    AddDefClass(CDefClassInit("CHitLocationsDef", CHitLocationsDef::Alloc));   // #101 call 0044e04b, Alloc 0x00453b92
    AddDefClass(CDefClassInit("CCoopSpiritDef", CCoopSpiritDef::Alloc));   // #102 call 0044e087, Alloc 0x0044f931
    AddDefClass(CDefClassInit("CContextSensitiveItemDef", CContextSensitiveItemDef::Alloc));   // #103 call 0044e0c3, Alloc 0x0044f969
    AddDefClass(CDefClassInit("MELEE_COMBAT_KNOCKDOWN_EFFECTS", CMeleeCombatKnockdownEffects::Alloc));   // #104 call 0044e0ff, Alloc 0x00462654
    AddDefClass(CDefClassInit("CAbilityDef", CAbilityDef::Alloc));   // #105 call 0044e13b, Alloc 0x0044ff53
    AddDefClass(CDefClassInit("CBonusItemDef", CBonusItemDef::Alloc));   // #106 call 0044e177, Alloc 0x0044f9af
    AddDefClass(CDefClassInit("CHeroMarriageDef", CHeroMarriageDef::Alloc));   // #107 call 0044e1b3, Alloc 0x0044f7e5
    AddDefClass(CDefClassInit("CHeroTitleDef", CHeroTitleDef::Alloc));   // #108 call 0044e1ef, Alloc 0x0044f9f5
    AddDefClass(CDefClassInit("CTattooDef", CTattooDef::Alloc));   // #109 call 0044e22b, Alloc 0x0044fa3b
    AddDefClass(CDefClassInit("CHairCardDef", CHairCardDef::Alloc));   // #110 call 0044e267, Alloc 0x0044fa96
    AddDefClass(CDefClassInit("UI", NUISystem::CUIDef::Alloc));   // #111 call 0044e2a3, Alloc 0x00433670
    AddDefClass(CDefClassInit("CCombatAbilityBlockUnarmedAttackDef", CCombatAbilityBlockUnarmedAttackDef::Alloc));   // #112 call 0044e2df, Alloc 0x004549ea
    AddDefClass(CDefClassInit("CCombatAbilityBlockLightWeaponAttackDef", CCombatAbilityBlockLightWeaponAttackDef::Alloc));   // #113 call 0044e31b, Alloc 0x00454a1c
    AddDefClass(CDefClassInit("CCombatAbilityBlockHeavyWeaponAttackDef", CCombatAbilityBlockHeavyWeaponAttackDef::Alloc));   // #114 call 0044e357, Alloc 0x00454a4e
    AddDefClass(CDefClassInit("CCombatAbilityBlockProjectileWeaponAttackDef", CCombatAbilityBlockProjectileWeaponAttackDef::Alloc));   // #115 call 0044e393, Alloc 0x00454a80
    AddDefClass(CDefClassInit("CCombatAbilityStrafeDef", CCombatAbilityStrafeDef::Alloc));   // #116 call 0044e3cf, Alloc 0x0044fdbd
    AddDefClass(CDefClassInit("CCombatAbilityBlockCounterAttackDef", CCombatAbilityBlockCounterAttackDef::Alloc));   // #117 call 0044e40b, Alloc 0x0044fd00
    AddDefClass(CDefClassInit("CCombatAbilityFlourishCounterAttackDef", CCombatAbilityFlourishCounterAttackDef::Alloc));   // #118 call 0044e447, Alloc 0x0044fd3f
    AddDefClass(CDefClassInit("CCombatAbilityUseProjectileWeaponDef", CCombatAbilityUseProjectileWeaponDef::Alloc));   // #119 call 0044e483, Alloc 0x0044fdfc
    AddDefClass(CDefClassInit("CCombatAbilityGetHitCounterAttackDef", CCombatAbilityGetHitCounterAttackDef::Alloc));   // #120 call 0044e4bf, Alloc 0x0044fd7e
    AddDefClass(CDefClassInit("COMBAT_SEQUENCE", CCombatSequenceDef::Alloc));   // #121 call 0044e4fb, Alloc 0x00461ebd
    AddDefClass(CDefClassInit("UI_MISC_THINGS_DEF", NUISystem::CUIMiscThingsDef::Alloc));   // #122 call 0044e537, Alloc 0x00432c40
    AddDefClass(CDefClassInit("CDragonActionSwoopDef", CDragonActionSwoopDef::Alloc));   // #123 call 0044e573, Alloc 0x0044fe3b
    AddDefClass(CDefClassInit("CDragonActionNapalmDef", CDragonActionNapalmDef::Alloc));   // #124 call 0044e5af, Alloc 0x0044fe81
    AddDefClass(CDefClassInit("CDragonActionHoverDef", CDragonActionHoverDef::Alloc));   // #125 call 0044e5eb, Alloc 0x0044fec7
    AddDefClass(CDefClassInit("UI_LOCALE_GRAPHICS_DEF", NUISystem::CUILocaleGraphicsDef::Alloc));   // #126 call 0044e627, Alloc 0x00463109
    AddDefClass(CDefClassInit("UI_ICONS_DEF", NUISystem::CUIIconsDef::Alloc));   // #127 call 0044e663, Alloc 0x00432e00
    AddDefClass(CDefClassInit("LIGHTNING", CLightningDef::Alloc));   // #128 call 0044e69f, Alloc 0x00461f17
    AddDefClass(CDefClassInit("CAugmentationDef", CAugmentationDef::Alloc));   // #129 call 0044e6db, Alloc 0x0044ff0d
    AddDefClass(CDefClassInit("CSummonerDef", CSummonerDef::Alloc));   // #130 call 0044e717, Alloc 0x004500b1
    AddDefClass(CDefClassInit("CSnowTrollDef", CSnowTrollDef::Alloc));   // #131 call 0044e753, Alloc 0x00450025
    AddDefClass(CDefClassInit("CGuildMasterDef", CGuildMasterDef::Alloc));   // #132 call 0044e78f, Alloc 0x0045006b
    AddDefClass(CDefClassInit("CJackDragonDef", CJackDragonDef::Alloc));   // #133 call 0044e7cb, Alloc 0x0044ff99
    AddDefClass(CDefClassInit("CBriarRoseDef", CBriarRoseDef::Alloc));   // #134 call 0044e807, Alloc 0x0044ffdf
    AddDefClass(CDefClassInit("CExplosiveTrailDef", CExplosiveTrailDef::Alloc));   // #135 call 0044e843, Alloc 0x004500f7
    AddDefClass(CDefClassInit("CSpecialAbilitiesDrainLifeDataDef", CSpecialAbilitiesDrainLifeDataDef::Alloc));   // #136 call 0044e87f, Alloc 0x0044f85f
    AddDefClass(CDefClassInit("CSpecialAbilitiesForcePushDataDef", CSpecialAbilitiesForcePushDataDef::Alloc));   // #137 call 0044e8bb, Alloc 0x0044f8a5
    AddDefClass(CDefClassInit("CONFIG_OPTIONS_DEFAULTS_DEF", CConfigOptionsDefaultsDef::Alloc));   // #138 call 0044e8f7, Alloc 0x00434660

    CAnimationEntry::StatRegisterClass();             // 0x44E90C call 0x5DD670
    CPhysicalPrimitiveInit::StatRegisterClass();      // 0x44E911 call 0x67F9B0
    SymbolPathList = headers;                         // 0x44E916: ecx=esi+0x40, push &[-0x20], call 0x433DF0
    CompilePathList = defs;                           // 0x44E922: ecx=esi+0x4C, push &[-0x2c], call 0x433DF0
    // 0x44E92E: CCharString [-4]; fld -1.0f (0x122DEE0); push 0; push float; dl=0; ecx=&[-4]; call 0x9E9F40; ~[-4]
    NProgressDisplay::DisplayProgress(CCharString("Game Definition Manager: Compile"), -1.0f, false, false);
    Compile();                                        // 0x44E95C call 0x9B08C0 (ecx=esi)
    NProgressDisplay::DisplayProgress(CCharString("Game Definition Manager: GetDefs"), -1.0f, false, false);   // 0x44E961
    // each: CCharString [-4]; push &member; push 1; push &[-4]; ecx=esi; call; ~[-4]. Return values discarded.
    GetDef(CCharString("GLOBAL"), 1, &PGlobalDef);            // 0x44E98D, +0xD0, call 0x43FEB0 GetDef<CGlobalDef>
    GetDef(CCharString("ENGINE"), 1, &PEngineDef);            // 0x44E9B7, +0xD4, call 0x430096 GetDef<CEngineDef>
    GetDef(CCharString("ENVIRONMENT"), 1, &PEnvironmentDef);  // 0x44E9E1, +0xD8, call 0x451354 GetDef<CEnvironmentDef>
    GetDef(CCharString("HERO_STATS"), 1, &PHeroStatsDef);     // 0x44EA0B, +0xDC, call 0x4513B9 GetDef<CHeroStatsDef>
}   // 0x44EA31: ~platform (0x99B510), ~defs (0x412130), ~headers (0x412130); pop edi/esi/ebx; leave; ret 0x44EA4D
```

## Notes for the author
- Whether the CCharString for AddDefClass is written explicitly or implicit (`CDefClassInit("X", ...)` with the header's
  `const CCharString&` ctor): the [-4] temp is built first either way; the implicit form is what the 138-line list above uses.
- The `mov [ebp-10h],Alloc` store floats (before `push eax`, after `mov ecx,esi`, ...) block to block: compiler scheduling, not source.
- String literals are pooled (/GF): "GLOBAL" 0x1231734 and "ENVIRONMENT" 0x1235ECC are the same addresses in AddDefClass and GetDef.
- vector<CWideString>: inline default ctor (zero stores), out-of-line push_back/operator=/dtor -- copy sibling 0x004336BC's local std::vector template.
- All GetDef template bodies must stay invisible (declaration-only template) -- retail calls four separate out-of-line instances.
- Alloc declarations: `static CDefClassBase* __fastcall Alloc();` per class (namespaces NUISystem:: for CUIDef, CUIMiscThingsDef,
  CUILocaleGraphicsDef, CUIIconsDef). $W/defclasses.tsv lists class + Alloc address for each.
