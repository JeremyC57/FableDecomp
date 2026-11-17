# 2026-11-17 assert pin audit: 457 FABLE_STATIC_ASSERTs moved by naming landings, pinned and rechecked

## What was wrong
`FABLE_STATIC_ASSERT` names its typedef `fable_static_assert_<__LINE__>`, and VC7.1's stack-slot order in some rows depends on those
names (landing fz, row 0x007E2A60; GOTCHAS). A header edit that moves an assert must pin it with `lane_merge/pin_asserts.py`
(`FABLE_STATIC_ASSERT_AT(<old line>, ...)`). The naming landings since NF33 copied agents' header overlays in whole and never checked
this: opt-ins added above an assert moved it. Between 49b943df65 (after landing p23, the last header merge proven over the whole
tree) and the nf70 landing, **457 asserts in 70 headers** (`MOVED_ASSERTS.txt`) no longer had the name they were proven with.

## What was done
1. **Pinned** (606d91b647): every moved assert respelled `FABLE_STATIC_ASSERT_AT(<its line at 49b943df65>, ...)`. One assert had
   been pinned during the nf70 landing to the line it then sat on, not its 49b943df65 line (CMap's sizeof: 196 instead of 171);
   pin_asserts skips asserts that are already pinned, so it was fixed by hand (6b02157ee8). `lane_merge/assert_names_check.py
   <base include> <new include>` now compares every assert's effective name between two trees: 0 mismatches against 49b943df65.
2. **Equivalence** (`lane_merge/equiv_check.py`, all 56,564 rows, exact text, assert names included):
   - tree before pinning vs pinned: 5,984 rows differ; tree at 49b943df65 vs pinned: 560 rows differ.
   - Rows unchanged since 49b943df65 whose text is back to their 49b943df65 text need no recheck: that is the text they were proven with.
   - **Rechecked (`RECHECK_ROWS.txt`, 477):** the 462 rows changed since 49b943df65 (round-7 landings) whose text changes with the
     pins, plus 15 unchanged rows whose text differs from 49b943df65 for another reason.
   - **Rechecked after the CMap fix (`RECHECK_ROWS_CMAP.txt`, 31 + 0x008F70F0):** the round-7 rows whose text changes with it (13
     rows, 18 EH funclets graded with `/EHsc`).
3. **Recheck (`logs/recheck_part0..2.log`, `logs/recheck_cmap_part0..1.log`):** 476 of 477 and 31 of 31 match (RELOCATION_MATCH or
   MATCH), every test PASS.

## The one row that did not grade
0x008F70F0 `CMap::AddThemeDefIndexToPalette` reported DIFFER(297v164) because the payload builder passed its `// Leaf:` line
`?AddThemeDefIndexToPalette@CMap@@IAEEK@Z` (protected, as both PDBs and its caller 0x0081EF30 spell it), while the row's object
defines the public `...QAEEK@Z`: engine/CMap.h's `FABLE_RETAIL_CMAP_THEMEPALETTE` declares the member public. With the leaf not
found the grader measured another function of the TU (ZeroAllUnusedThemePaletteEntries, 297 bytes). Measured by name the row is
RELOCATION_MATCH + PASS with today's headers (`logs/recheck_008f70f0_by_name.log`), and its preprocessed text is identical to its
landing day's (98297c04ee) apart from the assert names. This is an older naming defect, not an assert effect; queued
(`land1117fu` 0x008F70F0: make the member protected in CMap.h and regrade the row and its callers).

## Prevention
`workflows/land_naming_prep.sh` prints `ASSERT MOVED <header>` for every overlay that moves an assert; landings pin with
`lane_merge/pin_asserts.py rebuild/include <overlay> <header>` before copying it in, then check with `assert_names_check.py`.
