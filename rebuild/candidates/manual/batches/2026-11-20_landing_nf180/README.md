# Landing nf180 (Account A, 2026-11-20): persist traits / counted pointer / STL container names (15 rows; 0x0083EEE0 held)

Agent NF180 (round 10) handed back early (context) with 38 entries: 7 done, 2 done-already, 1 rejected, 6 blocked, 6 deferred, 16
open. Its files are in `naming/2026-11-20_agentNF180/`. Open entries, and entries deferred only because their rows were locked by
agents since landed, go back to the queue unclaimed.

- 15 rows land. Self renames (8): std::vector<pair<CCharString,CScriptInfo>>::_M_insert_overflow 0x00CB6FD0, std::sort 0x00CB7780 and
  CVectorMap<CCharString,CScriptInfo>::Sort / Update 0x00CB77E0 / 0x00CB7800 (q4221: Sort and Update were swapped stand-ins),
  map<CCharString,CCharString>::find 0x009EF360, map<CCharString,long>::find 0x00515700, and the CCountedPointer<CVillageTaskPatrol /
  CVillageTaskPickCrops> ??_G rows 0x0065E951 / 0x0065E96D. Binders moved: the list-clear rows 0x0065FE9A / 0x0065FECB, the
  CCodeSectionManager map rows 0x009F0570 / 0x009F0500 / 0x009F05A0, 0x00CB7210, 0x00CB5D80. Files follow the self renames.
- Header: stlport/fable_containers.h (`orig` equals HEAD) gets FABLE_RETAIL_STLP_SORT_FINAL_INSERTION_OUTOFLINE inside the
  SORT_FASTCALL_BODY block (add-only; only 0x00CB7780 uses it).
- Held: 0x0083EEE0 (q4190, LHNetworkLib::LHHttp2::GetUntilCRLF). Its port graded RELOCATION_MATCH, but its object newly emits
  vector<char>::_M_fill_insert at 266 bytes against the owner row 0x004110D0's 289 (advisor pick-any rule, EXTRA_RULES 2026-11-20).
  The row, its test, its catalog block and its manifest / oracle lines are back at HEAD (`logs/recheck_after_hold.log`: all 16 objects
  of the tree, RELOCATION_MATCH PASS; table rebound); the draft is in `naming/.../held/`, q4190 is requeued, and landed 0x0055F710's
  293-byte copy is queued as DEBT.
- Pick-any check (`logs/pickany_check.txt` plus hand comparisons against the owners' stand-in symbols): the iterator comparators
  (0x0041C820 / 0x0041C830), the pair<CCharString,CScriptInfo> copy ctor (0x00CB5F50, 57 bytes) and __uninitialized_copy (0x00CB60F0,
  106 bytes) copies are byte-identical to their owners.
- Coordinator calls: q3966 waits until 0x008F8C70's old-leaf binders move with it (self-rename rule; the global LSocket family);
  q4959's rejection stands and its 4-row Sort / Update respell is queued; 0x008CED20 / 0x007190F0 / 0x00741EB0 stay blocked on one
  definition.
- Recheck (`logs/recheck.log`): 16 RELOCATION_MATCH, every test PASS (before the hold). rebind multi-check 1 (baseline); split gate 0
  created; includers 1865 good, 0 bad (stlport/fable_containers.h is widely included); owner recheck good. tablecheck: eight addresses
  shrink (0x0099EC30 62 -> 61, 0x009D2DD0 4 -> 2, ...), none grows.
- Link (`logs/link_landing.log`): 0 LNK2005 against 1 at HEAD (`logs/link_control_head.log`).
