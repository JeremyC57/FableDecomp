# c2probe (2026-11-08): the c2.dll register-allocator debugger probe

Report: `docs/pipeline/C2_ALLOCATOR_PROBE.md`; tool: `tools/decomp_pipeline/linux_lane/lane_tools/c2probe/c2prio.py`. This directory holds the
probe micro-TUs (`probe/`, row variants in `probe/var/`), the probe output per parked row / variant (`rows/*.out`, grader diffs `rows/*.diff`),
`STATUS.tsv` (4 parked rows diagnosed, none matched) and `LESSONS.md`. The agent's README follows.

# c2probe (2026-11-08): the c2.dll register-allocator probe

`tools/c2prio.py <src.cpp> [--func NAME] [--flags "..."] [--trace] [--blocks] [--rotation] [--json f] [--keep]` reads
VC7.1 13.10.3077's own register-allocation decisions for the functions of a source file (priority, tie key, spill benefit,
allowed set, register, colouring steps, per-block priority shares, scratch-register rotation). `tools/c2dbg.py` is the
compile-under-winedbg harness. `C2_ALLOCATOR_PROBE.md` documents the method, addresses, structures, formulas, validation
and the per-row findings; `LESSONS.md` has the GOTCHAS one-liners; `STATUS.tsv` the four parked rows looked at (none
matched). `probe/` holds the micro-TU added here (consts2.cpp: the constant value node) and `probe/var/` the row spellings
tried; `rows/` their probe outputs and grader diffs. `tools/cfg.py`, `explore1.py`, `explore2.py` are the static
function-carving script and the two exploration gdb scripts that found the hooks (they need `work/c2.dis`, regenerate it
with `objdump -d -Mintel --no-show-raw-insn $FABLE_VC71/bin/c2.dll`; not committed).

Quick start on the lane: `source $S/wine/env.sh; python3 tools/c2prio.py probe/consts2.cpp --trace`.
