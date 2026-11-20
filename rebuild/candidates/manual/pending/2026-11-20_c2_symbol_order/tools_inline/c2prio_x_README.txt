c2prio_x.py = lane_tools/c2probe/c2prio.py (copied 2026-11-20, p25N) + three probes, same CLI:
- every `chosen` event also records `allowed_at_choice` (the candidate's +0x20 set when 0x107833f2 picks), i.e. AFTER the
  cost/interference builder 0x1078368e; compare with the `choose` event's set (before the builder).
- C2X_WATCHID=<id>: hardware write watchpoint on that candidate's allowed-set chunk (base 0) from its choose to its chosen;
  each write -> event `wp` {pc, new word, regs, candidates found in regs/stack, c2 return addresses}.
- C2X_XCMP=1: breakpoint at 0x107A00CF inside the builder: `cmp ecx,eax` of a per-tuple counter (ecx) against
  (sum of dwords 0x108A7B1C..0x108A7B28) - 1 (eax); equal -> jcc 0x107A00D1 -> 0x10737254: allowed &= set 0x108B0054
  ({ebp,esi,edi}, the callee-saved mask) via 0x1075A3D2 (bitset AND, the write at 0x1075A417). Events `xcmp`
  {id, ctr, total_m1, tup, tline (tuple +0x14, relative line), ...} and `xops` (operand guess: list at tuple +0x28).
Usage: source $S/wine/env.sh; cd repo; C2X_XCMP=1 python3 $W/tools/c2x/c2prio_x.py SRC --func NAME --flags "..." --json out.json
