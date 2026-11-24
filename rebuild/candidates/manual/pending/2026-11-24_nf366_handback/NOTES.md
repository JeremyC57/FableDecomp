# NF366 (Account A naming queue, 2026-11-24): the eleven rows that define the extern "C" `@DeleteIfSet@4` stand-in

Run dir: `batches/b1/` (land_scripted form), seven rows, RESULTS **ACCEPT** for all seven. Four rows skipped (T not proven by two sources).
No header change, no test change, no reloc_bindings change.

## Identity (step 1; `evidence/identity.tsv`, `evidence/xsites.txt`, `evidence/reset_callers.txt`)
Every one of the eleven bodies is the same 11-byte virtual slot-0 destroy. A DeleteData address's only absolute reference is the
`mov dword ptr [eax+4], <addr>` in its CCountedPointer<T>::Reset (each a 103-byte Reset here). T then comes from the object the Reset's
caller passes in (retail RTTI of the constructed object) and the FableWin.pdb DeleteData + Reset pair for that T:

| row | T (new self) | retail (Reset, caller, constructed object) | PDB |
|---|---|---|---|
| 0x00447660 | const CInputTypeBase | Reset 0x00447D90, call 0x00447216, CInputTypeXboxPad{Button,LeftStick,RightStick}Event (base CInputTypeBase) | FableWin DeleteData + Reset(const CInputTypeBase*) (the only CInputTypeBase counted pointer is const); Ego_r CGamePlayerInterface members |
| 0x00A28370 | CMeshDataBankEntry | Reset 0x00A29D20, CMeshDataBank::NotifyEntryAdded 0x00A26D40, C3DAnimationInfo (base CMeshDataBankEntry) | FableWin DeleteData + Reset; Ego_r CMeshDataBank::DynamicInfoBuffer |
| 0x00A281E0 | C3DMesh2 | Reset 0x00A299C0 (table already Reset<C3DMesh2>), C3DMeshLODInfo::BuildMesh 0x00A24520, C3DMesh2 | FableWin DeleteData + Reset; Ego_r C3DMeshLODInfo::Mesh (+0x1C, the Reset's this) |
| 0x00AA4310 | QuadricOptimiserInternals::COptimisedMesh | Reset 0x00AA4460, BeginOptimisation 0x00AA4210, COptimisedMesh | FableWin DeleteData + Reset + converting ctor to CCountedPointer<C3DOptimisedMeshBase>; no Reset / DeleteData for the base in either PDB |
| 0x00B3E080 | CLoadingMemoryFailureHandler | Reset 0x00B3E510, CEngineResourceManager::AllocatePools 0x00B3B240, CLoadingMemoryFailureHandler | FableWin DeleteData + Reset; Ego_r CEngineResourceManager::LoadingMemoryFailureHandler |
| 0x00CD42E0 | CScriptDefinitionManager | Reset 0x00CD43A0, only caller CScriptDefinitionManager::Set 0x00CD3F40 (static PClass 0x0143E920), CScriptInfoManager::Init 0x00CB5D80 news one (vtable 0x012C2648, no RTTI; slot 0 = 0x00CD4330 ??_G, slot 2 = InitAndCompile 0x00CD3F50 as the shared header maps them) | FableWin DeleteData + Reset + Set; Ego_r Set 12 B = retail 12 B |
| 0x00F38920 | CRenderTarget | Reset 0x00F39CA0, CSurfaceCompositor::CSurfaceCompositor 0x00F38990, CRenderTarget | FableWin DeleteData + Reset; Ego_r CSurfaceCompositor::RenderTarget |

None of the seven names is bound in reloc_bindings, named by a `// Leaf:` / self_symbols line, or defined by a `template <>` in rebuild/src.

Skipped (locks never claimed, nothing compiled; STATUS.tsv has the evidence):
- 0x0044AC20: no absolute reference and no caller (unreferenced, like its neighbour Release 0x0044AB10). Not CHeroLogBook (not polymorphic).
- 0x00A0BB40: its Reset 0x00A0BC90 is uncalled; not CCRCSymbolMap (that DeleteData is 0x00A018D0).
- 0x009A6C00 (CSoundManagerLH object) and 0x00991E10 (CLipSyncDataBank object): no counted-pointer name for those classes or their bases in
  FableWin, Ego_r or Ego_d, so derived v base cannot be chosen.

## Text (step 2)
The 0x00428B10 form: `FABLE_RETAIL_CCOUNTEDPOINTER_DELETEDATA_DECL_ONLY` + engine/CCountedPointer.h, `template <> void FABLE_FASTCALL
CCountedPointer<T>::DeleteData(void* data)` with HEAD's body over the local slot-0 `Obj` model, `// Leaf:` = the PDB symbol.
- Header measurement: for the four Ts with a shared header (CMeshDataBankEntry, C3DMesh2, CScriptDefinitionManager, CRenderTarget) both an
  include variant and a forward-declaration variant were graded. Both are MATCH and objsec OTHER_SAME against HEAD with `.drectve`, UND and
  the COMDAT set equal. The include variants are adopted (brief: prefer the header); the forward-declaration texts are in
  `batches/b1/variants/*_fwd.cpp`. Macros: CRenderTarget takes 0x00A10E10's FABLE_RETAIL_CSURFACE / _GETD3DSURFACE; CScriptDefinitionManager
  takes the owner row 0x00CD3EC0's set plus fable_string.h (without them clang fails on the size assert / CCharString).
- CInputTypeBase, COptimisedMesh and CLoadingMemoryFailureHandler have no class header: forward-declared (COptimisedMesh in
  `namespace QuadricOptimiserInternals`, as landed rows and QuadricOptimiserTypes.h spell it).

## Gates (step 3)
- **Grade:** pass.py one-path H / N: MATCH both sides for all seven; vland `--recheck-landed` MATCH PASS HEAD and new (`logs/`), the
  HEAD "leaf 'sub_<addr>' matches no symbol" warning gone; no new warning.
- **objsec** (`evidence/objsec/`): OTHER_SAME symdelta 0 for every row and variant; leaf bytes / relocations / characteristics equal, sel 1
  both sides, `.drectve` equal. Each new object's ONE defined external is exactly its Leaf (no UND at all).
- **Pick-any:** pk_all + pickany_vs_retail with `--leaf` per side (HEAD `@DeleteIfSet@4`): SAME, GATE PASS; reachflip 0 of 7; reversed
  control PASS; planted control (a leaf-called NEQ line added to N_00f38920) FAIL as it must.
- **Drop test** (`evidence/droptest.txt`): only `@DeleteIfSet@4` drops; 0 table lines; 0 UND binders (41 HEAD controls, 76,571 scratch
  objects); 0 code declarations outside the 11 definers.
- **Link** (`evidence/link/`, /FORCE, objects compiled at HEAD):
  - Each owner with its Reset binder (and the Reset's caller): the binder binds `?Destroy_<addr>@@YIXPAX@Z` (the reloc_bindings stand-in),
    unresolved at HEAD and after; the respell neither resolves nor breaks a reference.
  - New duplicates: 0x00A281E0 v 0x00A24520 and 0x00F38920 v 0x00F38990, each a pick-any copy (sel 2) whose body is EQUAL (cmpcomdat:
    11 B, 0 relocations). No different-body duplicate anywhere. Same shape as 0x00428B10 v 0x00626750 (R3).
  - The seven HEAD objects together: 6 LNK4006 on `@DeleteIfSet@4`; the seven new objects: 0.
  - 0x00AA4210 also leaves `?DeleteData@?$CCountedPointer@VC3DOptimisedMeshBase@@@@SIXPAX@Z` UND, H and N (it instantiates a same-TU
    Reset<C3DOptimisedMeshBase>, a stand-in copy of 0x00AA4460): pre-existing, unchanged.
- **Census of other definers:** HEAD-compiled controls (claimed, prechecked, released): the seven Resets, their callers, and every landed
  source that names one of the Ts with a Reset / ShareData / `new` (41 objects, `evidence/symscan_controls.txt`). Definers of a new name:
  only 0x00A24520 (C3DMesh2) and 0x00F38990 (CRenderTarget), both EQUAL. The scratch corpus found no other definer either.
  - Not compiled (T3 column 2, stay-off), excluded by reading the source: 0x00A24A80 (nf314) models CCountedPointer with its own local
    template and names no Reset / ShareData / DeleteData; 0x00A24D70 (nf294) Resets only C3DAnimation2 and the file-data pointer (and
    its 4 scratch-corpus objects define none of the seven names); 0x00A2ACC0 (nf303) instantiates operator= of CCountedPointer<const
    C3DMesh2>, whose ShareData takes an existing count block and references no DeleteData (and any copy there would be the `$$CB`
    symbol, a different name).
  - Why that census is complete for retail: each of the seven addresses has exactly one absolute reference in Fable.exe, the store in its
    Reset (evidence/xsites.txt), and every retail caller of each Reset was HEAD-compiled (evidence/reset_callers.txt).
- **PDB spelling:** each new self is a whole-string entry in the FableWin.pdb name set (7 of 7, evidence/pdb_exact.txt; Ego_r and Ego_d
  hold none of them).

## Rulings needed
- **N1 (binders, not blocking):** each address's table line is `?Destroy_<addr>@@YIXPAX@Z`, witnessed by its Reset row, which still binds
  it; so TABLE_CHANGES is empty (no `replace` while the witness references it) and the Reset rows are queued in NAME_FIXES.tsv with their
  PDB Reset spellings (0x00447D90, 0x00A29D20, 0x00A299C0, 0x00AA4460, 0x00B3E510, 0x00CD43A0, 0x00F39CA0). Is landing the owners first
  (binders stay unresolved, as at HEAD) fine, as NF302 Q2 / NF360 did?
- **N2 (equal-body LNK2005):** 0x00A281E0 and 0x00F38920 add one equal-body duplicate each against a pick-any copy (R3 accepted the same
  for 0x00428B10 v 0x00626750). Marked ACCEPT on that precedent; hold those two if the coordinator reads R3 more narrowly.

- **N3 (include v forward declaration, not blocking):** the landed 0x00428B10 forward-declares T; for the four Ts with a class header the
  brief prefers the include, and both forms are byte-equal (OTHER_SAME, `.drectve` equal). The include texts are the rows; the
  forward-declared texts are in `batches/b1/variants/*_fwd.cpp` (objsec in `evidence/objsec/<a>_fwd.txt`). With the include, 0x00A281E0
  joins engine/C3DMesh2.h's recheck set (about 50 includers), 0x00A28370 CMeshDataBankEntry.h's, 0x00CD42E0 CScriptDefinitionManager.h's
  (with CDefinitionManager.h and fable_string.h) and 0x00F38920 CRenderTarget.h's. Swap in the `_fwd` texts if the coordinator prefers
  the 0x00428B10 shape.

## Queue / coordinator follow-ups
- NAME_FIXES.tsv: the seven selves; the seven Reset binders (four with a wrong table spelling: CEntryCountedPointer::Reset,
  Reset<C3DOptimisedMeshBase>, CAllocatorPointer::Reset, CFixedAllocator::Allocate at 0x00CD43A0); 0x00CD3F40 =
  ?Set@CScriptDefinitionManager@@SIXPAV1@@Z (was ?operator_new_variant@@YIPAXI@Z / GFSetPDefinitionManager); 0x0143E920 =
  CScriptDefinitionManager::PClass (identity only, no data-symbol spelling), whose atexit destructor is 0x0122AEB0.
- RENAMES.tsv: seven self lines; seven file / catalog proposals (sub_<addr> -> CCountedPointer<T>::DeleteData, Module _gapscan ->
  CCountedPointer<T>, file -> CCountedPointer<T>_DeleteData_<addr>), NOT applied.
- QUEUE_PROPOSALS.tsv: research notes for the four skipped nf360 lines; T3 debt line for 0x00AA4210's same-TU Reset<C3DOptimisedMeshBase>.
- self_symbols.tsv lines for the seven follow at the next self_symbols.py pass.

## Process
- Reading list: BRIEF_NF_BATCH.md, EXTRA_RULES.md, COORDINATION.md and ASM_RULES.md read in full; GOTCHAS.md (152 KB) was grepped for
  the relevant keys (specialization, DeleteData, CCountedPointer, libcp, objsec, pk_all, reach, leaf, forward declaration), not read whole.
- objsec files: `evidence/objsec/<a>.txt` compares HEAD with the FINAL row text and checks that the object's only DEF equals the Leaf;
  `<a>_fwd.txt` is the non-adopted forward-declaration variant of the four include rows.
- Locks: the seven rows plus 41 HEAD controls (each compiled once, never edited), all released (evidence/locks_final.txt). Rows' tests
  prechecked (clang PASS) before claiming; HEAD and new sources and every control source prechecked after claiming, before any compile
  (work/pre.log). Stayed off NF367 / NF368 rows, B's claims, Codex reservations and T3 column-2 rows.
- Grades: pass.py H x7, N x7, include variants x4 (= 1 H + 1 or 2 N per row, budget 10); vland chain H and N once; controls 41 HEAD grades.
- Tools: tools/ (NF360's set retargeted to NF366, plus xsites366.py, callers366.py, rtti366.py, objctx366.py, mkrows366.py, mkbatch366.py).
- No retail bytes in the hand-back (the vland oracles stay in work/).
