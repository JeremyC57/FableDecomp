# 2026-11-15 landing ga -- the acctC hand-over: 104 matched rows landed (532,905 bytes)

Source: the acctC lane on branch `FullDecomp-B` (fast-forwarded into `FullDecomp` at 808869fa6; that branch only adds pending
folders and the 0x00564D50 boundary row). The lane's ledger `pending/2026-11-13_acctC/STATUS.tsv` listed 104 rows as
`MATCHED_STAGED` in 54 folders; all 104 landed: 100 RELOCATION_MATCH + 4 MATCH, every behaviour test PASS, with the strict
header check on (`LANDVERIFY_STRICT_HEADERS=1`). `LANDED.tsv` lists each row with its true length, flags and staging folder.
Not landed: the 12 rows in the `2026-11-15_acctC_wf*p` folders (rule audit pending) and the 77 parked rows.

Largest rows: 0x00CBFB7D RunCutsceneMacro (74,778), 0x00F0FB70 CQ_ArenaScript::Main (18,144), 0x005D1F0C / 0x005CE0E6
ConstructPersonalityList(PC) (17,873 / 15,910; the pair landing fy held back, now on PDB getters with an `// Inferred:` local),
0x006096E0 ConstructLogbookListPC (12,832), 0x0061E6D0 ConstructQuestListPC (12,356), 0x004804DE OpenInGameMenuPC (11,203).

## How the headers were merged (commit 1dc38c0f2)
The folders carry their header changes as diffs against `rebuild/include` (unchanged on this branch since 55262db83), 647 diffs
and 39 new-file copies over 420 headers. The lane's overlay grew over time, so the diffs overlap: a plain sequential 3-way merge
left 48 conflicts in 28 headers. `merge_tools/`:
1. `index.py` resolves every diff to its include path (three path spellings in the folders; `CSoundSystem.h` is `libpk/`).
2. `merge4.py`: per header, each folder's version = patch(base, diff) with the folder's own assert pins undone; start from the
   version that adds the most lines and 3-way merge the others into it, skipping a version whose additions are all present.
3. `resolve4.py`: the 15 versions that still conflicted, resolved block by block (closing lines first, a block whose opt-in is
   already present skipped, two opt-ins declaring the same members joined as `#if defined(A) || defined(B)`:
   CGraphicDataBank GETPIXELFORMAT / TEXTUREMANAGERACCESS and CTCInventoryMap MAPLIST / CONSTRUCTMAPLIST). One hand edit:
   CEnginePrimitiveManagerParticleSpriteTrailGroup.h, where two folders wrapped the whole class in different opt-ins (_CLASS
   and the plain name), gets the second closing `#endif`. New headers: the copy that contains every other copy
   (`fable_assert.h` = wfC1's, with FABLE_RETAIL_ASSERT_ARM; `CCarrySlotDef.h` = wfE11fix's, with the ALLOC opt-in).
4. `pin_asserts.py`: 4,336 moved asserts pinned automatically and 8 multi-line ones by hand (fable_retail_banks.h, CThingHolySiteDef.h).

Gates: every header balances; `hdrcheck` -- all 387 changed headers preprocess the same with no opt-in and with each existing
opt-in, none loses an opt-in (`logs/hdrcheck_default_path.txt`); whole-tree `equiv_check.py` (exact, assert names included):
56,174 of 56,187 landed rows see identical input; the other 13 differ only by a line split in CEnginePrimitiveOcclusionInfo.h
(`private:long` -> `private:` / `long`, same tokens) and all 13 regrade RELOCATION_MATCH (`logs/equivalence_changed_input_rows.tsv`).
No two opt-ins that declare the same member are enabled together by any row or landed source.

## Lander warnings (none blocking; `logs/vland_land.log`)
- RELOC_CONFLICT `CRenderStateManager::D3DAlphaFuncModeTable` 0x01396DD8 (0x00BC04F0, 0x00A059E0, 0x00B555A0) v 0x01396DE8 in landed
  0x00BBC460: the landed row binds an element address; the table base is 0x01396DD8 (acctC README item 8). Fix the landed row.
- RELOC_CONFLICT `vector<char>(unsigned)` 0x00410F30 here v 0x00411910 in landed 0x009EC890: byte-identical COMDATs; pick one name.
- RELOC_CONFLICT `_$E2` (0x01221D20, 0x0121F86F): a per-TU compiler-local name, benign.
- RELOC_CONFLICT `CCountedPointer<CComponent>::~CCountedPointer` 0x00429C53 v 0x004291DE (0x00601310): the known duplicate-dtor audit.
- STANDIN_LEAF on 0x004736C4: false alarm (the leaf is the manifest's name for that address).
- RELOC_UNCHECKED on 0x00A5CB90 (multi-block TU); 187 RELOC_NAME, mostly manifest labels the queued name fixes correct.

## Names
`ghidra_out/labels_landing_2026-11-15ga.tsv` (820 labels from the folders' LABELS.tsv; where one address had a mangled and a
plain spelling the mangled one is kept); 159 NAME_FIXES rows queued under tag `land1115ga`. The acctC README's item 8 lists
fixes to rows landed earlier (ConvertX / ConvertY 0x0052E470 / 0x0052E4D0, `C2DBoxF::ContainsPoint` returns bool,
`DELTA_NOTIONAL_ZERO` 0x0129BA3C, GPPrimitiveRenderer 0x01436E80 / SharedData 0x01436E78, twin 0x00651B04 `long HUDRingAlpha`).

## Owner review items carried forward (acctC README "Owner review items"; a rejected item takes its row back out)
0x00CBFB7D (36 dead asserts with invented conditions and a dead assert arm in the StartsWith opt-in), 0x00D0A830 (`do { } while (0)`
wrappers and `goto Terminate`), 0x00E50FD0 / 0x00E3BB70 (8 identical dead asserts each), 0x00652FA4 (fitted
`CIntelligentPointer::operator->`), 0x005FF190 (four no-byte loop guards and a force-inline map destructor), the 10 `/d2inlT<N>` rows,
0x00F0FB70 (scoped `#pragma inline_depth(0)`), the asm kernels `GF_SSE_MATRIX_ALIGNED_TRANSFORM_TRANSPOSED_ASSUME_W` and
`LIBPK_MMX_COPY64` (0x00A5CB90; shared-header kernels with FABLE_PORTABLE twins), FABLE_RETAIL_CMATRIX3X4_SETINVERSEUSRT_SCALE_SQUARED.
`fable_assert.h` uses MSVC spellings (`__declspec(dllimport)`, `__debugbreak`) only under FABLE_RETAIL_ASSERT_DEAD_ARM and never
under FABLE_PORTABLE. Two duplicated opt-in pairs (above) can be folded into one name each.
