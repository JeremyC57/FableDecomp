# chunk 799 notes

Six rows: 5 RELOCATION_MATCH, 1 parked (DIFFER, 41 lines). Each source has a full header comment (Identity / Callees / Layout / Shape). This file is the short index.

## 0x00EF0090 NScript::CPersonalScript_EmptyScript::Main (RELOCATION_MATCH, 3 attempts)
- Shape: the quest-Main binding model (CQ_RansomVictimScript::Main 0x00DE0850): 27 AddEntityScriptBinding, one flag bit per binding in ebx,
  spawned CheckForHeroActions 0x00EF5200, then the hero-action loop.
- Key spellings: a loop-head `do { ... } while (0)` with the first check repeated in each arm (retail splits it out); IsAlive and
  ~CScriptThing are inline.
- Header: the opt-in FABLE_RETAIL_CPERSONALSCRIPT_EMPTYSCRIPT_ENTITIES (hdr/README.md) completes the 27 nested classes.
- Callee names: all landed rows or Ego_r.pdb names (see the source header). Nothing descriptive except locals.

## 0x00E8DFA0 NScript::CV_GhostGrannyNecklaceScript::CGhostGrannyNecklace::Main (RELOCATION_MATCH, 4 attempts)
- Shape: the sibling CGhostGrannyDaughterInLaw::Main 0x00E8BEF0 plus the cast-map cutscene of 0x00DC1770. BrainState loops: GS_INTRO, the clue,
  and the farewell wait (its terminate exit is the final `jmp`).
- Key spelling: `__forceinline` CTimer::operator=/GetTime (the long-Main rule). Without it, GS_INTRO is laid out inline and the diff is 1427 lines.

## 0x00D9A190 NScript::CQ_HobbeToothContestScript::Main (RELOCATION_MATCH, 2 attempts)
- Shape: the binding model with 7 entity bindings and WatchForTermination 0x00D9B2F0, then intro/outro cutscenes through the cast map, and the named
  CreateCreature local `whisper` (destroyed last).
- Key spelling: FABLE_RETAIL_CWIDESCREENMAGICPAUSEENTITIES_FORCEINLINE_CTOR.
- Header: new engine/CQ_HobbeToothContestScript.h.

## 0x00DC27C0 NScript::CQ_OpeningGraveyardSecretPassageScript::Main (RELOCATION_MATCH, 2 attempts)
- Shape: the binding model with 14 bindings and threads, a `skeleton` CScriptThing that is constructed and never used (as in retail), and the
  Q_UndeadRising wait.
- Key spelling: `while (Game->IsQuestActive("Q_UndeadRising") != false)` (precedent 0x00D50800).
- Header: repo header, unchanged.
- New label: 0x00DC3D70 CSecretPassageSkeleton::Alloc (Ego_r.pdb name; not landed yet).

## 0x00E13F10 NScript::CQ_WhiteBalverineKnotholeGladeScript::Main (RELOCATION_MATCH, 2 attempts)
- Shape: 6 entity bindings and WatchForTermination 0x00E15500, then the BalverineState stage machine as plain ifs (0/1, then 2..7). The
  locals are `chief` and `village`.
- Key spelling: FABLE_RETAIL_CENTITYSCRIPTBINDING_FORCEINLINE_CTOR (precedent 0x00E479B0). The six nested classes are completed in the TU
  by the FABLE_WBK_ENTITY macro (static Alloc only).
- Header: new engine/CQ_WhiteBalverineKnotholeGladeScript.h.

## 0x00DE2610 NScript::CQ_RansomVictimScript::CVictimsFatherGuard::Main (PARKED, DIFFER(4420v4420), 41 lines, 11 attempts, best a6)
- Shape: the sibling CVictimsBrotherHelper::Main 0x00DEB280 TU: an alive-predicate, take the guard under the parent's seh_Guard +0x6C, then the
  talk/hit loop (ON_HIT_10/20/30) and the follow state machine.
- Key spellings that already match:
  - IsValid/Release through a local `CQ_RansomVictimScript& parent = ParentClass;` (0x00E776F0).
  - The switch with `case 3:` and `default:`.
  - SetEntityAsRegionFollowing(hero, Me, flag).
  - TRetVoid `_Fail_Val`.
- Residue: one site, `SetPredicate(CIsThingAlive(Me))`. Retail expands ~CIsThingAlive one level (vtable 0x012C32F0 held in ebp, so ebp is
  pushed in the prologue) and CALLS the nested ~CScriptThing. The same TU must expand ~CScriptThing at the phase-1 GetScriptThing
  temporaries, which needs `inline ~CScriptThing` visible. With that visible, VC7.1 either calls ~CIsThingAlive whole (a6) or expands both
  levels (__forceinline, implicit dtor, do-while lever, named local: a5/a7/a8/a9/a11).
- The landed precedent 0x00D08B40 gets the split only with `#pragma inline_depth(0)`, which is forbidden. Parked for the owner's ruling or a
  CIsThingAlive header-level spelling.

## Descriptive names (not retail-proven)
Local variables only: `chief`, `village` (0x00E13F10); `skeleton` (0x00DC27C0); `whisper` (0x00D9A190); `conversation`, `FollowState`, `line`
(0x00DE2610). All functions, callees, classes, members and literals are Ego_r.pdb or landed-row names. The shared float 0x0122DEDC (0.0f) has
no name decision yet; it is used through a literal.

## Labels to correct (NAME_FIXES.tsv)
- 0x00E15500: landed as Row00e15500::...::HandleWhiteBalverineQuestObjective in
  global_CScriptGameResourceObjectScriptedThingBaseHandle_00e15500.cpp. It is CQ_WhiteBalverineKnotholeGladeScript::WatchForTermination
  (Ego_r.pdb name, and Main spawns it under the "WatchForTermination" literal).
- 0x00CDBFB0: the manifest sync of 2026-11-03 re-applied CRetailTree_erase00cd344a. It is
  std::map<CCharString,CScriptGameResourceObjectScriptedThingBase>::~map (the cast-map dtor paired with the ctor 0x00CDBF70 in four rows of this chunk).

## Header debt
- CIsThingAlive (for 0x00DE2610): a plain-C++ spelling whose dtor VC7.1 expands one level only. Not found in this chunk; the candidates
  tried are in STATUS.tsv.
- Nothing else; the hdr/ changes are complete (hdr/README.md).
