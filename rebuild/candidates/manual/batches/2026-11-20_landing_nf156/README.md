# Landing nf156 (Account A, 2026-11-20): NUISystem names (22 rows + 0x00485524)

Agent NF156 (round 10) handed back 18 entries (7 done, 8 done-already, 3 blocked); its files are in `naming/2026-11-20_agentNF156/`.

- 22 rows: 4 self renames (RENAMES section 1: 0x0054EC70, 0x0052ECC0, 0x0052C870, 0x004854DF) and 31 caller respellings onto spellings
  other landed rows already bind; 0x00555390 is ported onto NUISystem_CScrollableList.h / NUISystem_CDeletion.h.
- Header: fable_ime.h, one declaration-only opt-in (FABLE_RETAIL_CIME_FINALISESTRING); its one changed line is a pinned assert.
- Coordinator addition (advisor decision, owner delegated): 0x00485524's local CActionParamCCountedPointer ctor now stores
  UI_ACTION_PARAM_TYPE_COUNTED_POINTER (9), as retail's 0x004854DF does. Its pick-any copy of that ctor and 0x004854DF's are now the
  same body (both 0x22 bytes, checksum 0x85748933); before, the linker could keep the wrong-tag copy at 0x004854DF. RELOCATION_MATCH.
- Same-name pick-any copies checked by COMDAT size and checksum: the four in-place deque rows' std::stack::top equals the header-built
  copy in 0x00596763 / 0x0059686D (5 bytes, 0x25c4a5ae). Accepted.
- Recheck (`logs/recheck.log`): 23 RELOCATION_MATCH, every test PASS. rebind multi-check 1 (baseline); split gate 0 created (no
  ??_G / ??_E split at 0x0054EC70); includers 122 good, 0 bad. tablecheck: seven addresses shrink; 0x0042BED4 shows 1 -> 2 only because
  the table now lists 0x0059DB82's N_0042bed4::CAction stand-in its HEAD object already bound (SAME; NF156 NAME_FIXES).
- Link (`logs/link_landing.log`): 5 LNK2005, against 3 at HEAD (`logs/link_control_head.log`). The increases are on symbols that
  already clash at HEAD: a fourth strong CViewport(long) (0x0054EC70 takes its twins' constructor-TU shape), and the CDeletion copy
  ctor, which HEAD row 0x005A887D already emits against owner 0x0042CD84 (`logs/link_control_head_cdeletion.log`). Queued as one DEBT.
- Queued: the q4911 GPreviousSelected header claim, 0x005959AB / 0x0059A238 (q6171), and NF156's NAME_FIXES.
