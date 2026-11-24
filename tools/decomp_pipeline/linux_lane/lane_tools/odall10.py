#!/usr/bin/env python3
"""odall10.py [--lib NAME] [--stl] <src.cpp> <substr>... [-- extra flags]: compile once with VC10 SP1 (cl 16.00.40219.01) using the compile line of
the Ego_d library the function came from, with VC10's own headers, and print the code bytes per source line of every PROC whose name
contains one of the substrings.

Ego_d.pdb / FableWin.pdb are VC10 SP1 builds (EGOD_FINDINGS.md). Their S_ENVBLOCK records differ by library (count of compilands):
  fablelib (871, FableLib / wishworld)  -W4 -Od -Oy- -DGAME_COMP_DEBUG -DUSE_VIRTUAL_KEYBOARD ... -GS- -GR -Gr   (/Yu standard_headers.hpp)
  bbb      (231, BBBLibrary)            -W4 -Od -Oy- -DLIB_COMP_DEBUG -GF- ... -GS- -GR -Gr                    (/Yu lib_standard_headers.hpp)
  engine   (100, FableEngine)           -W4 -Od -Oy- -DGAME_COMP_DEBUG -DCOMP_ENGINE_LIB ... -GS- -GR -Gr
  particle (20, ParticleSystem)         -W3 -Od -Oy- -DGAME_COMP_DEBUG ... -GS- -GR -Gr
  ego      (few, the game project)      -W3 -Od -Oy- -DGAME_COMP_DEBUG -D_STLP_DEBUG -GF- ... -GS -GR- -Gd
Pick the library from the function's Ego_d compiland (default fablelib). -Gr makes __fastcall the default for free functions, and -GR / -GS
change frames, so the wrong preset gives wrong sizes. -Zi and the PCH switches are omitted (they do not change code bytes).

Headers: by default the lane INCLUDE of verify_and_land.env() (VC7.1 CRT/STL + FABLE_HDR_OVERLAY + rebuild/include), so a text written on
the repo's engine headers compiles; its non-STL statements are then comparable. --stl puts VC10's own include tree first instead
($S/wine/vc10sp1/include, plus stubinc/ammintrin.h, which the package lacks); the repo's stlport/fable_containers.h clashes with it, so an
--stl text must be standalone (VC10 <vector> etc. plus local declarations). Validated 2026-11-21 on std::vector<unsigned long> (Ego_d
BBBLibrary\\Debug\\lib_bank_file.obj, bbb preset): all 13 of its members match byte for byte (ctors 106 / 94, both operator= 33, _Assign_rv
334, swap 222, insert<_Vector_iterator> 205, _Insert 911, back 140, front 101, empty 28, pop_back 112, get_allocator 31, ??_G 44).
A text that uses the repo's STLport / VC7.1 container models (stlport/fable_containers.h) is NOT comparable: Ego_d's library code uses
VC10's Dinkumware STL with checked iterators (/MDd), so measure STL statements with <vector> etc. from VC10.

Evidence only: never use it to build or grade a row (rows are VC7.1 3077). Install with ../fetch_vc10sp1.sh; FABLE_VC10 overrides
$S/wine/vc10sp1. Run from the repo root with the lane env sourced.
"""
import os, sys, re
from pathlib import Path
sys.path.insert(0, str(Path(__file__).resolve().parents[2]))
import verify_and_land as V

COMMON = ["/nologo", "/c", "/Od", "/Oy-", "/DLIB_NOT_DLL", "/DWIN32", "/DNDEBUG", "/D_WINDOWS"]
TAIL = ["/D_MBCS", "/Gm-", "/EHs", "/EHc", "/MDd", "/Gy", "/fp:precise", "/Zc:wchar_t", "/Zc:forScope", "/TP"]
CRT = ["/D_CRT_SECURE_NO_WARNINGS", "/D_CRT_NON_CONFORMING_SWPRINTFS"]
PRESETS = {
    "fablelib": ["/W4"] + COMMON + ["/DGAME_COMP_DEBUG", "/DUSE_VIRTUAL_KEYBOARD"] + CRT + TAIL + ["/GS-", "/GR", "/Gr"],
    "bbb":      ["/W4"] + COMMON + ["/DLIB_COMP_DEBUG"] + CRT + ["/GF-"] + TAIL + ["/GS-", "/GR", "/Gr"],
    "engine":   ["/W4", "/nologo", "/c", "/Od", "/Oy-", "/DLIB_NOT_DLL", "/DGAME_COMP_DEBUG", "/DWIN32", "/DNDEBUG", "/D_WINDOWS",
                 "/DCOMP_ENGINE_LIB"] + CRT + TAIL + ["/GS-", "/GR", "/Gr"],
    "particle": ["/W3"] + COMMON + ["/DGAME_COMP_DEBUG"] + TAIL + ["/GS-", "/GR", "/Gr"],
    "ego":      ["/W3"] + COMMON + ["/DGAME_COMP_DEBUG", "/D_STLP_DEBUG", "/GF-"] + TAIL + ["/GS", "/GR-", "/Gd"],
}

def env(vc10):
    e = V.env()
    ov = [Path(x) for x in os.environ.get("FABLE_HDR_OVERLAY", "").split(os.pathsep) if x]
    inc = [vc10 / "include", vc10 / "stubinc"] + ov + [V.ROOT / "rebuild" / "include"]
    e["INCLUDE"] = ";".join((V.wpath(x) if V.WINE else str(x)) for x in inc)
    return e

def main():
    a = sys.argv[1:]; extra = []; lib = "fablelib"; stl = False
    if "--" in a: i = a.index("--"); extra = a[i + 1:]; a = a[:i]
    while a[:1] in (["--lib"], ["--stl"]):
        if a[0] == "--stl": stl = True; a = a[1:]
        else: lib = a[1]; a = a[2:]
    if not a or lib not in PRESETS: sys.exit(__doc__)
    vc10 = Path(os.environ.get("FABLE_VC10") or (os.environ["S"] + "/wine/vc10sp1"))
    V.CL_EXE = vc10 / "bin" / "cl.exe"; V.CC_BIN = V.CL_EXE.parent
    if not V.CL_EXE.exists(): sys.exit(f"no VC10 compiler at {V.CL_EXE}: run tools/decomp_pipeline/linux_lane/fetch_vc10sp1.sh")
    if stl and not (vc10 / "include" / "vector").exists(): sys.exit(f"no VC10 headers at {vc10 / 'include'}: rerun fetch_vc10sp1.sh")
    src = Path(a[0]).resolve(); subs = a[1:]
    asm = src.parent / (src.stem + "_od10.cod")
    cp = V.cl(PRESETS[lib] + extra + ["/FAsc", f"/Fa{asm}", f"/Fo{src.parent / (src.stem + '_od10.obj')}", str(src)], env(vc10) if stl else V.env())
    if cp.returncode: print(cp.stdout[-3000:]); sys.exit(1)
    print(f"# preset {lib}, headers {'VC10' if stl else 'lane (VC7.1)'}")
    cur_fn = None; cur = None; sizes = {}; order = []; skip = False
    for l in asm.read_text(errors="replace").splitlines():
        m = re.match(r"^(\S+)\s+PROC", l)
        if m:
            cur_fn = m.group(1) if any(s in m.group(1) for s in subs) else None
            sizes = {}; order = []; cur = None; skip = False; continue
        if cur_fn and re.match(r"^\S+\s+ENDP", l):
            print(cur_fn); print("   " + " ".join("%d:%d" % (k, sizes[k]) for k in order) + "   total %d" % sum(sizes.values()))
            cur_fn = None; continue
        if not cur_fn: continue
        # EH unwind funclets (text$x) sit between PROC and ENDP but are not the function's lines in a PDB line table
        if re.match(r"^text\$x\s+SEGMENT", l): skip = True; continue
        if re.match(r"^_TEXT\s+SEGMENT", l): skip = False; continue
        if skip: continue
        m = re.match(r"^; (\d+)\s*:", l)
        if m:
            cur = int(m.group(1))
            if cur not in sizes: order.append(cur); sizes[cur] = 0
            continue
        # an instruction longer than 5 bytes wraps: "  0000a\t64 a1 00 00 00" then "\t00\t\t mov ..." (count both)
        m = re.match(r"^\s+[0-9a-f]{5}\t((?:[0-9a-f]{2} ?)+)", l) or re.match(r"^\t((?:[0-9a-f]{2} ?)+)\t", l)
        if m and cur is not None: sizes[cur] += len(m.group(1).split())

if __name__ == "__main__": main()
