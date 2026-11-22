# NF274 notes (2026-11-22, round 31)

## Entry 1: 0x00BEB490's ??_GCWaterPatchMesh copy (nf272 ruling 1) -- measured, shape found; landing needs one ruling
Shape (M1, = rows/00beb490.cpp): 0x00BEB490's row defines FABLE_RETAIL_C2DCOORDI_COPY, includes 0x00BEAAA0's landed file
(`../src/compiled/00/be/CWaterPatchMesh_CWaterPatchMesh_00beaaa0.cpp`, the nf251 / 0x00BEB4D0 fold pattern), then its unchanged constructor.
The destructor body and the class operator delete (that file's FABLE_RETAIL_CWATERPATCHMESH_OPERATORDELETE) are then visible in the ctor's TU.
No header change. Measurements (evidence/e1_*):
- Ctor leaf: RELOCATION_MATCH before and after; cmpobj HEAD v new: `??0CWaterPatchMesh@@QAE@PBV...` SAME (bytes with relocation fields
  zeroed + every relocation symbol); leaf relocations Load 0x00BEAE50 / ??_7CWaterPatchMesh 0x012A8134, x1 each, before and after.
- Both emitted vtables (??_7CWaterPatchMesh 0x012A8134, ??_7CMovableResource 0x0129B860): slot for slot the same symbols as HEAD's
  (vtcheck; NOLINE = no table line, as at HEAD).
- ??_GCWaterPatchMesh@@UAEPAXI@Z: HEAD 30 bytes NEQ -> 100 bytes, pk EQ 0x00BEB4D0; cmpobj SAME against the 0x00BEB4D0 and 0x00BEAAA0
  objects (all four shared COMDATs SAME: ??1 / ??_G of both classes). Other copies unchanged (??_GCMovableResource EQ 0x009D5110).
- Link check of every emitter of the CWaterPatchMesh vtable (0x00BEB490, 0x00BEAAA0, 0x00BEB4D0; no other source defines a CWaterPatchMesh
  ctor / dtor: 0x00BF5A30 only calls the ctor): with /OPT:NOREF /MAP, HEAD set: slot 0 = 0x00BEB490's 30-byte copy when it links first;
  new set: every order (3 tried) keeps an EQ 100-byte copy. So no link order can give slot 0 a wrong body any more.
- Cost: the ctor object now DEFINES ??1CWaterPatchMesh@@UAE@XZ strongly (identical to 0x00BEAAA0's: cmpobj SAME, pk EQ 0x00BEAAA0). Per-object
  link: LNK4006 1 -> 2 (both on ??1, identical bodies; HEAD's 1 is the 0x00BEB4D0 fold, nf251 ruling 1). Symbol diff HEAD -> new: DEF ??1
  (was UND), new UND DeleteVertexBuffer / FreeVirtual / GPLandscapeRenderer (the destructor's callees; all landed spellings).
- Recheck (evidence/e1_recheck.log): 0x00BEB490 / 0x00BEAAA0 / 0x00BEB4D0 / 0x00BF5A30 RELOCATION_MATCH PASS (0x00BF5A30: WARN RELOC_NAME on
  the ??2CWaterPatchMesh catalog label at 0x00BEA7E0, pre-existing).
- Census: no new names; ~CWaterPatchMesh keeps one body text (0x00BEAAA0's file, now included by two rows). 0x00BEB490's file is
  included by no other file; 0x00BEAAA0's file is now included by two rows (0x00BEB4D0, 0x00BEB490): a rename of it must follow both.
- Not measured (reason): an inline-visible destructor only in the ctor row (inline in one TU, out of line in 0x00BEAAA0's: an ODR mismatch, and
  a second body text under one name); the ODR-clean form (destructor inline in CWaterPatchMesh.h, 0x00BEAAA0 on nf88 pick-any scaffolding,
  0x00BEB4D0 following) rewrites three rows and a header: a re-port, not this measurement.

## Entry 2: D4, one definition (0x00BDA150 / 0x00BD9CE0) -- done (S1); option S3 needs a ruling
Baseline (evidence/e2_baseline_head.txt): HEAD 0x00BDA150 + 0x00BD9CE0 = 4 LNK4006 (FreeVirtual + the three BlockWhileBusy helpers, all
strong, NODUPLICATES); + the owner 0x00BD9B90 = 5. pk: the TU-mate FreeVirtual copies are 68 bytes v owner 0x00BD9B90's 97 (NEQ); the
helper copies 72 / 72 / 75 v retail 0x00BD9C00 / 0x00BD9C20 / 0x00BD9CC0 (NEQ; uncatalogued, no landed row). cmpobj: the two rows' copies of
all four are SAME as each other.
- **S1 (rows/00bda150.cpp, rows/00bd9ce0.cpp):** `inline` on the four out-of-class definitions in each row's existing local model (in place;
  no new class, no header). Both objects: objsec HEAD v new ALL_EQ (50 sections, bytes + relocation symbols), cmpobj 49 / 49 SAME; the only
  object change is the four COMDAT selections, NODUPLICATES (1) -> ANY (2) (evidence/e2_s1_cmpobj.txt). VC7.1 still emits the four (the rows
  call them; every call is expanded). Link (evidence/e2_s1_links_objsec.txt, /OPT:NOREF /MAP): the pair 4 -> 0 duplicates; with HEAD
  0x00BD9CE0 the HEAD 4 remain (so both rows land together); + owner 0x00BD9B90: 1 (pair first: the 68-byte pick-any FreeVirtual is kept over
  the owner's strong 97-byte copy, as the 68-byte strong copy was at HEAD) or 2 (owner first: the owner's copy kept). The FreeVirtual copies
  are HEAD copies that already differed (nf203: debt, changed only in selection); a strict link (no /FORCE) would report strong + pick-any as LNK2005: inferred from the nf88 GOTCHAS line, not measured here.
  Marked `// Fidelity: scaffolding, not retail` (the two Fidelity lines reworded in place, line count kept); final rows regraded
  RELOCATION_MATCH, objsec ALL_EQ against both HEAD and the S1 objects (evidence/e2_final_objsec_dealloc.txt).
- **S3 (option, work/d4/s3_*.cpp, not in rows/):** S1 + `// Flags: /d2inlT114` (the owner 0x00BD9B90's own CompilerFlags, its smallest
  matching threshold) on both rows. Both leaves still RELOCATION_MATCH and cmpobj SAME; the FreeVirtual copies become 97 bytes, pk EQ
  0x00BD9B90; the helper copies grow to 101 / 101 / 104 (still NEQ, owner-less); objsec NOT_EQ only in those five COMDATs (Deallocate 63 -> 92,
  FreeVirtual, the helpers). Link (evidence/e2_s3_links_objsec.txt): pair 0 duplicates; + owner 1 or 2, and every order keeps an EQ FreeVirtual.
  Deallocate's copy (63 -> 92) goes from NEQ to EQ against 0x009A4350 (FableWin ?Deallocate@CMemoryAllocator@@SIXPAX@Z out of line), as
  the owner 0x00BD9B90's own copy is (evidence/e2_final_objsec_dealloc.txt); no copy that matched at HEAD stops matching. Measured: T114 agrees
  with retail for FreeVirtual, Deallocate and both leaves; no single measured setting reproduces all six functions (the helper copies are
  101 / 101 / 104 at T114, 72 / 72 / 75 at the default, retail 29 / 29 / 28). The work/d4/s3_*.cpp files still carry the old Fidelity text and
  were not run through vland: whoever lands S3 regenerates them from rows/ and adds the flag. Needs CompilerFlags
  `/d2inlT114` on both rows (GOTCHAS 449 / 499) and the size-pragma-scaffolding conditions read for a flag (owner sets the same flag: yes).
- **Retail's out-of-line 0x00BD9C00 / 0x00BD9C20 / 0x00BD9CC0 (probe P2, evidence/e2_p2_retail_helpers_pk.txt):** the helper bodies as
  written, with FreeVirtual declaration-only (its body not visible), give 29 / 29 / 28-byte copies, pk EQ to all three (relocations: the
  virtual destructor through slot 0 / CTexture::Uninitialise 0x009F9F70, then FreeVirtual 0x00BD9B90 called). So rows for them need only the
  local model with FreeVirtual declared, not defined (VB / IB still from Ego_r order only: the two bodies are byte-identical). No rows
  authored. Once such rows land strongly, the pick-any helper copies in 0x00BDA150 / 0x00BD9CE0 (NEQ) meet them; making the rows'
  helper copies match them would need FreeVirtual not visible, which changes both leaves (P2's FreeDisposalList: DIFFER 255v488).

## Entry 3: the CLandscapeLayerMesh siblings 0x00BFDE20 / 0x00BFDD70 / 0x00BFDE80 -- open (not started: context), scouted
- 0x00BFDE20's Leaf is `??1CLandscapeLayerMesh@@QAE@XZ` (non-virtual, on the local CMovableResource whose destructor is non-virtual).
  Both PDBs give `??1CLandscapeLayerMesh@@UAE@XZ`, and the table already binds that spelling at 00bfde20 (a HEAD split). The shared
  engine/CMovableResource.h has `virtual ~CMovableResource()`, so the port makes the destructor virtual: the leaf moves to UAE and the TU emits
  ??_7 / ??_G CLandscapeLayerMesh (vtable 0x012A835C, where the table also has the stand-in `?CLandscapeLayerMesh_vtbl@@3PAXA`). So "each object
  unchanged" cannot hold for this row (REJECTED.tsv); it is a self rename + vtable unit in the nf272 CWaterPatchMesh shape (vtcheck, pk of the new
  ??_G against 0x012A835C slot 0, link check). 0x00BFDD70 (`?OnPostRelocation@CLandscapeLayerMesh@@QAEXXZ`, both PDBs) and 0x00BFDE80
  (`?RelocateData@CLandscapeLayerMesh@@EAEXJPAX0J@Z`, both PDBs) were not measured; they also carry D5's AppendFreeEdgeNode / FreeBlock stand-ins.

## Census (names and addresses)
- New symbol NAMES: none. Entry 1: 0x00BEB490 newly DEFINES `??1CWaterPatchMesh@@UAE@XZ` (an existing name; its only body text is still
  0x00BEAAA0's file, which 0x00BEB490 now includes) and its `??_GCWaterPatchMesh` changes body (pick-any, now EQ). Entry 2: no DEF / UND change
  in either object (sym diff empty); only COMDAT selections.
- Per address: no binding changes anywhere (every leaf's relocation list equal to HEAD's; cmpobj SAME), so no spelling count moves and no
  split is created. RENAMES.tsv and TABLE_CHANGES.tsv are empty; NAME_FIXES.tsv has no entries (the 0x00BD9C00 / 0x00BD9C20 / 0x00BD9CC0 names
  are already in nf267's NAME_FIXES; P2 adds the byte evidence, NOTES above).
- No header touched: hdr/ and orig/ are empty, no equiv_check needed.

## Decisions for the coordinator
1. **Entry 1:** land 0x00BEB490 as the fold (rows/00beb490.cpp)? It ends the two-bodies-under-one-name state of `??_GCWaterPatchMesh` (every link
   order now keeps retail's 100-byte body), at the cost of one more strong `??1CWaterPatchMesh` copy (identical bodies; LNK4006 per object
   1 -> 2). The folded object is a superset of the destructor's TU (that TU + the constructor), not the same TU compiled again: how nf251's
   one-object-per-TU counting applies is yours to rule. Two rows then include 0x00BEAAA0's file by path; a rename of it must follow both.
2. **Entry 2:** land S1 (rows/: `inline` scaffolding, both objects ALL_EQ, pair 4 -> 0 duplicates) with the two rows together, or S3 (S1 +
   CompilerFlags `/d2inlT114` on both: FreeVirtual copies then EQ the owner 0x00BD9B90 in every link order)? S1 leaves FreeVirtual's pick-any
   copies NEQ against the strong owner (HEAD debt, selection changed only); the owner's own port to pick-any (nf88) would be the follow-up for
   either.
3. **Entry 3:** requeue as a self-rename + vtable unit for 0x00BFDE20 (QAE -> UAE, emitted ??_7 / ??_G CLandscapeLayerMesh), with
   0x00BFDD70 / 0x00BFDE80 as base ports and D5's stand-ins.

## Recheck (recheck_final.log, landed tree, no overlay)
0x00BEB490 / 0x00BEAAA0 / 0x00BEB4D0 / 0x00BF5A30 / 0x00BDA150 / 0x00BD9CE0 / 0x00BD9B90: 7 RELOCATION_MATCH PASS, 0 DIFFER, 0 RELOC_CONFLICT.
Warnings: RELOC_NAME on the ??2CWaterPatchMesh label (0x00BF5A30) and GetNoAllocations (the three disposal / FreeVirtual rows), SHARED_HEADER_AVAILABLE
on the disposal rows' local models (HEAD debt, unchanged).

## Flags / funclets / descriptive names / locks / order / reading / slips
- Flags: none changed in rows/ (S3 would add `/d2inlT114` to two CompilerFlags entries). Funclets / path includers: none of the three changed
  files is included by another file; 0x00BEB490 now includes 0x00BEAAA0's file. Descriptive names: none new.
- Locks (rows.txt): 0x00BEB490, 0x00BEAAA0, 0x00BEB4D0, 0x00BF5A30 (released -> NF274), 0x00BDA150, 0x00BD9CE0 (new lock dirs), 0x00BD9B90
  (released -> NF274). None of NF273's rows touched; 0x00BFDE20 / 0x00BFDD70 / 0x00BFDE80 not claimed (not compiled).
- Order: every compile ran after its lock was NF274, in a later call than a passing clang precheck (new and HEAD sources, all tests; pre.sh control
  with a missing include caught; 0x00BDA150's HEAD test fails clang only through VC7.1 `<list>` / `<functional>` / `<cwchar>`: a two-line `<list>`
  control gives the same three errors), each grade with `timeout 900` on its background launch line; entry 1's recheck finished before entry 2's
  first compile.
- Reading: this brief, NF272 / NF271 briefs, NF214_BRIEF.md, ASM_RULES.md, EXTRA_RULES.md, COORDINATION.md, GOTCHAS.md (all 503 lines), the nf267 /
  nf272 landing READMEs, the NF272 and NF267 hand-backs (NOTES, LESSONS, NAME_FIXES, HDR_README, evidence/link_order_GCWaterPatchMesh.txt), plus the
  nf251 README rulings.
- Self-check: $S/w22/nftools/selfcheck.py (NF214 brief) is not in nftools (only absrefs / apply_rows / callers / mkpay / xref / xrow.sh), so it was not run.
- Slips: the first two entry-1 link batches (lnkmap.py, a few seconds each) ran in the foreground instead of in the background with a kill -0
  check; every later link ran in the background. tools/lnkmap2.py added (lnkmap.py + /OPT:NOREF). No git, no sleep, no rm, no __asm / naked /
  _emit; nothing written under /home/user/FableDecomp.
