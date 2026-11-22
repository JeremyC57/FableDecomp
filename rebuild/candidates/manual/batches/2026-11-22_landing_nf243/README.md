# Landing nf243 (Account A, 2026-11-22): the CBankFile progress wrappers, one DTOR_OUTOFLINE family; 0x004046B0 proven but held

Agent NF243. Hand-back: `pending/2026-11-22_nf243_handback/` (NOTES section 2 = the 19-row DTOR_OUTOFLINE census, per row).
- **Recheck** (`logs/recheck.log`, landed tree): 0x009CBD30 / 0x009CBD50 / 0x009CBD70 MATCH, 0x0063F360 / 0x0063E6F0 RELOCATION_MATCH, and the
  wrappers' caller 0x009CEC60 (unchanged) RELOCATION_MATCH; every test PASS, 0 RELOC_CONFLICT.
- **L108-L110:** CBankFile::SetBankUpdateStatus (protected), SetBankUpdateProgress (private), WriteBankLog (protected), both PDBs' S_PUB32;
  bodies = callback slots 1 / 0 / 2. New add-only engine/CBankFile.h `FABLE_RETAIL_CBANKFILE_PROGRESS_WRAPPERS_DECL` (14 lines; agent's
  equivalence over the 517 rows that reach the header: 0 differ). Each object defines only its leaf. Module CScriptGameResourceObjectScripted-
  ThingBase -> CBankFile, files renamed, 0x009CBD70 renamed from `func_009cbd70`.
- **0x0063F360** onto `std::_List_base<const CThingGameObject*>::~_List_base()` (its thunk 0x0063F470 is a standalone 5-byte jmp; was the
  `StdList_Clear` stand-in; catalog-only); its one referencer 0x0063E6F0 through `_BASE_DTOR_VISIBLE_NOINLINE` (only that relocation
  changes). 2 -> 1; reloc_bindings line swapped.
- **0x004046B0 (65 rows): held.** Retail: 0x00404690 `_List_base(const allocator&)`, 0x004046B0 `~_List_base`, 0x004046F0 `list(const
  allocator&)`, 0x00404710 `jmp 0x004046B0` (`~list`); census 18 rows bind `~_List_base`, 46 `~list`. Three rows are blocked by local
  vector / map models (0x00501D30, 0x00756D00, 0x007AE3D0).

## Coordinator rulings on NF243's questions
1. The three new LNK4006 (0x009CEC60's inline wrapper copies, all EQ): accepted (nf235 ruling 1).
2. 0x004046B0: (a). A later pass writes the CStringParser.h partial mode, grades the other rows and lands the self row with every binder it
   can move (the split goes from 46 + 18 to 3 + 61); the three blocked rows stay on `~list` until their vector / map ports. Queued (NF245).
3. Catalog: the `~list` thunk rows 0x00404710 / 0x0063F470 and the 0x00404690 / 0x004046F0 renames queued; 0x004B8940 (no thunk, one
   source) stays undecided.
4. The 65 entry-1 rows stay locked, handed to NF245.
