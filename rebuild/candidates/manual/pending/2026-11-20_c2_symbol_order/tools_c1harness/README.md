# c1xx / c2 compile-order harness (p25R, 2026-11-21)

Tools for observing VC7.1 13.10.3077's front end (c1xx.dll) and c2's function compile order under Wine + gdb.
Set up with `source $S/wine/env.sh; export W=<work dir>`.

## Front end (c1xx.dll)
- **c1dbg.py**: the lane's c2dbg method applied to c1xx.dll.
  - A copy of c1xx.dll gets `jmp $` at `_InvokeCompilerPass@12` (RVA 0x68C9F; the original bytes E8 EC are restored by gdb).
  - cl.exe runs with `/Bx<copy>`, and c1xx loads at its preferred base 0x10400000.
  - Runs go in `$W/tmp/c1runs/<token>` and can run in parallel.
- **host.py**: `host.py <gdb-script> <src.cpp> <out.json> [json-config]`. Set `"flags"` in the config for extra cl flags, and `KEEP=1` to keep the run dir. Each run takes about 6 s.
- **common.py**: gdb-side helpers (Hook, rd/u32/reg/cstr, stack(), run()).
- Scripts:
  - **wr.py**: logs every msvcr71 `fwrite` / `_write` c1 makes, which is the IL files. The symbol file is the fwrite that holds the decorated names.
  - **wp.py**: hardware write watchpoints on `cfg["watch"]`, logging pc, the new dword, the c1 return chain and registers.
  - **wpopt.py**: the same watchpoints, plus the strings found in registers and on the stack. This names the command-line option that wrote a byte: `-Og` writes 0x1058B7FC.
  - **symflags.py**: `symflags.py <wr.json> [substr]` prints each symbol-file record's name and the 16 bytes that follow it. The flag word is the 2- or 4-byte value c2 reads into sym+0x69.

## c2 side (run with the lane's `tools/sched/host.py`)
- **c2or69.py / c2or69s.py**: log every OR into a symbol's +0x69 (site, name, value, chain). At each compile start (0x107AFF88), c2or69s.py also logs the remaining compile list.
- **c2head.py**: logs writes to the compile-list head [0x10881D68].
- **c2edges.py**: at the needed-closure (0x1070D3DD) and at the topological sort (0x1070CFDF), dumps the function-entry list [0x1087F4C8]. Each entry has sym+0x37, sym+0x69, entry+0x50 and its callee edges (edge+0xC bit 1 = ignored back edge; callee +0x37 / +0x69).

## Facts measured with it (FINDINGS 2026-11-21)
1. The c1 inline-info record (IL flag 0x1000) is global. It copies the `-Og` byte, so no per-function source attribute clears it.
2. The bit that selects the inline heuristic (sym+0x69 & 2) is set by c2 at 0x107AFF88 when the function's OWN compile starts. "Budget path" therefore means the callee was not compiled before its caller.
3. Compile order:
   - The topological sort (0x1070CFDF) places callees first.
   - It ignores edges only when they are DFS back edges (0x1070CE78), i.e. cycles.
   - "Needed" (0x20) propagates over callee edges at read (0x1070D3DD, skipped when callee+0x37 & 0x400).
   - Functions reached only through vtables (??_G) are marked during a compile (0x107C0879) and compiled where the list walk reaches them.
- c2path.py (c2 side): per inline decision at 0x1078CF8D: the callee, its +0x69 flags (bit 2 = compiled already, 0x10 = own pass),
  whether it has a record, the path (heur 0x1078D1AA / budget 0x107284A3) and the result. Budget inputs: the running total
  [0x10882B5C], rejected when > 0x88B8 (35000); otherwise accepted when callee size word +0x6D <= 0x28 (40) or <= the limit
  argument [esp+0x20]. Also logs compile starts and pipe events.
- PlayWave 0x00F1EED0, natural base p25Q g2/n0/n0.cpp, overlay w22/wfE3/hdr (tmp/c1/pw_path.json): PlayWave is compiled 47th of 47.
  Every callee (~CTimer, CTimer ops, every vector<CScriptThing> member, ~CCountedPointer, ReleaseData, ~CScriptThing, ??_G) is
  compiled before it, and every non-forceinline decision in PlayWave is heuristic with bit 2 set. NO budget-path decision occurs in our build.
  The budget path has a running total (35000), so a caller whose callees were not yet compiled WOULD get per-site splits
  (early sites accepted, later sites rejected once the total passes 35000).
