# Landing 2026-11-03d: 39 inline-depth rows redone pragma-free, CV_AmbushScamScript::Main, chunk 732's three rows

## Result
- **39 rows re-landed pragma-free** (`// Revisit: inline-depth` 129 -> 90). Every one is RELOCATION_MATCH + PASS under `vland --land`
  on the final tree (`logs/land_redo.log`); all 39 were RELOCATION_MATCH before, none was MATCH.
  - Sources: `../2026-11-03_inline_split_research/matched/revisit/` (27 `plain`, 8 `dwall`, 3 `dw`, 1 MFC ASSERT), regenerated
    from the landed sources by `scripts/xform.py` (token-identical to the research sources except the point below) and
    commented by `scripts/comments.py`.
  - None has a `#pragma inline_depth`, a `/d2inl*` flag or a forced destructor; the marker line is gone.
  - Out-of-class `__forceinline X::~X() {}` became `inline X::~X() {}` in 10 rows. The research transform made them non-inline
    definitions; the `inline` form grades the same (`logs/g1_*.log`).
  - The 3 rows that wrap only some exits (0x00E2E520, 0x00E9CB20, 0x00ED9790) carry the line-2 note
    `// Inline-split: exits wrapped individually; retail's construct is unproven`.
  - 14 of their tests had copied the `#pragma inline_depth` lines into the test's body copy; the lines were dropped (behaviour unchanged).
  - 0x00CBABA0 re-landed under a template-derived file name; it was moved back to `std_pushheap_00cbaba0.cpp` (GOTCHAS).
  - No shared header was needed: every forced destructor these rows used is a row-local model. The two `VEC_DTOR` rows already define it empty.
- **0x00E2B090 NScript::CV_AmbushScamScript::Main landed** (RELOCATION_MATCH + PASS, `logs/land_main.log`), from
  `matched/00e2b090_Main_dwall.cpp`: all 23 terminate checks written `do { if (IsActiveThreadTerminating()) return; } while (0);`,
  plain-inline ~CScriptThing. New behaviour test (`scripts/mktest.py`): the row's body over mock engine types, five scenarios
  (good path, evil ending with the quest already active, evil activation, wrong entrance until termination, immediate termination).
  Header `engine/CV_AmbushScamScript.h` installed from chunk 730 (the one model; layout 0x94, nested CAS_* scripts); its only includer is the row.
- **Chunk 732's three rows landed** (RELOCATION_MATCH + PASS, `logs/land_c732.log`): 0x004B05C0 CQuestManager::TransferBoasts,
  0x006AA2E0 CThingPlayerCreature::DrawPreparePrimitivesForRendering, 0x006AABE0 CThingPlayerCreature::SetActionForMelee.
- **Held: none in scope.** 0x006AA720 stays parked (owner call, `pending/2026-11-03_linux_hand732/DECISION_006aa720.md`).
- **Checks:** `bindings_multi_check.py rebuild/oracles/reloc_bindings.tsv` prints 0. Coverage 46,543 genuine (93.90% of the catalog,
  was 46,539); 5,706,947 bytes (55.80%, was 55.74%); landed rows 54,085 -> 54,089.

## Headers (chunk 732 closure, one model per class)
Stage = the `cpp -M` closure of the three rows' sources and tests on chunk 732's grading mirror (rebuild/include + chunk 728 round 2
`hdr/` + chunk 732 `hdr/`), minus files equal to the tree. 3-way merges (`git merge-file`, base = the 728 round 2 copy) took only
chunk 732's own hunks for CGameCameraManager, CPlayer, CTCPhysicsBase, CThingCreatureBase, CThingPlayerCreature and fable_game.h;
728 round 2's hunks that landing c had trimmed stay out.
- **Changed (14):** C3DVector (RotateXY decl; IsNonZero / GetSquaredMagnitude2D statement form; the one-line GetSquaredMagnitude
  stays for 0x00AE0710), CCombatManager (three out-of-line queries), CDefString (ToWideString), CGameCameraManager (typed Camera,
  GetCamera), CPersistTraits (value-init ctor), CPlayer (IsZTargeting), CQuestManager (chunk 729's retail branch + CBoast + boast
  lists), CTCPhysicsBase (GetFacingVector overload order), CTCTargeted, CTCWeapon (retail branches), CThing (forceinline GetTC
  pair, PeekComponent, GetPlayer), CThingCreatureBase (GetNavigationLevelAt, GetCombatManager, protected virtual
  DrawPreparePrimitivesForRendering), CThingPlayerCreature (pack(1) retail branch), fable_game.h (typed PDebugFont /
  PCutSceneMainFont split of the pad, PeekDebugFont only; GetWorld / GetDisplayEngine / PeekCutSceneMainFont /
  GetCurrentServerFrame stay out, they serve parked rows).
- **New (10):** CEnginePrimitive, CEnginePrimitivePrepareInfo, CEnginePrimitiveText, CEnginePrimitiveThemeChannelDesc,
  CTCVolumeContainmentTracker (from chunk 728 round 2), CTCCarrying, NTargeting, NGraphicAppearance_CTCGraphicAppearance (chunk 732),
  CV_AmbushScamScript (chunk 730).
- **Not installed:** CTCCreatureNavigation.h (not in the closure; so its FABLE_RETAIL_CTCBASE StatGetInterfaceType guard is not
  needed yet), CTCCombat.h, NCreatureMode.h, the action headers, CCreatureAbilityDef.h (tree copy identical), 728 round 2's CWorld /
  CTCBase / fable_map / CIEngine hunks (the tree's CIEngine is 731's superset). FableWin's inline
  `CCombatManager::GetProximityForNavigation` was removed from the stage: only the parked 0x006AA720 uses it, and its form is the owner call.
- **CTCPhysicsBase overload swap verified:** retail vtable 0x0125D2C4 slot 69 = 0x006B1430 (`ret 8`, float + hidden result),
  slot 70 = 0x006B1410 (`ret 4`, hidden result only); VC7.1 lays an overload group out in reverse declaration order, so
  `GetFacingVector() const` is declared first. Ego_r.pdb defines the bodies in the same order (0x006EE000 XZ, 0x006EE020 M@Z).
  Every landed FABLE_RETAIL_CTCPHYSICSBASE includer that preprocesses differently was regraded (below).
- hdrcheck2 (`logs/hdrcheck2_732.txt`): every changed header prints `default=same` with a non-empty line count.

## Includer recheck (`includer_recheck_grades.tsv`)
685 landed sources and tests reach a changed header. 646 preprocess token-identically against the final tree (`logs/ppcheck.txt`,
`scripts/includers.py` + `scripts/ppcmp.py`). The other 39 were regraded with `vland --recheck-landed` on the full mirror, which is
identical to the final `rebuild/include` (`diff -rq` empty): 38 RELOCATION_MATCH + 1 MATCH (0x00697AB0), all PASS (`logs/rc*.log`).
They are exactly chunk 732's sibling list minus its 7 CTCCreatureNavigation rows, with the same grades.

## Names (`NAME_DECISIONS.tsv`)
- `ghidra_out/labels_landing_2026-11-03d.tsv` (57 labels: the 43 rows of this landing plus 14 targets), then `apply_labels.py`,
  `sync_catalog_names.py`, `sync_manifest_from_catalog.py --labels` (`logs/`). Net: catalog 0x004B8D70 Transfer<int>; manifest
  0x004B8D70, 0x007EA2F0 NTargeting::GetTargetingPositionOnThing, 0x00E2B090 NScript::CV_AmbushScamScript::Main.
- Chunk 732 NAME_FIXES: 12 of 15 are exact in both PDB name sets. None of their wrong binders is a row this landing re-landed: 8 are
  labelled and queued PRIORITY c732 (one line per address, naming each binder); 0x00415D90 and 0x006EB700 have no other binder and are only labelled. 5 lines are REVIEW: the three int-component spellings (FableWin / RTTI only)
  and two possibly linker-folded bodies (0x00733290 `xor al,al ; ret`, 0x007334B0).
- New finding: 0x00E2BAC0 is AssignMoralityEtcForGoodEnding (both PDBs order Good before Evil; the body reads MoralityGood; Main
  calls it in the talk branch). The landed row spells it AssignMoralityEtcForEvilEnding, outside NScript. Queued PRIORITY
  `land0311d` with 0x00E2BB10 (namespace), 15 queue lines in total.

## Procedure
1. Regenerated the 39 sources, graded read-only on the tree (3 splits), installed them with comments rewritten.
2. Staged chunk 732's closure + CV_AmbushScamScript.h, graded the 4 new rows on the full mirror, ran hdrcheck2, rechecked includers.
3. Installed the stage; `fixmove.py` and `keypayload.py` on the three payloads (0 texts changed). `git checkout` the edited rows,
   `repair_remove.py` the 39, then `vland --land` serially: the redo, chunk 732, Main. Then `git reset` (no staged changes).
4. Labels, queue, dashboard (`write_decomp_dashboard.py`, `update_readme_progress.py`), docs/pipeline/INLINE_DEPTH_REVISIT.md.
