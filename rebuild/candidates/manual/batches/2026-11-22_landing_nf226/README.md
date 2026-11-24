# Landing nf226 (Account A, 2026-11-22): CManager's ctor / dtor onto the shared header, InitGraphics' access, seven tests (8 rows, 3 header opt-ins)

Agent NF226. Hand-back: `pending/2026-11-22_nf226_handback/`. Entries q12566-q12570.

- **Recheck** (`logs/recheck.log`, HEAD control `logs/recheck_head.log`): 11 rows, all RELOCATION_MATCH + PASS on both trees, with no
  RELOC_CONFLICT. **Link:** 0 duplicates on both.
- **q12566 + q12567, one 4-row unit.** The rows cannot move separately: HEAD 0x0041E3F6 defined a stand-in `??_G...UAE` and a 1-slot
  vtable calling the `??1...UAE` the dtor port removes.
  - 0x0041E641 is ported onto engine/NUISystem_CManager.h; its leaf goes UAE -> `??1CManager@NUISystem@@IAE@XZ` (protected, non-virtual,
    all three PDBs).
  - 0x0041E3F6 is ported, `??0...QAE` -> `??0CManager@NUISystem@@IAE@XZ`. It uses the new `FABLE_RETAIL_COBSERVABLE_CTOR_DECL_ONLY`,
    the existing STLP_MAP / RELEASEDATA opt-ins, and `optimize("y", off)` limited to the ctor with a trailing `optimize("y", on)`.
    The trailing pragma is accepted as size-pragma scaffolding: it restores the TU default before its pick-any copies.
  - 0x0041E5F2 is ported (ctor binder QAE -> IAE).
  - 0x0041E1CD makes the member `virtual` in its existing local model: `?SetMetaLayer...QAEXJ` -> `UAEXJ` (both PDBs, vtable slot 34).
    This is accepted as an in-place respell: its object defines only its own leaf.
  - Pick-any (`logs/pickany_vs_retail.tsv`): 23 EQ in 0x0041E641 (6 at HEAD) and 34 EQ in 0x0041E3F6 (2 NEQ at HEAD).
    `~list<CObserver*>` (5-B jmp) v the table's 0x0042AC25 (~_List_base, 23 B) is a folded alias: not code-referenced, and the copies
    already disagreed at HEAD. Landed, split queued.
  - Nine stand-in binders move to PDB / STLport spellings; no address gains a code-relocation spelling.
- **q12568, 0x00494840:** leaf QAE -> AAE (private, Ego_r) via `FABLE_RETAIL_CFRONTENDGAMECOMPONENT_INITGRAPHICS`; the local
  CCharString now comes from fable_string.h. 3 spellings -> 2 (0x00495610's stand-in binder queued).
- **q12569, seven tests on the shared headers**, all PASS, each with a case per condition: 0x0041E641, 0x0041E3F6, 0x0041E5F2, 0x0042FB40,
  0x0042DB20, 0x00494840, 0x0054ED90.
  - 0x0054ED90's test uses `/alternatename` to bind stubs to symbols no shared header declares; accepted for tests only.
  - 0x0042DB20's PassPattern becomes `0042db20_TEST PASS`.
- **q12570:** report only. `~CCountedPointer<CFontBank>` is `{ ReleaseData(); }` (FableWin, 17 B, "marked inline"); retail kept no copy.
- **Headers:** add-only `FABLE_RETAIL_CMANAGER_SETGRAPHICSBANK`, `FABLE_RETAIL_COBSERVABLE_CTOR_DECL_ONLY` (CObservable's static
  assert pinned) and `FABLE_RETAIL_CFRONTENDGAMECOMPONENT_INITGRAPHICS`. pin_asserts 0, assert_names_check 0; equivalence in
  `logs/equivalence_differing_rows.tsv`.
- **reloc_bindings** (`logs/TABLE_CHANGES.tsv`): 12 stand-in lines deleted, 10 added. The `_List_base<CActionParam*>` ctor line at
  0x0042AB01 was held (`logs/reloc_bindings_held.tsv`) because another spelling still binds there.
- **Coordinator ruling:** vtable-slot relocations are binders. The 12 self rows whose leaf differs from their slot's PDB spelling are
  queued, not respelled here.
- **Queued** (tag land1122nf226: q12573 q12574 q12575 q12576 q12577 q12578 q12579 q12580 q12581 q12582 q12583 q12584 q12585 ): the 12 vtable-slot self rows; 0x0042AB01; the ~list folded alias; NF226's unapplied NAME_FIXES
  (0x0042BEC0 / 0x0042BE7B CObservable, 0x0055C950 ClearObservers, the tree / map placeholders, 0x00495610, and the 0x004299A8 /
  0x013B871C splits).

Equivalence (all landed rows, path-normalised equiv_check): only the landing rows 0x0041E641, 0x0041E3F6, 0x00494840 (src + test) differ; nothing else changes.
