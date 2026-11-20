# NF142 notes (round 10, PRIORITY slice 11 fifth pass: 8 queue lines)

Counts (APPLIED.tsv / STATUS.tsv / recheck_final.log):
- Entries (8): 7 done (q4291, q4292, q6168, q4295, q4299, q4659, q6305), 1 blocked (q4296: row 0x009E15E0 left). 0 done-already, 0 rejected,
  0 deferred, 0 open. The done entries close once nf139 (their self rows, already in the working tree) and this hand-back both land.
- Rows (STATUS.tsv, 17): 16 ported (all RELOCATION_MATCH + PASS, one recheck_final.log run), 0 not-a-binder, 1 blocked (0x009E15E0).
  The 13 rows.txt rows are all ported; 4 more were claimed (locks `released`, NF139's blocked rows): 0x009F61C0, 0x0041BB00, 0x0122A600
  (ported) and 0x009E15E0 (blocked). None is `handed off` / `reserved (acctB` in UNMATCHED_ROWS.tsv.
- RENAMES: 5 section-1 self renames (0x0040F0E0, 0x00A10E10 leaf-only; 0x009F61C0, 0x0041BB00, 0x0122A600 identity), 25 section-2 lines.
- recheck_final.log: 16 rows, 16 RELOCATION_MATCH + PASS; no HARD / RELOC_CONFLICT / HEADER_POLICY / RELOC_CHECK_ERROR. selfcheck.log: 0 problems.
- NAME_FIXES: 2 lines (existing self-vs-binder splits found on the way, not changed). Header opt-ins: none (hdr/README.md).
- grades_used in STATUS counts the new-form grades plus the vland grade (HEAD reference compiles in pdiff are not counted; work/grades.log
  lists the q.py grades). No row needed more than one form except 0x009E15E0 (blocked).
- Stale-row check (no git: file mtimes): REFS rows modified on 2026-11-20 are nf139's / nf136's own rows (already probed by them) plus
  0x004299A8 / 0x00643CDA / 0x00646762 / 0x00F37170 (NF139 re-probed); 0x009E4420 (nf136, 06:34) re-probed here: binds ??1CInputEvent.

## Coordinator decisions
1. **Self renames of rows with landed referencers (the brief's literal rule covers only unreferenced addresses).** 0x0040F0E0 and 0x00A10E10
   move their own leaf onto the spelling EVERY exe-scan referencer already binds (0x0042EC7C; 0x009FAF70 / 0x00B665E0 / 0x00B72180 /
   0x00B8B780 / 0x00F38930, each re-probed at HEAD). Both PDBs carry the new spellings. They close existing self-vs-binder splits and
   create none. If you prefer the literal rule, drop the two rows: 0x0040F0E0 is then the only binder left of q4291 (0x00405650 alone still
   respells), and 0x00A10E10 the only binder left of q6305.
2. **0x0122A600 (atexit destructor of GFontTextureCache).** Authored the NF92 0x01228CE0 way: the static defined in the row (anonymous
   namespace, as the landed font rows 0x009A3640 / 0x009A25D0 bind it), leaf `_$E2`. The data binding at 0x013C5A80 becomes
   ?GFontTextureCache@?A0x<this TU's hash>@@3VCTexture@@A in place of the stand-in ?g_AutoTinyThunkObject_013c5a80@@3UAutoTinyThunkTarget@@A.
   0x013C5A80 already carries two TU-hashed spellings (?A0x837960d0, ?A0x4d07cd46) plus stand-ins; the count does not grow, but the gate
   may report the new hash: please confirm. Its initializer 0x01223EE0 still defines the CStatic_009f9f70 stand-in (not changed).
3. **Compiler-generated scalar deleting destructors (0x009F61C0, 0x0041BB00).** Both re-authored as the nf86 pick-any scaffolding
   (`delete` of the header class in an anonymous-namespace helper, `// Fidelity: scaffolding, not retail`). Identity: FableWin.pdb
   ??_GCInputEvent@@QAEPAXI@Z (0x030D4AE0, beside ??_GCInputEventPackage) / ??_GCTexture@@QAEPAXI@Z (0x018BC3F0), plus the bodies equal
   VC7.1's emitted ??_G for the header classes; no landed row binds either address (0x00784260's exe-scan hit on 0x0041BB00 is a byte coincidence at +0x108:
   `e8 77ac2100` then `bb 41000000` = the dword 0x0041BB00 across an instruction boundary; its object binds nothing there). Rows that now emit ??_GCInputEvent / ??_GCTexture as pick-any COMDATs (0x009E45D0, and many earlier rows) link beside them.

4. **0x009E15E0 (q4296) is a landed row that does not match at HEAD.** vector<CQuickDrawTriInfo>::erase grades DIFFER(99v99) in this lane
   with its own leaf ?Erase@CQuickDrawTriInfoVectorView@@QAEPAUCQuickDrawTriInfoEraseView@@PAU2@0@Z (register colouring: retail keeps the new
   end in ebx and the old end in ebp, the row swaps them; the row has no Leaf line, so landing-time grades fell back to the largest function).
   The respelled draft (work/009e15e0.cpp: CTextureEraseView / CWideStringEraseView -> CTexture / CWideString) binds the claim and stays at
   DIFFER(99v99), so it is not handed back. Needs a re-author on engine/CQuickDrawTriInfo.h (the STLport erase its caller 0x009DA9F0 binds):
   authoring. Lock claimed (was released); the row is untouched. q4296 stays blocked on it.

## Rows and evidence
- 0x00405650 CIME::Initialise: CSystemManagerNative / CDisplayManagerNative / local C2DExtentsI -> engine/CSystemManager.h + CDisplayManager.h +
  C2DExtentsI.h (`C2DExtentsI dimensions = GFGetSystemManager()->GetDisplayManager()->GetRenderTargetDimensions();`). Also drops the stand-in
  ?GFGetSystemManager@@YIPAUCSystemManagerNative@@XZ at 0x009A4EC0. Object gains only the inline COMDATs ?Get@CCountedPointer<CDisplayManager> /
  ?GetDisplayManager@CSystemManager. Leaf line added (the row had none).
- 0x0040F0E0 ResetDimensions: onto engine/NUISystem_CUIScreenUnitConverter.h (RESETDIMENSIONS + ORIGINAL_DIMENSIONS); g_dimW / g_dimH ->
  the private statics BackBufferWidth / Height (NF114 spellings, already bound by 0x004299A8).
- 0x00499D90 / 0x009C1050: in-place respells inside their HEAD-local models (BeginRender void -> bool; CSurface::Release -> Uninitialise);
  defined-symbol sets identical to HEAD. 0x009C1050's standalone test respelled the same way.
- 0x0066CA80: Temp -> engine/CPixelFormat.h, SubObj -> engine/CSurface.h member (InitialiseAsImage / LoadFromBitmapFileToPreallocatedSurface,
  the header spellings other rows bind). The HEAD-local CRegionMinimap / CWideString models are otherwise unchanged (pre-existing).
- 0x009C1AC0 / 0x009C2520 (byte twins): typed `CQuickDrawTriInfo* self`, `self->~CQuickDrawTriInfo()` (Ego_r.pdb CQuickDrawTriInfo is the
  only PDB class with CWideString +0x14 / CTexture +0x18; FableWin.pdb has ??1CQuickDrawTriInfo@@QAE@XZ, Ego_r none: inline). Own extern "C"
  leaves kept (one of the twins may be the real ??1CQuickDrawTriInfo; not settled). The base call now binds ??1CWideString@@QAE@XZ at
  0x0099B510 instead of @FableBaseDtor_...@4 (a PDB spelling replacing a stand-in; the address's spelling count shrinks).
- 0x009E45D0: engine/CMaintainedEvent.h (Ego_r.pdb CMaintainedEvent: CBaseClass + CInputEvent Event +4; its ??_G 0x009E45F0 expands the same
  calls): `self->Event.~CInputEvent(); self->CBaseClass::~CBaseClass();`.
- 0x0082CD00: SMember08_24 -> engine/CTexture.h (CMinimapData::MinimapTexture +0x24); CMinimapData has no shared header (row-local kept).
- 0x00A10E10: NF37's DetachZBuffer form on engine/CRenderTarget.h (GETD3DSURFACE).
- 0x0066C7A0: the stand-in S's +0x24 member is CSurface (engine/CSurface.h). Own leaf kept: its caller 0x00829D90 binds
  ?FreeData@CRegionMinimap@@QAEXXZ, a name in NO PDB; Ego_r.pdb has two 16-byte candidates (?Deinitialise@CRegionMinimap@@QAEXXZ 0x006A12A0,
  ?ReleaseSurface@CRegionMinimap@@QAEXXZ 0x006A1800). One source each, so no NAME_FIXES line: an identity item for a later pass.
- 0x00B4B830: CShaderHolder_009f2e20 -> CSurface; local CTexture / CRenderTarget models -> engine/CTexture.h / CRenderTarget.h (gap +0x2F0 =
  RenderSurface re-padded for the real 8-byte CTexture). Its two binders spell the address differently (0x00B4FC10 QAEXXZ, 0x00B4D210
  UAEXXZ): existing split, not changed.
- 0x00642A60: CHolderBase / CHolder stand-ins -> engine/CSurface.h (COPYFROMMEMORY inline virtual destructor), vtable 0x0122F84C now binds
  ??_7CSurface@@6B@ (as 0x004979A0); local CTexture -> engine/CTexture.h. Own stand-in leaf kept (NAME_FIXES: ??1CMinimapData).
- Objects: every ported row defines its own leaf plus only shared-header inline / pick-any COMDATs (STATUS notes list them); none defines a
  symbol another landed row owns strongly (checked for ??1CQuickDrawTriInfo, ??_GCInputEvent, ??_GCTexture, ??_GCSurface / ??1CSurface).

## Tests
- Rewritten on the shared headers (each links the row; PassPattern unchanged): 0x0040F0E0, 0x009C1AC0, 0x009C2520, 0x009E45D0, 0x00A10E10,
  0x0066C7A0. 0x009F61C0 / 0x0041BB00 / 0x0122A600: rewritten on the headers (a delete / a static teardown that runs the same pick-any
  COMDAT the row emits; 0x0041BB00's old test was print-only).
- Respelled in place: 0x009C1050 (its standalone local CSurface model).
- Unchanged, standalone (self-contained link, still PASS): 0x00405650, 0x00499D90, 0x0066CA80, 0x0082CD00, 0x00B4B830, 0x00642A60.

## Files the coordinator may rename
global_ResetDimensions_0040f0e0.cpp (now NUISystem::CUIScreenUnitConverter::ResetDimensions), CLandscapeBackgroundPatch_vectordeletingdestructor_009f61c0.cpp
(CInputEvent scalar deleting destructor), CLandscapeBackgroundPatch_vectordeletingdestructor_0041bb00.cpp (CTexture scalar deleting
destructor), global_GlobalMethodTailThunk0122a600_0122a600.cpp (atexit destructor of GFontTextureCache). No EH funclet or other source
includes any changed row (grep of rebuild/src, rebuild/tests).

## Descriptive names
No new made-up names. The anonymous-namespace helpers ReferenceInputEventScalarDeletingDestructor / ReferenceTextureScalarDeletingDestructor
are TU-local scaffolding (hashed, never called), as nf86 / nf88.

## Flags lines
None added. 0x0082CD00 carries `// Flags: /Gr` with empty catalog CompilerFlags; HEAD grades RELOCATION_MATCH both with and without /Gr
(the template carries __fastcall), so nothing changes.

## Rule slips
- Entry order: q4296 was left non-terminal (its three released rows not yet retried) while q4299 was worked; it was finished right
  after q4299 (0x0041BB00 / 0x0122A600 / 0x009E15E0), then q4659 and q6305 in order.
- cpp precheck: the first compile of several rows (0x009E45D0, 0x00A10E10, 0x0066C7A0, 0x00B4B830, 0x00642A60, 0x00499D90, 0x0082CD00, 0x009C1050)
  went through pdiff.sh, which has no pre.sh precheck (g.sh has one). No compile hung.
- The pdiff / coll / defined-symbol diff of the three identity self renames (0x009F61C0, 0x0041BB00, 0x0122A600) was run only at the end
  (results as STATUS / RENAMES say; 0x0122A600's coll reports the expected NEW-SPELLING for the TU-hashed GFontTextureCache: decision 2).
- One python edit asserted on a whitespace mismatch for 0x0082CD00 and wrote nothing (redone with a regex). Every Bash call exited 0.
