# Landing nf294 (Account A, 2026-11-23): T1 "other" class / namespace splits, first 60 (5 rows, 1 header opt-in, 7 table lines)

Hand-back: `pending/2026-11-23_nf294_handback/`; helpers under `naming_batch/agent_tools/nf294/` (with the fixed flags.sh / srcof.sh, see
the toolkit README). Entries 1-60 of 655 reached: 2 done, 3 done-partial, 1 done-already, 1 table-only partial, 49 skipped-port, 4 held;
61-655 go to NF295.

- **0x00997800 ReplacePathExtension:** its free stand-in `MakePath` -> `CAFile::MakePath` 0x00997780 through a new guarded add-only opt-in
  `FABLE_RETAIL_CAFILE_MAKEPATH` in `engine/CAFile.h` (declaration only, kept apart from the CREATEDIRECTORY copy; the existing
  FABLE_RETAIL_CAFILE route would pull engine/CArray.h and add `/DEFAULTLIB:"libcp"` to `.drectve`, rejected). equiv_check: 0 of 2,951
  includer rows differ; the planted control flags 132.
- **0x00D80E40:** nested allocator struct `CBriarRoseImposter` -> `CBRIAR_ROSE_IMPOSTER` (both PDBs and the self row's Leaf).
- **0x00717BC0:** CActionTalkBase ctor takes `CThing&` and is protected, as the PDB gives (the call passes `*target`). A third spelling
  stays: the coordinator compiled 0x00717C50's HEAD object (`logs/witness_00717c50_head.log`): it binds the global
  `??0CActionTalkBase@@QAE@PAVCThing@@_N11@Z` (port).
- **0x00A5EEA0:** onto the existing `FABLE_RETAIL_CBANKFILE_ENTRYQUERY` (CBankFile::EntryExists 0x009CC190). **0x009CE8D0:** local
  `WriteZeros` -> `private: WritePadding` (0x009CCB20). Both owners still define `CBankFile_Retail` names (self ports, NAME_FIXES).
- objsec ALL_EQ for the five (`.drectve`, tables); links resolve the respelled calls against their owners or leave them unresolved as the
  correctly spelled callers do; no new duplicates.
- Table: entry 50 0x006ED9C0's `CCombatManagerCalls` line deleted (witness 0x00733580's HEAD object binds the PDB spelling); entry 34
  0x009A0460's `ResizeMap@CThingMap` line -> `?Append@?$CBasicString@D@@QAEXDK@Z` (witness 0x0099F3B0's HEAD object; FableWin + the
  retail call 0x0099F3C4 as the two sources). 4 deleted, 3 respelled.

## Recheck
`logs/recheck.log` (landed tree): 5 RELOCATION_MATCH PASS, 0 DIFFER, 0 RELOC_CONFLICT.
