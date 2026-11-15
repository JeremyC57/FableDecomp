# agentP6 best sources for the four parked Codex pass 22 rows (2026-11-15; work continuing)

Each compiles on the repo headers (Codex's private header profiles merged in landings gc and gd) and passes its behaviour test.
vland dry runs, strict headers:

| row | grade | measured reason |
|---|---|---|
| 0x00BF0B20 SetupOutlineStencilStates | DIFFER(1290v1290), 3 lines | scheduler tie: the B load and the G store both priority 0x17400, the lower index (G store) wins |
| 0x00F3E120 FindBestStartVertex | DIFFER(1411v1411), 2 lines | two immediate-only constant records (0x2aaaaaab, -1) set the reload order; the order that fixes the reloads also flips the prologue stores |
| 0x0064BFBA CDrawHighlightingTargetingCursor::Draw | DIFFER(2217v2223) | `vcall * Call().Y` evaluates max-first; only a const-ref binding gives dims-first, at the cost of a frame slot |
| 0x00675E20 CCreatureDef::Transfer | DIFFER(1912v1912), 177 lines | scratch-register rotation starts at the first inlined traits arm, where retail gives every stream push eax |

`LESSONS.md` has the measurements; `HEADERS.md` the header-profile merge.

## Session 3 (no new match)
- 0x00F3E120: c2.dll hooked (`tools/tfree.py`, `tools/tcnt.py`). After constant-candidate creation, c2 walks its 64-bucket constant hash
  (0x10882050, bucket = value & 0x3F) and frees every constant with use count 0 (0x10784D3B); the next root candidates reuse those
  records LIFO, so `this` takes -1's id and `vertex_link_map` takes 0x2aaaaaab's. The order is set by the constants' values. Retail's
  c,s,b source needs one more count-0 constant in buckets 44..63 walked after -1, most likely the -4 of the three `size() == 0` guards
  (`test reg,0xfffffffc`, bucket 60) in a spelling that leaves its use count 0.
- 0x00BF0B20: no source lever keeps retail's bytes (the B byte has no other consumer; the debug bodies fix the R,G,B,A order).
- 0x00675E20: a one-arm micro-TU rotates scratch registers from the first arm in every variant; retail's first arms look like allocator
  candidates. Next: hook c2's expression-temp routine 0x10775ACC.
- 0x0064BFBA: both PDBs give C2DExtentsI only constructors and Initialise, so no accessor spelling exists.
