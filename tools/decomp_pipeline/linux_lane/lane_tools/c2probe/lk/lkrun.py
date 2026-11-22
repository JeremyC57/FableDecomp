#!/usr/bin/env python3
"""lkrun.py -- compile one source under VC7.1 3077 with c2.dll's operand-sort comparator logged (gdb, Wine).

  lkrun.py <src.cpp> <out.json> [--func SUBSTR] [--ov OVERLAY_DIR] [--syms] [--depth N] [--obj OUT.obj] [--flags F ...]

Runs c2probe/c2dbg.probe with tools/lk_gdb.py (see its docstring for the hooks), keeps only the events of the
functions whose c2 name contains SUBSTR (default: every function), writes the JSON log and checks it is complete:
gdb saw the process exit, cl returned 0, the target function's PIPE event was seen, an object was produced.
A failed check exits 2 and says why (an incomplete log must never be read as data).
Environment: source $S/wine/env.sh; S must be the lane scratchpad.  C2PRIO_RUNS defaults to $S/c2lk_runs
(c2dbg deletes run dirs older than 6 h in that folder).  0x00B4D370 (7 KB): 15 min alone, ~38 min with --syms and two runs in parallel (LK_TIMEOUT, default 5400 s).
"""
import argparse, json, os, sys, time
sys.dont_write_bytecode = True
os.environ.setdefault("PYTHONDONTWRITEBYTECODE", "1")
S = os.environ.get("S") or sys.exit("lkrun: S is not set")
os.environ.setdefault("C2PRIO_RUNS", os.path.join(S, "c2lk_runs"))
HERE = os.path.dirname(os.path.abspath(__file__))
C2PROBE = os.environ.get("LK_C2PROBE", "/home/user/FableDecomp/tools/decomp_pipeline/linux_lane/lane_tools/c2probe")
sys.path.insert(0, C2PROBE)
import c2dbg  # noqa: E402


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("src")
    ap.add_argument("out")
    ap.add_argument("--func")
    ap.add_argument("--ov")
    ap.add_argument("--syms", action="store_true")
    ap.add_argument("--depth", type=int, default=4)
    ap.add_argument("--obj")
    ap.add_argument("--keep", action="store_true", help="keep the run directory")
    ap.add_argument("--flags", nargs=argparse.REMAINDER, default=[])
    a = ap.parse_args()
    if a.ov:
        os.environ["FABLE_HDR_OVERLAY"] = os.path.abspath(a.ov)
        # c2dbg.includes() reads the variable at call time
    cfg = {"func": a.func, "depth": a.depth, "syms": a.syms}
    t0 = time.time()
    tr, log, run = c2dbg.probe(a.src, os.path.join(HERE, "lk_gdb.py"), cfg, flags=a.flags, keep=a.keep,
                               timeout=int(os.environ.get("LK_TIMEOUT", "5400")), obj_out=a.obj)
    dt = time.time() - t0
    if tr is None:
        print("lkrun: FAILED, no trace:", log[-2000:])
        sys.exit(2)
    tr["meta"] = {"src": os.path.abspath(a.src), "flags": a.flags, "overlay": os.environ.get("FABLE_HDR_OVERLAY", ""),
                  "func": a.func, "depth": a.depth, "syms": a.syms, "seconds": round(dt, 1)}
    problems = []
    if tr.get("error"):
        problems.append("gdb error: " + str(tr["error"])[-400:])
    if not tr.get("exited"):
        problems.append("gdb did not see the compile exit")
    if tr.get("cl_rc") != 0:
        problems.append(f"cl rc {tr.get('cl_rc')}")
    if a.func and not any(f[2] for f in tr.get("functions", [])):
        problems.append(f"no PIPE event for a function containing {a.func!r}")
    if not tr.get("obj"):
        problems.append("no object produced")
    errs = [e for e in tr.get("events", []) if e[0] == "E"]
    if errs:
        problems.append(f"{len(errs)} hook errors, first: {errs[0][1][-300:]}")
    tr["meta"]["problems"] = problems
    json.dump(tr, open(a.out, "w"), separators=(",", ":"))
    n = {}
    for e in tr.get("events", []):
        n[e[0]] = n.get(e[0], 0) + 1
    print(f"lkrun: {a.out}: {dt:.0f} s, events {n}, dropped {tr.get('dropped')}, functions {len(tr.get('functions', []))}")
    if problems:
        print("lkrun: INCOMPLETE:", "; ".join(problems))
        sys.exit(2)


if __name__ == "__main__":
    main()
