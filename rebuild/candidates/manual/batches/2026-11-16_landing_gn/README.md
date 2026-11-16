# 2026-11-16 landing gn -- acctC hand-back (FullDecomp-B): 58 rows landed (249,619 bytes), reservation released

Owner: "acctC finished what they could and parked what they couldnt. their work is on FullDecompB. bring their work here, fix /
merge any header issues properly with no mistakes. then verify and land the matches and release the parked rows from reservation."

`FullDecomp-B` was merged into `FullDecomp` (3168c7b470; docs and pending folders only, no header or source changes). acctC's
ledger `pending/2026-11-13_acctC/STATUS.tsv` held 162 MATCHED_STAGED rows (161 reserved + the bonus 0x00B9E5B0) and 19 PARKED.
104 of the matched rows were landed earlier (landings fy..gm); the 58 still open are this landing (`payload.json`, `STATUS.tsv`).

## Landed
All 58: RELOCATION_MATCH with the behaviour test PASS, `LANDVERIFY_STRICT_HEADERS=1`, repo headers (`LANDED.tsv`,
`logs/vland_land.log`). They are the round-five / round-six workflow rows, mostly large (0x00E4B5B0 CV_BanditTollScript::CTollBandit::Main
10,594 bytes, 0x00D973D0 9,821, 0x00BA2350 SetupRenderModeShadersAndConstants 9,632, 0x00B783F0 CEngineWaterRenderer::Render 9,343,
0x004A3740 CWorld::UpdateRegionLoad 8,948, ...). Each row's evidence, FITTED spellings and devices (inline budgets, dead release
asserts with their `// Inferred:` notes, owner decision 2026-11-15) are in its source header and the staged folder's README / NOTES.
The payload is the staged folders' `payload.json` entries with true lengths from STATUS.tsv, each row's mangled `// Leaf:`, and
three changes:
- 0x00B6EDF0: `FABLE_RETAIL_CTRIANGLESTRIPIFIER2` -> `FABLE_RETAIL_PASS22_TRIANGLE_ORIGINAL_COMPLETE_DECLARATIONS` (see Headers).
- 0x00A91640: the local C3DClothPrimitive model is replaced by the shared header's new ctor / dtor opt-in (see Headers).
- Catalog names: the agents' PDB names replace placeholder catalog labels, e.g. 0x00A91640 (was `_gapscan::sub_00a91640`),
  0x006B2CA0 CEnvironment::UpdateSunAndMoonPositions (was `_global::CGameCamera_UpdateViewMatrix`), 0x00C79FD0 ResampleStereoMMX
  (was `_global::Audio_ConvolveFilter_WithGain`).

Lander warnings (none blocking): three RELOC_CONFLICTs, each an older landed caller binding the wrong symbol, already queued:
0x00686800 is `??0CTCBase@@QAE@AAVCThing@@@Z` (landed 0x004D2DDB binds the clock-sprite sub-primitive ctor there; 0x00BB1780 is
that ctor), 0x007448F0 is GetSightRadius (landed 0x00864620 binds GetExtendedSightRadius = 0x00744950), 0x004376B0 is
SetMoneyAsDisplaying (landed 0x00775B40 binds SetCrimeAsDisplaying = 0x00437770). 125 RELOC_NAME (catalog labels the queued fixes
correct), 9 RELOC_ALIAS.

## Headers
acctC handed back its header changes as per-folder diffs against the repo of the day (`hdrdiff/*.diff`) plus `NEW_<file>` copies,
48 staged folders in all. They touch 253 existing headers and add 30. Each was merged into the current `rebuild/include`, not
copied over it:

- **Merge.** For each header, every distinct folder version (pins undone) was merged into HEAD in turn with `git merge-file
  --diff3`, base = the tree the folders branched from. Hunks where one side's lines are a subset of the other's take the
  superset; everything else was resolved by hand, by the rules of the earlier landings: two opt-ins declaring the same member
  become `#if defined(A) || defined(B)`; differing bodies for one member become `#ifdef A .. #elif defined(B)`; an opt-in's
  declaration that another opt-in also declares is guarded `#ifndef` the other. No opt-in a landed row uses had its body changed.
- **Clean merges that were wrong.** `git merge-file` reported no conflict yet misplaced blocks in five headers (a duplicated
  ALIGNEDTRANSFORM block and a stray `#endif` in CMatrix4x4.h, lost `#endif`s in CGraphicFrame.h / CGraphicInfo.h, a duplicated
  GETALPHAINCLUDINGDISTANCEFADE block in CEngineInternalPrimitiveBase.h, blocks repeated up to seven times in
  CEnginePrimitiveRenderUtil.h). Found by an `#if` balance check and a directive-duplicate check; each was fixed by hand
  (CEnginePrimitiveRenderUtil.h, CTriangleStripifier2.h, C3DVector.h, CEngineCamera.h, C2DBoxF/I.h, C3DMesh*.h written by hand).
- **Two opt-in name collisions.**
  - `FABLE_RETAIL_CTRIANGLESTRIPIFIER2`: wfE6 wrapped the whole class in it, but HEAD already uses that name (chunk 730, landed
    0x00F3BDD0) for other content. The wrapper is dropped; row 0x00B6EDF0 instead defines the existing
    `FABLE_RETAIL_PASS22_TRIANGLE_ORIGINAL_COMPLETE_DECLARATIONS`, which declares the same class shape (a one-line change in its
    source, marked in a comment).
  - `FABLE_RETAIL_CMATRIX3X4_TRANSFORMVECTOR_SOURCE_DEST`: the landing rows 0x00B01900 / 0x00B03CA0 use it as a declaration-only
    opt-in (they define the body); the parked wfA4 probe used the same name for an inline body. The probe's block is renamed
    `..._SOURCE_DEST_INLINE` (guarded `!defined(..._SOURCE_DEST)`); the open wfA4 draft does not use it.
- **New headers.** Of the 30, 18 are new to the repo; 4 already existed with the same content as HEAD
  (fable_assert.h kept as HEAD's, the wfC1 copy with `FABLE_RETAIL_ASSERT_ARM`); 8 existed with other content and were merged
  (HEAD's PASS22 branch first, acctC's text in an `#else`, or one union body where the texts only differed in comments).
- **Assert pins.** Every moved `FABLE_STATIC_ASSERT` keeps its old number (`FABLE_STATIC_ASSERT_AT`). Headers with two identical
  asserts in exclusive branches need the numbers assigned in order, not by text: a first pass got that wrong in six headers
  (found by the equivalence check: rows 0x008BF500, 0x00929340, 0x00B4AC10, 0x008C12D0, 0x0092DA20 changed assert names) and
  also overwrote acctC's own pins of its new opt-in classes (incl. the deliberate `AT(1000)` of the two new sprite classes). The
  fix aligns each merged header line by line with HEAD and with every folder version: HEAD-origin asserts take HEAD's number,
  folder-origin asserts the folder's (54 pins corrected).
- **Found by the strict-header dry run** (`logs/vland_dry.log`; 54 of 58 matched first time):
  - NPlayerGui_CDrawHighlightingTargetingCursor.h: wfA2's `..._CLEAR` block (Clear / GetPTargetedThing) had been merged into the
    PASS22 class instead of the default class it was written against; the header is now HEAD plus that block where wfA2 put it
    (0x00439070 SRC_FAIL -> RELOCATION_MATCH).
  - CAIStateGroupBase.h: wfD4's `INTERACTWITH && AIACTIONS` declaration of AIAction_InteractWith duplicated the one HEAD already
    has under `FABLE_RETAIL_CAISTATEGROUPBASE_INTERACTWITH` (Codex pass 22; same protected section, same signature). wfD4's copy
    is dropped and its evidence kept on HEAD's (0x0090D030 SRC_FAIL -> RELOCATION_MATCH).
  - 0x00A91640 kept a local `C3DClothPrimitive` model although engine/C3DClothPrimitive.h exists (HEADER_POLICY). Its ctor / dtor
    declarations are now the shared header's `FABLE_RETAIL_C3DCLOTHPRIMITIVE_CTOR` opt-in and the row includes the header
    (RELOCATION_MATCH, same bytes).
  - 0x00BA2350 timed out in cl once under load and matched only in the lander's `"t"` pragma sweep; alone it matches plainly
    (`logs/vland_dry_rerun4.log`).
- Two bare `__forceinline` added by wfE10 in stlport/fable_map.h sit behind `defined(_MSC_VER)` (the pattern CVectorMap.h
  already uses).

Gates: every header balances, no conflict markers; no folder-added code line is missing from the merge (checked line by line,
modulo spelling); lines present more often than in any single source are distinct opt-ins. Whole-tree equivalence
(`lane_merge/equiv_check.py`, exact, HEAD's tree v the final one, 56,402 landed rows): identical input for every row except
0x00CBFB7D and 0x0047042A, whose only difference is the scratch tree's path in `__FILE__` (`logs/equivalence_changed_input_rows.tsv`).
An ordered pin check (each merged header aligned with HEAD and every folder version) finds no assert off its number.

## Reservation released
`pending/2026-11-13_acctC/RESERVED.tsv` is renamed `RELEASED.tsv`. The 19 PARKED rows are back in the normal pool; their latest
drafts, notes and header diffs are in `pending/2026-11-16_acctC_open/` and UNMATCHED_ROWS.tsv points at them. 0x00BA50A0
SetupStaticBlocks (matched inside the landed 0x00BA5260's TU, never staged as its own row) is parked with that lead. The two
reserved fragments 0x00537E70 / 0x00564D5D (re-cut to 0x005379F5 / 0x00564D50, both landed) leave UNMATCHED_ROWS.

## Names
`ghidra_out/labels_landing_2026-11-16gn.tsv` (58 labels, mangled leaf symbols). The staged folders' NAME_FIXES.tsv held 314 fixes:
261 were already queued with the same spelling, 49 are queued as `land1116gn` (0x00435F70 as PRIORITY: it is
CDisplayEngine::Render, the public 5-byte wrapper; it is landed as `CDisplayEngine_DoRender_00435f70` and an older queue line
spells it DoRender, while 0x00435530 now lands as the real DoRender, so until the respelling one name has two addresses), and 4 that name a different symbol than one already queued are held for review in
`NAME_FIXES_CONFLICTS_WITH_QUEUE.tsv` (0x01436E24, 0x009FC3D0, 0x007C2AB0, 0x00DCEC50).

## Files
`payload.json` (as landed), `LANDED.tsv`, `logs/` (dry run, rerun of the four, land, equivalence, ordered pin check),
`merge_tools/` (the merge as run: `seq.py` sequential diff3 merge with `res.json` / `rs/` hand resolutions, `assemble*.py`,
`post/` text patches, `fixpins.py` ordered pin repair, `final_checks.py`, `bookkeep.py`; paths point at the session scratch).
