#!/usr/bin/env python3
"""odall10.py <src.cpp> <substr>... [-- extra flags]: compile once with VC10 SP1 (cl 16.00.40219.01) using Ego_d's own compile line
and print the code bytes per source line of every PROC whose name contains one of the substrings.

Ego_d.pdb / FableWin.pdb are VC10 builds (EGOD_FINDINGS.md); their S_ENVBLOCK records the flags used here:
  -c -W3 -Od -Oy- -GF- -Gm- -EHs -EHc -MDd -GS -Gy -fp:precise -Zc:wchar_t -Zc:forScope -GR- -Gd -TP   (-Zi, -D_STLP_DEBUG omitted)
so a line size from this tool is comparable with Ego_d's line table byte for byte, where the VC7.1 /Od sizes only bound a text.
Evidence only: never use it to build or grade a row. Install the compiler with ../fetch_vc10sp1.sh; FABLE_VC10 overrides
$S/wine/vc10sp1. Run from the repo root with the lane env sourced (headers come from the lane INCLUDE plus FABLE_HDR_OVERLAY).
"""
import os, sys, re
from pathlib import Path
sys.path.insert(0, str(Path(__file__).resolve().parents[2]))
import verify_and_land as V

EGOD_FLAGS = ["/nologo", "/c", "/Od", "/Oy-", "/W3", "/GF-", "/Gm-", "/EHs", "/EHc", "/MDd", "/GS", "/Gy", "/fp:precise",
              "/Zc:wchar_t", "/Zc:forScope", "/GR-", "/Gd", "/TP"]

def main():
    a = sys.argv[1:]; extra = []
    if "--" in a: i = a.index("--"); extra = a[i + 1:]; a = a[:i]
    if not a: sys.exit(__doc__)
    vc10 = Path(os.environ.get("FABLE_VC10") or (os.environ["S"] + "/wine/vc10sp1"))
    V.CL_EXE = vc10 / "bin" / "cl.exe"; V.CC_BIN = V.CL_EXE.parent
    if not V.CL_EXE.exists(): sys.exit(f"no VC10 compiler at {V.CL_EXE}: run tools/decomp_pipeline/linux_lane/fetch_vc10sp1.sh")
    src = Path(a[0]).resolve(); subs = a[1:]
    asm = src.parent / (src.stem + "_od10.cod")
    cp = V.cl(EGOD_FLAGS + extra + ["/FAsc", f"/Fa{asm}", f"/Fo{src.parent / (src.stem + '_od10.obj')}", str(src)], V.env())
    if cp.returncode: print(cp.stdout[-3000:]); sys.exit(1)
    cur_fn = None; cur = None; sizes = {}; order = []
    for l in asm.read_text(errors="replace").splitlines():
        m = re.match(r"^(\S+)\s+PROC", l)
        if m:
            cur_fn = m.group(1) if any(s in m.group(1) for s in subs) else None
            sizes = {}; order = []; cur = None; continue
        if cur_fn and re.match(r"^\S+\s+ENDP", l):
            print(cur_fn); print("   " + " ".join("%d:%d" % (k, sizes[k]) for k in order) + "   total %d" % sum(sizes.values()))
            cur_fn = None; continue
        if not cur_fn: continue
        m = re.match(r"^; (\d+)\s*:", l)
        if m:
            cur = int(m.group(1))
            if cur not in sizes: order.append(cur); sizes[cur] = 0
            continue
        m = re.match(r"^\s+[0-9a-f]{5}\s+((?:[0-9a-f]{2}\s?)+)", l)
        if m and cur is not None: sizes[cur] += len(m.group(1).split())

if __name__ == "__main__": main()
