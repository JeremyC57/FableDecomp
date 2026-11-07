# Chunk 841 notes

Six script Mains. Every name is PDB-proven: each is spelled the same in the Ego_r.pdb and FableWin.pdb name sets, and each is confirmed by
its single .rdata reference, which sits in slot 1 of an entity vtable (slot 2 of a script vtable for the expression) next to the
class's already-landed labels. Each Leaf symbol was confirmed with `objdump -t`. The coordinator relabelled all six in
`labels_landing_2026-11-07n.tsv`, but `$S/batch/chunk841_oracle.tsv` still carries the old placeholders, so vland shows
WARN STANDIN_LEAF for 0x00E71F10.

Result: 1 RELOCATION_MATCH and 5 parked. All five parked rows hit one of the two residues the brief says to recognise and park
(per-exit ~vector / ~CTimer splits, and the CIsThingAlive / CIsThingConscious copy-inline case). Each was parked only after trying
spellings that are not in GOTCHAS (listed per row). Every test prints `<addr>_TEST PASS` under g++ and under VC7.1. No row uses a
/d2 flag, a pragma or asm.

## 0x00E71F10 NScript::CV_DemonDoorsScript::CDF_DressAsBandit::Main: RELOCATION_MATCH 1605v1605 (src/)
- Shape: CIsThingAlive predicate, then StartScriptingEntity retry, then a talk loop. Each talk runs under a paused movie. If all five
  bandit clothes are worn, a second paused movie wraps the OPENING line, then FadeOutFaceAndOpenDoor and SetRegionExitAsActive;
  otherwise the INTRO line plays.
- Key spellings:
  - `FABLE_RETAIL_CWIDESCREENMAGICPAUSEENTITIES_FORCEINLINE_CTOR`, so both pause objects expand.
  - The movie dtor is OUTOFLINE.
  - `break;` (not `return;`) after SetRegionExitAsActive in the dressed arm. That was c7, the match.
- Every relocation binds the retail symbol, and the binding check found no conflicts in reloc_bindings.

## 0x00EE3140 NScript::CV_TravellingHeroesScript::CTH_Whisper::Main: parked DIFFER(1350v1366) md115
- Shape: CIsThingAlive predicate. A gameflow > 2100 early-out drops the rumour and removes Whisper. Otherwise a setup run, then
  StartScripting through the parent's seh_Rival (chunk 808 parent-reference model), then the hit / talk / near-chatter loop.
- Callees:
  - `ParentClass.PlayCutscene` 0x00EE2DB0 (overlay opt-in; PDB name set).
  - GFIntToCharString 0x0099F830.
  - `operator+(const char*, const CCharString&)` 0x0099F690.
- Residue (Blocker 1, copy case): retail expands the by-value CScriptThing copy for SetIsPushableByHero(Me, false), while the
  predicate copy calls 0x004ABE90.
- Novel attempt: an explicit `CScriptThing(Me)` temporary. It changed nothing.

## 0x00DA8BF0 NScript::CQ_MinionClifftopChaseScript::CMinionSpawnAdv::Main: parked DIFFER(1546v1447) md84
- Shape: wait for the hero to be in range and the count to be under the limit. Then two marker loops (MinionHere gives WARDOG,
  MinionSummonerHere gives DREADWINGS), each step being effect, Pause(0.1f), CreateCreature. Then RemoveThing(Me).
- Callees:
  - `spawners.clear()` is the out-of-line erase 0x008AC9D0 (`FABLE_RETAIL_STLP_VECTOR_ERASE_RANGE_OUTOFLINE`).
  - ~vector 0x008AC970.
  - free 0x00BFEA14 is the STLport allocator.
- Residue (the ~vector<CScriptThing> per-exit split):
  - The post-loop-1 thread check expands the vector dtor, where retail cross-jumps it into the in-loop pad that calls 0x008AC970.
  - The diagnostic flags /d2inlT60 and /d2inlT66 give the right split (md79). So this is the function-wide inline budget, and the
    row is a park.
- Novel attempts:
  - b4: loop 1 as a while-loop. Every exit became cold, giving 1302 bytes.
  - b6: `continue;`.
  - b7: one shared loop index.

## 0x00DDD430 NScript::CQ_PrisonRaceScript::CPrisonRaceStartingGuard::Main: parked DIFFER(2112v2044) md102
- Shape: StartScripting; then the cast map (HERO / GUARD) and the intro cut-scene under a paused fixed-camera movie; then a CTimer
  hit loop that walks home.
- Callees:
  - `std::map<CCharString, STB>::operator[]` 0x00CD3D2E and ~map 0x00CDBFB0.
  - RunCutsceneMacro 0x00CBFB7D.
  - malloc 0x00BFEA0E is the STLport map node allocator.
  - IsDistanceFromThingToPositionOver 0x00CBE45C (`?IsDistanceFromThingToPositionOver@@YI_NABVCScriptThing@@ABVC3DVector@@M@Z`;
    Ego_r and FableWin agree, and it is bound only once).
- Residue (the ~CTimer per-exit split): the inline ~CTimer keeps TimerIndex in ebp as retail does, but it expands DeregisterTimer
  at three in-loop exits where retail calls 0x00CD4470 at one shared pad.
- Novel attempt (e6): the inline ~CTimer defined after Main.
- Uses the single-site do-while(0) Inline-split lever (chunk 833), with a comment.

## 0x00EE4DD0 NScript::CV_TourGuideScript::CTourGuideFollower::Main: parked DIFFER(1925v1946) md91
- Shape: StartScripting, then FollowThing on the guide. In the chunk 813 `for(;;NSF)` loop, answer the hero's talk with a random
  M/F follower line once the stand-still timer reaches 0. When the guide is killed or the tour finishes: clear information,
  retake control, walk to M_TG_ClosingTimeExit and FadeOutAndKillEntity, or idle forever if there is no exit.
- Uses the gotcha 2143 CTimer set (`__forceinline` operator= and GetTime).
- Residue (the ~CTimer per-exit split): the closing block's IsPerformingScriptTask wait exit expands DeregisterTimer, where retail
  calls 0x00CD4470.
- Novel attempts:
  - f4: break to a post-loop block.
  - f5: the wait loop as for(;;)/break.
  - f6: the retry loop as for(;;)/break.

## 0x00EEAEF0 NScript::CExpression_PickpocketScript::Main: parked DIFFER(1860v1906) md101
- Shape: a CIsThingConscious(hero) predicate and the quest set persistent. Quit if the target cannot be pickpocketed or a movie is
  running. Otherwise: movie and pause objects, the mini-game bar, a frame-count loop (button release, hit, spotted roll), then the
  outcome (spotted event or steal roll) and DeactivateQuestLater.
- Key spellings:
  - `!(progress >= 1.0f)`.
  - Def member on the left of the FPS product.
  - `long roll = rand() % 100;`.
- Residue (Blocker 1, dtor variant):
  - Retail expands ~CIsThingConscious but calls its nested ~CScriptThing, and it expands the loop temporary's ~CScriptThing.
  - VC7.1 instead calls the whole of ~CIsThingConscious 0x00EF7AB0.
- Novel attempts:
  - `__forceinline` dtor (md376).
  - Named predicate block.
  - Out-of-line ~CScriptThing (h6, md112).

## Descriptive names (not proven; the values are retail's)
- 0x00EEAEF0:
  - `EHeroTrainableStatType HERO_STAT_SKILL = 5`.
  - `EMiniGameType MINI_GAME_PICKPOCKET = 3`.
  - `ECutsceneBehaviour CUTSCENE_BEHAVIOUR_NONE / _FREEZE`.
  - The `(NEntityEvents::EEventType)0x13` cast. Its enumerator name is not proven.
  - Locals (`spotted_timer`, `count`, `progress`, and so on).
- 0x00EE4DD0: `ESex SEX_MALE / SEX_FEMALE` (values 1 and 2). `EScriptEntityMoveType` comes from earlier rows.
- `EScriptAIPriority`, `EHeroAbility HERO_ABILITY_HEAL_LIFE_SPELL = 14` and `ETextGroupSelectionMethod` come from landed rows.
- Local variable names everywhere.

## One symbol, one address
- 0x00CBE716 FadeOutAndKillEntity: the PDB gives `(const CScriptThing&, float, bool, bool)`. Thirteen landed rows declare
  `(…, bool, float, bool)`. The fastcall ABI is the same, so they byte-match, but the mangling is wrong. See NAME_FIXES.tsv. My row
  0x00EE4DD0 uses the correct spelling from fable_script_fade.h.
- 0x00EF7AB0 ~CIsThingConscious: bound under five spellings. The correct one is `??1CIsThingConscious@NScript@@UAE@XZ`. See
  NAME_FIXES.tsv and LABELS.tsv.
- Several older shared callees already carry multiple legacy spellings in reloc_bindings.tsv: CBaseClass, CCharString, the vector
  members, pDSTGame / g_pGameScriptInterface 0x0143E8F8, and pGlobalScriptDef. All of these predate this chunk. My rows use the
  spellings the shared headers emit.

## Header debt
- The 837/841 PrisonRace forward-declaration clash: merge the two opt-ins' forward declarations at landing (hdr/README.md).
