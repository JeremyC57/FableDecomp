# 2026-11-20 landing nf140: PRIORITY slice 11 fourth pass (32 rows + one owner fix)

Records: `naming/2026-11-20_agentNF140/` (selfcheck 0 problems). 5 entries: q4056, q4197, q4198 done; q4942 / q4943 open (not started:
NF143 has them).

## Rows (32): 1 MATCH + 31 RELOCATION_MATCH, every test PASS (`logs/recheck.log`)
- q4056 CDisplayManager::ResetDevice family (10 rows): ECreateDeviceSuccess and CCreateDeviceParameters move inside CDisplayManager
  (both PDBs), which also gives SetDisplayMode 0x009BFF40 (refresh rate `K`), SetExclusiveMode 0x009C0020 and the copy constructor
  0x009C11A0 their PDB spellings; callers 0x0041641F, 0x00417568, 0x00B24690, 0x009A59A0, 0x009C00C0 follow. In place on each row's local
  CDisplayManager model, except 0x009C11A0 (the compiler-generated copy constructor, on engine/CDisplayManager.h with the nf86 / nf88
  pick-any scaffolding).
- q4197 / q4198 CMemoryAllocator::EnterSynchronisation / LeaveSynchronisation (`KIXXZ`, both PDBs): self rows 0x009F2000 / 0x009F2010
  re-authored on engine/CMemoryAllocator.h; 20 referencing rows ported onto the header (8 through its inline Allocate, 12 through the new
  inline Deallocate opt-in). Every local CMemoryAllocator model and GF* stand-in in those rows is gone.
- Files renamed with their rows: 0x009C11A0, 0x009F2000, 0x009F2010 (sources and tests).

## Header (add-only opt-ins; token-identical without them)
engine/CMemoryAllocator.h `FABLE_RETAIL_CMEMORYALLOCATOR_ALLOCATE_FORCEINLINE` (0x009D52D0) and `FABLE_RETAIL_CMEMORYALLOCATOR_DEALLOCATE`
(12 rows). Both force inlining with `FABLE_FORCEINLINE`, as the landed 0x009F2B20 does. EXTRA_RULES (owner, 2026-11-08) prefers a
per-row `/d2inl` flag over a source device; the force is provisional, and the flag sweep (0x009D52D0, 0x00BD9B90 first) is queued. Advisor
decision (owner delegated): the rule is an order of preference, so land and sweep after.

## Decisions (advisor, owner delegated)
- Copy constructor: 0x009BFE70, 0x009BFF40 and 0x009C0020 already emitted 0x009C11A0's leaf as a COMDAT at HEAD, where it clashed with
  the strong owner (`logs/link_copyctor_control_head.log`, LNK2005). The four rows land together; the new owner is pick-any and they link
  clean (`logs/link_copyctor_new.log`, `logs/link_landing.log`). The three rows are queued to move onto engine/CDisplayManager.h (their
  copies come from local models).
- 0x009A4350 (out-of-line `?Deallocate@CMemoryAllocator@@SIXPAX@Z`, FableWin.pdb only) stays on its stand-in name; queued, needing the
  nf86 / nf88 scaffolding when renamed.

## Owner fix: 0x0122A9A0
The 12 Deallocate rows emit `?Destroy@CMemoryAllocator@@KIXXZ` as a header COMDAT; 0x0122A9A0 (leaf `_$E2`) defined it strongly from its
local CMemoryAllocator model, so the context link gave 10 LNK2005s. Its Destroy is now `inline` (pick-any); the row still grades
RELOCATION_MATCH PASS and the clash is gone. Queued to move onto the header.

## Checks
- 58 rows rechecked, all good; gate 0; bindings_multi_check 1 (the pre-existing NullCoord).
- Table: 0x0129BA40 4 -> 5 is a witness (0x009BFB30 binds a spelling another row already binds there). Shrank: 0x013CAA90 11 -> 8,
  0x013CAA94 5 -> 2, 0x013CAA70 4 -> 2, 0x009F2000 3 -> 1, 0x009F2010 3 -> 1, 0x009F2A40 3 -> 1, 0x009F2310, 0x009F1F10, 0x009F2920,
  0x009F2870, 0x009BFF40 each 2 -> 1.
- One definition (`logs/symbol_diff.txt`): own leaves, shared-header inlines (Allocate, Deallocate, Destroy, `??_GCMemoryAllocator`) and
  COMDATs. Context link (`logs/link_context.log`: the 32 objects + every rechecked includer, 64 objects, after the owner fix): no clash on
  any symbol this landing adds. Remaining LNK2005s are pre-existing: the CEngineLandscapeMemoryManager four (0x00BD9B90 / 0x00BD9CE0 /
  0x00BDA150, reproduced with HEAD objects in `logs/link_control_head.log`, already queued from nf139) and ClearPerformanceStats (0x009F26B0
  / 0x009F26D0, untouched rows; queued).
