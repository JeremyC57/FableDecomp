# Landing nf293 (Account A, 2026-11-23): T1 signature, NF290's continuation (51 rows + 1 test, 3 header opt-ins)

Hand-back: `pending/2026-11-23_nf293_handback/`; helpers (nf.py with block-bounded catalog lookups and NF_TREE full-tree prechecks,
decl.py, declsum.py, calls.py, ins.py) under `naming_batch/agent_tools/nf293/`. 44 of 390 entries reached; the rest go to NF297, except five
all-or-nothing families queued in T3_PORT_QUEUE.tsv as FAMILY UNITS (CCombatActionBase 0x00857F30, RegisterCombatant 0x006EDC50,
CCombatSequenceBase 0x008B0AB0, CCreatureActionBase 0x00693B30, PlayAnimation 0x008424F0).

- **NF290's ruled decisions (all done):** 0x006628E0 (R1: binder 0x006A98F0's TU emits no table; `virtual` on its local base and override,
  leading pad shrunk to keep every offset; the override's own definition moves QAE -> UAE as both PDBs give; the exe has only its vtable slot
  word, no call). Header opt-ins, asserts pinned first where unpinned: `FABLE_RETAIL_CTHREADEDFILEMANAGER_PROCESSOBJECTS_PRIVATE`
  (engine/CThreadedFileManager.h, 0x0098ECD0), `FABLE_RETAIL_CUSERPROFILEMANAGER_LOAD_PUBLIC` (fable_user_profile.h, 0x0040D350; 0x0040D400's
  test defines the macro too), `FABLE_RETAIL_CWORLDMAP_ALLOCATEMAPUID_PROTECTED` (engine/CWorldMap.h LOADFROMFILE branch, 0x004FCE20 as one
  unit with 0x00506F30 / 0x00507C30; rebased on NF292's landed CWorldMap.h). equiv_check over all 57,146 landed rows: 0 differ; the planted
  control flags its 5 includers. 0x009CC530 (allowed: only unreferenced pick-any local copies renamed; its slot-19 virtual, called by the
  made-up GetBankHandle, respelled to the protected const GetEnumerationType 0x009CBD10).
- **Signature respells (22 done, 5 partial):** pointer -> reference with `*arg` at the call, long -> unsigned long / enum / bool, const /
  access; entry 56 also drops a HEAD `const_cast` the PDB-const callee makes unnecessary. Partials leave a strong same-TU stand-in copy
  (0x004FE630), a PAX stand-in self (0x00A3AB90, port onto CCompressorZlib.h), and two CThing*-for-CThingCreatureBase& cast binders (T3 cast
  debt, ruling).
- Proof: objsec HEAD v new ALL_EQ for every changed row (`.drectve`, DEF sets, tables); every HEAD and new object MATCH / RELOCATION_MATCH;
  links: no duplicates, nothing HEAD resolved left unresolved. 6 self renames, 46 caller lines.
- Table: 15 deleted (2 stale), 16 respelled; no symbol gains a second target. NAME_FIXES 5 (incl. the PlayIntoLoopOutOfAnimation 5-arg
  ctor 0x00842960 and GetBankHandle in 0x009CC530's test), queued `land1123nf293`.

## Recheck
`logs/recheck.log` (landed tree): 51 RELOCATION_MATCH PASS, 0 DIFFER, 0 RELOC_CONFLICT.
