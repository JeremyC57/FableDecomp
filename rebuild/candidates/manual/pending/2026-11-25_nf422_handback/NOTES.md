# NF422 hand-back (2026-11-25): the C3DAnimFile / C3DMeshFile default constructors and loaders folded into the b7 units

| batch | rows | verdict | one line |
|---|---|---|---|
| b1 | 0x00A98E60 0x00A98F40 0x00A99420 0x00A99510 0x00A9A730 / 0x00A9AD80 0x00A9AE60 0x00A9B640 0x00A9B740 0x00A9D2A0 | ACCEPT | two five-member Option S units, re-owned by the default constructors; loaders call the private ctors; 6 add-only header opt-ins; 2 table replaces |
| b2 | 0x00A99E30 0x00A9C9C0 | ACCEPT | GetChunkDescription selves respelt to the PDB virtual members (vftable slot 1); lands WITH b1 |

No HOLD rows. All 12 rows stay locked as NF422. One ruling asked (RULINGS_NEEDED.tsv Q1: the per-row NEW_NEQ reading of the
implicit ??1CDataInputStream copy, which is SAME as HEAD at unit level). Nothing written to the repo, no git, no `wineserver -k`.

## What changed
- **Units (Option S).** One retail .obj slice = one TU:
  - lib_3d_anim_file.obj: owner 0x00A98E60 `C3DAnimFile::C3DAnimFile()`, then 0x00A98F40 (private root ctor), 0x00A99420
    `LoadAnimFile`, 0x00A99510 `LoadAnimFileCompressed`, and the compiler-generated 0x00A9A730 `??_GC3DAnimFile`.
  - lib_3d_mesh_file.obj: owner 0x00A9AD80, then 0x00A9AE60, 0x00A9B640, 0x00A9B740, 0x00A9D2A0.
  - Membership: Ego_r.pdb lists all five in the .obj (anim indexes 13/14/22/23/36, mesh 9/10/22/23/43), and so does FableWin.pdb.
  - The owner is the lowest member address (re-owned from NF419's 0x00A98F40 / 0x00A9AE60, now thin).
  - Bodies are in retail address order. No pragma or inline device is needed.
- **Why the loaders must be in the unit (measured, evidence/fold_probe.txt).**
  - With the root ctor only declared, 0x00A99420 is DIFFER(244v226): c2 reloads the root pointer's count block where retail keeps
    it in ebp.
  - With LoadAnimFile only declared, 0x00A99510 is DIFFER(268v250).
  - HEAD solved this by defining the ctor (and LoadAnimFile) in each loader's own TU under the public stand-in name, which gave
    strong duplicates plus the thunk-calling ??_G.
- **Header opt-ins (b1/hdr/engine, originals in orig/hdr_<Name>.h).** All are add-only and no existing row or header uses the
  macros. Static asserts are pinned with `_AT(<HEAD line>)`, so the typedef names do not move.
  - C3DAnimFile.h `FABLE_RETAIL_C3DANIMFILE_LOADER_FRIEND` and C3DMeshFile.h `FABLE_RETAIL_C3DMESHFILE_LOADER_FRIEND`: the loaders
    call the PRIVATE ctor (AAE in both PDBs; retail 0x00A99478 / 0x00A9B699), so the loader is a friend (access only).
  - C3DAnimFileRootChunk.h `FABLE_RETAIL_C3DANIMFILEROOTCHUNK_INLINE_CTOR` makes two changes:
    - the inline `"ANRT"` default ctor, which retail expands at 0x00A98E77; the mesh header already has its `"3DRT"` twin;
    - the IMPLICIT destructor: ??_G 0x00A9A2E0 calls ~CChunkedFileSuperChunk 0x00AA8490 directly, and the PDB release .obj has no
      ~C3DAnimFileRootChunk proc.
  - CChunkedFile.h `FABLE_RETAIL_CCHUNKEDFILE_SETTERS`: FableWin `SetVersion` / `SetFileComment`, inline. Retail expands them in
    the ctors and also keeps out-of-line copies at 0x00A98800 / 0x00A98810 (see homes below).
  - CChunkedFileLoader.h `FABLE_RETAIL_CCHUNKEDFILELOADER_LOADCHUNKEDFILE`: protected `LoadChunkedFile` (both PDBs IAE; table
    0x00AA79C0).
  - C3DMeshFileLoader.h `FABLE_RETAIL_C3DMESHFILELOADER_LOAD`: the two Load* declarations (both PDBs).
  - The mesh loader's method-only views (`CInputStreamMethods`, `CChunkPrototypeMethods`) and the anim rows' local stream /
    chunk models are gone. Streams now come from engine/CMemoryDataInputStream.h; `RootChunkType` from the loader headers.
- **b2.** The unit's vftable `??_7C3DAnimFileRootChunk` (now the retail 11 slots) references slot 1
  `?GetChunkDescription@C3DAnimFileRootChunk@@UBE?BVCCharString@@XZ`.
  - Row 0x00A99E30 defined it under a free stand-in class, so b1 alone adds one newly unresolved census name.
  - b2 respells 0x00A99E30 and its mesh twin 0x00A9C9C0. Bytes and relocations are identical; only the COMDAT name changes.
  - The mesh name was already unresolved at HEAD (0x00A9BBE0 emits that vftable).

## Gates (b1/CHECKS.tsv, b2/CHECKS.tsv; evidence under each batch)
- **vland --recheck-landed.** HEAD and new at one path, RELOCATION_MATCH PASS x12, plus a strict-leaf run on new; 0 LITERAL_DIFF.
  No new warnings; all 37 SHARED_HEADER_AVAILABLE warnings are gone.
- **Tests.** All 12 tests now link the row/unit object, with /alternatename stubs for the unit's full external set. They replace
  6 self-contained models and 4 NF419 tests that no longer link against the widened unit.
  - Per row they check every observable store, call and order:
    - the reference counts, with and without the base keeping a reference;
    - the DeleteData / ??_E path into the real ??_GC3D*RootChunk;
    - both LoadAnimFileCompressed paths;
    - SetPosition(GetPosition()) for mesh.
  - One mutant per row FAILs (evidence/mutation_controls*.txt). For ??_GC3DMeshFile the mutant uses a mutation-only header, kept
    as evidence/mutovl_C3DMeshFile.h.
- **pk_all + pickany_vs_retail.** Run per member leaf, then compared at UNIT level, because per-row diffs only show the TU
  composition change (tools/unitcmp.py).
  - Results: 0 regressions and no NEQ copy reached from a leaf.
  - Gone: the QAE stand-in definitions and the ??1C3DAnimFile / ??1C3DMeshFile thunks (NEQ).
  - ?DeleteData@?$CCountedPointer@VC3D*FileRootChunk: NEQ -> EQ.
  - The one NEQ copy left (??1CMemoryDataInputStream beside it is NOADDR), ??1CDataInputStream@@UAE@XZ, is SAME as HEAD 0x00A99510 / 0x00A9B740. It is byte-and-reloc identical
    in 6 census objects (R10), and it is NEQ only because the table binds that name to ~CDataStream (NAME_FIXES 00993b10).
- **Copies new to the unit (binned).**
  - ?SetFileComment: 8 B, with a UNIQUE reloc-identical retail home at 0x00A98810.
  - ?SetVersion: 10 B, reloc-free; retail copy at 0x00A98800, adjacent to SetFileComment.
  - ??C?$CCountedPointer<root>: 3 B, reloc-free; the mesh one is also tier 1 (0x00A9B840 / 0x00A9BBE0).
  - ??_GC3D*FileRootChunk: the local-model body became the retail one (reloc-identical at the vftable slot-0 targets 0x00A9A2E0 /
    0x00A9CE20).
- **vftables (pk does not walk them; evidence/vftables*.txt).**
  - ??_7C3DAnimFileRootChunk / ??_7C3DMeshFileRootChunk: 1 stand-in slot -> 11 slots. Every slot targets the same function as
    retail 0x0129DFF0 / 0x0129E090, and the next retail dword is string data.
  - 6 slots per table are bound under other rows' stand-in names (NAME_FIXES).
- **objsec (by COMDAT).**
  - Data changes: only the root-chunk vftables and pooled literals.
  - Removed: only the loaders' QAE ctor definitions and ??1 thunks.
  - Thin objects are identical to their owner objects.
- **/FORCE census link.** Two sets, one object per TU, each run in address order and in reverse (identical results):
  - Full: every landed row from 0x00A2E760 to 0x00ADECC0 (all lib_3d_* / lib_chunked_file), 3,559 TU objects, all compiled.
    b1+b2: duplicates 235 -> 231 (0 new; gone: the QAE root ctors and LoadAnimFile / LoadMeshFile that HEAD defined in two loader
    TUs each); unresolved 1,882 -> 1,879 (0 newly unresolved; resolved ??1C3D*FileRootChunk and the mesh GetChunkDescription).
  - Narrow (rows naming C3DAnimFile / C3DMeshFile / CChunkedFile* plus 0x00A98000-0x00AAA000, 757 TUs): b1+b2 duplicates 80 -> 76,
    unresolved 692 -> 689; b1 alone adds 1 newly unresolved name (the b2 name above), so b1 and b2 land together.
  - Definers on the full set in address order: ??_GC3D*File, the AAE root ctors and the default ctors only in the units (EQ to
    the unit); no ??1C3D*File anywhere; the QAE root-ctor spellings are defined nowhere.
- **Other gates.**
  - s_unit_check --objdir on a shadow root: OK x2. The loaders carry ?$ in their symbols, so the tool lists them as implicit;
    address order was checked by hand.
  - equiv_check over the 1,402 rows reaching the 6 headers: 0 differ. Positive control (one static assert un-pinned): DIFF reported.
  - apply_table --dry: 2 replaces, 0 misses.
  - reserved_check: 0.

## Debt exposed, not fixed (T3.txt new lines, NAME_FIXES.tsv)
- 0x00A98AB0 / 0x00A999B0 (anim) and 0x00A9A8C0 (mesh) still emit DIFF root-chunk vftables / ??_G from local models. 0x00A98AB0 /
  0x00A9A8C0 sort before the units in an address-ordered link, so their vftable wins there. This is the same as HEAD; the
  follow-up is to fold them into the units.
- Stand-in selves behind the vftable slots: 0x00A9A2E0, 0x00A9CE20, 0x00AA76B0, 0x00AA76C0, 0x00AA8460.
- Stale table line ??1CDataInputStream -> 0x00993B10 (witness 0x00A4FE60): the witness already calls ??1CDataStream and none of the 3,559 census objects has a .text reference to the name; delete it once a repo-wide check agrees.
- Catalog DeleteViaVtable0 labels on 0x00A99FE0 / 0x00A9CB20.

## Task 3
The catalog (auto-re-candidates.tsv, the file recheck / vland read) already spells `scalar deleting destructor` for 0x00A9A730 /
0x00A9D2A0. It was updated at 07:39 today, after my HEAD payload was cut. No edit is needed; T3.txt gives the optional label-file
edits.

## Names
Every name has two sources: PDB name sets plus .obj membership / vftable slot / retail call site. All are exact PDB spellings and
were confirmed with objdump. There are no descriptive names.

## Tools (evidence/tools in b1)
- ug.py: one-compile strict grader.
- tt.py: unit test runner with mutations.
- mktests.py / mkthin.py: generators.
- unitcmp.py: unit-level pk / copy binning.
- vt.py: vftable v retail.
- home.py: reloc-masked retail home scan with relocation-target identity.
- definers.py: census definers in link order.
- pkpair.sh, cen_run.sh.
- Reused from NF419 (path-adjusted): runpair.sh, census.py, link.py, objsec.py, vcmp.py, pkflip2.py, rdis.py, common.py.
