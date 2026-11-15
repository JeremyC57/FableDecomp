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
