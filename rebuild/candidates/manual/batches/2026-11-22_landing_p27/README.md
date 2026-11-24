# 2026-11-22 landing p27: Codex pass 27 hand-back (100 of 100 rows landed)

Owner: "here is codex 100 rows done." Package `FableDecomp_pass27_Claude_handoff.zip`: 99 VERIFIED + 1 LANDABLE_WITH_PREEXISTING_BINDING_DEBT
(0x00ACD0E0). All 100 land: 1 MATCH + 99 RELOCATION_MATCH, every test PASS (`logs/vland_land.log`).

## Gates
- Header patches: Codex's hdr/PATCHES.json (354; 44 new files) applied in order to HEAD's include tree, all as given
  (`logs/PATCHES_applied.json`). assert_names_check: 0 mismatches.
- Strict-header dry run on the patched tree: 1 DIFFER, 1 MATCH, 98 RELOCATION_MATCH. The DIFFER was mine: 0x00C95580
  (vorbis_book_init_decode) lost its `// Flags: /TC /MD /Gr /Ob1` in my MATCHES.tsv; with them it is RELOCATION_MATCH (910 B).
- Whole-tree equivalence (`logs/equivalence_differing_rows.tsv`): 77 landed rows preprocess differently. Recheck of those plus the swap
  rows below (143, `logs/recheck_143_first_pass.log`) found 14 regressions, all caused by two unconditional edits in Codex's patches.
  HEAD control (`logs/head_control_6.log`): the same rows pass on HEAD's headers.
  - `CBankFile.h` patch 56 added `FABLE_RETAIL_ASSERT_ARM(index != 0);` to the shared GetEntryDataType for every includer: 12 CBankFile /
    CHeightMap rows failed to compile (no fable_assert.h). Removed: no row needs it (`logs/cbankfile_assert_removed_regrade.log`: the 5 p27
    CBankFile users, 0x009CEC60 included, and the 4 first regressions all RELOCATION_MATCH without it).
  - `CTCPerceiveThing.h` made IsAwareOfThing declaration-only for every includer: 0x0091B3A0 DIFFER(453v455), 0x009578A0 DIFFER(131v148).
    Now declaration-only only under 0x0095EF80's FABLE_RETAIL_OPINIONREACTIONROOT_ISRUNNABLE_PASS27 (the row defines it inline under its
    size pragma); every other includer keeps the in-class body.
  After both fixes all 16 failing / touched rows pass (`logs/recheck_16_after_header_fixes.log`); the other 127 passed first time.
- Purity: no `__asm` / `naked` / `_emit` / `volatile` / `register` in any row, test or patch. Two `_MSC_VER`-guarded `__forceinline`s
  in stlport/fable_containers.h (FABLE_RETAIL_STLP_VECTOR_ULONG_ERASE_COPY_PASS27, used by 0x00A90AE0 only) are an inline-budget
  device under the 2026-11-08 decision: ablation run here (plain inline: the in-TU vector<unsigned long>::erase copy is 34 B, unequal to
  retail's 52-B owner 0x0046496F; /d2inlT100, /d2inlT300, /Ob2 keep that; /d2inlS50/S150/S300 fix erase but break clear, resize and
  ~CVertexGroupBlock), so the header device is the smallest that matches. 0x00A90AE0 got the `// Inline-budget:` line (comment only,
  regraded RELOCATION_MATCH + PASS). 16 other rows carry inline devices without that line (`logs/inline_devices_without_marker.txt`,
  as 7 p26 rows did): queued for Codex to add markers + per-device ablation.
- Pick-any (`logs/pickany_vs_retail.txt`): the MovieBase ??_G copies in 0x00CFCE90 (46 B) and 0x00F00C90 (30 B) are byte-identical
  to copies landed script rows already emit (debt, not a hold); retail's 87-B scalar comes from landed 0x00E17840.
- Link (`logs/link_check.txt`): 7 new duplicate-symbol lines, all with copies equal to retail:
  - 4 CTCQuickAccessMenu members: pair debt. 0x00628000 and 0x00627BE0 are two grading views of one retail TU, the NullString / PClass
    precedent; Codex says build 00628000.cpp once.
  - IsStillRunnable (0x0094FDF0): defined in both JackDragon rows, which share retail's TU with the landed owner. Same-TU debt.
  - GetSubDef<CTavernGameShoveHaPennyDef> (0x007D1260): 0x007D04F0's implicit instance against 0x007CF9A0's explicit specialization.
    Landed 0x007CF860 and 0x007CFF40 already do the same.
- reloc_bindings: the lander recorded 223 lines; 54 that would add a second spelling at an address are removed and queued
  (`logs/reloc_bindings_held_split_growth.tsv`); 169 stay.

## IsEventButtonHeld / IsEventButtonReleased swap (0x00687B30 / 0x00687BD0)
0x00688360 bound Held -> 0x00687B30 and Released -> 0x00687BD0, the reverse of the landed table (two RELOC_CONFLICTs). Codex is right:
0x00687B30 tests exactly the Ego_r.pdb EInputEvent IE_*_HELD codes (2, 5, 8, 11, 20, 27..31) and 0x00687BD0 the IE_*_RELEASED codes
(3, 6, 9, 12, 21, 32..36). FableWin.pdb (0x021D3E10 Held, 0x021D3F00 Released) and Ego_r.pdb (0x006B2C40, 0x006B2CE0) have the same order.
Chunk 829 had already found this and queued it (land0711e). Applied as a byte-neutral rename across 70 landed sources, tests and headers,
the two self rows (files renamed), the catalog, functions.tsv and the 3 reloc_bindings lines. Comments quoting the old manifest label are
kept as written. `ghidra_out/labels_ieb_swap_2026-11-22.tsv` holds the labels. All rows that use the names, and every includer of
CInputProcessGameBase.h, are in the 143-row recheck: all pass.

## Names
Codex's NAME_FIXES (174 lines) are queued under land1122p27 (CODEXP27), except the two IsEventButton lines applied above.

## Debt queued
0x00ACD0E0: the ??1 _Rb_tree<unsigned long> binding from 0x0051C920 to 0x00517850 uses signed erase and is wrong. Its unique correct
address is not proven, so the row keeps the unresolved name (`codex/rows/00acd0e0/LANDING_REVIEW.json`).
