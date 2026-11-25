# NF452 d1 (land_scripted run dir): the GameCODA plain-gain renderer family (CSalRenderDisable / SSE2 / MMX), 21 rows, one unit (00c55270)

Diff against HEAD 41b0f859dd (NF447 b1 landed); rechecked against dfaf7c8908 (HEAD at hand-back): every row, test and catalog entry of the batch is unchanged there and TABLE_CHANGES still applies (apply_table --dry rerun). Lands first; NF452's Pan batch (if any) is stacked on this batch's text and lands after it.

Rows (rows.txt): 00c55270 + 8 EH funclets 01216180..012161cd (text unchanged) + 00c55070 / 00c550d0 (option S thin rows, text unchanged;
they include the re-ported owner) + 00c550b0 / 00c55110 (SIMD ctors, NEW option S thin rows of 00c55270; NF441 NAME_FIXES) + 00c6e760 ctor,
00c6e780 dtor, 00c6eba0 Params3DInit (option S owner) / 00c6ebe0 UpdateFilters (thin row), 00c6ec50 SetGain, 00c6ec80 SetPosition,
00c6eef0 / 00c6fe10 SIMD Process. RESULTS.tsv: 21 ACCEPT. UNITS kind S: 00c55070 00c550d0 00c550b0 00c55110 (owner 00c55270), 00c6ebe0
(owner 00c6eba0). Unchanged-symbol rows carry `; target 00c550b0`.

Files: rows/, tests/ (21, all linked against the row / unit object), hdr/engine/ (3 NEW headers: CSalRenderDisable.h (size 0x38 asserted),
CSalRenderDisableSSE2.h, CSalRenderDisableMMX.h; no edit to an existing header, so no orig/hdr_*), HEADERS.tsv, RENAMES.tsv (10 renamed rows;
file names unchanged), CATALOG_CHANGES.tsv (10 Module + 2 CompilerFlags `(none)` -> `/O2 /Oy /W3 /EHsc` for the new thin rows),
TABLE_CHANGES.tsv (5 replace, 2 delete, 2 add; every witness is a run row), SELF_SYMBOLS.tsv (address, symbol, status, leaf_given),
UNITS.tsv, BEST_GUESS.tsv (header only: every name has two sources), NAME_FIXES.tsv (2 lines, other rows: 0x00C6E790 / 0x00C6E820 are the
SDK private statics CSalRenderDisable::MixFadeIn / MixFadeOut), head/ (HEAD payload + oracle, 21 rows, source_cpp = HEAD text, checked),
orig/ (HEAD rows + tests), evidence/.

Gates, HEAD v new (evidence/):
- strict verify_and_land --recheck-landed on the landing simulation (LANDVERIFY_STRICT_HEADERS=1): main 16/16 PASS + option S members 5/5
  PASS with LANDVERIFY_STRICT_LEAF=1 (vland_new.log); HEAD 20/21 (00c6ec50 HEADER_POLICY) (vland_head.log). WARN lines are STANDIN_LEAF /
  RELOC_NAME on catalog labels that RENAMES.tsv replaces.
- tests 21/21 PASS (tests_d1.txt). Mutants: 10/10 renamed rows change bytes and FAIL their test (mutants.txt; the thin SIMD ctors by an
  owner-site mutant that builds the other class, as NF447).
- pickany_vs_retail + pk_all: 0 NEQ HEAD and new on every row; reachflip 13 pairs, 0 flips (gateall_d1.txt, pk/). pk_all --addrs: every new
  pick-any copy is EQ at its retail home: ??0CSalRenderDisableSSE2 / MMX 0x00C550B0 / 0x00C55110, ??_G / ??1 CSalRenderDisableSSE2
  0x00C55520 / 0x00C55540, CSalRenderDisableMMX 0x00C555B0 / 0x00C555D0, ??_GCSalRenderDisable 0x00C6ECB0, Params3DInit / UpdateFilters.
- vt432 (VT_SELFMAP = self_symbols + SELF_SYMBOLS): HEAD 26 agree / 12 WRONG / 4 unknown -> new 86 agree / 22 WRONG / 2 unknown (more vftables
  per object now). Every new WRONG is slot 0 (the pre-existing ??_G stand-ins 0x00C6ECB0 / 0x00C55520 / 0x00C555B0 and the PanDelay ones);
  the HEAD slot 3 / 4 WRONG of 0x012BEDB8 (SetVolume / SetParameters stand-ins, NF445 DEBT_SLOTS) are gone; the 2 unknown are slot 2 of
  0x012BEDB8 = 0x00C6E8F0 (CSalRenderDisable::Process, no row).
- /FORCE link (forcelink.py; one object per TU, thin rows skipped): NF445 16-object set 36 -> 36 unresolved (5 spellings swapped); slots set
  (+ the batch rows) 47 -> 42 unresolved, duplicates 4 -> 2 (DUP- ??0C3DAnimationUtil, ?ComputeGainRamp@CVtable_012bedb8: the HEAD
  6eba0 / 6ebe0 pair defined it twice). The only UND+ is ?Process@CSalRenderDisable (0x00C6E8F0, no row), the respelling of the HEAD UND-
  ?Process@CVtable_012bedb8. Control symbol reported on every link.
- name_gate: FAIL lines 83 -> 48 (name_gate_diff.txt). New FAIL lines are only at batch-owned addresses: self agree_unverified (SDK names),
  split_unknown that TABLE_CHANGES resolves (00c550b0, 00c55110, 00c6e760, 012bedb8, 00c6ebe0), RELOC_NAME catalog labels RENAMES fixes.
  Funclets report "row does not match" HEAD and new alike (tool grades the funclet against the parent).
- apply_table --dry: replace 5, delete 2, add 2, misses 0, multi-target 1 -> 1. reserved_check 0 (21 rows; working-tree lists only, no git).
  descriptive_check --extra BEST_GUESS.tsv: 0 FAIL.
- includers: the owner 00c55270 file is included by the 8 funclets, 00c55070, 00c550d0 and the new 00c550b0 / 00c55110: all in the run.
  The owner 00c6eba0 file is included only by 00c6ebe0 (in the run).

Recheck at HEAD ea2cbd8ca8: orig/ = working tree for all 21 rows; apply_table --dry misses 0; RENAMES old names unchanged in the catalog.
