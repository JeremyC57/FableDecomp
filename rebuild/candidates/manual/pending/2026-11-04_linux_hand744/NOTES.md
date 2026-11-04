# Chunk 744 -- CQ_HangingTree{Evil,Good}Script quest-script rows (Linux lane, 2026-11-04)

## Results
| Address | Name | Grade | Attempts | Note |
|---|---|---|---|---|
| 0x00D69480 | NScript::CQ_HangingTreeEvilScript::Main | DIFFER(2118v2118) dl=15 | 4 grades + ~20 compile-only probes | fail-branch register rotation only (differ/) |
| 0x00D6F4B0 | NScript::CQ_HangingTreeGoodScript::Main | DIFFER(2106v2106) dl=13 | 1 grade + probes | same residue as 0x00D69480 (differ/) |
| 0x00D74430 | NScript::CQ_HangingTreeGoodScript::DoMission | RELOCATION_MATCH, test PASS, vland WIN | 3 | src/ + payload.json |
| 0x00D75060 | NScript::CQ_HangingTreeGoodScript::FinalRound | DIFFER(5237v4044) dl=680 | 1 | first draft (differ/) |
| 0x00D77690 | NScript::CQ_HangingTreeGoodScript::DoCutscene | not attempted | 0 | out of session budget |

vland (read-only): 1 WIN (0x00D74430), one WARN RELOC_NAME for 0x00D77340 (the GetGuardsHealth name fix below).

## Identities (two sources each)
- 0x00D69480 = ?Main@CQ_HangingTreeEvilScript@NScript@@UAEXXZ: slot 2 of vtable 0x012D1F98 (slots: ~ 0x00D6ED40, RegisterMain
  0x00D69390 landed, Main, Init 0x00D68FF0 landed, OnPersist 0x00D69450); strings HTE_* bindings, TEXT_QUEST_HANGING_TREE_EVIL_OBJECTIVE_01,
  spawns DoMission 0x00D6BFA0 / BanditYells 0x00D69CD0 (both Evil, landed). The catalog's "CQ_HangingTreeGoodScript::Main" was propagated.
- 0x00D6F4B0 = ?Main@CQ_HangingTreeGoodScript@NScript@@UAEXXZ: slot 2 of vtable 0x012D260C (RegisterMain 0x00D6F3C0 landed); HTG_* bindings.
- 0x00D74430 DoMission: Good Main stores it as the "DoMission" CSpawnedFunc (0x00D6F9A3); PDB QAEXXZ.
- 0x00D75060 FinalRound: called by DoMission at 0x00D74C92 after state 7; PDB ?FinalRound@...QAEXXZ; strings TEXT_QUEST_HANGING_TREE_GOOD_OBJECTIVE_02.
- 0x00D77690 DoCutscene(long): called by DoMission with 0 (0x00D74650) and 1 (0x00D74CC7), `ret 4`; PDB ?DoCutscene@...QAEXJ@Z.
- Callees named from DoMission's spawned-function strings + PDB name set: 0x00D74D30 MonitorHeroProximity ("MonitorHeroProximity"),
  0x00D767F0 DoConversations ("DoConversations"), 0x00D74CE0 WatchForMissionEnd, 0x00D773E0 MonitorRegionPresence.
- Nested Alloc addresses (the ClassAllocFunc stored in each binding by the Mains): Evil CHTE_AmbushPoint 0x00D69D50, CHTE_AssaultGuard
  0x00D6AD70, CHTE_FreedomFighter 0x00D6A3D0, CHTE_Audience 0x00D6B180, CHTE_Prisoner 0x00D69E30, CHTE_Executioner 0x00D69FD0,
  CGUARD1 0x00D6A150, CGUARD2 0x00D6A290; Good CHTG_Prisoner 0x00D71980, CHTG_Guard 0x00D70520, CHTG_AmbushPoint 0x00D6FD70,
  CHTG_AmbushPointRear 0x00D6FF30, CHTG_AmbushPointCond 0x00D6FE50, CHTG_AmbushPointBackup 0x00D70010, CHTG_Executioner 0x00D731A0,
  CHTG_Audience 0x00D720E0, CHTG_Boss 0x00D724D0, CHTG_FreedomFighter 0x00D700E0. Binding vtables: Evil 0x012D2078, Good 0x012D2704.

## Header overlay ($D/hdr/engine)
- NEW engine/CQ_HangingTreeEvilScript.h: Ego_r field order, retail = PDB - 0x14, 12-byte vectors, size 0xD0 (factory 0x00D6ED10 `push 0D0h`,
  ctor 0x00D68F00); CLevelScriptBase<CQ_SunnyvaleMasterData> base (as the Good script; master type unproven, reaches no mangled name);
  eight nested entity scripts with Alloc.
- CHANGED engine/CQ_HangingTreeGoodScript.h: opt-in FABLE_RETAIL_CQ_HANGINGTREEGOODSCRIPT adds the remaining PDB methods (RemoveDregs,
  CleanUp, RemoveParty(bool), DoMission, WatchForMissionEnd, MonitorHeroProximity, FinalRound, SetGuardsHealth, DoConversations,
  GetGuardsHealth, MonitorRegionPresence, DoCutscene(long)) and the ten nested CHTG_* classes with Alloc. hdrcheck2: default=same
  (both runs print cpp-err from the NSCRIPT #error guard, identical for repo and overlay). The only landed includer, SpawnParty 0x00D76030,
  does not define the macro (SIBLINGS.tsv).

## Header debt
- none new; CScriptDef offsets used from engine/CScriptDef.h (HTGoodCompletionMorality +0xF44, HTGoodHeroKilledPrisonerGoldReward +0xF48,
  ...RenownReward +0xF4C, HTEvilCompletionMorality +0xF5C), pGlobalScriptDef as CDefPointer<const CScriptDef> (ESTABLISHED_SPELLINGS).

## Callee names
- Interface slots (CGameScriptInterfaceBase header): SetQuestCardObjective 296, GetActiveQuestName 655, IsLevelLoaded 11, NewScriptFrame 7,
  PostAddScriptedEntities 64, Pause 376, SetQuestAsCompleted 288, SetQuestAsFailed 289, GiveHeroMorality 156, RemoveQuestCardFromGuild 293,
  SetQuestCardGoldReward 297, SetQuestCardRenownReward 298, FadeScreenOut 373, FadeScreenIn 374, SetCreatureGeneratorsEnabled 558,
  DeactivateQuestLater 281, GetThingWithScriptName 72, GetHero 70; pDSTGame SetTimer 89 (CTimer::operator=(long)).
- Globals: GFCharStringToInt 0x0099E7F0 (?GFCharStringToInt@@YIJABVCCharString@@@Z), AreAllThingsInVectorDead 0x00CBED00 and
  IsDistanceBetweenThingsUnder 0x00CBE2FF (both PDB name sets, fastcall).

## Name fixes
- 0x00D77340 is ?GetGuardsHealth@CQ_HangingTreeGoodScript@NScript@@QAEXXZ, not the landed descriptive "StoreGuardsHealth" (NAME_FIXES.tsv).
- 0x00D69480 catalog class wrong (Evil, not Good).

## DoMission (matched) -- levers
- `if (!PartyDefeated) { ... HTG_State = 2; } else { if (IsActiveThreadTerminating()) return; }` (the generated empty-else keeps only the
  terminate check) lays out like retail; the inverted form put the else first.
- The first PartySpawnPoint assignment (top level) expands CScriptThing::operator= but calls ~CScriptThing 0x004AA840; the second (inside the
  state-1 loop) expands both. Plain `inline` operator= (and the header's FABLE_RETAIL_CSCRIPTTHING_IMPLICIT_ASSIGNMENT) leave the top-level
  site a call to ??4CScriptThing; `__forceinline` on the TU definition matches (owner 2026-11-01: an out-of-line COMDAT 0x008AB980 exists, so
  this is recorded: the row needs the forced form). ~CScriptThing stays plain inline `{}` and VC7.1 splits it per call site by itself.

## Parked residues and next ideas
- Mains (0x00D69480 / 0x00D6F4B0): every byte matches except the failure branch, where VC7.1's scratch registers are rotated by one
  (retail FadeScreenOut loads the vtable into eax, ours edx; Pause / SetQuestAsFailed follow). In Good Main the success branch's identical
  FadeScreenOut gets eax in both builds; with the if/else inverted the first-laid-out Fade gets eax and the second edx, so the second
  occurrence is what rotates. Probes that left it unchanged (compile-only): FABLE_RETAIL_CRGBCOLOUR, 3-arg CRGBColour, local non-union
  CRGBColour, named colour local, default-argument colour, removing the success-branch tail, removing / replacing SetQuestAsFailed,
  dropping the else's terminate check, a cached interface pointer, `bool faded =`, `do{}while(0)` else, `== true`, `&& !MissionFailed`,
  for(;;) top-break wait loop, inverted if (layout worse). Next idea: a permuter-style sweep of declaration/statement forms around the
  join (merge block), or the inline SpawnFunction helper the RegisterMain notes mention (an IR-only difference upstream).
- FinalRound 0x00D75060: draft follows retail's statement order and call targets (structure in the source); residue is the four exit
  flavours: (a) guard-loop / after-loop returns expand ~CScriptThing and the vector's element dtors + free inline, (b) the approach-loop
  return also expands the three CScriptGameResourceObjectScriptedThingBase dtors, (c) after the spawn loop ~vector is called (0x008AC970),
  (d) the final loop's returns call everything out of line; plus frame-slot order. Next idea: per-exit loop-guard / do{}while(0) shapes
  (INLINE_DEPTH_REVISIT 2026-11-03), executioner.IsAlive() is a vtable call in retail while GetPos/GetDataString are expanded.
- DoCutscene 0x00D77690: not drafted. Structure: switch on the long (0/1/2 paths at 0x00D776A3/0x00D776AA/0x00D776B1), StartMovieSequence
  (370) + PauseAllNonScriptedEntities (379), HTG_Prisoner / HTG_Executioner things, three scripted-thing resources, CameraMoveToPosAndLookAtThing
  (407), GetNearestWithDefName (81) "REGION_EXIT_POINT", CCutsceneCastMap (0x00CDBF70 ctor / 0x00CDBFB0 dtor / operator[] 0x00CD3D2E with
  "EXEC" / "PRIS" / "HERO"), RunScriptedCutscene 0x00CBFB7D "CS_HANGINGTREE_GOOD" / "CS_HANGINGTREE_GOOD_EXECUTION", IsDistanceFromThingToPositionOver
  0x00CBE45C; per-exit inline splits as in FinalRound.

## Helper calls (asm)
- none (no row uses an asm helper).

## GOTCHAS one-liners
- (chunk 744) Quest Mains: an identical FadeScreenOut in both arms of the final if/else gets eax in the first-laid-out arm and edx in the second
  under VC7.1, where retail has eax in both; ~20 source forms do not move it (0x00D69480 / 0x00D6F4B0 parked).
- (chunk 744) A top-level `Member = Game->GetThingWithScriptName(...)` whose retail bytes expand CScriptThing::operator= but call ~CScriptThing
  needs a `__forceinline` operator= in the TU; plain/implicit leave ??4 a call (0x00D74430).
- (chunk 744) 0x00D77340 "StoreGuardsHealth" is the PDB's GetGuardsHealth.
