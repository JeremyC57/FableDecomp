## 0x00D17A80

- (wf19f, 0x00D17A80) A per-site inline split where the EARLIEST call sites get the least (trivial ctor called, later dtor sites fully expanded) is c2's BUDGET inliner, not site frequency: c1 flags functions for the heuristic inliner only with /Og on the command line; with `#pragma optimize("g", on)` + `// Flags: /Og- /d2vol10` c2 gives each inline site remaining-budget/remaining-sites (1000 per function) and calls any callee over 40 IL instrs the share does not cover. Count the function's inline call sites exactly (accessors like GetMasterData() are sites).
- (wf19f) c2's inline decisions can be traced: run cl under gdb (`gdb --args /usr/lib/wine/wine cl.exe ...`, `catch exec`, hbreak *0x107724b7, then breakpoints at 0x1078D1AA heuristic / 0x107284A3 budget / 0x107723E7 expand); scripts in w22/wf19f/00d17a80/a4/ (trace.py, trace_gdb.py, btrace_gdb.py).
- (wf19f) /d2inlT is the heuristic's threshold (default 74, `score < T`), /d2inlS the always-inline size (default 9, +5); neither is a budget. inline_depth default is 16.
- (wf19f repair, 0x00D17A80) The global at 0x0143E8F8 is `pDSTGame` (never `g_pGameScriptInterface`); a row that needs a `/d2` switch must say in its own header comment which retail bytes need it and that it emulates an unknown build setting (here /d2vol: threshold 8, vol 7 fails).

