# Landing nf219 (Account A, 2026-11-21): the NullString static pair, the CircularQueue names, CWideString::UnassignString (8 rows)

Agent NF219 (owner: "we can start a new agent when one lands / finishes"). Hand-back: `pending/2026-11-21_nf219_handback/`. NF219 did
three of its eight entries (q12387, q12388, q12389); q12226-q12229 (census only) and q12383 stay open for NF220.

- 8 rows land, every one RELOCATION_MATCH + PASS; the funclets 0x01214F80 / 0x01214F88 and 0x00C43810 regrade unchanged.
- Recheck (`logs/recheck_summary.txt`, rows in `logs/recheck_rows.txt`): 4,269 rows = the direct includers of fable_string.h, the rows,
  the same-TU / funclet rows and every landed caller of the renamed addresses (callers.py). 3,674 RELOCATION_MATCH, 593 MATCH, 2 DIFFER
  = 0x00DA8510 / 0x00E0A510, the bare-CompilerFlags artifact; both RELOCATION_MATCH with their own `// Flags:` (`logs/regrade_partial_flags.txt`).
- q12387 (EXTRA_RULES 2026-11-20 static-pair rule): 0x0122A5E0 is re-authored as the 0x01223EC0 TU's `_$E2` on fable_string.h and is now
  ``CCharString::`dynamic atexit destructor for 'NullString''`` (file renamed). 0x01223EC0 is ported onto the header; its object equals
  HEAD in code, relocations and symbols (`logs/defs_check.txt`). Both rows define NullString, the pair's known LNK2005, recorded as DEBT;
  no other rechecked object defines it, and the live-string count stays defined once.
- q12388 (descriptive, libpk has no PDB): 0x00C44550 `CircularQueue::Release`, 0x00C3CF30 `CircularQueue::~CircularQueue` (was the
  free `_Forward`), 0x00C44360 `CircularQueue::Allocate` (was CEventDataBuffer); binders 0x00C3CF40 / 0x00C43EC0 respelled. Old-leaf
  census: the only callers of 0x00C3CF30 / 0x00C44360 (0x01214F88, 0x00C43EC0) land here. The destructor duplicate (strong owner v
  byte-equal pick-any copies) stays: a destructor's address cannot be taken, so the nf86 / nf88 helper does not apply. Queued.
- q12389: 0x0099B4D0 is `CWideString::UnassignString` (both PDBs; catalog fixed) on fable_string.h, leaf `?UnassignString@CWideString@@IAEXXZ`.
  The HEAD object of 0x0099C120 already defines that symbol strongly (59 B, local CWideString model), so no linked byte changes; the
  port of 0x0099C120 onto the header is queued. Its 8 binders keep their spellings (listed in the queue).
- Header (add-only): fable_string.h `FABLE_RETAIL_CCHARSTRING_NULLSTRING` (declaration only; no other row defines the macro).
- Link (`logs/link_check.txt`, all 4,269 objects): 447 -> 452 LNK4006: NullString +1 (pair debt), ??1CircularQueue +3 (one cause,
  equal bytes), ??1CCharString +1 (0x0122A5E0's header pick-any copy beside the strong owner, the kind 0x01223EC0 already shows at HEAD).
- reloc_bindings (`logs/tablecheck.txt`): 8 stand-in witnesses removed, 3 added; no target grows.
- Queued (tag land1121nf219, q12392-q12401): the three DEBT lines, 0x0099C120's header port, 0x00C443D0's self name (CircularQueue::Clear),
  0x00C3CF40 as CAudioObject's destructor (to verify), 0x013BD800 `?NoStrings@CCharString@@1JA` (to verify), the 0x013BD804 / 0x0099E9B0
  stale witnesses, the 0x0099B4D0 binder spellings.
