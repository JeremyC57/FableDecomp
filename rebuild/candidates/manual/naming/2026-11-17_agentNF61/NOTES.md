# NF61 notes (round 7, slice land1117fu: q10552..q10636, 85 entries, 137 rows)

All counts below come from STATUS.tsv, APPLIED.tsv, recheck_final.log, RENAMES.tsv and NAME_FIXES.tsv in this directory.

## Rows (STATUS.tsv, 137)
- 107 ported, all in one final vland run (recheck_final.log: 6 MATCH + 101 RELOCATION_MATCH, all PASS; overlay hdr/; rerun after the hand-back fixes, the first run is work/recheck_final_run1.log):
  - 43 `_M_create_node` (the protected STLport member, IAE) as explicit instantiations of stlport/fable_map.h (inline-budget
    `/d2inlS20`, or `/d2inlS15` for 0x00579F3E / 0x00579F5F / 0x00CD2C5B / 0x005BBBA5; 0x005B200C `/Gr`);
  - 27 `_M_copy` (private, AAE; overlay opt-in FABLE_RETAIL_STLP_TREE_MCOPY_BODY, `/Oa` size TUs);
  - 6 iterator operator-- / ++ COMDATs;
  - 31 other binders / callers moved onto shared headers: 3 map::operator[], 11 GFSerialiseMapBinaryIn instances
    (engine/CPersistTraits.h), 0x00851CC0, 0x005812AE, 0x00CD36C2, 0x006DC070, 0x0085D010, 0x006DBE00, 0x009CDA70, 0x005786B5,
    0x005785B3, 0x008E6780, 0x0078B410, 0x00694020, 0x00AD91F0, 0x005CDCCB, 0x005907AB, 0x0047E07F, 0x00A7A7F0.
- 28 not-a-binder (reasons in STATUS.tsv).
- 2 blocked:
  - 0x00868CA0 CHitParameters::AddDamageWithAttribute: re-author ready (work/ready/00868ca0.cpp, RELOCATION_MATCH, 5 grades) but
    held: its PDB self spelling would split the landed out-of-slice caller 0x007E2600 (binds `...QAEXJM@Z`). Respell 0x007E2600
    (NAME_FIXES 0x00868CA0) and ship both together.
  - 0x0070A570 CTCTextureDecal::PeekEngineDecals: blocked/port (2 grades; attempt work/attempts/0070a570_try1.cpp DIFFER 815 v 518).
    It models a dozen classes locally (a whole STLport list / vector / map / tree with same-TU helper bodies, CRGBColour,
    CCompositeBlendDesc, CEngineTextureDecal, CTCTextureDecal). Needs first: a CTCTextureDecal header and per-instantiation
    out-of-line control of vector<CCompositeBlendDesc>::push_back / copy (retail calls them, while vector<CEngineTextureDecal>
    ::push_back stays inline) and of map<long,CEngineTextureDecal>::find 0x0070A860 (no landed row).
- Grades per row: at most 9 (STATUS.tsv grades_used).

## Entries (APPLIED.tsv, 85): 79 done, 6 blocked
- q10559: binder 0x00868CA0 held (above).
- q10560: binder 0x006DBE00 done; binder 0x0046190A (GFSerialiseMapBinaryIn<float,EDamageAttribute>) is not in rows.txt.
- q10580: binder 0x0070A570 blocked/port (above).
- q10584 / q10585: binders 0x00A84640 / 0x00A84BE0 (list dtor stand-ins) are not in rows.txt (NAME_FIXES 0x00A84110).
- q10636: the coordinator's (header change).

## Minimal ports (kept local views)
Rows 0x009CDA70, 0x008E6780, 0x0078B410, 0x00694020, 0x005CDCCB, 0x005907AB, 0x0047E07F now use the shared container / CTCHaste
headers for every call this slice is about, but still model other classes locally (vland warns SHARED_HEADER_AVAILABLE on these 7).
Their class moves (CBankFile, CTCInventoryStats, CTCInventory, CTCInGameMenu, CCreatureActionBase, the action classes) are separate
work. 0x005CDCCB's self is still `?FrameUpdate@CTCInventoryStats@NInventory@@QAEXXZ` (PDB: global, virtual; NAME_FIXES). The one
RELOC_CONFLICT warning (0x005907AB: ??0CAction@NUISystem@@QAE@XZ -> 0x0042BED4 here, 0x0042BE50 in 0x0056F615) predates NF61: the
row's local `CAction(); // 0x0042BED4` declaration is line-for-line the old one (work/orig/005907ab.cpp line 62), and the landed
0x0042BE50 defines ??0CAction@NUISystem@@QAE@XZ while 0x0042BED4 is landed as NUISystem::CDeletion::CDeletion; not resolved here.
Other RELOC_NAME warnings on targets NF61 did not rebind (0x0042ABCA, 0x0042BED4, 0x0042CD84, 0x00440C40, 0x005B1B20, 0x006962C0,
0x00696790, 0x013B8A98) come from the same untouched local views or catalog-label formatting. The stats-def weighting call in
0x0078B410 was moved onto engine/CHeroAbilityStatWeight.h (GetParameter 0x00851BD0) at hand-back after the advisor's coverage check.

## Renames (RENAMES.tsv)
- Section 1: 96 self renames (76 tree COMDATs + 20: operator[] x3, GFS x11, 0x00851CC0, 0x005812AE, 0x00CD36C2, 0x006DC070,
  0x006DBE00, 0x0085D010). Each was checked against landed callers (work/splitchk.txt): 66 out-of-slice caller bindings now match,
  39 bound a different spelling before NF61 (37 flagged OTHER, plus 0x00560720 -> 0x005692B0 and 0x006DCFA0 -> 0x006DBE00, which the
  script flagged only because the old Leaf was a prefix: the old rows defined the 3-argument map / pointer-form spellings, not the
  callers' ones); all 39 are in NAME_FIXES; 0 newly split.
- Section 2: 126 old -> new callee bindings in 61 caller rows (old: reloc_bindings.tsv; new: xref of the graded objects).

## NAME_FIXES.tsv: 59 addresses (23 by hand, 36 generated from the callers that bind another spelling of a changed row).

## Tests
No `rows/*_test.cpp` was written: every PASS comes from the repo's behaviour tests (rebuild/tests/...), several of which embed an
older copy of the row source (e.g. 0x005CDCCB), so PASS shows the test still builds and behaves, not that it tests the new text.

## Inline-budget markers
Rows that select a `__declspec(noinline)` / FABLE_FORCEINLINE header opt-in carry an `// Inline-budget:` line: the tree rows
(`/d2inlS*`, FABLE_FORCEINLINE _M_clone_node), 0x0078B410, 0x00469381, and (added at hand-back) 0x005907AB, 0x005CDCCB, 0x0047E07F,
0x00AD91F0. Declaration-only opt-ins (FABLE_RETAIL_STLP_LIST_CLEAR_OUTOFLINE in 0x00A7A7F0) are not inline controls and carry none.

## Checks
- EH funclets: no `rebuild/src/compiled/01/21/*Unwind*` file names a changed row.
- Overlay: 8 files, nothing removed; token check in work/overlay_token_check.txt; see hdr/README.md.
- Locks: released the 30 rows left unchanged (28 not-a-binder + the 2 blocked); 107 changed rows stay locked NF61.
- Rule slips: 6 (work/slips.txt): 5 Bash calls exited non-zero (two read-only checks, one generator KeyError, one shell-parse error
  that ran nothing, one marker-insert IndexError) and one shell-escaped `\&` reached a row (1 wasted grade).
