# Landing nf292 (Account A, 2026-11-23): four generated-branch opt-ins and the first made-up-name respells (47 rows)

Hand-back: `pending/2026-11-23_nf292_handback/` (`TABLE_CHANGES_applied.tsv` is what landed); helpers (proof2.py: objsec with an explicit
rename map built from the respell) under `naming_batch/agent_tools/nf292/`. 39 of 933 entries reached: 23 done, 2 done pending Q1 (landed),
7 done-partial, 4 skipped-held, 3 skipped-port; the other 894 go to NF296.

- **Generated-branch opt-ins (ruling NF291 Q4), 5 binders:** add-only declarations in the flat branches of `engine/CWorldMap.h`
  (0x006E11F0 IsLoadingAnyMap), `engine/CMemoryDataOutputStream.h` (0x00A99840 / 0x00A9BA70; its flat assert first pinned to
  `FABLE_STATIC_ASSERT_AT(58, ...)` under its own equiv_check, 1,418 rows, 0 differ), `engine/CTCBuyableHouse.h` (0x00895ED0), `engine/CTCDoor.h`
  (0x007311F0, the member `protected:` as the PDB's IAE). equiv_check of the whole overlay: 1,693 rows, 0 differ; the planted control flags 80.
- **Made-up-name respells, 42 rows:** each binder's own local model takes the PDB name and signature (two also add `const` for the PDB's
  QBE); every exe caller found with xsites, not only the table witness (entry 54: 7 binders; 188: 9). Every new symbol in both PDB name
  sets, except SetCurrentGroupNames (FableWin plus 7 retail call sites).
- objsec HEAD v new ALL_EQ for all 47 (the three CTCDoor rows differ also by the compile-path anonymous-namespace hash, mapped). Links: every
  stand-in unresolved at HEAD now resolves; no new duplicates.
- Q1: 0x009CB960 (in T3_PORT_QUEUE.tsv for other members) took only its two CBankFileManager member lines: landed.
- Q5 (coordinator compiled both witnesses, `logs/witnesses_009d6490_00bfc770_head.log`): 0x009D6490 still binds
  `?RemoveBank@CLoadingMemoryFailureHandler@@QAEXPAVCBankFileAsync@@@Z`, so that line stays (rewitnessed to 0x009D6490 instead of deleted);
  0x00BFC770 binds `?WriteUShort@CDataOutputStream@@QAEXG@Z`, so its rewitness applies.
- Findings queued: owner self rows 0x0070D920 (`?op_del@@YGXPAX@Z`) and 0x006925E0 (GetMovementRatio) define names in neither PDB set
  (T3 self ports); 0x00B59760 is a 20-byte body with two PDB names (identity research, untouched); four landed rows define
  `CMemoryDataOutputStream::Clear` (owner 0x00996B60 plus 0x00A66860, 0x00A66C00, 0x009E42C0).
- Table: 33 deleted, 15 added, 2 rewitnessed; no symbol gains a second target. The catalog-lookup helper bug (NF294) re-checked: two
  link-only owner rows had a neighbour's `/Gr`; regraded with their own flags, same results; no changed row affected.

## Recheck
`logs/recheck.log` (landed tree): 47 RELOCATION_MATCH PASS, 0 DIFFER, 0 RELOC_CONFLICT.
