# OUTLINE 0x00D30030 NScript::CQ_EndGameBossBattleScript::JackOfBladesBossBattle()

## Frame
- `sub esp,0C8h; push ebx; push ebp; push esi; mov esi,ecx; mov [esp+0Ch],0 (flag word); ... push edi` ; no EBP frame, no EH, no /GS.
- Body offsets below are esp after the 4 pushes (saved edi 0x00, esi 0x04, ebp 0x08, ebx 0x0C; locals 0x10..0xD7; ret addr 0xD8).
- Exit: `pop edi; pop esi; pop ebp; pop ebx; add esp,0C8h; ret` (main 0x00D321B2; extra copies 0x00D30529/0x00D30C7D/0x00D30DFC/0x00D30F2C).

| body off | local | type | ctor / dtor evidence |
|---|---|---|---|
| 0x10 | spawn-name construction flag (zeroed in prologue), later reused as CCharString temp | dword / CCharString | test @0x00D313CC; ~CCharString lea [esp+10h] from 0x00D31457 on |
| 0x14 | CCharString temp (waits) / bookcase count | CCharString / long | |
| 0x18 | guildComplex | CScriptThing (vptr, PImp 0x1C/0x20) | built by GetNearestWithDefName @0x00D30130; last dtor @0x00D321AD |
| 0x24 | CCharString temp ("JackOfBladesPhaseWatcher" etc.) | CCharString | |
| 0x28 | bookcases (OBJECT_GUILD_BOOKCASE_SELECT) | vector<CScriptThing> | |
| 0x34 | candleTrios (Candle_Flame_Trio_01) | vector | |
| 0x40 | chandeliersLong (GENERIC_CHANDELIER_LONG) | vector | |
| 0x4C | candleRounds (Candle_Flame_Round_01) | vector | |
| 0x58 | chandeliers (GENERIC_CHANDELIER) | vector | |
| 0x64 | sconces (GENERIC_SCONCE) | vector | |
| 0x70 | CCharString temp / chandelier count spill | | |
| 0x74 | trio count | long | |
| 0x78 | secretBookcase ("SecretBookcase") | CScriptThing | built @0x00D30F9F, dtor @0x00D3216E |
| 0x84 | wardogs ("CREATURE_MINION_WARDOG") | vector | zeroed @0x00D314F2, dtor @0x00D32165 |
| 0x90 | CScriptThing temps; epilogue movie (0x90 vptr, CCountedPointer 0x98/0x9C) | CScriptThing / CScriptGameResourceObjectMovieBase | |
| 0xA0 | candleRound count | long | |
| 0xA4 | chandelierLong count | long | |
| 0xA8 | CScriptThing temps (GetThingWithScriptName results) | CScriptThing | |
| 0xB4 | guildDoors | CScriptThing | dtor @0x00D32141 |
| 0xC0 | teleportMarker | CScriptThing | dtor @0x00D3214D |
| 0xCC | mainGuildMap | CScriptThing | dtor @0x00D32159 |
Sconce count lives in edi, chandelier count in ebp (spilled to 0x70).
Destruction order at the main exit (0x00D3213A..0x00D321AD): guildDoors, teleportMarker, mainGuildMap, [wardogs 0x00D3215E],
[secretBookcase 0x00D3216A], [trio, round, bookcase, chLong, chandelier, sconce 0x00D32173], guildComplex 0x00D321A9
=> declaration order: guildComplex, sconces, chandeliers, chandeliersLong, bookcases, candleRounds, candleTrios, secretBookcase,
wardogs, mainGuildMap, teleportMarker, guildDoors.

Exit targets: 0x00D321B2 (nothing live), 0x00D32173 (vectors+guildComplex), 0x00D3216A (+secretBookcase), 0x00D3215E (+wardogs),
0x00D3213A (+three late things). The first-pass limbo loops 2..6 get their own inline-expanded cleanups (0x00D303A6, 0x00D304B3,
0x00D30693, 0x00D30903, 0x00D30B28, 0x00D30E07 ...): compiler-generated, write plain `return`.

## Body (retail order)

```cpp
void NScript::CQ_EndGameBossBattleScript::JackOfBladesBossBattle()
{
    Game->SetTeleportingAsActive(false);                                   // 0x00D3004B slot 565
    while (!Game->IsLevelLoaded("HeroGuildComplex")) {                     // 0x00D30051 / rotated retest 0x00D3009A
        Game->NewScriptFrame();                                            // 0x00D30088
        if (IsActiveThreadTerminating()) return;                           // 0x00D3008D -> 0x00D321B2
    }
    if (IsActiveThreadTerminating()) return;                               // 0x00D300CC -> 0x00D321B2
    Game->TransitionToTheme("ENVIRONMENT_EOW_04", 0.0f);                   // 0x00D300F8 (push ebx=0)
    CScriptThing guildComplex = Game->GetNearestWithDefName(Game->GetHero(), "VILLAGE_GUILD_COMPLEX_INSIDE"); // 0x00D30121/0x00D30130
    Game->SetVillageLimbo(guildComplex, true);                             // 0x00D3014B
    Game->SetThingAsUsable(Game->GetThingWithScriptName("MainGuildMap"), false); // 0x00D30174/0x00D3017E
    std::vector<CScriptThing> sconces, chandeliers, chandeliersLong, bookcases, candleRounds, candleTrios; // zeroed 0x00D301A4..E8
    long nSconces        = Game->GetAllThingsWithDefName("GENERIC_SCONCE", sconces);               // 0x00D30200 -> edi
    long nChandeliers    = Game->GetAllThingsWithDefName("GENERIC_CHANDELIER", chandeliers);       // 0x00D30230 -> ebp
    long nChandeliersLong= Game->GetAllThingsWithDefName("GENERIC_CHANDELIER_LONG", chandeliersLong); // 0x00D30264
    long nBookcases      = Game->GetAllThingsWithDefName("OBJECT_GUILD_BOOKCASE_SELECT", bookcases); // 0x00D30299
    long nCandleRounds   = Game->GetAllThingsWithDefName("Candle_Flame_Round_01", candleRounds);   // 0x00D302CB
    long nCandleTrios    = Game->GetAllThingsWithDefName("Candle_Flame_Trio_01", candleTrios);     // 0x00D30300
    // first pass: hide everything (EntitySetInLimbo slot 453, args (thing, true, true))
    for (long i = 0; i < nSconces; ++i) { if (IsActiveThreadTerminating()) return; Game->EntitySetInLimbo(sconces[i], true, true); } // 0x00D30313..4B
    if (IsActiveThreadTerminating()) return;                               // 0x00D30355
    for (chandeliers ...)            /* 0x00D30362..93 */  if (ITT) return; // 0x00D30399
    for (chandeliersLong ...)        /* 0x00D30534.. base [esp+40h] */  if (ITT) return; // 0x00D30579
    for (candleRounds ...)           /* 0x00D307A3.. base [esp+4Ch] */  if (ITT) return; // 0x00D307E8
    for (candleTrios ...)            /* 0x00D30A13.. base [esp+34h] */  if (ITT) return; // 0x00D30A53
    for (bookcases ...)              /* 0x00D30C88.. base [esp+28h] */  if (ITT) return; // 0x00D30CC7 (continue 0x00D30F37)
    // VERIFIED order (element base read before each EntitySetInLimbo): first pass sconces, chandeliers, chandeliersLong,
    // candleRounds, candleTrios, bookcases -- NOT the GetAll order. Second pass (below) is sconces, chandeliers, bookcases,
    // chandeliersLong, candleRounds, candleTrios. Check each loop's count operand (edi / ebp / [esp+A4h] / [esp+A0h] / [esp+74h] / [esp+14h]).
    Game->SetTeleporterAsActive(Game->GetThingWithScriptName("HERO_GUILD_TELEPORT_MARKER"), false); // 0x00D30F5B/0x00D30F65 slot 563
    CScriptThing secretBookcase = Game->GetThingWithScriptName("SecretBookcase");  // 0x00D30F9F -> body 0x78
    Game->OpenDoor(secretBookcase);                                        // 0x00D30FB8 slot 425
    Game->SetThingPersistent(secretBookcase, true);                        // 0x00D30FCA slot 477
    Game->CacheMusicSet(MUSIC_SET_CUTSCENE_JACK_BOSS_FIGHT_INTRO);         // 0x00D30FD7 (54)
    Game->CacheMusicSet(MUSIC_SET_CUTSCENE_JACK_BOSS_FIGHT_OUTRO);         // 0x00D30FE4 (55)
    Game->FadeScreenIn();                                                  // 0x00D30FEF
    while (!Game->IsLevelLoaded("FrescoDome")) { Game->NewScriptFrame(); if (ITT) return; } // 0x00D30FF5..0x00D3106E, exits 0x00D3216A
    if (IsActiveThreadTerminating()) return;                               // 0x00D31072
    Game->TransitionToTheme("ENVIRONMENT_JACK_OF_BLADES_FADE", 0.0f);      // 0x00D3109B
    Game->OverrideMusic(MUSIC_SET_BOSS, false, false);                     // 0x00D310B5 (23)
    Game->SetAllowScreenFadingIfAlreadyFaded(true);                        // 0x00D310C2
    Game->FadeScreenOutUntilNextCallToFadeScreenIn(0.5f, 0.0f);            // 0x00D310D4
    Jack = Game->GetThingWithScriptName("EndGameJackOfBladesBoss");       // 0x00D310FC; implicit operator= -> CCountedPointer op= 0x00704580 on +0x64
    if (seh_Jack.IsValid()) seh_Jack.Release();                            // 0x00D31128 / 0x00D31133
    while (!Game->StartScriptingEntity(Jack, seh_Jack, SCRIPT_AI_PRIORITY_HIGHEST)) { // 0x00D31144 (edi=&Jack, ebx=&seh_Jack), rotated 0x00D31170
        Game->NewScriptFrame(); if (IsActiveThreadTerminating()) return;  // 0x00D31155/0x00D3115A
    }
    if (IsActiveThreadTerminating()) return;                               // 0x00D31179
    Game->EntitySetAsDrawable(Jack, false);                                // 0x00D3118E
    Game->EntityTeleportToThing(Jack, Game->GetThingWithScriptName("MK_JACK1_HOLDJACK"), false); // 0x00D311B8/0x00D311C3
    Game->NewScriptFrame(); if (IsActiveThreadTerminating()) return;      // 0x00D311E3
    Game->NewScriptFrame(); if (IsActiveThreadTerminating()) return;      // 0x00D311FA
    Game->EntityEnableGravity(Game->GetThingWithScriptName("CS_AEONS"), false); // 0x00D31230/0x00D3123A
    Game->NewScriptFrame(); if (IsActiveThreadTerminating()) return;      // 0x00D3125A
    Game->NewScriptFrame(); if (IsActiveThreadTerminating()) return;      // 0x00D31271
    Game->EntityTeleportToThing(Game->GetThingWithScriptName("CS_AEONS"), Game->GetThingWithScriptName("CS_AEONS_HOMEPOS"), false); // 0x00D312BB..DD ("CS_AEONS_HOMEPOS" string built first)
    Game->SetThingAsUsable(Game->GetThingWithScriptName("FrescoDoor"), false); // 0x00D3133B
    JackOfBladesBossFightCutscene();                                       // 0x00D31358
    AddSpawnedFunction(new CSpawnedFunc<CQ_EndGameBossBattleScript>("JackOfBladesPhaseWatcher", 0, this,
                       &CQ_EndGameBossBattleScript::JackOfBladesPhaseWatcher), CCharString("")); // 0x00D3135D..0x00D313D7 (vtbl 0x012CC620, fn 0x00D32630)
    seh_Jack.Release();                                                    // 0x00D313DF (unconditional)
    Game->EntitySetAsDrawable(Jack, true);                                 // 0x00D313EC
    Game->NewScriptFrame(); if (IsActiveThreadTerminating()) return;      // 0x00D313F7 -> 0x00D3216A
    IntroFinished = true;                                                  // 0x00D3140C
    Game->DisplayQuestInfo(true);                                          // 0x00D31414
    Game->AddQuestInfoBarHealth(Jack, CRGBColour(255,255,255,255), "HUD_QUEST_ICON_JACK_OF_BLADES", 1.0f); // 0x00D31451 (bl=0xFF kept for later)
    while (Game->EntityGetBossPhase(Jack) != 4 && Game->GetHealth(Jack) > 10.0f) { // 0x00D31466 / 0x00D31477, retest 0x00D314A7
        Game->NewScriptFrame(); if (IsActiveThreadTerminating()) return;  // 0x00D3148F
    }
    if (IsActiveThreadTerminating()) return;                               // 0x00D314B4
    Game->RemoveThing(Jack, false, true);                                  // 0x00D314CA
    Game->NewScriptFrame(); if (IsActiveThreadTerminating()) return;      // 0x00D314D5
    std::vector<CScriptThing> wardogs;                                     // zeroed 0x00D314F2
    Game->GetAllThingsWithDefName("CREATURE_MINION_WARDOG", wardogs);     // 0x00D3151E (result ignored)
    KillAllThingsInVector(*Game, wardogs, false);                          // 0x00D31538 (__fastcall ecx=Game, edx=&wardogs, push 0)
    Game->GiveHeroExperience(pGlobalScriptDef->JackFightDefeatJackXP);     // 0x00D3154F
    FinalDecision = true;                                                  // 0x00D31558
    Game->StopOverrideMusic(false);                                        // 0x00D3155F
    while (!FinalCutscenePlayed) { Game->NewScriptFrame(); if (ITT) return; } // 0x00D31565..8C -> 0x00D3215E
    if (IsActiveThreadTerminating()) return;                               // 0x00D31590
    Game->SetQuestAsCompleted(Game->GetActiveQuestName(), true, false, false); // 0x00D315AB/0x00D315B5
    if (MasterScriptData->JackBossBattleResult == 1) {                     // 0x00D315C4
        if (IsActiveThreadTerminating()) return;                           // 0x00D315D8
        CScriptGameResourceObjectMovieBase movie;                          // 0x00D315EC (body 0x90)
        Game->StartMovieSequence("", movie);                               // 0x00D3162C
        CGameScriptInterfaceBase* game = Game;                             // edi
        game->PauseAllNonScriptedEntities(true);                           // 0x00D31644
        if (Game->GetHeroMorality() < 0.5f) {                              // 0x00D3164F
            if (IsActiveThreadTerminating()) { game->PauseAllNonScriptedEntities(false); return; } // -> shared 0x00D317E8
            MasterScriptData->JackBossBattleHeroGoodAtEnd = false;         // 0x00D31676
            Game->PlayAVIMovie("Data\\Video\\9_evil_epilogue_comp_Sisterkilled.xmv");   // tail-merged ctor 0x00D316A2, call 0x00D316B5
        } else {
            if (IsActiveThreadTerminating()) { game->PauseAllNonScriptedEntities(false); return; }
            MasterScriptData->JackBossBattleHeroGoodAtEnd = true;          // 0x00D31696
            Game->PlayAVIMovie("Data\\Video\\9_evil_epilogue_comp_Sistersaved.xmv");
        }
        Game->ActivateQuest("Hook_Fresco_01_EvilEpilogue");                // 0x00D316DE
        AddLogbookStoryEntryString("TEXT_QST_LOG_STORY_PLATINUM_295");     // 0x00D31701
        Game->GiveHeroMorality(pGlobalScriptDef->EGJ_MoralityLoss);        // 0x00D3170F load, shared call 0x00D318AA
        game->PauseAllNonScriptedEntities(false);                          // shared 0x00D318B5; movie dtor 0x00D318C2..D9
    } else if (MasterScriptData->JackBossBattleResult == 2) {              // 0x00D3171F
        if (IsActiveThreadTerminating()) return;                           // 0x00D3172A
        CScriptGameResourceObjectMovieBase movie;                          // 0x00D3173E
        Game->StartMovieSequence("", movie);                               // 0x00D3177E
        CGameScriptInterfaceBase* game = Game;
        game->PauseAllNonScriptedEntities(true);                           // 0x00D31796
        if (Game->GetHeroMorality() < 0.5f) {                              // 0x00D317A1
            if (IsActiveThreadTerminating()) { game->PauseAllNonScriptedEntities(false); return; } // 0x00D317BF..: movie dtor 0x00D317F3, jmp 0x00D3215E
            MasterScriptData->JackBossBattleHeroGoodAtEnd = false;         // 0x00D317D1
            Game->PlayAVIMovie("Data\\Video\\10_good_epilogue_comp_Sisterkilled.xmv"); // merged ctor 0x00D3182C, call 0x00D3183F
        } else {
            if (IsActiveThreadTerminating()) { game->PauseAllNonScriptedEntities(false); return; } // 0x00D317DF -> 0x00D317E8
            MasterScriptData->JackBossBattleHeroGoodAtEnd = true;          // 0x00D31820
            Game->PlayAVIMovie("Data\\Video\\10_good_epilogue_comp_Sistersaved.xmv");
        }
        Game->ActivateQuest("Hook_Fresco_02_GoodEpilogue");                // 0x00D31868
        AddLogbookStoryEntryString("TEXT_QST_LOG_STORY_PLATINUM_290");     // 0x00D3188B
        Game->GiveHeroMorality(pGlobalScriptDef->EGJ_MoralityGain);        // 0x00D31899 / 0x00D318AA
        game->PauseAllNonScriptedEntities(false);                          // 0x00D318B5
    }
    Game->DeactivateQuest("Q_EndGameBossBattleScenery", 0);                // 0x00D318F9 slot 280
    Game->SetAllowScreenFadingOnNextRegionChange(false);                   // 0x00D3190E
    Game->FadeScreenOut(1.0f, 0.0f, CRGBColour(0, 0, 0, 255));             // 0x00D31930 (colour by value, A from bl=0xFF)
    Game->OverrideMusic(MUSIC_SET_GUILD_DAY, false, false);                // 0x00D3193F (5)
    Game->EnableSounds(false);                                             // 0x00D3194B
    Game->SetThingAsUsable(Game->GetThingWithScriptName("FrescoDoor"), true); // 0x00D3197F
    Game->SetTeleportingAsActive(true);                                    // 0x00D319A1
    Game->SetRegionTextDisplayAsActive(false);                             // 0x00D319AD
    Game->EntityTeleportToThing(Game->GetHero(), Game->GetThingWithScriptName("GuildTrainingHSP"), false); // 0x00D319DA/E5/EF
    while (!Game->IsRegionLoaded("HeroGuildComplexInside")) { Game->NewScriptFrame(); if (ITT) return; } // 0x00D31A0A.., exits 0x00D3215E
    if (IsActiveThreadTerminating()) return;                               // 0x00D31A8B
    Game->SetAllowScreenFadingOnNextRegionChange(false);                   // 0x00D31A9F
    sconces.clear(); chandeliers.clear(); chandeliersLong.clear();         // erase 0x008AC9D0 @0x00D31AB3/AC6/AD9
    bookcases.clear(); candleRounds.clear(); candleTrios.clear();          // @0x00D31AEC/AFF/B12
    nSconces = Game->GetAllThingsWithDefName("GENERIC_SCONCE", sconces);   // 0x00D31B36 ... same six in the same order, same count vars
    ... nCandleTrios = Game->GetAllThingsWithDefName("Candle_Flame_Trio_01", candleTrios); // 0x00D31C36
    Game->ResetToDefaultTheme(5.0f);                                       // 0x00D31C53
    // second pass: (thing, false, true); same loop form; every exit -> 0x00D3215E
    for sconces 0x00D31C59; if ITT 0x00D31C94; chandeliers 0x00D31CA1; ITT 0x00D31CDF; bookcases (base 28h) 0x00D31D07;
    chandeliersLong (40h) 0x00D31D52; candleRounds (4Ch) 0x00D31D9F; candleTrios (34h) 0x00D31DE7; final ITT 0x00D31E07
    CScriptThing mainGuildMap = Game->GetThingWithScriptName("MainGuildMap");           // 0x00D31E36 (body 0xCC)
    Game->SetThingAsUsable(mainGuildMap, true);                                          // 0x00D31E54
    CScriptThing teleportMarker = Game->GetThingWithScriptName("HERO_GUILD_TELEPORT_MARKER"); // 0x00D31E7C (0xC0)
    Game->SetTeleporterAsActive(teleportMarker, true);                                   // 0x00D31E9A
    CScriptThing guildDoors = Game->GetThingWithScriptName("GuildDoors");                // 0x00D31EC2 (0xB4)
    Game->SetThingAsUsable(guildDoors, true);                                            // 0x00D31EE0
    guildComplex = Game->GetNearestWithDefName(Game->GetHero(), "VILLAGE_GUILD_COMPLEX_INSIDE"); // 0x00D31F00/F12, op= 0x00D31F20
    Game->EnableVillagerDefTypes(guildComplex, true, "CREATURE_GUILD_APPRENTICE_MALE");  // 0x00D31F5B
    Game->EnableVillagerDefTypes(guildComplex, true, "CREATURE_GUILD_APPRENTICE_FEMALE"); // 0x00D31F8B
    Game->EnableGuards(guildComplex, true);                                // 0x00D31FA6
    Game->SetVillageLimbo(guildComplex, false);                            // 0x00D31FB8
    Game->EntityTeleportToThing(Game->GetHero(), Game->GetThingWithScriptName("MemorialGardenHSP"), false); // 0x00D31FE6..FFB
    while (!Game->IsRegionLoaded("OakvaleMemorialGarden")) { Game->NewScriptFrame(); if (ITT) return; } // 0x00D32016.., exits 0x00D3213A
    if (IsActiveThreadTerminating()) return;                               // 0x00D32093
    Game->ResetToDefaultTheme(0.0f);                                       // 0x00D320A7
    Game->FadeScreenIn();                                                  // 0x00D320B2
    Game->SetRegionTextDisplayAsActive(true);                              // 0x00D320BF
    Game->SetAllowScreenFadingOnNextRegionChange(true);                    // 0x00D320CC
    Game->EnableSounds(true);                                              // 0x00D320D9
    Game->StopOverrideMusic(false);                                        // 0x00D320E6
    Game->SetQuestAsCompleted(Game->GetActiveQuestName(), false, false, false); // 0x00D320FC/0x00D32106
    Game->DeactivateQuestLater(Game->GetActiveQuestName(), 0);             // 0x00D32121/0x00D3212B
}                                                                          // dtors 0x00D3213A..0x00D321AD, ret 0x00D321BC
```

Notes
- No jump tables; no x87 beyond the two fcomp tests (constants 0x0122F59C = 0.5f, 0x01237F0C = 10.0f) and float immediates
  (0x3F000000 0.5, 0x3F800000 1.0, 0x40A00000 5.0, 0 for 0.0f).
- The ITT-return inside the evil morality-if jumps into the good branch's `{ PauseAll(false); movie dtor; return }` tail at 0x00D317E8:
  the compiler cross-jumped identical blocks; do not try to share code by hand.
- String literals (each its own constant): "HeroGuildComplex" 0x012C5AFC, "ENVIRONMENT_EOW_04" 0x012CC274,
  "VILLAGE_GUILD_COMPLEX_INSIDE" 0x012C5CA0, "MainGuildMap" 0x01291BBC, "GENERIC_SCONCE" 0x012CC984, "GENERIC_CHANDELIER" 0x012CC970,
  "GENERIC_CHANDELIER_LONG" 0x012CC958, "OBJECT_GUILD_BOOKCASE_SELECT" 0x012CC938, "Candle_Flame_Round_01" 0x012CC920,
  "Candle_Flame_Trio_01" 0x012CC908, "HERO_GUILD_TELEPORT_MARKER" 0x01257238, "SecretBookcase" 0x012CC8F8, "FrescoDome" 0x012C4578,
  "ENVIRONMENT_JACK_OF_BLADES_FADE" 0x012857F0, "EndGameJackOfBladesBoss" 0x012CC678, "MK_JACK1_HOLDJACK" 0x012CC8E4, "CS_AEONS" 0x012CC8D8,
  "CS_AEONS_HOMEPOS" 0x012CC8C4, "FrescoDoor" 0x012CC8B8, "JackOfBladesPhaseWatcher" 0x012CC89C, "" 0x0122D70E,
  "HUD_QUEST_ICON_JACK_OF_BLADES" 0x012CC87C, "CREATURE_MINION_WARDOG" 0x012CB9F4, "Q_EndGameBossBattleScenery" 0x012CC63C,
  "GuildTrainingHSP" 0x012CBF70, "HeroGuildComplexInside" 0x01238FCC, "GuildDoors" 0x01258B0C, "CREATURE_GUILD_APPRENTICE_MALE" 0x012CC2AC,
  "CREATURE_GUILD_APPRENTICE_FEMALE" 0x012CC288, "MemorialGardenHSP" 0x012CC740, "OakvaleMemorialGarden" 0x012C4824, movie / hook /
  logbook literals 0x012CC848, 0x012CC818, 0x012CC7FC, 0x012CC7DC, 0x012CC7A8, 0x012CC774, 0x012C1CB8, 0x012CC754.
