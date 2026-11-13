# fwsize.py <src.cpp> [name-regex]  -- FREE check, no retail comparison.
# Compiles the TU with VC7.1 at plain /Od (no /RTC) under the current FABLE_HDR_OVERLAY and compares the size of EVERY function in the
# object (the row and each inline helper it instantiates) with the FableWin debug build (ghidra_out/naming_stragglers/
# FableWin.exe.signatures.tsv: body bytes + instruction count per mangled name). Plain /Od reproduces FableWin byte for byte for helpers
# whose text is right (measured: PrepareForRender 158i/565b, the render-state setters 17i/49b, TouchResource 33i/92b,
# C4DVector(MMMM) 20i/51b, ...). A helper whose numbers differ by a few instructions has a different TEXT in the original (e.g.
# C2DVector operator+ : FableWin 26i/68b = `C2DVector result(...); return result;`, `return C2DVector(...)` is 22i/54b).
# FableWin was built with C++ exception handling: for ROWS (and helpers with destructible locals) compare with FW_EXTRA=/EHsc in the
# environment (GetFrame: 147i/464b v FableWin 146i/461b; plain /Od 136i/413b). Leaf helpers match at plain /Od.
# FableWin is a later debug build WITH asserts: gaps of +60 and more (CArray::operator[], operator->, SetRenderState ...) are assert
# code and say nothing. Use the size as a hint for which body to try; the retail grade is the judge.
import sys, os, re, subprocess
sys.path.insert(0, "/home/user/FableDecomp/tools/decomp_pipeline")
import verify_and_land as V
from pathlib import Path
from capstone import Cs, CS_ARCH_X86, CS_MODE_32
src = Path(sys.argv[1]).resolve(); flt = sys.argv[2] if len(sys.argv) > 2 else ""
work = Path(os.environ.get("FWSIZE_WORK", str(src.parent/"_fwsize"))); work.mkdir(parents=True, exist_ok=True)
sp = work/("p_" + src.name); sp.write_text(V.vc71(src.read_text()), encoding="utf-8")
obj = work/(src.stem + ".obj"); obj.unlink(missing_ok=True)
flags = [f for f in V.BASE_FLAGS if not f.startswith('/O')] + ['/Od'] + os.environ.get('FW_EXTRA','').split()
cp = V.cl(["/nologo","/c"]+flags+[f"/Fo{obj}", str(sp)], V.env())
if cp.returncode or not obj.exists(): print("SRC_FAIL", cp.stdout[-2000:]); sys.exit(1)
fw = {}
for l in open('/home/user/FableDecomp/ghidra_out/naming_stragglers/FableWin.exe.signatures.tsv'):
    p = l.rstrip('\n').split('\t')
    if len(p) < 5 or not p[3].isdigit() or int(p[3]) <= 5: continue
    fw.setdefault(p[1], (int(p[3]), int(p[4])))
out = subprocess.run([V.OBJDUMP, '-t', str(obj)], capture_output=True, text=True).stdout
md = Cs(CS_ARCH_X86, CS_MODE_32); seen = set(); rows = []
for l in out.splitlines():
    m = re.search(r'\(sec\s+(\d+)\).*\(scl\s+2\).*\(nx 0\) 0x0+ (\S+)', l)
    if not m or m.group(1) == '0': continue
    name = m.group(2)
    if name in seen or (flt and not re.search(flt, name)): continue
    seen.add(name)
    try: b, sec, sym = V.obj_text(str(obj), name)
    except Exception: continue
    if sym != name: continue
    n = sum(1 for _ in md.disasm(b, 0)); f = fw.get(name)
    if f is None: rows.append((9999, f"{n:5d}i {len(b):5d}b   FableWin: not found          {name[:120]}"))
    else:
        dn, db = f[1]-n, f[0]-len(b)
        tag = "SAME" if dn == 0 and abs(db) <= 1 else ("assert?" if dn >= 55 else "DIFF")
        rows.append((abs(dn) if tag != "SAME" else -1, f"{n:5d}i {len(b):5d}b   FableWin {f[1]:4d}i {f[0]:5d}b  d={dn:+4d}i {db:+5d}b {tag:7s} {name[:120]}"))
for _, r in sorted(rows, key=lambda x: x[0]): print(r)
