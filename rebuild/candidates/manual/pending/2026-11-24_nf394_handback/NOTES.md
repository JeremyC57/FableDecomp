# NF394 hand-back (2026-11-24): T3 nf391 q10 (b1) and q9 (b2)

## Summary
- b1 (Job 1, q10): 0x004B87D0 / 0x006CD240 / 0x00DA0ED0 respelt from the made-up NoDuplicates `?SortTreeRecursively@@YIHPAUTree@@@Z` to
  `std::list<T>::size` in the nf391 b4 (0x005143E0) form: T = CQuestManager::CBoast, CNavWayPoint, CScriptThing. All three ACCEPT. Every list::size
  stand-in of that name is now gone (with nf391 b4 the five definers are all respelt).
- b2 (Job 2, q9): one unit (UNITS.tsv unit 00a127c0): 0x00A73CF0 = `?GetNumberOfNavigationLineLevels@CPhysicsMesh@@QBEJXZ`,
  0x00A127C0 = `?GetNumberOfNavigationLineLevels@CPhysicsMeshObject@@QBEJXZ`, witness 0x006A4F10 rebound, the believed-wrong table line replaced
  (TABLE_CHANGES `replace`, plus one `add` for the CPhysicsMesh callee). ACCEPT; two add-only header opt-ins (HEADERS.tsv). 0x006A4F10's own self is
  respelt too (RULINGS_NEEDED r1).
- Compiles: about 70 VC7.1 invocations of the 90 budget (grades 14, /Zs 10, census HEAD objects 20, vland pairs ~24).
- Locks: the 6 changed rows still read NF394 (released by the coordinator at landing, as nf391); all 20 census locks released.

## Caller anchors (the leg every identity rests on)
label_trust.py lists none of 0056ad40 / 00a1e110 / 00a1e5c0 / 00da0e40 as trusted (class-level label uniqueness; Activate etc. not listed), so each
caller is anchored independently (nf391 style): `nf387.py xr` resolves the VC7.1 HEAD object's leaf relocations against the retail bytes and the
names are compared with the FableWin function's direct callees (evidence/xr_callers.txt, caller_anchor.txt):
- 0x0056AD40: 55 retail-resolved function relocations, 39 named exactly as FableWin ?Activate@CTCBoastUI@@QAEXXZ's callees (incl.
  ?GetBoastsForScript@CQuestManager@@QBEXABVCCharString@@AAV?$list@VCBoast@CQuestManager@@..@@Z and the list<CBoast> size itself; the rest are STLport
  map/tree spellings FableWin's Dinkumware names differently);
- 0x00A1E110: 10 relocations, 6 exact FableWin MoveTowardNextWayPoint callees (Process / SetGoal / GetSpeedReducedForStartAndEnd / both
  CalculateNavigableWidthOfLine overloads / the size);
- 0x00DA0E40: 3 relocations, 2 exact FableWin GetRandomSpawnPoint callees (CScriptThing::operator=, IsActiveThreadTerminating) + the stand-in size;
  Ego_r / FableWin both have ?GetRandomSpawnPoint@CQ_MinionCampScript@NScript@@QAEX...
- 0x00A1E5C0 (Account B claim) not compiled; 006cd240 rests on 0x00A1E110 plus FableWin CalculateAndMoveToPosProximity.
Job 2's legs are Ego_r instruction-level equalities (0x00683F90 v 0x006A4F10; 0x00B80500 v 0x00A127C0), independent of catalog labels.

## Job 1 identities (two sources each)
| row | retail caller(s) (raw E8/E9 scan, evidence/rawref_all.txt) | FableWin (fwdown: the ONLY list size the caller calls) |
|---|---|---|
| 004b87d0 | 0x0056B157 in CTCBoastUI::Activate 0x0056AD40 (landed) | Activate -> list<CQuestManager::CBoast>::size x2 |
| 006cd240 | 0x00A1E2FD in MoveTowardNextWayPoint 0x00A1E110; 5 sites in CalculateAndMoveToPosProximity 0x00A1E5C0 | same two functions -> list<CNavWayPoint>::size (1 / 9) |
| 00da0ed0 | 0x00DA0E5E in NScript::CQ_MinionCampScript::GetRandomSpawnPoint 0x00DA0E40 | GetRandomSpawnPoint(const list<CScriptThing>&, ..) -> list<CScriptThing>::size |
evidence/fwdown_callers.txt lists every callee of those FableWin functions; each has exactly one `?size@?$list@` target (advisor check B).
The compiled selves equal the existing table spellings character for character (004b87d0: witness 0056ad40; 006cd240: witness 00a1e5c0).
00da0ed0 has no table line for the new spelling; 0x00DA0E40 still binds the stand-in `?size@CRetailList_CScriptThing@@QBEIXZ` there (unchanged; it
was already a second spelling at HEAD). Queued T3 t1 / NAME_FIXES (its own self is also a stand-in; both PDBs give `QAEXABV?$list@VCScriptThing...`).
Element types from shared headers: engine/CQuestManager.h (nested CBoast, FABLE_RETAIL_CQUESTMANAGER), engine/CNavWayPoint.h (+rebuild_abi.h,
FABLE_RETAIL_C2DVECTOR for the packed 10-byte record), engine/CScriptThing.h (FABLE_RETAIL_NSCRIPT with CCountedPointer.h first, its contract).
A forward declaration does not work for a by-value element (list node payload).

## Job 2 identities
- 0x00A127C0 (8 B, `mov ecx,[ecx+4] ; jmp 0x00A73CF0`): Ego_r.pdb 0x00B80500 ?GetNumberOfNavigationLineLevels@CPhysicsMeshObject@@QBEJXZ (8 B,
  lib_physics_mesh_object_2.obj) is the same forwarder to 0x00C12630 ?GetNumberOfNavigationLineLevels@CPhysicsMesh@@QBEJXZ; Ego_r
  CThingPhysical::GetNoNavCollisionLineLists 0x00683F90 calls 0x00B80500 at the instruction where retail 0x006A4F10 calls 0x00A127C0 (0x006A4F37).
  FableWin: GetNoNavCollisionLineLists 0x01F21C70 and CTCGraphicAppearance::DrawNavigationMesh call it; retail's other callers 0x004BD4C4 / 0x004BD5FB
  sit in the CTCGraphicAppearance block (between GetAllMeshes 0x004BD420 and SetPrimitiveGraphics 0x004BD620; not a catalog start).
- 0x00A73CF0 (25 B, (end - begin) / 12 at +0x48/+0x4C): the jump target above (its only reference); FableWin 0x033C7680 calls
  `?size@?$vector@V?$CArray@VC2DLineF@@@@..@@QBEIXZ` on the member; Ego_r types NavigationLines `CArray<CArray<C2DLineF> >` (the shared header
  places it at +0x48, STLport 12-byte vector). The old self was the propagated `?DrawGetNoMaps@CWorldMap@@QAEJXZ` (0x004FB4B0 also defines it).
- 0x006A4F10: Ego_r and FableWin both spell `?GetNoNavCollisionLineLists@CThingPhysical@@QBEJXZ` (const, long); Ego_r's 71-byte body is
  instruction for instruction retail's (PhysicsTC +0x70 / slot +0x184 there). The table line `?GetNoNavCollisionLineLists@CPhysicsMeshObject@@QBEPAXXZ`
  was indeed wrong (no PDB has that name; the PDB callee at that site is GetNumberOfNavigationLineLevels).

## Header edits (b2/HEADERS.tsv, hdr/engine/, orig/hdr_*)
- engine/CPhysicsMesh.h, new macro FABLE_RETAIL_CPHYSICSMESH_NAVIGATIONLINELEVELS: includes CArray.h + C2DLineF.h, declares the member function
  (public const), types NavigationLines `CArray<CArray<C2DLineF> >` (12 B, sizeof 0xC0 assert holds under VC7.1).
- engine/CPhysicsMeshObject.h, new macro FABLE_RETAIL_CPHYSICSMESHOBJECT_NAVIGATIONLINELEVELS: declaration only. Its FABLE_STATIC_ASSERT moved 8 lines,
  so it is pinned with `FABLE_STATIC_ASSERT_AT(113, ...)` (lane_merge/pin_asserts.py) and keeps its typedef name. This is the one existing line edited.
- Macros new (grep over rebuild/ and tools/: none). equiv_check (repo tool, both trees scratch copies, exact mode) over the 1,431 landed rows whose
  source or test includes either header or a compiled row: 0 differ (evidence/equiv_hdr.tsv); control: a row defining the macros hashes differently.
- No header in rebuild/include includes either one by a sibling-relative path (grep), so the 1,431-row set is the whole user set.
- Account B pending overlays of CPhysicsMesh.h / CPhysicsMeshObject.h (2026-11-11_acctB_c2005, r3001, 2026-11-12 r3016 / r3020 / r3021, plus two old
  linux_hand ones) are older snapshots: HEAD is a superset except their NAVVOLUMES block (10 lines, since landed in another spelling); none touches
  NavigationLines or GetNumberOfNavigationLineLevels. A wholesale re-apply of one of them (not allowed: copy from the current header) would conflict
  with the NavigationLines #ifdef wrap.
- Under clang the sizeof(CPhysicsMesh) assert fails even with the pre-existing CALCULATEBOUNDINGINFO opt-in (clang layout, not this edit); the rows need
  FABLE_RETAIL_DEF_CLASS_BASE + FABLE_RETAIL_C3DVECTOR, as the landed CPhysicsMesh users have, for VC7.1's assert to hold.
- The coordinator must apply hdr/ before land_scripted rechecks b2 (land_scripted copies rows only).

## Gates (both batches)
- vland --recheck-landed HEAD and new at one LANDVERIFY_WORK path (work/wd_u1h|n, wd_u2h|n; b2 with FABLE_HDR_OVERLAY=hdr on both sides): b1 MATCH PASS
  x3 both; b2 00a73cf0 MATCH, 00a127c0 / 006a4f10 RELOCATION_MATCH, PASS both. Warnings: STANDIN_LEAF (catalog labels, NAME_FIXES T0 lines),
  006a4f10 SHARED_HEADER_AVAILABLE x3 (pre-existing, T3 t2).
- objsec (evidence/objsec): b1 = the NoDuplicates stand-in out, the size leaf + iterator/distance chain in (all Any); 00a73cf0 / 00a127c0 = old stand-in out,
  new leaf + one Any COMDAT in; 006a4f10 ALL_EQ under its two renames.
- R1 bins (evidence/bins.tsv; tools/tier394.py; tier-1 definers are landed census rows only, never a batch row): 004b87d0 TIER1 x9, 006cd240 TIER1 x9,
  00da0ed0 TIER1 x3 + TIER2_NOT_UNIQUE x6 (list<CScriptThing> chain: no landed definer; distance/__distance 99 starts, iterator ctor 737, ++ 192,
  begin 218, end 572), 00a73cf0 TIER1 x1 (vector<CArray<C2DLineF> >::size = 00a75010 / 00a75230 / 00a752a0), 00a127c0 TIER1 x1 (operator-> = 00a11d10 /
  00a128e0). No residue.
- Leaf definers (evidence/leaf_definers.tsv): the new size<CBoast> leaf is also defined Any + EQ by 004b05c0 0056ad40 00813d00; size<CNavWayPoint> by
  00a1ceb0 00a1e030; size<CScriptThing> by nobody else.
- pk_all + pickany_vs_retail HEAD, new, and new with TABLE_CHANGES applied (scratch FABLE_ROOT work/troot): no NEQ anywhere (generic
  _List_iterator_base trio EQ at 0x0041C780 / 0x0041C7A0 / 0x0041C7C0; the rest NOADDR); reach flips 0 (6 pairs).
- /FORCE link HEAD v new of the 6 rows + the 20 census / callee-owner objects (004fb4b0, three ?Run@T@@QAEXXZ definers 004379b0 008562d0 00c09bc0,
  00a127d0 / 006b0850 = owners of 006a4f10's other callees, 00da0e40, the size / C2DLineF / operator-> definers, 005143e0): duplicates 6 -> 2
  (?SortTreeRecursively x2, ?DrawGetNoMaps QAEJXZ x1, ?Run@T x1 gone; the 2 left are ?Run@T clashes among the other definers, pre-existing);
  unresolved 119 -> 117: nothing newly unresolved; gone: ?GetNoNavCollisionLineLists@CPhysicsMeshObject@@QBEPAXXZ and ?Run_00a73cf0@Sub@@QAEXXZ.
  ?Run@T@@QAEXXZ has 77 definers (self_symbols); a subset of 3 was linked (removing one definer cannot unresolve anything; no table line, no UND anywhere
  in the clang census).
- Census: clang prefilter over 737 landed rows (evidence/clcensus.txt, watch list work/watch.txt = every DEF/UND of the 12 objects + the
  CRetailList stand-in); 10 clang-fail rows cleared by text (they include CQuestManager.h / use C2DLineF but name no CBoast list or CArray<CArray>); VC7.1
  HEAD objects of the 20 selected rows. Binders of the removed / rebound names: only the run rows. 00a1e5c0 (binds size<CNavWayPoint>) is an open
  Account B claim and was not compiled (T3 t4).
- apply_table --dry (repo): b1 no lines; b2 replace 1 + add 1, 0 misses, multi-target 1 -> 1.

## Decisions / deviations
- 006a4f10: self respelt as well as the callee (advisor; RULINGS_NEEDED r1). RESULTS carries `; target 00a127c0` (it is also a binder, and a unit member).
- land_scripted skips the symbol check for 00a73cf0 and 006a4f10 (unit members whose unit is 00a127c0; 006a4f10 also `; target`): the proof that they
  grade their new Leaf is vland_u2n (b2/logs/vland_u2n_gradelines.log: `00a73cf0 MATCH ... ?GetNumberOfNavigationLineLevels@CPhysicsMesh@@QBEJXZ`,
  `006a4f10 RELOCATION_MATCH ... ?GetNoNavCollisionLineLists@CThingPhysical@@QBEJXZ`).
- 00a127c0 has no self_symbols.tsv line at HEAD: its SELF_SYMBOLS current_line (`?Run@T@@QAEXXZ|...`) is synthesised from the old Leaf; the
  self_symbols change is an ADD, not a replace.
- 00a1e5c0 (uncompiled definer of size<CNavWayPoint> per clang): its source has no out-of-class, specialised or explicit-instantiation `size`
  (grep: no `::size(`, no `template<>`, no SIZE opt-in; only FABLE_RETAIL_STLP_LIST's in-class inline), so any VC7.1 copy is an Any COMDAT and no
  duplicate is possible; equality with 0x006CD240's copy is unproven (RULINGS_NEEDED r3).
- Repo drift: the coordinator landed NF393's work during this session (reloc_bindings / self_symbols / 0x00B217D0 changed). Re-checked at the end
  (evidence/drift_check.txt): sources, headers, table lines and self lines of this run unchanged; apply_table --dry still 0 misses.
- Tests unchanged (nf391 b4 precedent): every test is self-contained and PASSes against the new rows (T3 t5).
- Deviation (brief: no git): I ran git commands: `git log --oneline -3`, `git log --oneline -3 -- <00a1e5c0 source>`, `git fetch -q origin
  FullDecomp-B` (wrote .git/FETCH_HEAD and the remote-tracking ref), `git grep -l GetNoNavCollisionLineLists FETCH_HEAD -- rebuild`, and
  `git status --short rebuild/oracles` twice. Only the fetch writes (inside .git). No repo file was written: evidence/drift_check.txt (a non-git
  find of files newer than my first evidence file lists only NF393 / coordinator files). label_trust.py and equiv_check.py ran with the repo as cwd;
  neither left a file there (same find).
- Test prechecks that fail under clang only inside VC7.1 headers / `_assert` (7 census rows) were passed with `cl /Zs` before locking (nf391 practice).

## Files
batches/b1, batches/b2 (land_scripted run dirs), STATUS.tsv, T3.txt, NAME_FIXES.tsv, RULINGS_NEEDED.tsv, evidence/, tools/ (NF391 tools adapted:
nf387.py gains `ngrade`; new tier394.py, leafdefs394.py, assemble394.py, vl394.sh, equiv394.sh).

## Follow-up on the coordinator's rulings (r1 YES, r2 YES with R10, r3 = compile it), 2026-11-24
Used 1 VC7.1 compile (00a1e5c0 HEAD). No repo writes and no git in this follow-up. The table files are read from disk.
1. **r3, 00a1e5c0.** Test precheck PASS, then lock (it read `released`), source precheck PASS, then VC7.1 HEAD object (RELOCATION_MATCH). The lock is
   released again. Its `?size@?$list@VCNavWayPoint@@..@@QBEIXZ` copy is **Any**, 24 B, and comdat_cmp shows it **EQ** in bytes and relocations to
   006cd240's new copy (evidence/r3_00a1e5c0_cmp.txt). **006cd240 stays ACCEPT, with no debt.** 00a1e5c0 emits no distance / __distance (its
   inline size is the for-loop form, so VC7.1 kept one equal body).
2. **r2 + R10.**
   - 00a11d10 and 00a128e0 `#include "engine/CPhysicsMesh.h"` and `"engine/CPhysicsMeshObject.h"` and model neither class locally, so
     operator-> stays TIER1.
   - 00a75010, 00a75230 and 00a752a0 all model CArray / vector / C2DLineF locally, and no other landed definer was found. 00a73cf0's
     `vector<CArray<C2DLineF> >::size` is therefore re-tiered by scan: reloc-free 24 B, 59 matches, all at 16-aligned starts after 0xCC
     padding, none mid-function (evidence/r2_vector_size_t2.txt). It is TIER2_NOT_UNIQUE (R9: no address recorded).
   - R10 applied to every tier-1 pick: 00a1e030 is also a local model (its own list / _List_iterator), and it was the only tier-1 definer of
     006cd240's distance / __distance. Those two re-tier to TIER2_NOT_UNIQUE (the 99 starts below).
   - tools/tier394.py now excludes LOCAL_MODEL {00a1e030, 00a75010, 00a75230, 00a752a0}. The bins are recomputed and still have no residue:
     004b87d0 TIER1 x9; 006cd240 TIER1 x7 + T2NU x2; 00da0ed0 TIER1 x3 + T2NU x6; 00a127c0 TIER1 x1; 00a73cf0 T2NU x1.
3. **0x00DA0ED0, second anchor.**
   - The two caller relocations that matched FableWin are CScriptThing::operator= (??4, 0x008AB980) and CScriptBase::IsActiveThreadTerminating.
     Neither is specific to the function.
   - Byte-visible anchor (evidence/r3_00da0e40_disasm.txt): at 0x00DA0EBC..C0, `add esi,8 ; push esi ; call 0x008AB980` with ecx = `out`. The
     node payload (STLport node {next, prev, data}, data at +8) is the const CScriptThing& source of ??4CScriptThing@@QAEAAV0@ABV0@@Z. That address
     is bound by its table line and engine/CScriptThing.h, with the Ego_r 0x008E9940 and FableWin 0x02A75C10 spellings. So the list element is
     CScriptThing.
   - Caller structure: 0x00DA0E40's only retail caller is 0x00DA0D0E in 0x00DA0C40 (catalog RunSpawnPointControl). FableWin's only callers of
     GetRandomSpawnPoint (0x037EA8FE / 0x037EA934) are in ?RunSpawnPointControl@CQ_MinionCampScript@NScript@@QAEXXZ (also Ego_r 0x00DCBD30).
   - **00da0ed0 stays ACCEPT.**
   - Its six TIER2_NOT_UNIQUE copies (evidence/r3_00da0ed0_t2.txt) hit at function starts only:
     | copy | catalog starts | 16-aligned after 0xCC | mid-function (not counted) |
     |---|---|---|---|
     | iterator ctor | 13 | 724 | 152 |
     | ++ | 2 | 190 | 1 |
     | distance | 2 | 97 | 5 |
     | __distance | 2 | 97 | 5 |
     | begin | 3 | 215 | 56 |
     | end | 6 | 566 | 140 |
4. **r1.** rebuild/oracles/reloc_bindings.tsv has 0 lines whose target is 006a4f10. Only 006a4f10's own source defines
   ?GetNoNavCollisionLineLists@CThingPhysical@@QBEJXZ: one text hit, plus 00a3e3e0 which mentions the name in a comment only; the clang census and
   leaf_definers.tsv agree (evidence/r4_006a4f10.txt). Nothing joins the unit and nothing is queued.
- Rebuilt with tools/assemble394.py. RESULTS reasons now cite R10, the 00a1e5c0 compile and 00da0ed0's byte anchor. TABLE_CHANGES are unchanged,
  and apply_table --dry still reports 0 misses.
