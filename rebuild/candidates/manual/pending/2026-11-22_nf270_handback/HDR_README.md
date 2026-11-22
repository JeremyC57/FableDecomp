# NF270 overlay headers

## engine/CCreatureActionBase.h (entry 1, row 0x006A0D50)
- Base: the repo working tree copy (orig/engine/CCreatureActionBase.h, byte-equal to rebuild/include at copy time and re-compared after
  the 20:11 landing commit: still equal).
- Added (add-only, retail branch only, inside `#ifdef FABLE_RETAIL_CCREATUREACTIONBASE`, right after `virtual ~CCreatureActionBase();`,
  under `public:`): `#ifdef FABLE_RETAIL_CCREATUREACTIONBASE_COPYCTOR` -> the declaration `CCreatureActionBase(const CCreatureActionBase& other);`
  with a two-line comment. Evidence: Ego_r.pdb 0x0042B6B0 + FableWin.pdb 0x018C2740 ??0CCreatureActionBase@@QAE@ABV0@@Z (public);
  retail body 0x0062EBC0 (landed row, Leaf ??0CCreatureActionBase@@QAE@ABV0@@Z), bound so by 0x0062EB50 / 0x0062EF60 in reloc_bindings.
  Declaration only: no slot, no layout change.
- Default path: token-identical (the new macro is defined by no landed row; only rows/006a0d50.cpp defines it).
- pin_asserts.py: pinned 0 (every assert in the file is already FABLE_STATIC_ASSERT_AT).
- Landed includers: 476 files under rebuild/src/compiled, rebuild/tests, rebuild/include name the header; none defines the new macro.
- `FABLE_RETAIL_CCREATUREACTIONBASE_COPYCTOR` is a new macro name (grep of rebuild/: no hit).
- Whole-tree equiv_check (tools/eq.sh: repo rebuild/include v work/tree_new = the repo tree with this one file replaced; every landed
  source and test): 57,125 rows checked, 0 differ, DONE 0 (evidence/equiv_e1.log).

## engine/CCreatureAction_UnsheatheItemFromInventoryWithHasteAction.h (entry 2, row 0x0069FCD0)
- Base: the repo working tree copy (orig/, byte-equal to rebuild/include, re-compared after the 20:11 landing commit: equal).
- Added (add-only, retail branch only, inside `#ifdef FABLE_RETAIL_CCREATUREACTION_UNSHEATHEHASTE`, after the UNSHEATHEHASTE_CTOR block and
  before `virtual void FinishAction();`, under `public:`): `#ifdef FABLE_RETAIL_CCREATUREACTION_UNSHEATHEHASTE_PERFORMACTION` -> the declaration
  `virtual void PerformAction();` with a two-line comment. Evidence: Ego_r.pdb 0x006C4FB0 + FableWin.pdb 0x02212790
  ?PerformAction@CCreatureAction_UnsheatheItemFromInventoryWithHasteAction@@UAEXXZ (public virtual; both PDBs list it before FinishAction);
  retail vtable 0x0125CDDC slot 3 = 0x0069FCD0. An override of an inherited virtual: no new slot, no layout change.
- pin_asserts.py: pinned 4 (the 5 added lines moved the retail-branch size assert and the three generated-branch asserts; each is now
  FABLE_STATIC_ASSERT_AT(<old line>, ...), so the typedef names are unchanged).
- Default path: token-identical (the new macro is defined by no landed row; only rows/0069fcd0.cpp and rows/0069fcd0_test.cpp define it).
- Landed includers: 0x006A02B0 and 0x008CDD40 (+ engine/INDEX.tsv); neither defines the new macro. Both rechecked on the merged tree:
  RELOCATION_MATCH + PASS (evidence/vl_e2.log).
- `FABLE_RETAIL_CCREATUREACTION_UNSHEATHEHASTE_PERFORMACTION` is a new macro name (grep of rebuild/: no hit).
- Whole-tree equiv_check of both overlay files together (tools/eq.sh, run 2: work/tree_new = the repo tree as of 20:33, after the nf271
  landing, with the two files replaced; every landed source and test): 57,125 rows checked, 0 differ, DONE 0 (evidence/equiv_e2.log). A
  first run 2 was aborted when the nf271 landing changed engine/CEngineLandscapeMemoryManager.h under it (its old tree moved); restarted
  on a rebased tree; the repo did not move during the restarted run.
