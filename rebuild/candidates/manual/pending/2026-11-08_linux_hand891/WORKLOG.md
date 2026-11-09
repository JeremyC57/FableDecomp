# Chunk 891 worklog
- Rows: 0x005A4CDD Sell (3668), 0x005A5B31 Wanted (3128), 0x005AC10E Buy (3532). Extents verified with xdis: each ends in `ret`
  and the next function's `push ebp` follows directly (0x005A5B31, 0x005A6769, 0x005ACEDA). Single functions.
- Plan: Wanted first (smallest, fewest unknowns), then Buy and Sell (share the subtitle / desc code).
- 0x005A5B31 Wanted: RELOCATION_MATCH at grade 4 (w4): drop optimize(y,off) (biased lea ebp frame), FORCEINLINE_TEXT_EQ, same-TU CBasicString<T>::Compare/operator==(const T*) bodies under auto_inline(off) (mod/ref: no re-test of PStringData after the out-of-line ==).
- NOTE: one probe grade was run on 0x005A8C66 (sibling, landed) by mistake (sp1 experiment: || temps keep the jmp form there); not repeated.
- 0x005A4CDD Sell: drafted from Buy + Wanted (s1, 9 hunks) -> s4 1 hunk (named `pBranch` key copy, std::make_pair insert value).
  Residue: loop-2 head (retail duplicates the head test and threads its empty exit past the ItemsToBuy insert loop; ours cross-jumps
  to the shared bottom test). Wrapping both loops in `if (!buyableItems.empty())` gives retail's flow but one `mov esi,eax`
  (c2prio: begin() temp prio 14 > `it` prio 10). 18 grades + 30 compile-only probes. PARKED (differ/005a4cdd.cpp = s4).
- 0x005AC10E Buy: b1 2 hunks unchanged after 58 compile-only probes; c2prio: begin() temp (2 refs, prio 20) colours eax before `it`
  (13); our optimizer rotates the loop whenever a call follows it (truncation probes), retail did not. PARKED (differ = b1).
- Wanted test written (C++03 model; g++ PASS, VC PASS); src comments finalised, regraded RELOCATION_MATCH.
