# NF80 notes (round 7, land1117fu: q10763 q10920 q10923 q10931 q10939 q10943)

Slice: NF75's 17 blocked rows + the 4 operator< copy rows (rows.txt, 21), plus 6 rows claimed during the work (0x00686F10,
0x0049124E, 0x006C37D0, 0x004B3CE0, 0x004B4260, 0x00520D00): 27 rows, all `ported` (STATUS.tsv), every one RELOCATION_MATCH or
MATCH with its test PASS (recheck_final.log). No row ended blocked, deferred or not-a-binder. All six entries are done for the slice
(APPLIED.tsv); what remains outside the slice is in NAME_FIXES.tsv.

## Descriptive names (descriptive-by-library: STLport instances are in no PDB)
- 0x00756250 / 0x007AD060 / 0x005E2580 `std::_Rb_tree<CCharString, ..>::insert_unique`, 0x004B9D00 set<CCharString> `_M_lower_bound`,
  0x004DFDBB / 0x00CB68C0 `std::__adjust_heap<..>`, 0x004E1C8A `std::__linear_insert<..>`: STLport 4.6 template instances, named by
  their landed callers' bindings (each spelling was already bound by a PDB-named or ported caller, or is the template instance of a
  catalogued sibling) and by the body shape. Module spellings follow the catalog's existing rows for the same tree / algorithm.
- Kept from earlier rows: the anonymous-namespace `TITLE_NAME` globals (0x013B8B4C) and `CTemporaryNarrowBuffer` in 0x0099B2C0 (a
  row-local helper with no original name found; it models no class that has a header).

## q10763 / q10923: one definition of CBasicString<char>::operator< (0x00429950)
- fable_string.h `FABLE_RETAIL_CBASICSTRING_LESS_BODY` is a template member definition (`template<class T> __declspec(noinline)
  bool CBasicString<T>::operator<(..) const`); the self row explicitly instantiates it and MATCHes. The first version (an explicit
  specialisation, `inline` in includers and an ordinary definition under a `_DEFINE` macro in the self row) graded the same but
  did not link: under /Gy the self row's copy was a nodup COMDAT (selection 1) and every includer's a pick-any one (selection 2),
  so linking 0x00429950 with any includer gave LNK2005. Now every object's copy is selection 2 (`comdat.py` after the change on
  0x00429950, 0x009EFB80 and 0x006C31B0) and a link of those three objects (`work/lk`, `LINK=` cleared) succeeds.
- CTCBase::AllocateTC 0x00686F10 still uses BODY + `_DEFINE` and has the same link problem (Clone's object emits a selection-2
  copy even though it expands every call; `__forceinline` does not change that; 0x00686F10's copy is selection 1; linking the two
  gives LNK2005). It is not a template, so the fix above does not apply, and without `_DEFINE` the self row emits nothing. Left
  for the coordinator (hdr/README.md CTCBase row). NF70's engine/CMap.h GETSTRONGESTENGINETHEMEAT_BODY/_DEFINE is the same
  pattern and likely has the same issue (not tested). Grades are per object, so nothing fails today.
- Strong copies removed: 0x009EFB80 and the three rows outside the slice that defined the same explicit specialisation strongly
  (0x004B3CE0, 0x004B4260, 0x00520D00, claimed). A multi-line grep (`template<>` on its own line, optional inline /
  `__declspec(noinline)` / FABLE_ macro) over rebuild/src, rebuild/include and rebuild/tests finds no other strong definition in
  a row; the only hits are two test files (0x0042AAB3's test defines a strcmp stand-in for the callee in its test TU; 0x00667450's
  test only declares it), which are test stubs.
- Template-member copies removed from 0x006C31B0 / 0x00756350 / 0x007ACC90 (ported onto fable_map.h). Still carrying a row-local
  copy (out of the slice, COMDAT, the same body on a local model; source grep excluding this slice's 27 rows): 29 files define it
  as a template member of a local `CBasicString<T>` and 74 as an in-class noinline body of a local string model (103 distinct
  files). They should take the opt-in as they move onto fable_string.h.
- Callers (nftools/callers.py 00429950): 353 retail call sites in 184 landed rows. 18 are this slice's rows. The other 166 were
  compiled from the repo sources and their relocations to 0x00429950 read (`work/cscan.sh`, `work/cscan.tsv`; all 166
  RELOCATION_MATCH): 121 bind the right spelling `??M?$CBasicString@D@@QBE_NABV0@@Z` (0x0042AAB3 among them, through a row-local
  CBasicString template model), 45 bind a stand-in spelling: 26 `??MCBasicStringChar@@QBE_NABU0@@Z`, 5
  `??MCBasicStringChar@@QBE_NABV0@@Z`, 2 `??MCCharStringData@@QBE_NABV0@@Z`, 1 `??MCBasicStringChar@std@@QBE_NABU01@@Z` and
  11 `?Less@CCharString@@QAE_NPAV1@@Z` (the `_Lbound` / `_Ubound` stand-ins; 0x009AADB0 is map<CCharString,
  CBankFileManager::CBankInfo>'s _M_lower_bound, the rest need their trees identified). All are listed in NAME_FIXES 0x00429950;
  none was claimed (the brief's rows and the q10763 strong copies come first). The earlier "9 `_Lbound` rows" count was a source
  grep and missed 0x006BCEC0 and 0x009EEC10.

## Judgment calls for the coordinator
1. **Pass-through overlay copies.** hdr/fable_retail_banks.h and hdr/fable_game.h are unchanged copies, present only so their
   `#include "fable_string.h"` resolves to the overlay while grading (otherwise MSVC includes both copies: C2011, as the first grades
   of 0x009AC5D0 and 0x00520D00 showed). Nothing to merge from them.
2. **0x0049124E / 0x006C37D0 (q10939 binders).** Only the member declaration `ConvertToCharString` (in no PDB) inside each row's
   pre-existing local CWideString model was respelled `ToCharString`, with its call sites; the models stay (a full port of
   CSubtitleRenderer / CSkeletalMorphResourceManager was out of scope). Same category as NF70's in-place declaration fixes on local
   STLport models; if that counts as respelling a local model of a shared-header class, hold these two.
3. **0x005907AB** uses `PeekTC` (the const lookup, as retail's const LowerBound 0x004365B0 shows) and `IS_THING_ALIVE` with
   `FABLE_RETAIL_IS_THING_ALIVE_NOINLINE` (retail calls the 0x0040DCE0 COMDAT). Its CDeletion record sets `Method =
   UI_DELETE_THIS_INSTANCE` (1). `SetDefaultChild((long)0)` is CList slot 158 (the old stand-in's "SetSelectedIndex"); the grade
   confirms the slot.
4. **0x00835C80** binds CThingGameObject::ConstructFromParams 0x008388D0 under the q10920 spelling through the new inline
   CThingDeadCreature::ConstructFromParams (Ego_r.pdb `?ConstructFromParams@CThingDeadCreature@@IAE_NJABVC3DVector@@JABVCCharString@@@Z`,
   protected; no retail copy). Its caller 0x00663F50 still binds a pointer-form Create (NAME_FIXES).
5. **0x004DFDBB / 0x004E1C8A** gained `// Flags: /Gr` (the retail algorithm TU is /Gr, as the landed 0x00BEEA00 / 0x00808AA0 recipes;
   the old rows spelled `__fastcall` by hand).

## Self renames that move files (RENAMES section 1, 13 lines)
0x00756250, 0x007AD060, 0x005E2580, 0x004B9D00, 0x004DFDBB, 0x00CB68C0 change module/name; 0x00429950, 0x009AC5D0, 0x004B27F0
(QAE -> QBE), 0x004E1C8A, 0x0099B2C0 (file CWideString_ConvertToCharString_0099b2c0.cpp -> ToCharString), 0x009C8060, 0x00835C80 are
leaf-only. No EH funclet includes any changed row (rebuild/src/compiled grep at hand-back).

## Rule slips
- Two Bash calls exited 1: (1) a `grep -c` with no match ended the 0x004B4260 / 0x00520D00 grade loop without `; true`; (2) the
  call that claimed the 0x0049124E / 0x006C37D0 locks ended with a `cat` of the lock owners that returned non-zero. Neither
  changed any file it should not have; later calls end in `; true`.
- The first section-2 pairing of 0x00756250 read a stale object (the old-row probe had overwritten it); regraded (2 grades) and the
  pairing redone. No write outside this directory; no git.
