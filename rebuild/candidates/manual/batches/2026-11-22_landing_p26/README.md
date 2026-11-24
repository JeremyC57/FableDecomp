# 2026-11-22 landing p26: Codex pass 26 hand-back (95 of 100 rows landed)

Owner: "here is codex's work. verify and land the matches." Package `Fable_pass26_verified_checkpoint.zip` (892 files; SHA256SUMS verify).
Codex graded 95 rows VERIFIED, 3 IN_PROGRESS and 2 HELD_PICKANY. All 95 VERIFIED rows land, each RELOCATION_MATCH + PASS.

## Gates
- Header patches: Codex's hdr/PATCHES.json (327) applied in order to HEAD's include tree; 326 as given, 1 re-anchored
  (stlport/fable_containers.h: nf214 had inserted a block between the original anchor lines; same insertion, anchored on the NF213 line).
  153 headers change, 60 are new (`logs/PATCHES_applied.json`).
- Assert names: pin_asserts pinned 21 moved asserts; `assert_names_check` then found 4 mis-pins (two duplicate-expression blocks in
  CNavigatorBase.h pinned to the first block's lines, and CEngineTextureDecal's AT(12) for a line-11 assert), fixed by hand: 0 mismatches.
- Whole-tree equivalence (exact, `logs/equivalence_differing_rows.tsv`): 75 landed rows preprocess differently; all 75 recheck
  MATCH / RELOCATION_MATCH + PASS on the final tree (`logs/recheck_equiv_rows.log`).
- The 95 rows: strict-header dry run against the final tree, then `--land` with no overlay: 95 RELOCATION_MATCH + PASS.
- Purity: no `__asm` / `naked` / `_emit` / `volatile` / `register` in any row, test or patch. Two rows call shared retail-asm helpers and
  were missing the evidence the purity gate needs: 0x00A9DC20 (GFFloatToLongNear, three `fistp` sites) and 0x00417747
  (GFFPUSetDoublePrecision / GFFPUSetRoundingNearest, two fnstcw/fldcw pairs, each checked against the helper's mask) got
  `// Retail-asm-helper:` / `// Retail-asm:` lines citing the retail instructions; both grade `retail_asm`.
- Pick-any (new-row rule, `logs/pickany_vs_retail.txt`): 475 copies equal Fable.exe at the table's address; 7 differ, none
  leaf-referenced, all at stale table lines; each equals retail at Codex's corrected address (queued).
- Strong same-TU copies: 15, all equal retail at their owners' addresses (the nf96a / nf218 TU-mate kind; Codex's fold notes queued).
  Three are functions not landed yet: SetCameraState 0x00B30B50 (in 0x00B31160), FractureShape 0x00763060 (in 0x007634E0) and
  PeekLandscapeHeight 0x00B439C0 (in 0x00B41770; it lands in this pass as its own row).
- Link (`logs/link_check.txt`): control 86 -> 155 LNK4006 with the 95 objects: shared header-inline copies beside strong owners (equal
  bytes where the table names an owner), the strong TU-mates above, and a few ownerless copies (e.g. `?floor@@YAMM@Z`). Debt, queued.
- Owners rechecked (`logs/recheck_owners.log`): the 229 landed owners of the rows' copies, all pass.
- reloc_bindings: the lander recorded 137 first witnesses; 51 that would add a second spelling at an address are removed and queued
  (`logs/reloc_bindings_held_split_growth.tsv`); 86 stay.
- One RELOC_CONFLICT warning: 0x007DAC10 binds `_mult_to_stay_off_table` at 0x01383C94 (FableWin CodeView file-static), landed
  0x007DAEF0 at 0x01383C98 (DealCard's own local static): 0x007DAEF0's spelling is queued.

## Names
- Catalog names come from each row's verified leaf where the manifest / parked label was a placeholder or a mangled string
  (`logs/catalog_names_from_leaf.tsv`, 44 rows); 39 of those leaves are in a PDB dump verbatim, the four audio rows (0x00C4DE00,
  0x00C74850, 0x00C76AB0, 0x00C76DD0) are named by the GameCODA 2.10 debug and release libraries. 0x006C1ED0 is STLport
  `std::copy_if<...>` (the parked label was Ego_d's VC10 `_Copy_if`).
- Codex's 125 NAME_FIXES are queued under land1122p26 (CODEXP26) with no respells in this landing.

## FractureShape (0x00763060)
0x007634E0's TU carries a FractureShape whose VC7.1 copy equals retail (1146 B). AUTH8 measured that text with VC10 (odall10 --stl):
Ego_d-exact on lines 195-241 and on all 23 named locals; its four whole-function gaps (280, 283, debug-only 288 / 291) are the same as
g1's. So Codex's text is a second Ego_d-exact decode, and unlike g1 it also matches retail: the n4 owner question is withdrawn and
0x00763060 is queued as its own row (needs a test).

## Not landed
0x00BAA870, 0x005F59C7, 0x00723E50 (IN_PROGRESS) and 0x00EB5750, 0x009F3DA0 (HELD_PICKANY) are parked with Codex's files in
`../../pending/2026-11-22_codex_pass26_open/<addr>/`.
