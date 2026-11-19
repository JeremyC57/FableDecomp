# NF104 notes (2026-11-19)

Slice: 40 PRIORITY "self leaf differs" entries. Result: 24 done, 2 done-already, 2 rejected, 2 blocked (q4413 1 row; q4194 family),
10 open (not started: q4258 q4452 q4453 q4524 q4645 q4769 q5080 q5205 q6212 q6236). 42 rows ported (recheck_final.log: 8 MATCH +
34 RELOCATION_MATCH, all test PASS), 23 not-a-binder (rows.txt rows probed as already right or belonging to done / rejected entries),
5 blocked: 1 attempted (008ed590, q4413) and 4 untouched rows of blocked entries (00497890 q4413 self; 004046b0, 00998a30, 005dca60
of the q4194 family). The 38 untouched rows.txt rows of the 10 open entries have no STATUS line (selfcheck accepts that; the list is
work/status_dropped.txt, and work/STATUS.with_untouched.tsv has the version that listed them as blocked).

## Claims and evidence (two sources each; details in RENAMES.tsv evidence and APPLIED.tsv)
- Several queue lines carry the real claim in the evidence column, not the correct_symbol column (q6235, q6239, q6429, q6543, q6539):
  read both. q6539: the correct_symbol column says PAV, the evidence (and 17 ghidra_out PDB hits) says AAV (reference) -- AAV applied.
- q4777 rejected: 0x00429B72 is `e9 e9f5ffff` = jmp 0x00429160, the 5-byte destructor COMDAT; 0x00429160 is ReleaseData (NAME_FIXES).
- q4671 rejected: superseded by the 2026-11-19 engine/CDefString.h re-pointing.
- q4194 blocked: family (63 rows, 47 `~list` vs 16 `~_List_base`): the PDBs are Dinkumware, they cannot settle an STLport COMDAT that
  ~90 byte-identical destructors fold onto. Needs a coordinator decision on direction; both row lists are in APPLIED.tsv.
- q4413 blocked: 008ed590 sits on rebuild/include/candidates/object_family_compiled.h (GFROR13 declared YIKK at line 21) and grades
  DIFFER unchanged in this lane; NAME_FIXES carries ?GFROR13@@YIJK@Z.

- NAME_FIXES new finding: 0x00411010 is the member template ??$Transfer@VCCharString@@@CPersistContext@@QAEXPBDAAVCCharString@@@Z
  (18 PDB hits; 0x006E0730 and 0x00681BA0 bind it); its self row (ported here for q4610 binder work only) still defines the non-template.

## Descriptive names (marked as required)
- Descriptive-by-library (STLport 4.6 bodies, spelled from the instantiation, PDB-confirmed where a PDB name exists):
  0x0042B707 list<NUISystem::CList*>::size, 0x0042B646 list<NUISystem::CActionParam*>::push_back, 0x004861DF
  vector<unsigned char>::resize(size_type), and the NAME_FIXES entry 0x0042AADC list<CActionParam*>::insert (no PDB name set lists
  that instantiation; the 0x0042B687 / 0x0042B6B8 twins call the matching insert).
- No made-up names were introduced.

## Forms used
- Pick-any scaffolding (`// Fidelity: scaffolding, not retail`, anonymous-namespace function under `#pragma inline_depth(0)`): 0x0041BEB0
  (C2DBoxF/CEnginePrimitive2DSprite header body) and 0x004371A0 (engine/C2DBoxF.h inline corners ctor, FABLE_RETAIL_C2DBOXF_CORNERS).
  0x004371A0 was first a strong definition (MATCH); redone in the scaffolding form after review because the header has the inline body.
- Same-TU strong callee copy (`// Fidelity: same-TU strong definition of 0x... (retail TU-mate)`): 0x009AD410 (CCRC::Calc).
- Explicit instantiation over stlport/fable_containers.h: 0x0042B707, 0x0042B646, 0x004861DF, 0x00410890 / 0x004AB840 (GetDef<T>).
- In-place respelling inside pre-existing local models (no new local model of a class with a shared header): 00411010 007e36a0
  007ef070 00681ba0 (member template Transfer), 00479369 00685f80 (reference), 004e448d 004e5d08, 00431f10, 0065595c, 0043b050, 0082be60.
- Local stand-ins removed in favour of shared headers: 00555cf0 (Stack -> engine/NUISystem_CList.h CList::GPreviousSelected),
  009e95f0 (CArray -> engine/CArray.h), 005cd074 (CActionParamList -> std::list), 004793f8 (CDataInputStream -> engine/CMemoryDataInputStream.h),
  00487fb0 (CPlayerManagerCalls -> engine/CPlayerManager.h), 00432c70 00432e60 00434450 004345a0 (CDefPointeeBase on engine/CResource.h).

## Tests
- Tests that link the row were respelled with it: 004793f8_test (now on the shared stream header; raw storage, fields by retail
  offset, no constructor), 004861df_test (rewritten: it linked the old stand-in; now exercises the real resize and supplies STLport's
  _M_fill_insert by explicit instantiation -- the first vland run was LINK_FAIL on that symbol), 00685f80_test (defines the callee),
  0059a238/005958f5/007132c0/009ad410/00487fb0 tests (earlier batch).
- Self-contained tests that do not link the row (a frozen copy of the body) were left unchanged: 00555cf0, 00411010, 007e36a0, 00681ba0,
  0042b646, 005cd074, 009e95f0. 00479369_test was respelled anyway for consistency (pointer -> reference).

## Header overlay
- hdr/engine/CDrawMiniMap.h, opt-in FABLE_RETAIL_NPLAYERGUI_CDRAWMINIMAP_UPDATEMARKERS (declaration only), one assert pinned by
  pin_asserts.py; see hdr/README.md. No other header changed.

## EH funclets
- Checked rebuild/src/compiled/01/21/*Unwind* and every other landed .cpp under rebuild/src and rebuild/tests for the file name of each
  of the 42 changed rows (re-run after the last row): no funclet includes any of them, so no funclet regrade and no moved parent
  (self renames 0x0042B646, 0x004861DF, 0x004793F8 included). One non-include mention: a comment in
  00/40/CUserProfileManager_SetDefaultValuesVideo_004093e0.cpp names the 0x00410890 file and its old GetDef_00410890 spelling (stale
  comment only; no binding, nothing to regrade).

## Warnings seen in vland (pre-existing, not introduced here)
- RELOC_NAME on callees outside the slice (0x00643BEF ToScreenBox in 0082be60, 0x007E39D0 in 007e36a0, 0x0099F180 in 00681ba0,
  0x006815F0 in 00479369, 0x0099AE50 / 0x0099E4C0 in 009e95f0) and SHARED_HEADER_AVAILABLE on pre-existing local models; the
  RELOC_NAME on 0x00643E9A (004378c0) and 0x0042B707 (00555cf0) are fixed by NAME_FIXES / this slice's RENAMES section 1.
- STANDIN_LEAF on 0041beb0 / 00681ba0: the catalog names the old stand-in; RENAMES section 1 (0041beb0) carries the fix; 00681ba0's own
  leaf is outside this slice.

## Rule slips (self-reported)
- 0x004371A0 first landed in this workspace as a strong definition although engine/C2DBoxF.h has the inline corners body; redone as
  scaffolding (both MATCH).
- The CDrawMiniMap.h overlay edit first moved the class-block FABLE_STATIC_ASSERT's line; fixed with pin_asserts.py (pinned 1) and
  0x004378C0 regraded after.
- The EH-funclet / includer check was first run on 30 rows only; re-run over all 42 before hand-back (no hits).
- First vland of 004861df: LINK_FAIL (the rewritten test did not supply _M_fill_insert); fixed in the test, PASS.
- RENAMES section 1 for 0x0042B646 / 0x004861DF first carried the stand-in names as old_catalog_name; corrected to the catalog names
  (catalog already right) after comparing all 17 section-1 rows with callers.py.
- A stray empty `D:\Documents\FableTLC` tree appeared in the workspace (see Housekeeping).

## Housekeeping
- An empty directory tree named `D:\Documents\FableTLC/rebuild/build/landverify` appeared in this workspace (a Windows-path default
  resolved relative to the cwd by a lane tool); removed. The repo root has a same-named empty tree dated 13:14 (before this slice);
  not touched.
