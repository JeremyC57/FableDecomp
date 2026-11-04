# Chunk 745 -- NScript::CV_HeroDuelScript (4 rows): 4/4 RELOCATION_MATCH, 0 parked

## Results
| Address | Name | Grade | Attempts | Test | Note |
|---|---|---|---|---|---|
| 0x00E987C0 | NScript::CV_HeroDuelScript::Main | RELOCATION_MATCH | 1 | PASS | DoMission loop (same TU) + CV_MayorsInvitationScript::Main binding/spawn model |
| 0x00E9A970 | NScript::CV_HeroDuelScript::FirstFleeCutscene | RELOCATION_MATCH | 1 | PASS | SecondFleeCutscene shape + CWideScreenMagicPauseEntities |
| 0x00E9B240 | NScript::CV_HeroDuelScript::DuelLost | RELOCATION_MATCH | 1 | PASS | same cutscene shape + pGlobalScriptDef rewards |
| 0x00E9BE60 | NScript::CV_HeroDuelScript::Duel | RELOCATION_MATCH | 1 | PASS | Thunder's handle function-scoped, hero's in the inner block |

All four flags `/Gr` (the TU flag of the landed same-TU DoMission 0x00E9B690; the PDB's static Alloc / StatGetClassName are
`SI` = static __fastcall, consistent with a /Gr NScript TU). vland (read-only, `vland_result.txt`): 4 RELOCATION_MATCH + PASS,
no purity / header-policy warnings; only RELOC_NAME warnings for 0x00CDBFB0 (catalog label wrong, see NAME_FIXES.tsv).
Call targets checked per row with calls.py (retail vs object relocations, difflib-aligned): Main 110/110, FirstFlee 93/93,
DuelLost 70/70, Duel 77/77 (RunCutsceneMacro 0x00CBFB7D only shows as an unnamed address on the retail side; our symbol is the
bound `?RunCutsceneMacro@@YIXABVCCharString@@AAV?$map@...` spelling from engine/fable_script_cutscene.h).

## Identity evidence (two sources each)
- Vtable 0x012ED0D4 (CV_HeroDuelScript, no RTTI; read from the retail image): slot 0 0x00E9C340 scalar deleting dtor (landed),
  1 RegisterMain 0x00E98700 (landed), 2 **Main 0x00E987C0**, 3 Init 0x00E986D0 (landed), 4 0x00CBD4E0 (CScriptBase). Ego_r.pdb
  name set: ?Main@CV_HeroDuelScript@NScript@@UAEXXZ. Main spawns this class's own threads (literal + member address pairs).
- **FirstFleeCutscene 0x00E9A970**: ?FirstFleeCutscene@CV_HeroDuelScript@NScript@@QAEXXZ (Ego_r name set); sole caller the landed
  CThunder::Main 0x00E9A5C0 at 0x00E9A6F8 (ParentClass->FirstFleeCutscene()); literals CS_GIBBETHILL_HERODUEL_CLIFF(_W/_N).
- **DuelLost 0x00E9B240**: ?DuelLost@CV_HeroDuelScript@NScript@@QAEXXZ; sole caller CThunder::Main at 0x00E9A95A; literals
  CS_GIBBETHILL_HERODUEL_END(_W), OBJECT_TROPHY_THUNDER; script-def HeroDuelDefeatThunderXP / Morality.
- **Duel 0x00E9BE60**: ?Duel@CV_HeroDuelScript@NScript@@QAEXXZ; sole caller the landed DoMission 0x00E9B690 at 0x00E9BBA8
  (between GotToExecutionTree = true and DuelStarted = true); literals CREATURE_RIVAL_HERO_THUNDER, ThunderWaitPos,
  CS_GIBBETHILL_HERODUEL_HILLTOP_A/B.
- The catalog names of all four rows were already right; the shared header engine/NScript_CV_HeroDuelScript.h declares them.

## Headers
- No header changes, no overlay (hdr/ empty), no siblings to recheck. Everything comes from shared headers: NScript_CV_HeroDuelScript.h
  (this), CQ_SunnyvaleMasterData.h (DefeatedThunder +0x61, LostToThunder +0x62, WhisperKilledByHero +0x74 -- retail offsets agree),
  CScriptThing.h, CScriptGameResourceObjectScriptedThingBase.h / MovieBase.h (implicit ctor/dtor: VC7.1's per-site choice gives
  retail's expanded-at-first-exit / called-elsewhere mix), CWideScreenMagicPauseEntities.h, fable_script_cutscene.h + fable_map.h
  (FABLE_RETAIL_STLP_MAP: map ctor 0x00CDBF70 called with `this` only), CEntityScriptBinding.h, CSpawnedFunc.h,
  fable_script_def_global.h (pGlobalScriptDef).
- Only local declarations: the nested CThunder / CSpectator classes (forward-declared in the class header) with their static
  `Alloc` (spelling = landed 0x00E996B0 / 0x00E997E0 leaves) in Main, and the `inline CScriptThing::~CScriptThing() {}` body the
  CScriptThing header contract asks for.
- Header debt: none found.

## Callee names (all existing bound spellings, none invented)
IsActiveThreadTerminating 0x00CB7940, CleanUpHeroDuel 0x00E98FB0, AddEntityScriptBinding 0x00CB8230, PostAddScriptedEntities
0x00CB8930, CSpawnedFuncBase ctor 0x00CDD450, AddSpawnedFunction 0x00CB7E50, CThunder::Alloc 0x00E996B0, CSpectator::Alloc
0x00E997E0, threads WatchForLeaving 0x00E99260 / ShowEnemyHealth 0x00E99470 / WatchForHeroEnteringCave 0x00E9A270 / DoMission
0x00E9B690; CCharString 0x0099EBF0 / 0x0099EC30 / ~ 0x0099EAE0; ~CScriptThing 0x004AA840; ScriptedThingBase IsValid 0x00CD23B9 /
Release 0x00CD2770 / ~ 0x007E74D0 / operator= 0x008ABD10; ~MovieBase 0x006E7B80; std::map<CCharString,ScriptedThingBase> ctor
0x00CDBF70 / dtor 0x00CDBFB0 / operator[] 0x00CD3D2E; RunCutsceneMacro 0x00CBFB7D; pGlobalScriptDef 0x0143E90C (shared header
spelling `CDefPointer<const CScriptDef>`; Ego_r names it NGlobalScriptDefs::pGlobalScriptDef per naming queue c640 -- owner
decision pending, established spelling kept). Interface slots: 7, 8, 11, 64, 70, 72, 91 CreateCreature, 121 GiveHeroObject, 128
GiveHeroExperience, 156 GiveHeroMorality, 235 EntitySetAsAllowedToFollowHero, 284 IsQuestActive, 293 RemoveQuestCardFromGuild,
296 SetQuestCardObjective, 370, 371, 376 Pause, 379, 480 SetEntityAsRegionFollowing, 512, 516 EntitySetAsDamageable, 565
SetTeleportingAsActive; CScriptThing slot 6 GetPos.

## Asm / helpers
None. No `__asm`, no helper header, no pragma, no inline_depth, no two-view CScriptThing, no `__forceinline` added in the rows.

## Parked
None.

## GOTCHAS one-liners
- (chunk 745) A cutscene exit that destroys a later CScriptThing local BEFORE `PauseAllNonScriptedEntities(false)` (0x00E9A970
  exit 0x00E9AD55, 0x00E9BE60 exit 0x00E9C170) proves an RAII pause guard: write `CWideScreenMagicPauseEntities pause(*Game);`
  (engine header; Game cached in edi) instead of an explicit `game->Pause(false); return;`, which unpauses first.
- (chunk 745) CV_HeroDuelScript TU is /Gr: Main / FirstFleeCutscene / DuelLost / Duel all match first time with `// Flags: /Gr`
  and the shared headers; the scope of each scripted-thing handle (inner block vs whole function) is readable from which exits
  share the epilogue (Duel: Thunder's handle outlives the inner block because Pause(1.0f) runs before its dtor).
