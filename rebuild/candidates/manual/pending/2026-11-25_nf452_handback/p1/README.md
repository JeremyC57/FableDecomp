# NF452 p1 (land_scripted run dir): the GameCODA volume-panner family (CSalRenderPan / SSE2 / MMX), 22 rows, one unit (00c55270)

STACKED ON d1: lands only after ../d1 has landed. head/payload.json and orig/ carry d1's new text for 00c55270 / 00c550b0 / 00c55110 and d1's
tests for every unit row (= HEAD at p1's landing); every other row's base is HEAD dfaf7c8908 (= 41b0f859dd for these rows). TABLE_CHANGES
lines do not depend on d1's lines (apply_table --dry passes on HEAD alone and on HEAD + d1).

Rows (rows.txt): 00c55270 (owner, re-ported on d1's text) + 8 EH funclets + 00c55070 / 00c550d0 / 00c550b0 / 00c55110 (option S thin rows,
text unchanged from d1) + 00c55090 / 00c550f0 (SIMD ctors, NEW option S thin rows; NF441 NAME_FIXES) + 00c6d9d0 ctor, 00c6da30 dtor,
00c6de60 Params3DInit (option S owner) / 00c6dea0 UpdateFilters (thin), 00c6e020 SetGain, 00c6e410 / 00c6f990 SIMD Process.
RESULTS.tsv: 22 ACCEPT. UNITS kind S: 00c55070 00c550d0 00c550b0 00c55110 00c55090 00c550f0 (owner 00c55270), 00c6dea0 (owner 00c6de60).
Unchanged-symbol rows carry `; target 00c55090`.

Files: rows/, tests/ (22, linked), hdr/engine/ (3 NEW headers: CSalRenderPan.h (size 0x78 asserted), CSalRenderPanSSE2.h, CSalRenderPanMMX.h;
CSalRenderPan.h shares d1's FilterPan through FABLE_GAMECODA_FILTERPAN_DEFINED), HEADERS.tsv, RENAMES.tsv (9 renamed rows; file names
unchanged), CATALOG_CHANGES.tsv (9 Module + 2 CompilerFlags `(none)` -> `/O2 /Oy /W3 /EHsc`), TABLE_CHANGES.tsv (5 replace, 3 add),
SELF_SYMBOLS.tsv, UNITS.tsv, BEST_GUESS.tsv (header only), NAME_FIXES.tsv (0x00C6DA40 / 0x00C6DAD0 = CSalRenderPan::MixFadeIn / MixFadeOut),
head/, orig/, evidence/, tools/.

Gates (base = HEAD + d1 for the unit rows, HEAD for the Pan rows; evidence/):
- strict verify_and_land --recheck-landed on the STACKED landing simulation (HEAD + d1 + p1, LANDVERIFY_STRICT_HEADERS=1): 22 main + 8
  option S (LANDVERIFY_STRICT_LEAF=1) = 30/30 PASS, covering every d1 and p1 row (vland_new_stacked.log). Base: d1's vland_new.log for the
  unit rows; HEAD Pan rows 6/9 (00c55090 / 00c550f0 / 00c6e020 HEADER_POLICY) (vland_head_panrows.log).
- tests 22/22 PASS (tests_p1.txt); mutants 9/9 renamed rows change bytes and FAIL their test (mutants.txt).
- pickany_vs_retail + pk_all: 0 NEQ base and new on every row; reachflip 14 pairs, 0 flips (gateall_p1.txt). pk_all --addrs: ??0 / ??_G /
  ??1 of CSalRenderPanSSE2 (0x00C55090 / 0x00C554F0 / 0x00C55510) and CSalRenderPanMMX (0x00C550F0 / 0x00C55580 / 0x00C555A0), ??_GCSalRenderPan
  0x00C6E1D0, Params3DInit / UpdateFilters: all EQ at their retail homes.
- vt432 (BASE_BATCH=d1: base selfmap / table = HEAD + d1; new = + p1): base 82 agree / 24 WRONG / 6 unknown -> new 158 / 44 / 18. Every new
  WRONG is slot 0 (pre-existing ??_G stand-ins); the base slot 3 WRONG of 0x012BED90 (SetVolume stand-in) is gone; unknowns are 0x00C6E050
  SetPosition (slot 4 of 0x012BED90 / 0x012B8A78 / 0x012B8AB4) and 0x00C6DBA0 Process (slot 2 of 0x012BED90): no rows.
- /FORCE link (base objects = HEAD + d1): NF445 set 36 -> 36 (5 spellings swapped); slots set 45 -> 40 unresolved, duplicates 3 -> 2
  (DUP- ??0C3DAnimationUtil). UND+ only ?Process / ?SetPosition@CSalRenderPan (no rows; the respellings of the UND- CVtable_012bed90 ones).
  The MMX tap mixers of 0x00C6F990 are in `namespace Row00c6f990` (NOTES 4): without it the two MMX Process rows define the same globals.
- name_gate: FAIL lines 86 -> 51 (base = repo HEAD for Pan rows, d1-new for unit rows; name_gate_diff.txt). New FAIL lines only at
  batch-owned addresses (self agree_unverified SDK names; split_unknown resolved by TABLE_CHANGES at 00c55090 / 00c550f0 / 00c6d9d0 / 012bed90
  / 00c6de60 / 00c6dea0; RELOC_NAME catalog labels fixed by RENAMES) plus LOCAL_CLASS CAudioMaster in 0x00C6DE60, moved from the HEAD 0x00C6DEA0
  text of the same option S slice (HEAD debt, as NF447's 0x00C6CEF0).
- apply_table --dry: replace 5, add 3, misses 0, multi-target 1 -> 1 (HEAD alone and HEAD + d1). reserved_check 0 (22 rows; working tree,
  no git). descriptive_check --extra: 0 FAIL.
- includers: owner 00c55270 -> 8 funclets + 6 thin rows, all in the run; owner 00c6de60 -> 00c6dea0 (in the run).
HEAD now ea2cbd8ca8: batch orig/ = working tree for every d1 row and every p1 Pan row; apply_table --dry rerun on it (d1 5/2/2, p1 5/3, misses 0); RENAMES old names still in auto-re-candidates.tsv.
