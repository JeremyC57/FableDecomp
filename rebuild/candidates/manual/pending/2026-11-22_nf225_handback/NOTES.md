# NF225 notes (2026-11-22, round 10): the nf224 follow-ups (q12562-q12564)

Base: the repo working tree with nf224 landed. Locks claimed NF225: 00494840 0054ed90 0042db20 0042fb40 (were `released`), 0041e5f2 (new
mkdir). No AUTH12 lock (only 00cf4ec0) or Codex pass 27 row touched; neither has an overlay of the two headers. Tools: NF224's, re-pathed
(tools/), plus tools/objsyms.py (per-COMDAT size / hash with relocation fields zeroed / relocation symbols). PDB dumps: the llvm-pdbutil
symbol dumps of the 4 KB repacks already on disk at $S/codex27/pdbx/ (read only); raw greps of $S/dl_debug/files*/.

## Entry 1 (q12562): 0x00494840 / 0x0054ED90 GetFont binders -- done
- Rows: NF223's drafts (NF224 work/e1/00494840.cpp, 0054ed90.cpp) plus the new size-body opt-ins (hdr/README.md) and, in 0x0054ED90, the
  existing FABLE_RETAIL_CCOUNTEDPOINTER_RELEASEDATA. Each row includes engine/CCountedPointer.h itself, first, with the macros defined.
- Mechanism: explicit specializations of the header's own bodies under `optimize("s", on)` (the nf213 push_back pattern). The /O2 callers
  still expand them, and the size-built COMDATs keep the calls to ReleaseData / ShareData out of line, as the owners do.
- Pick-any (pk.py, work/pk_e1.tsv; owners.tsv + reloc_bindings):

  | row | copy | size | owner | result |
  |---|---|---|---|---|
  | 0x00494840 | ReleaseData<const CFontBank> | 44 | 0x0042F948 | EQ |
  | 0x00494840 | ShareData<const CFontBank> | 38 | 0x0042F9F2 | EQ |
  | 0x00494840 | ??$?4<CFontBank> of <const CFontBank> | 23 | 0x0042FE34 | EQ |
  | 0x0054ED90 | ReleaseData<CFontBank> | 53 | 0x004057A0 | EQ |
  | 0x0054ED90 | ShareData<CFontBank> | 38 | 0x004190E2 | EQ |
  | 0x0054ED90 | ??4<CFontBank> | 23 | 0x00419463 | EQ |

  - 0x00494840: HEAD had 3 NEQ copies (??1 / ReleaseData / ShareData <const CFontBank>); now 0. ??1<const CFontBank> (owner 0x0042FC0E)
    is no longer defined (the temporary is now CCountedPointer<CFontBank>).
  - 0x0054ED90: HEAD's copies were NUISystem::CCountedPointer<NUISystem::CFontBank> (NO-OWNER); now 3 EQ.
  - ??1<CFontBank> (53 B, both rows) has no retail owner and no reference (no E9 thunk into 0x004057A0; no binding line); 0x0042DDB3 defines
    the same symbol as a 5 B `jmp ReleaseData` (its RELEASEDATA_DECL_ONLY). Unreferenced, so the image keeps neither; recorded, not blocking.
- Condition 1: leaf and ??1 bytes identical with the macros removed (work/e1/syms_draft_* v syms_new_*); only the 3 owner COMDATs differ.
- Link (work/link_check.txt): the two rows + owners 0x0042F948 0x0042F9F2 0x0042FE34 0x004190E2 0x00419463 0x004057A0 + 0x0042DDB3:
  0 duplicates; every copy hash-equal to its owner's object.
- Census: 0x009E2C80 had 3 spellings (CFontManager: 11 rows incl. witness 0x0054F4B0; the CFontBank stand-in: 0x00494840; the
  NUISystem::CFontBankSource stand-in: 0x0054ED90) -> 1. callers.py: 13 landed callers; the other 11 already bind the CFontManager spelling (9 through engine/CFontManager.h, 0x0043B570 and 0x0054F4B0 through local CFontManager models with the same signature; NF224 census 3).
- Forms: 0x00494840 1 (f1), 0x0054ED90 2 (f1 with a sixth opt-in, f2 with the existing RELEASEDATA; f2 kept). Budget 6 each.
- Not changed (HEAD state, queued in NAME_FIXES): 0x00494840's Leaf line says AAE (Ego_r private) but the local model emits QAE, so vland
  warns "leaf matches no symbol". Both tests are HEAD host models (standalone CCountedPointer / CFontBank stand-in models, not linked
  with the row) and still PASS; a rewrite on the shared headers is queued, not done.

## Entry 2 (q12563): 0x0042DB20 ~CManager -- done (3-row unit; coordinator to confirm the evidence reading)
- Evidence check (brief: "do Ego_r.pdb / FableWin.pdb show CManager::Clear inlined anywhere"):
  - S_INLINESITE: 0 records of that kind in either PDB (the whole dump has none: VC7.1 does not emit them), so that arm can show nothing.
  - Inline copy in another compiland: YES, in both. Clear's only S_GPROC32 is in `release\new_frontend_game_component.obj` (Ego_r, Mod
    0808, 39 bytes = retail 0x0042FB40) and `debug\new_frontend_game_component.obj` (FableWin, Mod 0807), with S_FRAMEPROC flags
    "marked inline" in both. CManager's own methods (ctor, ~CManager, GetInstance, Action) are in ui_manager.obj (Ego_r Mod 0843, FableWin
    Mod 0842) without that flag. FableWin also puts NUISystem::CManager::`scalar deleting destructor' (code 44, "marked inline") in the
    front-end compiland: that is 0x0042DB20's leaf, and retail 0x0042DB20 sits in the same TU's range as 0x0042FB40.
  - Not shown: an expansion site. Ego_r's Clear lacks the "inlined" flag that e.g. CCountedPointer<CIEngine>::Reset carries.
  - Reading taken (advisor): the gate is met on its second arm. **Coordinator: confirm or overrule.** If overruled, the unit drops whole
    (rows 0x0042DB20 / 0x0042FB40 / 0x0041E5F2 back to HEAD, hdr NUISystem_CManager.h opt-ins dropped); entry 3 alone would then
    still be possible for 0x0041E5F2 only (in-place, no header member).
- Unit (forms: 1 each):
  - Header: FABLE_RETAIL_CMANAGER_CLEAR_INLINE + FABLE_RETAIL_CMANAGER_GPUNIQUEINSTANCE (hdr/README.md); the static is protected
    (mangles `1`, as all three PDBs), the block ends `public:`, exclusive with the existing FABLE_RETAIL_CMANAGER_CLEAR.
  - 0x0042DB20: leaf ??_GCManager@NUISystem@@IAEPAXI@Z; RELOCATION_MATCH; calls ??1CManager@NUISystem@@IAE@XZ (0x0041E641).
  - 0x0042FB40: ported onto the header; leaf ?Clear@CManager@NUISystem@@SIXXZ is now a pick-any COMDAT (was strong): COMDAT meets
    COMDAT. RELOCATION_MATCH.
  - Both use the same anonymous-namespace scaffold `ReferenceManagerClear` (descriptive, internal: the 0x00415B80 / 0x00BB1740 pattern).
    No friend, no stand-in renamed into the namespace: both drop their HEAD-local models (global `struct CManager`; NUISystem::CObservable
    / CManager).
- Pick-any (owners.tsv gained the four unit symbols first, so nothing passes as NO-OWNER): Clear in 0x0042DB20's object 39 B EQ
  0x0042FB40; ??_G in 0x0042FB40's object 30 B EQ 0x0042DB20. Cross-object hashes equal (work/link_check_e2.txt).
- Link (work/link_check_e2.txt): 0x0042DB20 0x0042FB40 0x0041E5F2 0x0041E641 0x0042E98F (caller at 0x0042EA1E) 0x005954AD (caller at
  0x005954E5): 0 duplicates. 0x0041E641 defines ??_GCManager@NUISystem@@UAEPAXI@Z (its local virtual model), a different symbol.
- Census: 0x0042DB20 has no referencer (callers.py, absrefs.py) -> leaf-only self rename. 0x0041E641 binders: IAE (0x0042FB40) +
  ?Dtor@CManager@@QAEXXZ (0x0042DB20) -> IAE only. The self row 0x0041E641 itself emits ??1CManager@NUISystem@@UAE@XZ (local model,
  public virtual; all three PDBs say IAE): unchanged, NAME_FIXES. 0x0042FB40's callers 0x0042E98F / 0x005954AD already bind ?Clear.

## Entry 3 (q12564): 0x013B8710 GPUniqueInstance -- done (with entry 2's unit)
- ?GPUniqueInstance@CManager@NUISystem@@1PAV12@A: raw names in Ego_r.pdb, FableWin.pdb and Ego_d.pdb (1 hit each).
- absrefs.py: only 0x0041E5F2 (x2) and 0x0042FB40 (x2) reference it. 0x0042FB40 binds it through the header opt-in; 0x0041E5F2 is respelled
  in place: a `protected: static CManager* GPUniqueInstance;` in the row's existing local NUISystem::CManager model replaces the namespace
  global (same class token, objdump diff = the one UND). It is not ported onto the header because the header's protected ctor would
  move its ctor binder to ??0CManager@NUISystem@@IAE@XZ while self row 0x0041E3F6 still emits QAE (a new split); that pair is queued
  (NAME_FIXES). 0x013B8710: 2 spellings -> 1. TABLE_CHANGES: 2 deletes + 1 add.

## Objects v HEAD (sc.sh / pk.py: work/sc_e1.log, sc_e2.log, pk_e1.tsv, pk_e2.tsv)
- 0x0042DB20: HEAD defined ?ScalarDeletingDestructor@CManager@@QAEPAXI@Z (bound ?Dtor@CManager@@QAEXXZ -> 0x0041E641); now defines the
  leaf ??_GCManager@NUISystem@@IAEPAXI@Z, ?Clear@CManager@NUISystem@@SIXXZ (pick-any, EQ 0x0042FB40) and the hashed scaffold; UND
  ??1CManager@NUISystem@@IAE@XZ and ?GPUniqueInstance@CManager@NUISystem@@1PAV12@A.
- 0x0042FB40: HEAD already defined ?Clear (strong) and ??_GCManager@NUISystem@@IAEPAXI@Z (pick-any from its local model's delete, EQ
  0x0042DB20 at HEAD and now); the only defined-symbol change is the added scaffold; the data reference moves Instance -> GPUniqueInstance.
- 0x0041E5F2: only the UND data symbol changes. 0x00494840 / 0x0054ED90: the CCountedPointer instantiation symbols listed in sc_e1.log.

## Tests (rule slip, coordinator decision)
- All five rows keep their HEAD tests; none is rewritten on the shared headers. vland PASS on all, but:
  - 0x00494840 / 0x0054ED90: standalone host models (local CCountedPointer / CFontBank stand-ins, not linked with the row).
  - 0x0042FB40: a host copy of HEAD's local NUISystem::CObservable / CManager model with the old `Instance` name. Not rewritten: on
    engine/NUISystem_CManager.h the class has a protected ctor / dtor and some 40 out-of-line virtual and plain members, so a host test
    would need a derived scaffold plus a stub per member; queued.
  - 0x0042DB20: a stub that only prints PASS_0042db20 (no behaviour).
  - 0x0041E5F2: a host copy of the row's model under the old g_pUIManagerInstance name (still describes the body).

## Self-check
- rows/: 00494840 0054ed90 0042db20 0042fb40 0041e5f2 (.cpp only; tests are HEAD's). recheck_final.log: those 5 + unchanged 0x0041E641
  0x0042E98F 0x005954AD = 8 rows, all RELOCATION_MATCH + PASS.
  - Warnings: 0x0042DB20 STANDIN_LEAF (new: HEAD had no Leaf line) compares the ??_G leaf with the catalog name
    NUISystem::CManager::`scalar_deleting_destructor'. Landed ??_G rows carry the same warning for the same catalog spelling (54 lines in
    batches/*/logs/*.log, e.g. 0x00AACEA0 ??_GCSoundEmitterLH@@UAEPAXI@Z, 0x004DBFAF ??_GCTCActionUseTorch@@UAEPAXI@Z), so the catalog name
    stays as is (RENAMES section 1). 0x0054ED90 STANDIN_LEAF and the RELOC_NAME lines on 0x0041E641 / 0x005954AD are HEAD's.
- Census: 0x009E2C80 3 -> 1, 0x013B8710 2 -> 1, 0x0041E641 binder spellings 2 -> 1, 0x0042DB20 self moves (no referencer). Nothing grows.
- Flags lines: none. EH funclets: none (no row here has one). Locks: rows.txt.
