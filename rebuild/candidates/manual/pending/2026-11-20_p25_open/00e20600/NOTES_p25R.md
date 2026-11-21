# 0x00E20600 CTraderToEscort::Main -- p25R (from acctC wfE4 draft, 14 sessions; read wfE4/NOTES.md first)
- Baseline on HEAD: rows/e20600/base.cpp = wfE4/00e20600.cpp, overlay hdrE4 (only NScript_CQR_EscortTraderScript.h TRADERTOESCORT;
  the other hdrdiffs are in HEAD now), default flags: DIFFER(14526v14510) (reproduces).
- Devices in the draft: 4 depth-0 regions (lines 54/58 predicate dtor defs, 507/516 door-wait + M_TraderExit), 4 row-local
  FABLE_FORCEINLINE (CTimer op= / GetTime, CScriptThing::GetPos / IsOpenDoor), FITTED for(;true;NSF), FITTED duplicated AddItemToContainer.
- Open residue (a): ~CIsThingAlive / ~CPredicate_And<CIsThingAlive,CIsThingAlive> behave budget-path in retail (c1 IL flag 0x1000
  inline-info record / sym+0x69 bit 2). New angle: read c1xx.dll for the writers of those flags.
## Device ablation (p25R, default flags, hdrE4; tools/gqe.sh = q.py + wfE4 hunks.py, per-variant work dirs; rows/e20600/D*.cpp)
| variant | grade | h |
|---|---|---|
| base (all devices) | DIFFER(14526v14510) | 173 |
| D1a no depth-0 on ~CIsThingAlive def | 14494 | 158 |
| D1b no depth-0 on ~CPredicate_And<CITA,CITA> def | 14558 | 174 |
| D1 neither predicate region | 14494 | 177 |
| D2 no door-wait region | 14546 | 179 |
| D3 no M_TraderExit MoveToPosition region | 14558 | 174 |
| D4 CTimer op= / GetTime plain inline | 14478 | 157 |
| D5 GetPos / IsOpenDoor plain inline | 14494 | 169 |
| D6 for(;;NSF) | 14542 | 172 |
| D7 one AddItemToContainer after the if/else | 14510 | 177 |
(h counts differing lines; the base's predicate residue shifts frame offsets everywhere, so h is coarse: D1a/D4 have FEWER lines but wrong length.)
- Ego_d (ref/e20600/egod_lines.tsv, 329 lines, 38124 B) is a different, much larger revision (e.g. line 509 278 B, 529 265 B, 537 255 B
  inside the bonus block); the bonus-arm and loop-header lines cannot be mapped one-to-one, so (c) for(;true;NSF) / the duplicated
  AddItemToContainer stay Inferred-undecidable unless a mapping is found.
- c1 RE started: c2 reads the symbol flag dword via 0x1076e2a6 (2-byte, or 4-byte when the high bit of byte 2 is set) in the record
  [dword +0x71, dword +0x75 opt][word +0x6d][dword flags +0x69][word +0x6f]; c1xx (vc71rtm, .text 0x10401000..) encoders inline the
  same 0x7fff/0x8000 split (e.g. 0x104bb0b1, 0x10484851). The function-symbol writer is not located yet; c1xx needs its own gdb
  harness (c2dbg only hooks the c2 copy).

## c1xx/c2 harness and the budget-path mechanism (p25R, 2026-11-21)
Tools (W/tools/c1/): c1dbg.py (c2dbg's method on c1xx.dll: copy with `jmp $` at _InvokeCompilerPass@12 RVA 0x68C9F, cl /Bx<copy>,
c1xx at 0x10400000), host.py <gdb-script> <src> <out.json> [cfg], common.py; scripts wr.py (every fwrite/_write c1 makes: the IL
files), wp.py (hardware write watchpoints, cfg watch=[addr..]), wpopt.py (watch + strings in regs/stack = which option wrote it);
symflags.py (c1 symbol-file records: name + following bytes). c2-side scripts for tools/sched/host.py: c2or69.py / c2or69s.py
(every OR into a symbol's +0x69 with site, name, chain; compile-list order at each compile start), c2head.py (compile-list head writes).
Measured facts (minis tmp/c1/t1..t5.cpp, and the row):
1. c1 composes the function-symbol flag word at 0x1046FF83..0x104700B9 (written by 0x1044F414). Bit 0x1000 = the GLOBAL byte
   [0x1058D0E8] = copy of the -Og option byte [0x1058B7FC] (wpopt: written by option '-Og'). So the inline-info record exists for
   EVERY function iff /Og: no per-function c1 attribute clears it (pragma optimize("g") does not touch it: t2).
2. The heuristic's bit sym+0x69 & 0x2 is NOT from c1: c2 sets it at 0x107AFF88 in the compile driver when a function's own compile
   STARTS (flags 0x1ce8 -> 0x1cea, then 0x10 at 0x10772476 = own inline pass). So "budget path" = the callee has NOT been compiled yet
   when the caller is compiled.
3. Compile order: the driver walks the list [0x10881D68] (+0x91 links) compiling functions with 0x20 set; the list is built by the
   topological sort 0x1070CFDF (a function is placed once all its callee edges are placed or ignored); edges are ignored (edge+0xC bit 1)
   only as DFS back edges (0x1070CE78, roots = the 0x1087F4C8 function list), i.e. only in call-graph CYCLES. 0x20 ("needed") is set at
   read for functions reachable from needed ones through callee edges whose callee+0x37 lacks 0x400 (0x1070D3DD); functions reached
   only through vtables get 0x20 during a compile (0x107C0879, e.g. every ??_G) and are compiled where the list walk reaches them.
So retail's Main was compiled BEFORE ~CIsThingAlive and the CPredicate_And<CITA,CITA> members (also matches retail's layout: those
COMDATs follow Main). In c2 that needs Main -> predicate edges to be back edges (a cycle through Main) or absent at sort time, or the
predicate functions not needed until Main's compile marks them. Retail has no direct call to Main (only its vtable slot 0x012E151C+4).
Next: dump Main's callee-edge list (entry+0xC chain, edge+0xC bit 1, callee +0x37 & 0x400) in our build; find the source construct
that makes the predicate callees non-edges / late-needed (callee+0x37 & 0x400 is the first candidate: what sets it).
- Edge dump (tools/c1/c2edges.py on rows/e20600/base.cpp, tmp/c1/e2_edges.json): Main has direct callee edges to ~CScriptThing,
  CScriptThing ctor/GetPos/IsOpenDoor, CIsThingAlive ctor/dtor, ~CPredicate_And<CITA,CITA>, operator&&; no edge is a back edge (bit 1
  clear) and NO symbol in the TU has +0x37 & 0x400 (values 0x20a2/0x20a3/0x20a7), so everything is needed at read and sorted
  callees-first. The only c2 0x400 setter feeding a symbol template is 0x1070A03F (IL symbol read, kind not identified).
- Leads not yet tested (main 2026-11-21): (a) retail reaches the predicate dtors only through the vtable / ??_G (no direct edge from
  Main: destroyed via a base pointer, or the expansion comes from a devirtualised call); (b) a same-TU function referencing them first;
  (c) definition order in the TU (probably moot: the sort ignores input order except for ties). Harness copy + README: batches/tools_c1harness/.
- Budget path rule (0x107284A3): reject if the running total [0x10882B5C] > 35000; accept if callee size (+0x6D) <= 40 or <= the limit
  arg; so in a budget-path world splits within one caller come from the running total. PlayWave dump: see batches/tools_c1harness/README.md.
