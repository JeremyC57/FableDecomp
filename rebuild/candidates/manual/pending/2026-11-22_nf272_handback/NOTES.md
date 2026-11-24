# NF272 notes (2026-11-22, round 30): entry 1 DONE (the CWaterPatchMesh unit with the 0x00BFDD40 split); entry 2 (D4) not started

Handed back after entry 1 (context). Six rows changed (4 self renames, 2 binder respells), two add-only header opt-ins, four tests (two rewritten
on the shared headers, one stub respelled, three self-contained host-model tests unchanged). One vland --recheck-landed run with the overlay on
(recheck_final.log): 0x00BEAA00 / 0x00BFDD40 / 0x00BF38F0 / 0x00BEAAA0 / 0x00BEB4D0 + witnesses 0x00BEB490 / 0x00BDA0D0 RELOCATION_MATCH PASS (7),
0x00BF4220 DIFFER(615v634) PASS (its HEAD grade: a July behaviour lift, nf46 "blocked, authoring"); 0 RELOC_CONFLICT. Warnings: STANDIN_LEAF on the
four self rows (the catalog names this hand-back renames), RELOC_NAME on 0x00BF38F0 (same), SHARED_HEADER_AVAILABLE on 0x00BF38F0 (its row-local
CEngineLandscapeMemoryManager, HEAD debt, unchanged). Repo root (`ls -A`) before / after: identical. No git, no sleep, no __asm / naked / _emit.

## First measurement (brief): what 0x00BF4220 binds for 0x00BEAA00
The HEAD object of 0x00BF4220 (no overlay) relocates both E8 sites of 0x00BEAA00 (0x00BF43C3 / 0x00BF445D, exe scan) to
`?ClearTextures@CWaterPatchMesh@@QAEXXZ` (x2): a member of the local `class CWaterPatchMesh { void ClearTextures(); }` in
candidates/landscape_patch_update_foreground_compiled.h (evidence/bf4220_head_binding.txt). xref.py prints nothing for it (the row grades DIFFER),
so objdump -r is the measurement. No table line existed for that stand-in. So 0x00BEAA00 had two spellings (self ClearBlock@CTextureOwner_00beaa00,
binder ClearTextures@CWaterPatchMesh); both now end on `?ReleaseBuffers@CWaterPatchMesh@@QAEXXZ`.

## The unit, three sub-families (each leaves its addresses on one spelling)
1. **0x00BEAA00 + 0x00BF4220 (ReleaseBuffers).** Self row ported onto the retail branch of engine/CWaterPatchMesh.h, which already declares
   `ReleaseBuffers()` public; its row-local `struct CTextureOwner_00beaa00` goes. objsec ALL_EQ with the one rename (2 sections; no .drectve change).
   Binder: in-place respell of the candidates header's existing local model under the new opt-in (hdr/README.md); the row defines the macro and calls
   ReleaseBuffers; a full Leaf line added (pinned from the HEAD object's defined symbol; HEAD had none). objsec ALL_EQ with the one rename
   (5 sections). The test defines the stub, so row + test + header land together (Q3). pk: its ~CCountedPointer copy NEQ at HEAD and after (debt).
2. **0x00BFDD40 + 0x00BF38F0 (DeleteForegroundVertexBuffer, the HEAD split).** CLandscapeLayerMesh's only header is quarantined (header-less for the
   local-model rule). 0x00BFDD40: row-local `class CLandscapeLayerMesh : public CMovableResource` in the landed siblings' shape (0x00BFDE20 /
   0x00BFDD70: IndexBuffer +0x34, ForegroundVertexBuffer +0x38, Ego_r names) but on the SHARED retail CMovableResource (engine/CMovableResource.h;
   the siblings model CMovableResource locally, which fails "base from a shared header": Q2). 0x00BF38F0: its local `struct CTextureOwner` (the chain
   element: virtual dtor, link +0x3C) becomes `class CLandscapeLayerMesh : public CMovableResource` (virtual ~CLandscapeLayerMesh, `Next` +0x3C,
   DeleteForegroundVertexBuffer); the local CEngineLandscapePatch's +0x08 member respelled `LayerMeshList` (Ego_r name; quarantined
   CEngineLandscapePatch.h). Proof: objsec ALL_EQ with the one rename for both (2 / 3 sections); DEF/UND diff: only that name moved (no vtable, no
   ??1CLandscapeLayerMesh emitted). Site 0x00BF3773 also calls 0x00BFDD40 but lies in no landed row. A full Leaf line added to 0x00BF38F0 (HEAD none).
3. **0x00BEAAA0 + 0x00BEB4D0 (~CWaterPatchMesh, ??_G).** 0x00BEAAA0 leaves candidates/outline_glow_compiled.h (cast vtable immediates, stand-in
   members Blur2Texture / field_10) for engine/CWaterPatchMesh.h's retail class (virtual ~CWaterPatchMesh already declared) plus the new
   `_OPERATORDELETE` opt-in. The TU now also emits ??_7CWaterPatchMesh / ??_GCWaterPatchMesh / ??_7CMovableResource / ??_GCMovableResource /
   ??1CMovableResource (pick-any). 0x00BEB4D0 is that TU folded (`#include "../src/compiled/00/be/CWaterPatchMesh_CWaterPatchMesh_00beaaa0.cpp"`, no
   code of its own: the nf251 0x00B2AB10 pattern; retail's ??_G inlines the destructor, so only the destructor's TU can produce it). objsec ALL_EQ is
   not available here: proof instead (evidence/leafcmp_*, pk, vtcheck_unit3.txt):
   - leaf bytes (relocation fields zeroed) equal HEAD's, 71 = 71 and 100 = 100; the only relocation change is the two vptr stores (+10 and
     +65 / +70), cast immediates at HEAD, now `??_7CWaterPatchMesh@@6B@` / `??_7CMovableResource@@6B@` (both tabled: 012a8134 / 0129b860);
   - pk.py: ??_GCWaterPatchMesh EQ 0x00BEB4D0 (100), ??_GCMovableResource EQ 0x009D5110 (31), the fold's ??1CWaterPatchMesh EQ 0x00BEAAA0 (71);
     ??1CMovableResource (7 bytes, no retail owner) is already defined by 0x00BEB490 at HEAD;
   - vtcheck: both emitted tables equal 0x00BEB490's HEAD tables slot for slot (slot targets 00beb4d0 / 00beab20 / 00beaa50 / 009d5100 and
     009d5110 / 00bfea08 / 009d50f0 / 009d5100; slot 0 is the ??_E weak alias of ??_G, as in 0x00BEB490);
   - the fold include resolved only through the overlay copy (no repo file has the post-rename name), so the grade read it.
   Tests: both rewritten on the shared headers and link the row: 0x00BEAAA0's calls the destructor non-virtually over 4 member cases (frees,
   clears, DeleteVertexBuffer while the derived vptr is in place, ends on another vptr, no free); 0x00BEB4D0's captures the derived vptr while the
   row's destructor runs, then reaches ??_G through vtable slot 0 (`delete` = flag 1: FreeVirtual(this); unqualified destructor call = flag 0: none).
   PassPatterns kept. Their TUs define no CWaterPatchMesh ctor / dtor, so by construction they emit no vtable / ??_G of their own (not objdump'd:
   vland builds the test objects inside its own work dir).
   candidates/outline_glow_compiled.h has no includer left after this (both rows and both tests moved off): the coordinator may retire it.

## Census (objects, exe scan, table, source)
- 0x00BEAA00: 2 spellings -> 1 (self + 0x00BF4220 on ReleaseBuffers). 0x00BFDD40: 3 -> 1 (self `_00bfdd40` stand-in, binder `ClearBlock@CTextureOwner`
  -> DeleteForegroundVertexBuffer). 0x00BEAAA0: self `??1CEngineScreenEffectOutlineGlow` -> `??1CWaterPatchMesh@@UAE@XZ`, which 0x00BEB490's HEAD
  ??_G copy already called (UND), so 2 -> 1. 0x00BEB4D0: self stand-in -> `??_GCWaterPatchMesh@@UAEPAXI@Z`, which 0x00BEB490's vtable slot 0 already
  bound, so 2 -> 1. No address grows; no split created.
- New NAMES (source grep of src / tests / include, comments excluded): ReleaseBuffers and DeleteForegroundVertexBuffer have exactly one body (the self
  rows); ~CWaterPatchMesh one body (0x00BEAAA0; the fold 0x00BEB4D0 is the same TU). ??_GCWaterPatchMesh: EQ pick-any copies in 0x00BEAAA0 /
  0x00BEB4D0, plus 0x00BEB490's HEAD copy (30 bytes, NEQ): Q1.
- Local models of CWaterPatchMesh elsewhere (0x00BEAB20, 0x00BEA7E0, 0x00B77E20, 0x00BF3980, 0x00BF44A0 and several tests) only DECLARE members
  (no ctor / dtor body), so they emit no ??1 / ??_G / ??_7CWaterPatchMesh: the ??_G bodies are exactly the three listed above.
- Per-slot status of the emitted tables (identical to 0x00BEB490's HEAD tables, so carried debt, not created here): 0x012A8134 slot 0 ??_E -> ??_G =
  0x00BEB4D0 (now defined; Q1 for the second body); slot 1 ?RelocateData@CWaterPatchMesh@@EAEXJPAX0J@Z: no landed definer (0x00BEAB20's self leaf
  is UAE) -> unresolved at link; slot 2 ?CanRelocateData@CWaterPatchMesh@@EBE_NJ@Z: no landed definer (0x00BEAA50 defines a CEngineLandscapePatch
  stand-in) -> unresolved; slot 3 / 0x0129B860 slot 3 ?Verify@CMovableResource@@UBEKXZ: no landed definer (0x009D5100 a free fastcall stand-in);
  0x0129B860 slot 0 ??_GCMovableResource (pick-any, EQ 0x009D5110; the landed self row defines it too), slot 1 __purecall (CRT), slot 2
  ?CanRelocateData@CMovableResource@@UBE_NJ@Z: no landed definer (0x009D50F0 a C009d50f0::M stand-in). The four self-row fixes are NAME_FIXES.
- Leaf comparison (evidence/leafcmp_00beaaa0_00beb4d0.txt, every relocation field of either object zeroed): 71 = 71 and 100 = 100, no differing
  offset; relocations only in the new objects: the two vptr stores; none only in HEAD.
- Links (LINK unset, /NODEFAULTLIB /FORCE /OPT:NOREF /MAP, tools/lnkmap.py; evidence/link_check.txt): the five non-fold new objects + HEAD
  0x00BEB490: 0 duplicates; + the fold 0x00BEB4D0: 1 LNK4006 on ??1CWaterPatchMesh (identical body: nf251 ruling 1, one object per TU); control
  (0x00BEAA00 twice): 1; the HEAD set: 0 (and at HEAD slot 0 resolved to 0x00BEB490's 30-byte copy, the only ??_G body then).
- Table: TABLE_CHANGES.tsv (ClearBlock@CTextureOwner line -> the PDB line; ReleaseBuffers optional).

## Decisions for the coordinator
- **Q1 (link order, HEAD debt made visible):** 0x00BEB490 (ctor, unchanged) emits a pick-any `??_GCWaterPatchMesh@@UAEPAXI@Z` that calls ??1 and
  `::operator delete` (30 bytes, pk NEQ v 0x00BEB4D0's 100; it frees through the wrong allocator). It did so at HEAD, where it was the ONLY body for
  slot 0 (and its ??1 call was unresolved). Now two bodies share the name and the link order decides (evidence/link_order_GCWaterPatchMesh.txt:
  0x00BEB490 first -> its copy; 0x00BEAAA0 first -> the retail copy). No spelling on the ctor row removes it (its TU cannot see the destructor body
  without a second strong ~CWaterPatchMesh). So "no slot resolves to a wrong body" is NOT clean; it is the nf203 "copy that differed at HEAD stays
  debt" case. Options: (a) accept as debt and queue the one-TU-for-several-addresses rule (nf251) for CWaterPatchMesh's ctor / dtor / ??_G;
  (b) also define `_OPERATORDELETE` in 0x00BEB490 so its copy at least frees through FreeVirtual (bytes still NEQ; its own leaf unchanged): not done.
- **Q2 (local models, quarantined class):** 0x00BFDD40 / 0x00BF38F0 model CLandscapeLayerMesh locally (quarantined header) on the shared retail
  CMovableResource; the siblings 0x00BFDE20 / 0x00BFDD70 / 0x00BFDE80 sit on a local CMovableResource (a class with a shared header). Accept, with
  0x00BF38F0's stand-in-to-real-token rename justified by the quarantine rule? Queue the siblings' base port.
- **Q3 (landing sets):** {0x00BEAA00, 0x00BF4220 + test + candidates-header opt-in} together (the test defines the stub; a HEAD test against the new row
  fails to link); {0x00BFDD40, 0x00BF38F0}; {0x00BEAAA0, 0x00BEB4D0, both tests, CWaterPatchMesh.h opt-in} together, with 0x00BEAAA0's file renamed
  to `CWaterPatchMesh_CWaterPatchMesh_00beaaa0.cpp` (the name 0x00BEB4D0's include uses; rename the include if batch_rerow picks another), and
  funclet_includes_check run after the move.
- **NAME_FIXES:** 0x00BEAA30 is `?OnPostRelocation@CWaterPatchMesh@@QAEXXZ` (catalog says CBankFileAsyncData::ChangeLoadingPriority; the table already
  binds the PDB spelling there through 0x00BF3980); 0x00BEAB20's self leaf is public UAE where both PDBs, the header and the vtables say private EAE.

## Not started
Entry 2 (D4 one definition in 0x00BDA150 / 0x00BD9CE0): APPLIED `open`.

## Flags / funclets / descriptive names / locks / order / reading / slips
- Flags: none (no CompilerFlags entry on any of the six). Path includers of the six old files: none. New path include: 0x00BEB4D0 -> 0x00BEAAA0.
  Descriptive names: none new (LayerMeshList / Next / IndexBuffer / ForegroundVertexBuffer are Ego_r member names). Leaf lines: added to 0x00BF4220
  and 0x00BF38F0, pinned from the HEAD objects. Host-model tests left unchanged: 0x00BEAA00, 0x00BFDD40, 0x00BF38F0 (self-contained; they still
  carry ClearBlock / CTextureOwner in their own models: the queued host-model test debt).
- Locks (rows.txt, NF272): the six rows (released -> NF272), witnesses 0x00BEB490 / 0x00BF5A30 (new lock dirs) and 0x00BDA0D0 (released -> NF272).
  0x00BF5A30 was compiled once at HEAD only (control of its symbols). None of NF270's rows touched.
- Order: every compile ran after its lock was NF272, in a later call than a passing clang precheck (new and HEAD sources, new and HEAD tests,
  witnesses; pre.sh control with a missing include caught), with `timeout 900` on each background launch line (and inside hk/gr/rf); equiv_check
  under `timeout 2700`, finished before handing back.
- Reading: this brief, NF271 / NF269 briefs, NF214_BRIEF.md, ASM_RULES.md, EXTRA_RULES.md, COORDINATION.md, GOTCHAS.md (all 503 lines), the nf267 /
  nf269 / nf271 landing READMEs, the NF269 and NF271 hand-backs (NOTES, LESSONS, NAME_FIXES), plus the nf251 README rulings for the fold.
- Slips: none known. (One `clean.py` on work/lnk, then re-created; helper tools/lnkmap.py added: lnk.py + /OPT:NOREF /MAP.)
