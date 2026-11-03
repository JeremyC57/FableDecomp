#!/usr/bin/env python3
"""Build and run the dual-path tests of the retail inline-asm helper headers (owner rule 2026-11-03: every retail asm
helper has a byte-matching `__forceinline` asm path and a FABLE_PORTABLE C++ twin; a dual-path test per helper is
mandatory).

For every rebuild/tests/portable/test_*.cpp:
  vc71-asm        VC7.1 /O2, no define            -- the matching build's path against the golden values
  vc71-dual       VC7.1 /O2 /DFABLE_DUAL_PATH_TEST -- asm path and fable_portable twin, compared input by input
  vc71-portable   VC7.1 /O2 /DFABLE_PORTABLE       -- the twin alone (the header maps the retail name to it)
  gxx-portable    host g++ -std=c++03 -DFABLE_PORTABLE -Wall -Wextra -pedantic, built and run (64-bit host: the twins
                  use explicit 32-bit arithmetic where retail wraps)
  gxx-m32-compile host g++ -m32 -std=c++03 -DFABLE_PORTABLE -fsyntax-only (the 32-bit model; no 32-bit runtime is needed)
and every header that declares `// Retail-asm-helper:` is compiled alone under -DFABLE_PORTABLE by both compilers.
Rows that keep their retail asm in an in-row two-path form (the asm block is the row's own function, not a shared
helper -- see rebuild/candidates/manual/batches/2026-11-03_asm_twins/README.md):
  vc71-row-portable   every rebuild/src/compiled source that spells FABLE_PORTABLE, compiled with /DFABLE_PORTABLE
                      (the matching path is graded by vland.py --recheck-landed)
  vc71-row-dual       every rebuild/tests/portable/rows/*_driver.cpp (it #includes its row) built as-is and with
                      /DFABLE_PORTABLE, run, and the two outputs compared line for line
The VC7.1 steps run through the Linux lane (Wine; source the lane env first) or natively on Windows -- the same
compiler / env setup as tools/decomp_pipeline/verify_and_land.py.

usage (repo root): python3 tools/decomp_pipeline/portable_twins_check.py [--out results.tsv] [--work DIR] [--no-vc71] [--no-gxx] [--no-rows]
Exit status 0 when every step that ran passed.
"""
from __future__ import annotations

import argparse
import os
import shutil
import subprocess
import sys
import tempfile
from pathlib import Path

ROOT = Path(__file__).resolve().parents[2]
INC = ROOT / "rebuild" / "include"
TESTS = ROOT / "rebuild" / "tests" / "portable"
sys.path.insert(0, str(ROOT / "tools" / "decomp_pipeline"))

VC_MODES = [("vc71-asm", []), ("vc71-dual", ["/DFABLE_DUAL_PATH_TEST"]), ("vc71-portable", ["/DFABLE_PORTABLE"])]
GXX_FLAGS = ["-std=c++03", "-DFABLE_PORTABLE", "-Wall", "-Wextra", "-pedantic", "-O2"]


def helper_headers():
    out = []
    for h in sorted(INC.rglob("*.h")):
        if "// Retail-asm-helper:" in h.read_text(encoding="utf-8", errors="ignore"):
            out.append(h)
    return out


def vc71_runner():
    import verify_and_land as V   # noqa: E402  (needs the lane env: FABLE_WINE / VC paths)
    e = V.env()

    def compile_run(src: Path, defines, work: Path, link=True, timeout=180, full_output=False):
        tag = "_p" if "/DFABLE_PORTABLE" in defines else ("_d" if defines else "")
        obj = work / (src.stem + tag + ".obj")
        exe = work / (src.stem + tag + ".exe")
        for p in (obj, exe):
            p.unlink(missing_ok=True)
        args = ["/nologo", "/c", "/O2", "/W3", "/EHsc", f"/I{TESTS}"] + list(defines) + [f"/Fo{obj}", str(src)]
        r = V.run_tool(V.CL_EXE, args, e) if timeout is None else _timed(V, V.CL_EXE, args, e, timeout)
        if r.returncode or not obj.exists():
            return "COMPILE_FAIL", (r.stdout + r.stderr).strip().splitlines()[-3:]
        if not link:
            return "OK", []
        r = _timed(V, V.VC / "bin" / "link.exe", ["/nologo", f"/out:{exe}", str(obj)], e, timeout)
        if r.returncode or not exe.exists():
            return "LINK_FAIL", (r.stdout + r.stderr).strip().splitlines()[-3:]
        cmd = ([V.WINE] if V.WINE else []) + [str(exe)]
        try:
            rn = subprocess.run(cmd, capture_output=True, text=True, errors="replace", env=e, timeout=timeout)
        except subprocess.TimeoutExpired:
            return "RUN_TIMEOUT", []
        lines = rn.stdout.strip().splitlines()
        if full_output:
            return ("RAN" if rn.returncode == 0 else "RUN_FAIL"), lines
        return ("PASS" if rn.returncode == 0 and lines and "PASS" in lines[-1] else "FAIL"), lines[-6:]
    return compile_run


def _timed(V, exe, args, e, timeout):
    cmd = ([V.WINE] if V.WINE else []) + [str(exe)] + V.tool_args(args)
    try:
        return subprocess.run(cmd, capture_output=True, text=True, errors="replace", env=e, timeout=timeout)
    except subprocess.TimeoutExpired:
        return subprocess.CompletedProcess(cmd, 124, "", "timeout")


def gxx(src: Path, work: Path, extra, run=True):
    cxx = shutil.which("g++")
    if not cxx:
        return "NO_GXX", []
    exe = work / (src.stem + ("_gxx" if run else "_gxx_m32"))
    cmd = [cxx] + GXX_FLAGS + extra + [f"-I{INC}", f"-I{TESTS}", str(src)]
    cmd += (["-o", str(exe)] if run else ["-fsyntax-only"])
    r = subprocess.run(cmd, capture_output=True, text=True, errors="replace")
    msgs = (r.stdout + r.stderr).strip().splitlines()
    if r.returncode:
        if "-m32" in extra and any(k in (r.stdout + r.stderr) for k in ("libc-header-start.h", "gnu/stubs-32.h")):
            return "SKIPPED", ["no 32-bit glibc headers on this host (the VC7.1 steps are the 32-bit build)"]
        return "COMPILE_FAIL", msgs[-4:]
    warn = [m for m in msgs if "warning" in m]
    if not run:
        return ("OK_WARN" if warn else "OK"), warn[:3]
    rn = subprocess.run([str(exe)], capture_output=True, text=True, errors="replace", timeout=60)
    lines = rn.stdout.strip().splitlines()
    st = "PASS" if rn.returncode == 0 and lines and "PASS" in lines[-1] else "FAIL"
    return st, (warn[:3] + lines[-6:])


def main() -> int:
    ap = argparse.ArgumentParser()
    ap.add_argument("--out")
    ap.add_argument("--work")
    ap.add_argument("--no-vc71", action="store_true")
    ap.add_argument("--no-gxx", action="store_true")
    ap.add_argument("--no-rows", action="store_true", help="skip the in-row two-path checks")
    a = ap.parse_args()
    work = Path(a.work) if a.work else Path(tempfile.mkdtemp(prefix="portable_twins_"))
    work.mkdir(parents=True, exist_ok=True)
    results = []

    def rec(subject, step, st, msgs):
        results.append((subject, step, st, " | ".join(m.strip() for m in msgs)))
        print(f"{subject:34} {step:16} {st}" + (f"   {msgs[-1].strip()}" if msgs else ""), flush=True)

    tests = sorted(TESTS.glob("test_*.cpp"))
    headers = helper_headers()
    # one TU per helper header: the header alone must compile on the portable path
    hdr_tus = []
    for h in headers:
        tu = work / f"hdr_{h.stem}.cpp"
        rel = h.relative_to(INC).as_posix()
        pre = ""
        for line in h.read_text(encoding="utf-8", errors="ignore").splitlines():
            if "// Portable-check-prelude:" in line:   # the includer-supplied types a vendor header expects
                # inlined, not #included: a /I path is not translated for Wine's cl, and a missing include hangs it
                pre += (TESTS / line.split(":", 1)[1].strip()).read_text(encoding="utf-8") + "\n"
        tu.write_text(f'{pre}#include "{rel}"\n#include "{rel}"\nint fable_hdr_{h.stem}_tu;\n', encoding="utf-8")
        hdr_tus.append((h, tu))
    if not a.no_vc71:
        try:
            compile_run = vc71_runner()
        except Exception as ex:   # no lane / no compiler
            compile_run = None
            rec("vc71", "setup", "SKIPPED", [str(ex)])
        if compile_run:
            for h, tu in hdr_tus:
                for step, defs in (("vc71-hdr-asm", []), ("vc71-hdr-portable", ["/DFABLE_PORTABLE"])):
                    st, m = compile_run(tu, defs, work, link=False)
                    rec(h.relative_to(INC).as_posix(), step, st, m)
            for t in tests:
                for step, defs in VC_MODES:
                    st, m = compile_run(t, defs, work)
                    rec(t.name, step, st, m)
            if not a.no_rows:
                for src in sorted((ROOT / "rebuild" / "src" / "compiled").rglob("*.cpp")):
                    if "FABLE_PORTABLE" in src.read_text(encoding="utf-8", errors="ignore"):
                        st, m = compile_run(src, ["/DFABLE_PORTABLE"], work, link=False)
                        rec(src.name[-12:-4], "vc71-row-portable", st, m)
                for drv in sorted((TESTS / "rows").glob("*_driver.cpp")):
                    outs = []
                    for defs in ([], ["/DFABLE_PORTABLE"]):
                        st, m = compile_run(drv, defs, work, full_output=True)
                        outs.append((st, m))
                    (s1, o1), (s2, o2) = outs
                    if s1 != "RAN" or s2 != "RAN":
                        rec(drv.name, "vc71-row-dual", s1 if s1 != "RAN" else s2, (o1 if s1 != "RAN" else o2)[-3:])
                    elif o1 == o2 and o1 and o1[-1].strip() == "DONE":
                        rec(drv.name, "vc71-row-dual", "PASS", [f"{len(o1) - 1} lines identical"])
                    else:
                        diff = [f"{x!r} vs {y!r}" for x, y in zip(o1, o2) if x != y][:3]
                        rec(drv.name, "vc71-row-dual", "FAIL", diff or ["output lengths differ"])
    if not a.no_gxx:
        for h, tu in hdr_tus:
            st, m = gxx(tu, work, [], run=False)
            rec(h.relative_to(INC).as_posix(), "gxx-hdr", st, m)
        for t in tests:
            st, m = gxx(t, work, [])
            rec(t.name, "gxx-portable", st, m)
            st, m = gxx(t, work, ["-m32"], run=False)
            rec(t.name, "gxx-m32-compile", st, m)
    if a.out:
        with open(a.out, "w", encoding="utf-8") as f:
            f.write("subject\tstep\tstatus\tdetail\n")
            for r in results:
                f.write("\t".join(r) + "\n")
    bad = [r for r in results if r[2] not in ("PASS", "OK", "OK_WARN", "SKIPPED")]
    print(f"{len(results)} steps, {len(bad)} failed")
    return 1 if bad else 0


if __name__ == "__main__":
    sys.exit(main())
