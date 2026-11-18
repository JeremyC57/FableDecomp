# 2026-11-17 landing nf85: name-fix round 7, NF85 (47 rows: the nf82 follow-ups, q11056..q11096)

Owner: "Continue the naming fix queue with two agents until complete", "Keep going until queue is done", then "Decrease to 2 agents
at a time once next agent finishes".

NF85 took the 41 entries queued at the nf82 landing (landed rows that bound NF82's addresses under stand-in or wrong-element spellings).
Brief, list and records: `naming/2026-11-17_agentNF85/`.

## Rows (STATUS.tsv, 103 = 99 locked + 4 claimed through the lock): 47 ported, 56 not-a-binder, 0 blocked
- **STLport sort / heap helpers** (`__pop_heap`, `sort_heap`, `__pop_heap_aux`, `__make_heap`, `__adjust_heap`, `__insertion_sort`, `__copy_backward`,
  `__introsort_loop`, `__final_insertion_sort`, `__push_heap`) of the CCharString-keyed vectors, through new fable_containers.h opt-ins; several
  were catalogued as `SGI::...` or `_global::...` stand-ins, and 0x004E36C3 `make_heap` was catalogued as `CMemoryAllocator::operator_delete`.
- **`map<CCharString, ...>::operator[]`** for seven value types, CCharString tree `erase` / `_M_upper_bound`, and the quest-persistence
  `CVectorMap` LowerBound / UpperBound (count-first bodies under new CVectorMap.h opt-ins) with ClearQuestPersistScript's callees 0x004B84F0
  `__copy` and 0x004B93E0 `__destroy_aux`.
- **Others:** 0x005A731F CTCInventoryTrade::ConstructSellActionsListPC (now on the shared CThingPlayerCreature header's retail branch),
  0x008364B0 `CCountedPointer<CAnimFilterBase>::DeleteData` (was `sub_008364b0`), 0x009EAFF0 CConsole::SetGuessMode.
- **0x004E41DD** (q11073, done-already): its catalog name was wrong (`_Sort_heap<...>`); the coordinator applied NF85's section-1 line and
  regraded the row.

## Entries (APPLIED.tsv, 41): 36 done, 5 done-already, 0 blocked
REJECTED.tsv lists 6 stale binder claims (5 WORKLIST rows and 0x006276F0); none changes an entry's symbol.

## Rows and checks
- **Section 1: 39 self renames (66 file moves).** Modules match their siblings' catalog spellings.
- **Headers (10 opt-ins in 4 files):** fable_retail_banks.h (`NBANKFILEMANAGER_CCONTAINEDBANK_CTOR`, 2 asserts pinned), stlport/fable_containers.h
  (`STLP_SORT_COPY_BACKWARD_AUX_INLINE`, `_SORT_HEAP_BODY`, `_POP_HEAP_AUX_FASTCALL_BODY`, `_MAKE_HEAP_FRONT_BODY`, `_FINAL_INSERTION_SORT_BODY`),
  engine/CVectorMap.h (`CVECTORMAP_LOWERBOUND_COUNT_FIRST`, `_UPPERBOUND_COUNT_FIRST`, `_ERASEFROMKEY`), engine/CQuestManager.h
  (`CQUESTMANAGER_CLEARQUESTPERSISTSCRIPT`). Token-identical without the opt-ins for 60 includers (`tokid.log`); `lane_merge/assert_names_check.py`:
  0 mismatches against 49b943df65.
- **Funclets:** none affected (1,355 checked, 0 broken).
- **Recheck (`logs/recheck.log`):** all 47 rows plus 0x004E41DD graded from the repo after landing: 46 RELOCATION_MATCH and 2 MATCH, every
  test PASS.
- **Checks:** `naming/rebind.py` removed 69 binding lines and added 44; bindings_multi_check is at its baseline (`NullCoord`).

## Name fixes queued (`land1117fu`, 29)
NF85 swept all 349 retail call sites of its renamed addresses (`sites_check.txt`): 27 (target, caller) pairs in 25 landed rows still bind
stand-ins (NAME_FIXES.tsv). Plus two lines: 0x008AA480 (a caller whose landed row probes DIFFER and models a Dinkumware map) and 0x005A45F6
(the twin of 0x005A731F, still on a local CThingPlayerCreature).

## Rule slips
- Three Bash calls exited non-zero (a `cat` of a missing lock, a wrong pin_asserts.py path, an `ls` of a file not yet written).
- 0x005A731F's first port copied its twin's local CThingPlayerCreature class; NF85 moved it onto the shared header before hand-back.
