# c2 inline-decision hooks (Account B p25N, 2026-11-20)

gdb-side hooks on the VC7.1 c2.dll inliner, run through the repo's `tools/decomp_pipeline/linux_lane/lane_tools/c2probe` (c2dbg).
- `inl.py` (host: `inl_host.py <src.cpp> <out.json>`): logs every plain-inline decision at c2 0x1078D1AA: callee name, size, penalty, savings, cost, frequency f, score and result.
- `binl.py`: the same for the budget inliner (/Og- /d2volNN plus `#pragma optimize("g", on)`, the 0x00D17A80 recipe). It records each site's budget share.
Measured frequency multipliers (heuristic inliner): FINDINGS_LOG 2026-11-20, p25N, 0x008E18D0.
- `c2prio_x.py` (p25N, 0x00C73050): c2prio with the allowed register set at the choice, a write watchpoint on one candidate's set (C2X_WATCHID) and a probe on the cost/interference builder (C2X_XCMP). Usage in c2prio_x_README.txt.
