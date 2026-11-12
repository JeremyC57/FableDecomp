# Hand chunk 4001 (NScript::CGameflowScript::Main): 1 of 1 byte-exact

Row 0x00CE7670, 32,057 bytes (code to 0x00CEF054, then four jump tables to 0x00CEF3A9; start and length of the catalog row are
right, no SPLITS.tsv). q.py and the read-only vland both say RELOCATION_MATCH with `// Flags: /d2inlT89690`; the behaviour test
passes under g++ and VC7.1. 14 full-function grades (g01..g14) and 3 prefix checks (p01..p03), all in grades.log.

IMPORTANT for whoever grades or lands it: the row needs the overlay (`export FABLE_HDR_OVERLAY=/opt/fable_lane/w22/agent4001/hdr`):
it includes the new header engine/CGameflowScript.h and a new opt-in in engine/CScriptGameResourceObjectBase.h. Without the overlay
q.py does not print SRC_FAIL quickly: cl.exe under Wine spun at 99% CPU until I stopped my own process (seen once, gall.sh run
without the export; with the export the grade takes 1.5 s). Every full-function compile was wrapped in `timeout 600` (tools/cc.py);
the q.py / gall.sh calls in `timeout 100..300`.

## What matched (1)
- NScript::CGameflowScript::Main (0x00CE7670): spawns the CoreQuestReminder and CheckBarrowFieldsGuards threads, then runs the
  whole story as one `switch (MasterScriptData->PostSavePosition)` with 36 case labels that fall through from stage to stage;
  each stage stores its own EGameflowPosition first, waits for its quest / level / region, then hands out quest cards, rumours,
  logbook entries and auto-saves; it ends with the credits cut-scene and a frame loop that runs until the thread is told to end.
  Shape lessons: see LESSONS.md. The three that decided the match: `TRetVoid _Fail_Val; ... return _Fail_Val.Return();` for
  every terminate check (it places the shared `~seh_hero ; ret` tail at the function end and its 129 one-instruction `Return()` expansions raise the
  caller size c2 sees past 9000); `case EGP_WHITE_BALVERINE_KHG:` sharing the label of EGP_WHITE_BALVERINE_WITCHWOOD; and the
  credits block's inline split, reproduced by out-of-line declarations plus the smallest `/d2inlT` that fits (below).

## Section map (stage, enumerator, retail address of the stage's PostSavePosition store, calls, frames, terminate checks, waits)
The case label of a stage is the `mov ecx,[esi+44h]` 3 bytes before its store `mov dword ptr [ecx+4],<stage>`.
| stage | enumerator | store at | calls | frames | checks | waits for |
|---|---|---|---|---|---|---|
| 0 | EGP_INTRO | 0x00CE77D7 | 18 | 1 | 2 | Q_NewOakValeIntro |
| 100 | EGP_TRAINING | 0x00CE8000 | 35 | 2 | 6 | Q_GuildTraining, HeroGuildComplex |
| 150 | EGP_WAITING_FOR_WASP_BOSS | 0x00CE88BD | 9 | 2 | 5 | Q_WaspBoss |
| 200 | EGP_WASP_BOSS | 0x00CE8A9F | 15 | 4 | 7 | Q_WaspBoss |
| 300 | EGP_MAZE_MEETING | 0x00CE8E5A | 15 | 1 | 2 | QS_GuardianSisterInfo |
| 400 | EGP_WAITING_FOR_ORCHARD_FARM | 0x00CE91B2 | 9 | 3 | 7 | Q_OrchardFarmRaidGood, Q_OrchardFarmRaidEvil |
| 450 | EGP_ORCHARD_FARM | 0x00CE9440 | 16 | 1 | 2 | Q_OrchardFarmRaidEvil, Q_OrchardFarmRaidGood |
| 500 | EGP_WAITING_FOR_TRADER_ESCORT | 0x00CE9903 | 3 | 1 | 2 | Q_TraderEscort |
| 550 | EGP_TRADER_ESCORT | 0x00CE99B1 | 15 | 2 | 3 | Q_TraderEscort |
| 600 | EGP_SECOND_MAZE_MEETING | 0x00CE9D78 | 12 | 1 | 2 | QS_GuardianSisterInfo2_SisterInBanditCamp |
| 700 | EGP_BANDIT_CAMP | 0x00CEA058 | 18 | 1 | 2 | Q_BanditCamp |
| 800 | EGP_MAZE_TELEPORT_TO_WW | 0x00CEA4EE | 8 | 1 | 2 | QS_GuardianTrophyDealerInfo |
| 850 | EGP_TROPHY_DEALER | 0x00CEA6B0 | 11 | 1 | 2 | V_TrophyDealer |
| 856 | EGP_WITCHWOOD_POST_TROPHY_DEALER | 0x00CEA94B | 13 | 1 | 4 | Q_WhiteBalverineKnotholeGlade (KnotholeGlade message loop) |
| 865/870 | EGP_WHITE_BALVERINE_KHG / _WITCHWOOD | 0x00CEAD02 | 15 | 1 | 2 | Q_WhiteBalverineKnotholeGlade |
| 875 | EGP_WITCHWOOD_WAITING_FOR_ARENA | 0x00CEB065 | 3 | 1 | 2 | Q_Arena |
| 900 | EGP_ARENA | 0x00CEB111 | 19 | 2 | 3 | Q_Arena |
| 1000 | EGP_MEET_SISTER | 0x00CEB564 | 13 | 3 | 4 | QS_MeetSister |
| 1050 | EGP_WAITING_FOR_MCC | 0x00CEB8CD | 7 | 1 | 2 | Q_MinionClifftopChase |
| 1100 | EGP_MINION_CLIFFTOP_CHASE | 0x00CEBA7B | 12 | 3 | 4 | Q_MinionClifftopChase |
| 1200 | EGP_GRAVEYARD | 0x00CEBDB1 | 7 | 3 | 4 | Q_OpeningGraveyardSecretPassage |
| 1250 | EGP_PRISON | 0x00CEBF6F | 15 | 4 | 5 | Q_PrisonEscapeRescueMother |
| 1300 | EGP_HOOK_COAST | 0x00CEC351 | 3 | 1 | 2 | Q_EndGame |
| 1450 | EGP_WAITING_FOR_WIZARD_BATTLE | 0x00CEC404 | 16 | 1 | 4 | Q_WizardBattle (HeroGuildComplexInside message loop) |
| 1500 | EGP_WIZARD_BATTLE | 0x00CEC89A | 7 | 1 | 2 | Q_WizardBattle |
| 1550 | EGP_WAITING_FOR_FOCAL_SITES | 0x00CECA4F | 18 | 1 | 2 | HeroGuildComplexInside |
| 1600 | EGP_FOCAL_SITES | 0x00CECE5E | 13 | 3 | 4 | Q_EndGameFocalSites |
| 1700 | EGP_JACK_BOSS_FIGHT | 0x00CED171 | 33 | 3 | 6 | Q_EndGameBossBattle (completed, then no longer active), OakvaleMemorialGarden |
| 1900 | EGP_FIRE_HEART | 0x00CED9B9 | 10 | 3 | 5 | Q_FireHeart |
| 2100 | EGP_THE_SHIP | 0x00CEDC74 | 15 | 4 | 6 | Q_SummoningTheShip, LostBay |
| 2300 | EGP_THE_ORACLE | 0x00CEE04C | 4 | 3 | 4 | Q_AwakeningTheOracle |
| 2400 | EGP_SCYTHE_MEETING | 0x00CEE18C | 9 | 3 | 4 | QS_ScytheInfo |
| 2500 | EGP_HERO_SOULS | 0x00CEE3E8 | 6 | 3 | 4 | Q_HeroSouls |
| 2600 | EGP_DRAGON_FIGHT | 0x00CEE5AE | 25 | 6 | 11 | Q_DragonBossFight (completed, then no longer active), FrescoDome, credits block 0x00CEE864..0x00CEEA4C, NorthernWastes3 |
| 2800 | (store only; in the 2600 flow) | 0x00CEEB6F | 24 | - | - | 22 expressions, AutoSave, FadeScreenIn |
| 2800 | case EGP_FINISHED_GAME | frame call 0x00CEF008 | 0 | 1 | 1 | `do { NewScriptFrame(); } while (!IsActiveThreadTerminating());` |
Head 0x00CE7670..0x00CE7785: frame 0x824, the two `new CSpawnedFunc<CGameflowScript>` + AddSpawnedFunction. Switch dispatch from 0x00CE7785 (compare tree, then jump tables 0x00CEF054 / 0x00CEF134 / 0x00CEF1E0 / 0x00CEF2CC). Shared exits:
0x00CEF016 (plain return), 0x00CEF03C (`~seh_hero` then return, used by the checks inside the credits block).
The enumerators 733, 766, 1216 and 1233 (WAITING_FOR_BANDIT_CAMP_TWINBLADE, BANDIT_CAMP_TWINBLADE, WAITING_FOR_FINALGRAVEYARD,
FINALGRAVEYARD) have no case: the function returns after the two spawns (tested).

## Decisions and their evidence
- Generated-script wait loop: `while (!Game->X("..")) { Game->NewScriptFrame(); if (IsActiveThreadTerminating()) return ..; }
  if (IsActiveThreadTerminating()) return ..;` -- no check before the loop. c2 rotates it (question at the top and again at the
  bottom). `while (Game->IsQuestActive(..) != false)` gives the `setne bl` form of the two "no longer active" waits
  (0x00CED227.., 0x00CEE687..). The compound `!A && !B` wait (0x00CE947D) keeps its top test.
- if-bodies open with a terminate check, except the two WhisperKilledByHero if / else (0x00CEB1C4, 0x00CEB43E) and the
  JackBossBattleResult if / else-if (0x00CED4F0). `W ? 175 : 170` compiles to neg/sbb; the if / else gives retail's
  `mov ecx,0AFh ; jne ; mov ecx,0AAh`. `switch (JackBossBattleResult)` gives dec/je; if / else-if gives `cmp eax,1 .. cmp eax,2`.
- EGP_WAITING_FOR_ORCHARD_FARM is `while (true) { check; if (good) { check; frame; check; remove evil card; break; } if (evil)
  { check; frame; check; remove good card; break; } frame; }` then a check: the check sits at the loop top and there is none after
  the bottom frame (retail 0x00CE91C6 / 0x00CE9259 / 0x00CE925E).
- The two guild-master message loops (0x00CEA983, 0x00CEC431) keep a block-scoped `bool GaveMessage`; the frame comes first in
  the loop body. Retail merges the first check of the if arm and the else arm into one call (0x00CEA9EC, 0x00CEC49A).
- The credits block is the house form of the other script Mains (HobbeTooth 0x00D9A190): `CScriptGameResourceObjectScriptedThingBase
  seh_hero; if (seh_hero.IsValid()) seh_hero.Release(); while (!Game->StartScriptingEntity(Game->GetHero(), seh_hero,
  SCRIPT_AI_PRIORITY_HIGHEST)) {..}`, a `std::map<CCharString, CScriptGameResourceObjectScriptedThingBase> cast`, a movie object,
  `CWideScreenMagicPauseEntities pause(*Game)` and `RunCutsceneMacro("CS_FABLE_CREDITS", cast, 0, 0, false, true)`.
- `MasterScriptData->PostSavePosition = EGP_FINISHED_GAME;` is a plain statement of the 2600 flow (0x00CEEB6F); the case label
  of 2800 is at the final loop (frame call 0x00CEF008).

## Inline budget (the only device in the row)
c2 inline trace of the final source (tools/inl.sh, tools/inl/f.py; sc = c1*(1/f+6)/7, expanded iff sc < T, T = 74 by default;
the block runs at static frequency f = 6.5e-5):
| callee in the credits block | score | retail |
|---|---|---|
| CScriptGameResourceObjectScriptedThingBase::operator= (top layer; calls CBaseObject::operator= 0x0099A3B0 and 0x008AB1E0) | 89689 | expanded |
| CScriptGameResourceObjectMovieBase ctor | 72189 | expanded |
| implicit CScriptGameResourceObjectMovieBase dtor (calls 0x006E7AB0, 0x0099A430) | 67814 | expanded |
| CWideScreenMagicPauseEntities ctor | 63439 | expanded |
| CScriptGameResourceObjectScriptedThingBase ctor 0x007E72A0 | 18067 | called -> existing opt-in FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTSCRIPTEDTHINGBASE_OUTOFLINE_CTOR |
| CScriptGameResourceObjectBase ctor 0x006E7A60 (nested in the movie ctor, f = 1) | 21 | called -> new overlay opt-in FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTBASE_OUTOFLINE_CTOR |
| map ctor 0x00CDBF70 | 164066 | called |
| scripted-thing dtor 0x007E74D0 | 203442 | called |
| map dtor 0x00CDBFB0 | 807208 | called |
| map operator[] 0x00CD3D2E | cost 465 > 450 cap | called |
`/d2inlT89690` is the smallest threshold that expands operator= (T89689: 32037v32057, first difference 0x00CEE922, grade g12);
everything up to 164066 stays a call, and no other site of the function flips (7605 / 7605 instructions equal). Control-flow
spellings were tried first and fail: the block would need a static frequency about 1000 times higher; a `do { } while (0)` gives
a factor of 10 per level and makes the first difference 600 instructions earlier (g10). The three forced header opt-ins (g08, g11) reached 7191
instructions but cannot expand the implicit movie dtor, so the flag replaced them; the source now has no forced inline at all.
Open question (not needed for the match): why retail's c2 saw this block as hot. Likely the original was a cut-scene macro whose
expansion had loops around the block; nothing in the bytes pins it down.

## Retail v Aeon's Lua port, per section (tools/luacmp.py -> map/lua_diff.txt; frames and terminate checks left out)
The Lua has the same 35 stage functions in the same order and, argument for argument, the same calls in 16 stages (WaspBoss,
TraderEscort, BanditCamp, MazeTeleportToWW, TrophyDealer, MinionClifftopChase, Graveyard, Prison, HookCoast, WizardBattle,
WaitingForFocalSites, FocalSites, TheShip, TheOracle, ScytheMeeting, HeroSouls). Disagreements (retail bytes decide):
- All stages: the Lua writes CoreQuestWaiting with `SetStateBool("CoreQuestWaiting", ..)`; retail stores the member at +0x48
  directly, at the same places (16 stores). The Lua's `NewScriptFrame()` returns the terminate state; retail calls
  IsActiveThreadTerminating() after every frame, after every wait loop and at the top of most if-bodies (129 call sites).
- Main: the Lua dispatches through a table in a `while currentStage` loop with Log lines; retail is one switch with fall-through
  and no logging. The Lua has no entry for EGP_WHITE_BALVERINE_KHG (865); retail's table sends 865 to the 870 stage.
- Intro (0): retail makes two ActivateMultipleQuestsWithoutLoadingResources calls (4 quests, then 21 background quests) from two
  separate CArray<CCharString> locals; the Lua passes one table.
- Training (100): retail tests only IsAlive() of TheRealGuildmaster / GuildDoors (no null tests, the marker is not tested) and
  looks TheRealGuildmaster up a second time for the teleport; EntityTeleportToThing has a third argument `false`.
- WaitingForOrchardFarm (400): same calls; retail has the terminate check at the loop top and, in each arm, a check before
  and after the frame.
- WitchwoodPostTrophyDealer (856) and WaitingForWizardBattle (1450): retail runs the frame FIRST in the loop body, then the
  region test; the Lua runs it last. So retail never gives the message in the frame the stage is entered.
- Arena (900), JackBossFight (1700), FireHeart (1900): the Lua reads the master data through GetMasterGameState; retail reads
  WhisperKilledByHero +0x74 / JackBossBattleResult +0x88 directly, WhisperKilledByHero twice (two separate if / else).
- DragonFight (2600): retail's credits sequence is StartScriptingEntity(hero, handle, 4) in a wait loop, a cast map with
  "Hero", StartMovieSequence("", movie), PauseAllNonScriptedEntities(true), RunCutsceneMacro("CS_FABLE_CREDITS", cast, 0, 0,
  false, true), PauseAllNonScriptedEntities(false); the Lua uses its own StartCutscene / RunCutscene / EndCutscene wrappers and
  null-tests the hero and the door. Retail's teleports pass a third argument `false`.
- FinishedGame (2800): retail stores 2800 and gives the 22 expressions, AutoSave and FadeScreenIn inside the 2600 flow; a game
  loaded at 2800 only runs the frame loop (the Lua's stage function would give the expressions again). Retail's loop is
  `do { frame } while (!terminating)`; the Lua's is `while true`.

## Names and evidence
- `?Main@CGameflowScript@NScript@@UAEXXZ`: Ego_r.pdb and FableWin.pdb name sets; slot 2 of the script vtable 0x012C3FA4;
  RegisterMain 0x00CE75B0 spawns it. Class layout: hdr/engine/CGameflowScript.h (see hdr/README.md).
- All 26 call targets of the built object pair one-to-one with the retail targets (tools/rel.py v8 --all), 25 with the spelling
  rebuild/oracles/reloc_bindings.tsv already binds; 0x006E7A60 had no binding yet and is the landed row
  CScriptGameResourceObjectBase::CScriptGameResourceObjectBase (`??0CScriptGameResourceObjectBase@@QAE@XZ`). vland found no
  RELOC_IDENTITY problem. LABELS.tsv lists them.
- ETutorialCategory TUTORIAL_CATEGORY_QUEST = 27, TUTORIAL_CATEGORY_RENOWN = 29: ghidra_out/installed_game/header_enums.csv
  (misc_def_types.h) and the Lua port. EScriptAIPriority SCRIPT_AI_PRIORITY_HIGHEST = 4: the same csv and the landed script Mains.
- Master data members (engine/CQ_SunnyvaleMasterData.h, Ego_r): PostSavePosition +4, WhisperKilledByHero +0x74,
  JackBossBattleResult +0x88, BreakSiegeFinished +0xE0, BodyGuardsInLimbo +0xE4.

## Descriptive (made-up) names -- locals only, no source has them
`GaveMessage` (the bool of the two message loops; the Lua calls it gaveMessage), `GuildDoors`, `quests` (the two CArray locals),
`seh_hero`, `cast`, `movie`, `pause` (the spellings of the landed script Mains). No made-up function, member or global name.

## Header changes (overlay hdr/, documented in hdr/README.md)
- NEW engine/CGameflowScript.h (class model; the landed sibling rows 0x00CEF3B0 / 0x00CEF8E0 still carry local models -- debt:
  retype them with this header at landing).
- engine/CScriptGameResourceObjectBase.h: one opt-in, default text unchanged.
- Row-local declarations kept as in the landed script Mains: `enum EScriptAIPriority`, `enum ETutorialCategory` (two values),
  the three AddLogbook* free functions, and three declared-not-defined template members (0x008AB1E0, 0x006E7AB0, 0x00414EA0).
  Debt: these belong in shared headers (the enums in particular).

## The behaviour test (test/00ce7670.cpp, generated by tools/mktest.py)
The row's function body runs against a recording model of the script interface. The expected order (729 entries: calls with
arguments, frames, terminate checks, member stores) is generated from the RETAIL disassembly (map/events.txt) in address order,
not from the source; tools/mktest_regions.txt letters the conditional regions by retail address. Checks: two full runs (one per
side of every branch) equal the retail order exactly, including the member values at every call; a start at each of the 36
handled stage values runs the retail tail; values without a case only spawn; a terminate request at the n-th check stops there
(every check of both runs); loop shapes by counts (orchard loops, message given once per region visit, JackBossBattleResult 0,
three frames at the end). Mutation-checked by hand: a changed logbook number, a dropped CoreQuestWaiting store, a dropped KHG
case, a wrong JackBossBattleResult test and a dropped `GaveMessage = false` each fail it.

## Tools (all in tools/, copies or own work; nothing shared was edited)
- `python3 tools/build.py <out.cpp>`: work/draft.frozen.txt + work/patch.txt + work/head.inc + work/tail.inc -> full source
  (this is how src/00ce7670.cpp was made; do not regenerate the frozen draft).
- `bash tools/var.sh <tag>`: build + preflight + compile + diff summary. `python3 tools/cc.py <src> <tag> [flags]`: compile
  (timeout 600) to tmp/b/<tag>/x.obj. `python3 tools/pd.py <tag> [--lo A] [--hi A] [-n ctx] [--sum]`: relocation-masked
  instruction diff against retail. `python3 tools/rel.py <tag> [--all]`: call-target identity.
- `bash tools/inl.sh <src> <tag> [flags]` then `python3 tools/inl/f.py tools/inl/<tag>.json`: c2 inline decisions of Main.
- `python3 tools/map.py` (retail -> map/events.txt), `tools/gen.py` (events -> first draft), `tools/dv.py`, `tools/skel.py`.
- `python3 tools/mktest.py [--list]`: the test; `python3 tools/luacmp.py`: retail v Lua per stage.

## Parked for Fable 5.1 (0)
None.

## Skipped
None.

## Label corrections
None. The manifest row of 0x00CEF550 has module `CGameflowScript` without the namespace (it is NScript::CGameflowScript::
CheckBarrowFieldsGuards, as the PDB name sets and this row's `CSpawnedFunc<CGameflowScript>` at 0x00CE7740 show); 0x006E7A60's
manifest parameter type `C3DAnimationUtil *` is a propagated guess (it is the CScriptGameResourceObjectBase ctor).
