# NF279 notes (2026-11-22, round 34)

Entry 1 done (four rows, one header opt-in, one test). Entries 2 and 3: see their sections. One header (hdr/ + orig/, hdr/README.md).

## Entry 1: the 0x00BFD480 water sub-patch destructor unit -- done
Rows: 0x00BFD480 (self rename QAE -> UAE + new test), 0x00BFDC90 (fold), 0x00BFD5D0 / 0x00BFD390 (leaf-only self renames onto the PDB slot
spellings the new table binds). Header: engine/CEngineWaterBackgroundSubPatch.h, new add-only opt-in `FABLE_RETAIL_CENGINEWATERBACKGROUNDSUBPATCH_VIRTUALS`.
- **Why four rows (advisor, before the first compile):** the destructor TU emits `??_7CEngineWaterBackgroundSubPatch@@6B@`, whose slots 1 / 2 bind
  the PDB spellings `?RelocateData@CEngineWaterBackgroundSubPatch@@EAEXJPAX0J@Z` (0x00BFD5D0) and `?CanRelocateData@CEngineWaterBackgroundSubPatch@@EBE_NJ@Z`
  (0x00BFD390). Both self rows defined stand-ins (a derived shell `CEngineWaterBackgroundSubPatch_RelocateData`, and `C::Can` on `struct S` / `struct C`),
  so without their self moves each address would gain a second spelling (the nf276 0x00BFDDB0 case, accepted by nf276 ruling 2). Both renames are
  leaf-only: both PDBs agree (Ego_r 00a4bd00 / FableWin 02e06160; Ego_r 00a4bac0 / FableWin 02e06380), RTTI vtable 0x012A8334 slots 1 / 2, and the
  exe scan (absrefs.py / callers.py) finds no call site, only the `.rdata` slots 0x012A8338 / 0x012A833C; no reloc_bindings line names either address.
- **Header opt-in (hdr/README.md):** the virtual destructor, the class delete (declared; FableWin `??3CEngineWaterBackgroundSubPatch@@SIXPAX@Z`, no
  Ego_r out-of-line copy, so inline in retail; the destructor's TU defines it inline), and the two private overrides, on the existing
  `FABLE_RETAIL_WATER_OCCLUSION_ORIGINAL_CHAIN_PASS22` path (`: public CMovableResource`) with `FABLE_RETAIL_CMOVABLERESOURCE`. Add-only, no existing
  line edited; asserts already `_AT`-pinned (pin_asserts: 0). equiv_check (exact, whole tree, repo include v repo + this file): 57,125 rows
  checked, 0 differing (evidence/e1_equiv.txt), so no landed row or test changes; no recheck beyond the unit was needed.
- **0x00BFD480** (rows/00bfd480.cpp; evidence/e1_00bfd480.txt): row-local CMovableResource (placeholder `v00`) and the local sub-patch / manager
  models replaced by engine/CMovableResource.h (retail), engine/CEngineWaterBackgroundSubPatch.h (PDB layout + VIRTUALS), the retail branch of
  engine/CEngineLandscapeMemoryManager.h with the landed DELETEINDEXBUFFER / DELETEVERTEXBUFFER opt-ins and CEngineLandscapeRenderer.h (0x00BFDE20's
  set). Leaf RELOCATION_MATCH at HEAD (QAE) and after (UAE); leafcmp 144 = 144, bytes equal with relocation fields zeroed, relocation lists equal;
  sc.sh relocation diff empty. New COMDATs: `??_GCEngineWaterBackgroundSubPatch@@UAEPAXI@Z` 38 bytes pk EQ 0x00BFDC90 (the destructor called out of
  line, then the class delete inlined to FreeVirtual); `??_GCMovableResource` pk EQ 0x009D5110; `??1CMovableResource@@UAE@XZ` (7 bytes, no retail
  owner) and `??_GCMovableResource` cmpobj SAME as the landed 0x00BFDE20 object's (NF277's D_00bfde20.obj). No `??3...` copy (objdump -t).
  - vtcheck: `??_7CEngineWaterBackgroundSubPatch` 4 slots on retail 0x012A8334: 0 ??_E (alias of ??_G) -> 0x00BFDC90, 1 RelocateData EAEXJPAX0J -> 0x00BFD5D0,
    2 CanRelocateData EBE_NJ -> 0x00BFD390, 3 ?Verify@CMovableResource@@UBEKXZ -> 0x009D5100 (no landed definer). `??_7CMovableResource` on 0x0129B860:
    ??_E 0x009D5110, __purecall 0x00BFEA08, CanRelocateData 0x009D50F0, Verify 0x009D5100 (the shared header's table, as every other emitter). NOLINE
    everywhere = no table line for vtable slots (as nf272 / nf276 / nf277). HEAD's 0x00BFD480 tables were 1 slot each (`?v00@...` on slot 0).
- **0x00BFDC90** (rows/00bfdc90.cpp; evidence/e1_00bfdc90_fold.txt): by-path include of 0x00BFD480's file, no code (nf251 / nf272 / nf277 fold
  pattern). Leaf RELOCATION_MATCH before and after, leafcmp equal, objsec of the fold object against the new 0x00BFD480 object ALL_EQ (7 sections), so
  the fold emits exactly the destructor TU's COMDATs and tables. Gone from the object: the July local models' 1-slot `??_7CEngineWaterBackgroundSubPatch`
  (slot 0 only), `??0CEngineWaterBackgroundSubPatch@@QAE@XZ` (model ctor), `?Construct_00bfdc90@@YAPAVCEngineWaterBackgroundSubPatch@@PAX@Z` and a
  placement `??2@YAPAXIPAX@Z` copy (EQ 0x00405C10); UND `??0CMovableResource@@QAE@XZ` gone. New: a strong `??1CEngineWaterBackgroundSubPatch@@UAE@XZ`
  identical to 0x00BFD480's (pk EQ): one LNK4006 (the nf251 / nf274 / nf277 accepted shape).
  For the grades and vland the fold resolves `../src/compiled/00/bf/...00bfd480.cpp` through the overlay: a copy of rows/00bfd480.cpp sits at
  `src/compiled/00/bf/` in my folder (hdr/../src). At landing the repo path holds the new file.
- **0x00BFD5D0** (rows/00bfd5d0.cpp; evidence/e1_slot_rows.txt): the member itself on the header (VIRTUALS), and the row-local `CIndexBuffer` model
  replaced by engine/CIndexBuffer.h (retail branch; declares `OnPostRelocation`). RELOCATION_MATCH; leafcmp 147 = 147, relocations equal; objsec ALL_EQ
  (4 sections) with the leaf renamed. The two `RelocatePointer<T>` helper COMDATs (row-local template, HEAD debt, no retail owner) are unchanged.
  vland WARN RELOC_NAME `?OnPostRelocation@CIndexBuffer@@QAEXXZ -> 00ac1f60 labelled _global::Nop_ret0_00ac1f60` is pre-existing (same symbol at HEAD,
  table line `... 00ac1f60 00bfd5d0`): NAME_FIXES.tsv.
- **0x00BFD390** (rows/00bfd390.cpp): `return Map->MemoryFreeingLocked == false;` on the header, with a local CEngineLandscapeMap in the landed
  0x00BF7F70 / 0x00BFDDB0 shape (its only header is quarantined: header-less for the local-model rule). MATCH; objsec ALL_EQ (2 sections) with the
  leaf renamed.
- **Test (rows/00bfd480_test.cpp, PassPattern `00bfd480_TEST PASS` kept):** the NF277 harness on the shared headers: never constructs the class
  (test object: no `??_7` / `??_G` / `??1` of it; defines only the slot stubs RelocateData / CanRelocateData / CMovableResource::CanRelocateData /
  Verify; UND `??1...UAE@XZ`: evidence/e1_testobj_syms.txt). Cases: the destructor called non-virtually with DeletableBuffers (index / vertex / bridge
  pairs in retail order, the third bridge's vertex buffer null, the main vertex buffer null; the derived vptr in place during the calls, the base vptr
  at the end, pointers left as they were, no free) and without it (no calls); then the captured vptr planted in raw storage and slot 0 reached by
  `delete` (flag 1: the destructor's calls then FreeVirtual(this)) and an unqualified destructor call (flag 0). The landed July test linked and
  passed ALONE (never tested the row). New test: alone LNK2019 `??1CEngineWaterBackgroundSubPatch@@UAE@XZ`; with the 0x00BFD480 object PASS; with the
  fold 0x00BFDC90 object PASS (evidence/e1_tests.txt); vland RELOCATION_MATCH PASS.
- **vland (evidence/e1_vland.log, overlay hdr/):** 0x00BFD480 RELOCATION_MATCH PASS, 0x00BFDC90 RELOCATION_MATCH PASS, 0x00BFD5D0 RELOCATION_MATCH
  PASS (+ the pre-existing WARN above), 0x00BFD390 MATCH PASS. The other three rows keep their landed tests (self-contained host models; they still
  pass; entry-3-style debt).
- **Link (evidence/e1_link_orders.txt, /FORCE /NODEFAULTLIB, LINK unset):** new set (the four objects), both orders of the two emitters: 1 LNK4006 on
  the identical strong `??1...UAE`; slots 1 / 2 resolve to the new self leaves; only `?CanRelocateData@CMovableResource@@UBE_NJ@Z` (0x009D50F0) and
  `?Verify@CMovableResource@@UBEKXZ` (0x009D5100) stay unresolved (no landed definer). HEAD set: 0 duplicates, but `??1...UAE@XZ` (bound by
  0x00BFDC90) unresolved and both tables' slot 0 on `?v00@...` stand-ins. The only emitters of `??_7CEngineWaterBackgroundSubPatch` are these two rows
  (source grep: no other row defines its ctor / dtor or news it; 0x00BE65A0's local class model and 0x00BFD160's are non-polymorphic; 0x00BFD420
  stores the table as a cast constant).

## Entry 2: the July rows that define a wrong CMovableResource::Verify -- 3 rejected, 2 blocked (decision needed)
HEAD measured first (g2.sh on the landed sources, no flags; evidence/e2_head.txt): all five leaves RELOCATION_MATCH.
- **0x00AE04E0 / 0x00AE0540 / 0x00AE05A0 (CParticleProgram buffer getters): rejected, no change.** Their objects define only the leaf and one
  `??$AllocateType@...@CSimpleMemoryPool@@...` COMDAT; no CMovableResource symbol at all. The TU never constructs or destroys a CMovableResource or
  CParticleProgram, so no vtable is emitted and the local model's inline Verify is never instantiated. The ruling's list came from a source grep.
  (Their local CMovableResource / CBaseClassNonCopyable / CSimpleMemoryPool / CParticleProgram models and alias includes stay HEAD debt; a port
  onto the retail CParticleProgram.h, which already declares GetMatrixBuffer / GetVectorBuffer, is ordinary re-port work, not a Verify fix.)
- **0x00BE8E60 (??_GCLandscapeBackgroundPatch) / 0x00BFBD80 (??_GCLocalDetailPrimitiveMeshZSpriteBatch): blocked, two forms measured.**
  - HEAD: each defines `?Verify@CMovableResource@@UBEKXZ` (3 bytes, returns 0; pk NEQ 0x009D5100, 6 bytes), `?CanRelocateData@CMovableResource@@UBE_NJ@Z`
    (pk EQ 0x009D50F0), `??_GCMovableResource` (EQ 0x009D5110) and 14-16 local-model COMDATs; `ForceNew_<addr>` constructs the class. vtcheck: the
    class tables are 3 slots (slot 1 Verify on retail's RelocateData slot, slot 2 CMovableResource::CanRelocateData on the class's own slot), retail
    has 9+ (0x012A803C) / 10 (0x012A82D8); `??_7CMovableResource` is 3 slots shifted (Verify on the __purecall slot).
  - Form 1, the brief's (alias include of engine/CMovableResource.h and the local CMovableResource removed, FABLE_RETAIL_CMOVABLERESOURCE first; proposed/form1/<addr>.cpp):
    clang: `allocating an object of abstract class type 'CLandscapeBackgroundPatch'` / `'CLocalDetailPrimitiveMeshZSpriteBatch'` in ForceNew. The shared
    base's RelocateData is pure and the local derived models (classes with shared headers) declare no override; making them concrete means new virtual
    declarations in local models of header classes (not allowed), and retail's 9+ / 10-slot tables need the real class headers. Not graded.
  - Form 2 (proposed/form2/<addr>.cpp; NOT in rows/): the existing local model's `virtual unsigned long Verify()const{return 0;}` made declaration-only,
    plus a 3-line comment. One grade each (evidence/e2_form2.txt, e2_form2_cmpobj.txt): leaf 38 = 38, bytes and relocations equal; cmpobj HEAD v form 2
    every function COMDAT SAME (18 / 16) and `?Verify@CMovableResource@@UBEKXZ` DEF -> UND (it now binds the real 0x009D5100 symbol, no landed definer);
    tables unchanged in symbols per slot (vtcheck), so the 3-slot table debt stays. vland with the landed tests: both RELOCATION_MATCH PASS
    (evidence/e2_form2_vland.log; 23 SHARED_HEADER_AVAILABLE warnings, all HEAD's local models). In-place edit of a local model the row had at HEAD
    (no new class token): count-reducing (one wrong pick-any body fewer per row), but NOT the port onto the shared header the brief asked for.
  - The real fix is a fold of each ??_G row onto its destructor row (the 0x00BFE900 / 0x00BFDC90 pattern), which waits on those rows' own ports:
    0x00BE6090 (`??1Patch@` stand-in leaf on `Movable` / `Water` / `Patch` stand-ins) and 0x00BFB590 (`??1CLocalDetailPrimitiveMeshZSpriteBatch` on
    local CMovableResource / CIVCountedPointeeBase / CEnginePersistentPrimitiveBase / CEnginePrimitiveHandle / CLocalDetailPrimitiveBase models).
  - Census: of the 99 landed rows with a local CMovableResource model, these five are the only ones whose model defines an inline Verify body (grep
    `Verify() const { return`); 18 mention Verify at all.

## Entry 3: landscape host-model tests -- open (not started; advisor: hand back after entry 2)

## Census (names and addresses)
- 0x00BFD480: spellings {self QAE, binder 0x00BFDC90 UAE} -> {UAE} (2 -> 1). 0x00BFD5D0: {stand-in self leaf} -> {PDB leaf = the new table's slot 1}
  (1 -> 1). 0x00BFD390: {`?Can@C@@QAE_NH@Z`} -> {PDB leaf = slot 2} (1 -> 1). 0x00BFDC90: unchanged (`??_G...UAEPAXI@Z`). 0x012A8334:
  {`??_7CEngineWaterBackgroundSubPatch@@6B@`} unchanged (0x00BFD420 stores a constant, binds nothing).
- New symbol NAMES and definers (source grep of rebuild/src/compiled, include, tests, comments excluded; plus my rows): `??1CEngineWaterBackgroundSubPatch@@UAE@XZ`
  one body (0x00BFD480's file; 0x00BFDC90 includes it); `?RelocateData@CEngineWaterBackgroundSubPatch@@EAEXJPAX0J@Z` one body (0x00BFD5D0);
  `?CanRelocateData@CEngineWaterBackgroundSubPatch@@EBE_NJ@Z` one body (0x00BFD390). The new 0x00BFD480 test defines stubs of the last two (test TU only,
  never linked with those rows by vland). `??_G` / `??_7 CEngineWaterBackgroundSubPatch`: compiler-generated in 0x00BFD480's TU text only.
- Names that leave: `??1CEngineWaterBackgroundSubPatch@@QAE@XZ`, `??1CMovableResource@@QAE@XZ`, `?RelocateData@CEngineWaterBackgroundSubPatch_RelocateData@@QAEXJPAX0J@Z`,
  `?Can@C@@QAE_NH@Z`, `??0CEngineWaterBackgroundSubPatch@@QAE@XZ`, `?Construct_00bfdc90@@...`, the UNDs `?v00@CEngineWaterBackgroundSubPatch@@UAEXXZ` /
  `?v00@CMovableResource@@UAEXXZ` / `??0CMovableResource@@QAE@XZ`.
- RENAMES.tsv: section 1 has the three self renames (catalog names and file names unchanged); section 2 empty (no binder moved).

## Findings (NAME_FIXES.tsv; not applied)
- 0x00BFD420 is `??0CEngineWaterBackgroundSubPatch@@QAE@PBVCEngineLandscapeMap@@@Z` (stores vtable 0x012A8334 + the sub-patch's 0x60-byte layout; both
  PDBs), catalogued as CLandscapeLayerMesh::CLandscapeLayerMesh with a free-function stand-in leaf; its binder 0x00BE7D70 reaches it through a
  stand-in helper. A port (authoring-sized) is needed before the rename; not touched.
- 0x00AC1F60 is `?OnPostRelocation@CIndexBuffer@@QAEXXZ` (both PDBs; three landed binders bind that spelling); its self row is the stand-in
  `?M@C00ac1f60@@QAEXXZ` (catalog `_global::Nop_ret0_00ac1f60`): an existing self-v-binders split, queued.

## Decisions for the coordinator
0. Entry 2: land form 2 for 0x00BE8E60 / 0x00BFBD80 (Verify declaration-only inside the existing local model; leaf and every other COMDAT unchanged,
   the wrong pick-any Verify gone), or hold both rows for the dtor-row ports and folds? Files: proposed/form2/<addr>.cpp (+ landed tests unchanged).
   Link consequence: these two rows are the only landed definers of `?Verify@CMovableResource@@UBEKXZ` (grep: no out-of-class definition anywhere;
   the CParticleProgram models never emit theirs), so form 2 on both leaves Verify with no definer until a 0x009D5100 row is authored (the
   symbol is then unresolved in a whole-image link instead of bound to the wrong 3-byte body). Release the five entry-2 locks either way (no row
   of entry 2 is in rows/).
0b. 0x00BFD390 now defines a row-local `class CEngineLandscapeMap` (HEAD had `struct S`): a new local-model token, in the shape of the landed
   0x00BFDDB0 / 0x00BF7F70 (the class's only engine header is quarantined). A second definition exists outside engine/: include/candidates/
   landscape_map_open_compiled.h:36 (`class CEngineLandscapeMap`, a candidates model). Accept on the 0x00BFDDB0 precedent, or keep 0x00BFD390 on
   `struct S` (then its leaf still moves, but the Map read stays a stand-in)?
1. Land entry 1 as one set: rows 0x00BFD480 (+ test), 0x00BFDC90, 0x00BFD5D0, 0x00BFD390 and the header opt-in. Splitting it either leaves the table's
   slots 1 / 2 as second spellings (without the slot rows) or the opt-in unused.
2. Accept the two leaf-only slot renames inside the unit (the nf276 0x00BFDDB0 precedent).
3. 0x00BFD420 / 0x00AC1F60 findings: queue (NAME_FIXES.tsv).

## Flags / funclets / descriptive names / locks / order / reading / slips
- Flags: none (no row of the unit has CompilerFlags; none added).
- Funclets / path includers: 0x00BFDC90 now includes 0x00BFD480's file by path (a rename of that file must follow it; funclet_includes_check).
  Grep of rebuild/src, tests and include for all four file names: no includer (only engine/RECONCILE.tsv names one, as data), so no other row's
  source changes with them (0x00BE65A0's comment about reusing 0x00BFD5D0's helper refers to its own copy).
- Descriptive names: none new. `RelocatePointer<T>` in 0x00BFD5D0 is HEAD's made-up helper, kept (FableWin has a real `GFRelocateData<T>` fastcall
  template: a later port could use it; not measured).
- Locks (rows.txt): 00bfd480 00bfdc90 (released -> NF279), 00bfd5d0 00bfd390 (new lock dirs): entry 1, all compiled. 00be8e60 00bfbd80 (released
  -> NF279), 00ae04e0 00ae0540 00ae05a0 (new): entry 2, HEAD-measured (and form 2 for the first two); nothing of theirs in rows/: release all five.
  None of NF278's rows touched.
- Order: every compile ran after its lock was NF279; the new rows, fold and test, and the landed tests of the unit, passed a clang precheck in an
  earlier call than their grades (missing-include control caught). Grades launched in the background with `timeout 900` on the launch line.
  Entry 1's equiv_check finished before entry 2's first compile.
- Slips (all graded results stand; prechecks passed when run): (1) the HEAD sources graded as the `pH` side of gr.sh e1a / e1b / e1c (0x00BFD480,
  0x00BFDC90, 0x00BFD5D0, 0x00BFD390) were not prechecked before those grades; pre.sh on all four later: clean. (2) work/e1d.sh compiled the
  landed (HEAD) 0x00BFD480 test before the call that prechecked it (precheck later: clean). (3) gr.sh e1c graded two rows under one
  `timeout 900` launch line (each sc.sh inside also under `timeout 900`); the same shape in entry 2: work/e2h.sh (five HEAD grades) and
  work/e2f.sh (two form-2 grades) each ran under one `timeout 900` launch line (every grade inside also under `timeout 900`).
- Reading: this brief, NF277 / NF276 briefs, NF214_BRIEF.md, ASM_RULES.md, EXTRA_RULES.md, COORDINATION.md, GOTCHAS.md (all 503 lines), the nf276 /
  nf277 landing READMEs, the NF277 and NF276 hand-backs (NOTES, LESSONS, REJECTED).
- No git, no sleep, no rm, no __asm / naked / _emit; nothing written under /home/user/FableDecomp.
- Repo drift: 50 files under rebuild/ are newer than BRIEF.md (another landing: e.g. NF278's CMainGameComponent rows, landed_metrics.json); none
  under rebuild/include, src/compiled/00/bf, 00/be, 00/ae, tests/00/bf or build_candidates.ps1, so every measurement and recheck_final.log ran on
  the tree my rows land on; orig/ still equals the repo header.
- Recheck (recheck_final.log, overlay hdr/): 0x00BFD480 RELOCATION_MATCH PASS (new test), 0x00BFDC90 RELOCATION_MATCH PASS, 0x00BFD5D0
  RELOCATION_MATCH PASS (+ the pre-existing RELOC_NAME warning), 0x00BFD390 MATCH PASS. Form-2 rows of entry 2: evidence/e2_form2_vland.log.
