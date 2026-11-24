# Landing nf223 (Account A, 2026-11-22): the CCountedPointer tail and two split follow-ups (9 rows, 1 header opt-in, 1 label)

Agent NF223. Hand-back: `pending/2026-11-22_nf223_handback/`. Entries q12548, q12550-q12558.

- **9 rows land.** Recheck (`logs/recheck.log`, HEAD control `logs/recheck_head.log`): 20 rows = the 9 changed rows, the unchanged
  rows that reference the touched addresses, and 0x00B2CB40 (the header's other includer). 3 MATCH + 17 RELOCATION_MATCH, all PASS, on
  both trees; no RELOC_CONFLICT.
  - q12551-q12553: the DeleteData self rows 0x00419036 / 0x00428AE7 / 0x0042F864 move from the `@DeleteIfSet@4` / `sub_` placeholders to
    the FableWin / Ego_d spellings (MATCH). Every binder already used those spellings. Files and catalog names move.
  - q12555 OpenGraphicBank 0x009F83D0, 3 spellings -> 1:
    - 0x00416C8A gets a local `CGraphicBankManager` declaration. That is allowed by the three-condition rule: no header defines the
      class (only forward declarations), the landed 0x0042DDB3 has the same shape, and it has no base.
    - 0x0041E3F6 and 0x005BFF07 move onto engine/CGraphicDataBankInit.h, which also puts 0x00415B80's binders on one spelling.
  - q12554 GetFont 0x009E2C80, 5 spellings -> 3: 0x00438AA0 and 0x00405020 move. 0x00494840 / 0x0054ED90 are held (below).
  - q12557: 0x00416C8A's ShareData / `??4<CGraphicDataBank>` copies, which differed at HEAD, are gone (existing ASSIGN_DECL_ONLY).
  - q12548: 0x00765C80 (push_back) is ported onto stlport/fable_containers.h; its leaf becomes STLport push_back. 0x00765B70 and
    0x00765C80 now have one spelling each.
  - q12550: the 0x00B2FD60 label is fixed to CEngineCamera::SetupGamut in labels_forge_autoname4.tsv line 1598 and engine_api.tsv
    line 37056 (both PDBs).
  - q12558: the `mutable` on the AcquireFont Font member stays. CodeView has no mutable bit, and the const method re-seats Font. Report only.
- **Header** engine/CGraphicDataBankInit.h: add-only `FABLE_RETAIL_CGRAPHICDATABANKINIT_CTOR_DECL_ONLY`, used by 0x0041E3F6 / 0x005BFF07.
  It keeps a differing 48-B pick-any ctor copy out of their size TUs. assert_names_check: 0 mismatches. Equivalence:
  `logs/equivalence_differing_rows.tsv`.
- **Link** (`logs/link_check.txt`, 20 objects): duplicates 8 at HEAD, 6 at landing (the two `@DeleteIfSet@4` duplicates are gone). The
  remaining six are the accepted same-TU kind (0x007634E0 v 0x00763060) and the unrelated CBasicString::Compare pair.
- **reloc_bindings** (`logs/TABLE_CHANGES.tsv`): 6 stale stand-in lines deleted; 1 first witness added
  (`_M_insert_overflow<vector<C3DVector>>` -> 0x00765B70, witness 0x00763060; the line held at landing auth9).

## Not landed (queued, tag land1122nf223: q12559 q12560 q12561 )
- **0x00494840 / 0x0054ED90 (GetFont): held.** Respelling gives their CCountedPointer copies retail owners they differ from. At HEAD the
  same bodies had stand-in names and no owner, so these are new differing copies, and a new differing copy holds its row. Drafts are in
  `work/e2/`.
- **0x0042DB20 (q12556 ~CManager): rejected.** The draft moved the global stand-in CManager into namespace NUISystem with a TU-internal
  friend scaffold. NUISystem::CManager has a shared header (engine/NUISystem_CManager.h), so that renames a stand-in to a class that has
  a header (nf95b). The row stays as at HEAD; port it onto the header. The draft is in `rows_held/`.
- The 0x00415B80 catalog name (SNavigationLayerDefaults -> CGraphicDataBankInit::CGraphicDataBankInit).

## Header proof
- pin_asserts: 0 pinned (NF223 had already pinned the moved assert as `FABLE_STATIC_ASSERT_AT(32, ...)`). assert_names_check: 0 mismatches.
- equiv_check over 57,020 landed rows and tests (`logs/equivalence_differing_rows.tsv`): 9 files differ.
  - 2 are the opt-in users 0x0041E3F6 / 0x005BFF07 (graded in `logs/recheck.log`).
  - The other 7 files belong to the same 6 rows that also differed in nf222's check, for an unrelated header: 0x00CBFB7D, 0x0047042A
    src + test, 0x00BA2350, 0x00BA5260, 0x00BA50A0, 0x0093AB70. So they differ on every run whatever the header change (cause not
    examined; see GOTCHAS). All 6 recheck RELOCATION_MATCH + PASS on the tree with both headers (`logs/recheck_equiv_rows.log`).
