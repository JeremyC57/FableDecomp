# NF277 notes (2026-11-22, round 33)

No header touched (hdr/ and orig/ empty, no equiv_check needed). No binding, table or name change (RENAMES / TABLE_CHANGES / NAME_FIXES
carry headers only).

## Entry 1: 0x00BFE900 as a fold of 0x00BFDE20 -- done
- **Host test first** (evidence/e1_tests.txt): the landed 20 KB test is a standalone host model. It defines its own ~CLandscapeLayerMesh,
  ForceNew_00bfe900, FreeVirtual and a local CMovableResource, and it links and PASSes ALONE (tst.py, no row object), before and after the
  fold, so it never exercised the row. vland links a test alone first and only then with the row object (verify_and_land.behaviour_of).
- **Row (rows/00bfe900.cpp):** line 1, Leaf `??_GCLandscapeLayerMesh@@UAEPAXI@Z`, Identity / TU comments, then
  `#include "../src/compiled/00/bf/CLandscapeLayerMesh_CLandscapeLayerMesh_00bfde20.cpp"` and no code (the nf251 0x00B2AB10 / nf272 0x00BEB4D0
  fold pattern). No Flags line (CompilerFlags of 0x00BFE900 unchanged: none).
- **Leaf** (evidence/e1_cmpobj_objsec_vtcheck.txt, work/gr_e1.log): RELOCATION_MATCH at HEAD and after (q.py; vland evidence/e1_vland.log);
  sc.sh leaf relocation diff (target, symbol, count) EMPTY; cmpobj HEAD v fold: `??_GCLandscapeLayerMesh` SAME (bytes with relocation fields
  zeroed + every relocation symbol). pk: the fold's `??1CLandscapeLayerMesh` (93) EQ 0x00BFDE20, `??_GCMovableResource` (31) EQ 0x009D5110.
- **Object identity:** objsec landed 0x00BFDE20 object v the fold object: ALL_EQ (7 sections, bytes + relocation lists). So the fold emits
  exactly the destructor TU's COMDATs and tables, and no wrong copy.
- **Tables (vtcheck, retail targets):** fold `??_7CLandscapeLayerMesh` 4 slots on 0x012A835C: 0 ??_E -> 0x00BFE900, 1
  ?RelocateData@CLandscapeLayerMesh@@EAEXJPAX0J@Z -> 0x00BFDE80, 2 ?CanRelocateData@CLandscapeLayerMesh@@EBE_NJ@Z -> 0x00BFDDB0, 3
  ?Verify@CMovableResource@@UBEKXZ -> 0x009D5100 (each the landed leaf of the slot's row; 0x009D5100 has no landed row). `??_7CMovableResource`
  on 0x0129B860: ??_E 0x009D5110, __purecall 0x00BFEA08, CanRelocateData 0x009D50F0, Verify 0x009D5100. HEAD's 0x00BFE900 tables were 3 slots
  each, shifted (slot 1 Verify on 0x00BFDE80, slot 2 CMovableResource::CanRelocateData on 0x00BFDDB0, Verify on the __purecall slot). NOLINE
  everywhere = no table line for vtable slots (as nf272 / nf276).
- **Symbols that leave the object** (HEAD DEF, gone): `?ForceNew_00bfe900@@YAPAVCLandscapeLayerMesh@@XZ`, `??0CLandscapeLayerMesh@@QAE@XZ`,
  `??0CMovableResource@@QAE@XZ`, `?Verify@CMovableResource@@UBEKXZ` (3 bytes, returned 0; retail 0x009D5100 is 6 bytes, returns 0x7B: pk NEQ),
  `?CanRelocateData@CMovableResource@@UBE_NJ@Z` (no owner). New DEF: `??1CLandscapeLayerMesh@@UAE@XZ` (strong, identical to 0x00BFDE20's: pk EQ,
  cmpobj SAME). New UND: the destructor's callees (DeleteIndexBuffer / DeleteVertexBuffer, landed spellings) and the table slots.
- **Link** (evidence/e1_link_orders.txt, /OPT:NOREF /MAP, LINK unset; the only emitters of `??_7CLandscapeLayerMesh` are 0x00BFDE20 and
  0x00BFE900 by source grep, not objdump: no other landed row defines its destructor; the constructor rows 0x00BFDDD0 / 0x00BFD420 model the
  class without virtuals, 0x00BFDDD0 storing the vptr through the `?CLandscapeLayerMesh_vtbl@@3PAXA` stand-in global): new set, both orders: 1 LNK4006 on the identical strong
  `??1` (the nf251 / nf274 accepted shape), the kept 4-slot `??_7CLandscapeLayerMesh` and `??_7CMovableResource` equal in either order (ALL_EQ
  objects). HEAD set: 0 duplicates, but with 0x00BFE900 first the 3-slot table is kept, and in both orders `?Verify@CMovableResource` resolves to
  0x00BFE900's wrong 3-byte body. New set: Verify unresolved in this pair (no landed definer of 0x009D5100).
- **Test (rows/00bfe900_test.cpp, PassPattern `00bfe900_TEST PASS` kept):** rewritten on the shared headers (CMovableResource,
  CEngineLandscapeMemoryManager + DELETE opt-ins, CEngineLandscapeRenderer GPLANDSCAPERENDERER, CEngineLandscapeMeshBuilder MAPPINGBLEND) with
  the landed 0x00BFDE20 row's class block copied verbatim (quarantined header: header-less for the local-model rule; same token the landed rows
  define). It never constructs the class: objdump -t of the test object shows no `??_7` / `??_G` / `??1 CLandscapeLayerMesh` (evidence/e1_testobj_syms.txt;
  it defines only the stub slots RelocateData / CanRelocateData / CMovableResource::CanRelocateData / Verify, from the row object's UND list).
  Cases: the row's destructor called non-virtually (unshared index buffer and foreground buffer to the manager, the buffer pointer cleared, the index
  pointer kept, the derived vptr in place during the calls, base vptr at the end, no free; shared index buffer left alone); then the captured vptr
  planted in raw storage and slot 0 reached by `delete` (flag 1) and an unqualified destructor call (flag 0), with a chained Next mesh deleted
  through its own slot 0 first (sequence IVFF / IVF, FreeVirtual(next) before FreeVirtual(this)). Self link: LNK2019 `??1CLandscapeLayerMesh`
  (evidence/e1_tests.txt); with the fold object: PASS; vland: RELOCATION_MATCH PASS (evidence/e1_vland.log).
- **Census:** no new symbol name. `??1CLandscapeLayerMesh@@UAE@XZ` keeps one body text (0x00BFDE20's file, now included by 0x00BFE900);
  `??_GCLandscapeLayerMesh` / `??_7CLandscapeLayerMesh` compiler-generated in the same TU text only. Per address: no binding change (leaf
  relocations unchanged); the vtable-data spellings HEAD's 0x00BFE900 tables put on 0x00BFDE80 / 0x00BFDDB0 / 0x00BFEA08 leave (count-reducing).
  No file includes 0x00BFE900's file; 0x00BFDE20's file is now included by 0x00BFE900 (a rename of it must follow; funclet_includes_check).

## Decisions for the coordinator
1. Land 0x00BFE900 (row + test) as one set. Cost: one more strong identical `??1CLandscapeLayerMesh` (one LNK4006, nf251 / nf274 shape).
2. `?Verify@CMovableResource@@UBEKXZ` (0x009D5100) has no landed definer; with 0x00BFE900's wrong copy gone it is unresolved in this pair. Other
   landed July rows still define it pick-any from the same local CMovableResource model returning 0 (source grep: 0x00BE8E60, 0x00BFBD80,
   0x00AE04E0, 0x00AE0540, 0x00AE05A0), so a whole-image link can still bind slot 3 to a wrong body (and those rows presumably also emit
  the shifted 3-slot `??_7CMovableResource`: inferred from their source, not measured): queue those rows' ports onto the shared
   CMovableResource (and a 0x009D5100 row, authoring) as debt; not touched here.
3. Tests: the 0x00BFDDB0 test uses a test-only `friend struct CLandscapeLayerMeshTestAccess;` in its copy of the class block (the row's
   member is private; friends do not enter mangled names, the qualified call binds the row's EBE_NJ leaf). Accept, or prefer another access route?
4. 0x00BFD480 (blocked, REJECTED.tsv / NAME_FIXES.tsv): queue it as a row unit in the nf276 0x00BFDE20 shape (QAE -> UAE on the shared
   CMovableResource + engine/CEngineWaterBackgroundSubPatch.h, emitted ??_7 / ??_G, the binder 0x00BFDC90 (catalog CEngineWaterBackgroundSubPatch scalar deleting destructor) as a fold); its test follows the unit.
5. Landing sets: {0x00BFE900 row + test}; {0x00BFDE20 test}; {0x00BFDDB0 test}: independent (rows unchanged for the two tests).

## Entry 2: landscape host-model tests -- 2 done, 1 blocked, 4 open (context)
- **0x00BFDE20** (rows/00bfde20_test.cpp, PassPattern `00bfde20_TEST PASS` kept): the 0x00BFE900 harness (the row's object emits the same
  ??_G / tables: objsec ALL_EQ above), header comment rewritten; replaces the July CLayerMesh stand-in model with AppendFreeEdgeNode / FreeBlock
  stubs. Self link LNK2019 `??1CLandscapeLayerMesh@@UAE@XZ`; with the row object PASS (evidence/e2a_tests.txt); vland RELOCATION_MATCH PASS
  (evidence/e2a_vland.log). Test object defines no CLandscapeLayerMesh table / ??_G / ??1 (evidence/e2a_testobj_syms.txt). Row unchanged.
- **0x00BFDDB0** (rows/00bfddb0_test.cpp, PassPattern `RB_bfddb0_OK` kept): the row's CEngineLandscapeMap and CLandscapeLayerMesh blocks
  verbatim (both headers quarantined) + the test-only friend; cases unlocked (true for two handles), locked (false), reads only. Self link
  LNK2019 `?CanRelocateData@CLandscapeLayerMesh@@EBE_NJ@Z`; with the row object PASS; vland MATCH PASS. Row unchanged.
- **0x00BFD480**: blocked (REJECTED.tsv): the row's leaf is QAE on a row-local CMovableResource; both PDBs and the table say UAE.
- **0x00BFB590** (scouted, not started): the row (10 KB) is UAE on local models of CMovableResource / CIVCountedPointeeBase /
  CEnginePersistentPrimitiveBase / CEnginePrimitiveHandle (all have shared headers, included under Donor renames) and a local
  CLocalDetailPrimitiveBase / MeshZSpriteBatch (no `class` / `struct` definition of either anywhere under rebuild/include: grep). A shared-header test must match the row's
  local layouts byte for byte; check that before writing it. 0x00BE6090 / 0x00BFAAE0 / 0x00B684F0 (32-58 KB host tests): not started.

## Recheck (recheck_final.log, landed tree, no overlay)
0x00BFE900 (rows/ row + test) RELOCATION_MATCH PASS, 0x00BFDE20 (landed row + rows/ test) RELOCATION_MATCH PASS, 0x00BFDDB0 (landed row +
rows/ test) MATCH PASS; no WARN / RELOC_CONFLICT lines.

## Census (names)
- No new symbol name in any row object. Test-only names (test TUs only, never in a row): `CLandscapeLayerMeshTestAccess::CanRelocate`
  (descriptive), the file-static helpers of the two harnesses. `??1CLandscapeLayerMesh@@UAE@XZ` one body text (0x00BFDE20's file).

## Flags / funclets / descriptive names / locks / order / reading / slips
- Flags: none. Funclets / path includers: 0x00BFE900 now includes 0x00BFDE20's file by path (a rename must follow it); nothing includes
  0x00BFE900's file. Descriptive names: CLandscapeLayerMeshTestAccess (test only).
- Locks (rows.txt): 00bfe900 00bfde20 00bfddb0 00bfd480 00bfaae0 00bfb590 00b684f0 (released -> NF277), 00be6090 (new lock dir). Only
  00bfe900 / 00bfde20 / 00bfddb0 were compiled; release the other five. None of NF275's rows (00418dca 00418b7a 00a96da0) touched.
- Order: every compile ran after its lock was NF277 and in a later call than a passing clang precheck of the code it graded (fold, HEAD
  source, landed 0x00BFDE20 / 0x00BFDDB0 sources, all new and HEAD tests; missing-include control caught). Entry 1's vland finished before
  entry 2's first compile.
- Slip: the entry-2 batch (work/e2a.sh: one g2.sh grade, four tst.py runs, one vland) was launched as `nohup timeout 2000 bash e2a.sh`; each
  grade / vland line inside carries `timeout 900`, but the outer launch line said 2000, not 900. Every other launch line carried `timeout 900`.
- Reading: this brief, NF276 / NF274 briefs, NF214_BRIEF.md, ASM_RULES.md, EXTRA_RULES.md, COORDINATION.md, GOTCHAS.md (all 503 lines), the
  nf272 / nf274 / nf276 landing READMEs, the NF276 and NF274 hand-backs (NOTES, LESSONS).
- No git, no sleep, no rm, no __asm / naked / _emit; nothing written under /home/user/FableDecomp. The repo files newer than BRIEF.md (full
  find) are the coordinator's nf275 landing (NF275's three rows, seven engine headers of its unit, reloc_bindings.tsv, NAME_FIXES_QUEUE,
  COORDINATION_CLAIMS, HANDOFF, the nf275 hand-back and batch folders); none is an include of my rows, and nothing under rebuild/include,
  rebuild/src/compiled/00/bf, rebuild/tests/00/bf or build_candidates.ps1 is newer than recheck_final.log, so the recheck ran on today's tree.
- Citations checked: Ego_r 00a4a7f0 + FableWin 02ddd6c0 `??_GCLandscapeLayerMesh@@UAEPAXI@Z`; rtti-vtables.tsv CLandscapeLayerMesh
  0x012A835C slots 0 / 1 / 2 = 0x00BFE900 / 0x00BFDE80 / 0x00BFDDB0.
