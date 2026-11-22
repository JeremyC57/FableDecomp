# Landing nf228 (Account A, 2026-11-22): the nf227 follow-ups (callee self rows onto STLport spellings, 0x0042BE7B, 0x0041E3F6)

Agent NF228. Hand-back: `pending/2026-11-22_nf228_handback/`. Entries q12589-q12604 (minus 0x0042BEC0, which needs a ruling).

- **Recheck** (`logs/recheck.log`): 18 rows (12 changed + 6 test-only), 1 MATCH + 17 RELOCATION_MATCH, all PASS, no RELOC_CONFLICT.
  HEAD control (`logs/recheck_head_control.log`): 0x0042AB01 was DIFFER(35v27) there. Link (`logs/link_check.txt`): LNK4006 2 -> 0, LNK2005 0.
- **Entry 1, the callee self rows:** eight become explicit instantiations of the header members, using the binder row's defines / pragma
  (tree `clear` 0x0042C02F / 0x0042ACC3 / 0x0042AC76; tree ctor 0x0042B76C / 0x0042B7AD / 0x0042B7EE; `_List_base<CActionParam*>` ctor
  0x0042AB01; `_M_find<long>` 0x0042A879, MATCH). 0x0042D246 `map<long,EEngine2DLayer>::operator[]` (STLP_MAP + TREE_BOUNDS_OUTOFLINE).
  0x0041E1CD SetMetaLayer is ported onto engine/NUISystem_CManager.h, so the nf227-held `operator[]` line is added. Catalog names are now
  the STLport demangled names (files not renamed, as nf227). Every owned pick-any copy is EQ (`logs/pickany_vs_retail.tsv`).
  Spellings: 0x0042D246 3 -> 1, 0x0042A879 2 -> 1, the six tree rows 1 each, 0x0042CE46 2 -> 1, 0x0042AB01 3 -> 2 (the queued folded alias).
- **Entry 2, 0x0042BE7B** (CObservable ctor): NF227's port, regraded; its 11-slot vtable binds every slot at 0x01230044 under the slot's
  self-row spelling. 0x0055CB10 2 -> 1. New test on the header.
- **Entry 3, 0x0041E3F6** (CManager ctor): calls the header's static `CUIScreenUnitConverter::SetIsUsingRelativeCoords` (0x004299A8 2 -> 1)
  and reads the front-end component through the header's `Get()` and the new `CGameComponent::IsRunning`. The row-local
  CNewFrontendGameComponent model is gone; six landed rows still use the `?g_` stand-in for 0x013B871C (queued).
- **Header opt-ins** (add-only, new macros no other row defines; `pending/.../hdr/README.md`):
  - engine/NUISystem_CUIScreenUnitConverter.h `FABLE_RETAIL_CUISCREENUNITCONVERTER_SETISUSINGRELATIVECOORDS`: declaration only
    (Ego_r.pdb + FableWin.pdb).
  - engine/CGameComponent.h `FABLE_RETAIL_CGAMECOMPONENT_ISRUNNING`: inline `bool IsRunning() { return Running; }`. The name is in FableWin.pdb
    only (Ego_r has no such method); the retail body (0x0041E558 `cmp byte ptr [eax+9],bl`) is the second source for what it does.
    The static assert is pinned (`FABLE_STATIC_ASSERT_AT(22, ...)`, same typedef name).
  - Proof: assert_names_check 0 mismatches; equivalence over 57,022 landed rows: only 0x0041E3F6 differs, the user (rechecked above; `logs/equivalence_differing_rows.tsv`).
- **Coordinator rulings on NF228's questions:**
  1. 0x0041E1CD's row-tail `#pragma optimize("y", on)`: accepted. It restores the command-line /Oy default (size-pragma rule; 0x0041E3F6
     carries the same device at HEAD).
  2. IsRunning: accepted on FableWin + the retail body; marked as one PDB in NOTES.
  3. The folded aliases stay (0x0042AB01 list ctor; `~list<CObserver*>` from 0x0042BE7B at 0x0042AC25); splits queued.
  4. The test-only `FABLE_RETAIL_CNEWFRONTENDGAMECOMPONENT_INIT` define in 0x0041E3F6's test: accepted (test-only, like `/alternatename`).
- **reloc_bindings** (`logs/TABLE_CHANGES.tsv`): 8 stand-in lines deleted, 20 added (incl. 5 CObservable vtable-slot first witnesses).
  Held 1 (`logs/reloc_bindings_held.tsv`): `_M_erase<CCharString tree>` at 0x0042B8FC, whose self row still emits `?BuildTreeArray@CTextTreeWalkThrough`.
- **Queued** (tag land1122nf228): the seven second-layer self rows on placeholders (0x0042B8FC, 0x0042A264, 0x0042A237, 0x0042AC9F,
  0x0042ACEC, 0x0042AD10, 0x0042AE33), 0x0042AC0A, the six `?g_` users of 0x013B871C, the held line, 0x0042AC25's folded alias, and the
  13 rows still on HEAD tests (0x0042BE50 + the 12 vtable-slot rows).
