#!/usr/bin/env python3
"""vtprobe.py <probe.cpp> <vtable-symbol> <expected.txt>: compile the probe with VC7.1 (worktree include path) and list the
relocation symbols of the named vtable in slot order; compare with expected (one mangled name per line, '-' = skip)."""
import sys, os, subprocess, re
R = os.environ["FABLE_ROOT"]; S = os.environ["S"]
sys.path.insert(0, R + "/tools/decomp_pipeline"); import verify_and_land as V
src, vsym, exp = sys.argv[1:4]
E = V.env(); obj = src[:-4] + ".obj"
if os.path.exists(obj): os.unlink(obj)
cp = V.cl(["/nologo", "/c", "/O2", "/W3", f"/Fo{obj}", src], E)
if cp.returncode: print(cp.stdout[-3000:]); sys.exit(1)
# find the section holding the vtable symbol (COMDAT .rdata), then its relocations
sy = subprocess.run(["objdump", "-t", obj], capture_output=True, text=True).stdout
sec = None
for l in sy.splitlines():
    if l.rstrip().endswith(" " + vsym):
        m = re.search(r"\(sec\s+(\d+)\)", l); sec = int(m.group(1)); off = int(re.search(r"0x([0-9a-f]+) ", l.split(")")[-1] if False else l).group(1), 16) if False else None
if sec is None: sys.exit("vtable symbol not found")
# section names in order
hd = subprocess.run(["objdump", "-h", obj], capture_output=True, text=True).stdout
names = re.findall(r"^\s*(\d+)\s+(\S+)", hd, re.M)
target = [n for i, n in names if int(i) == sec - 1]
rr = subprocess.run(["objdump", "-r", "-j", target[0], obj], capture_output=True, text=True).stdout if target else ""
# objdump -r -j picks the first section of that name; parse all reloc blocks instead and pick by matching count
blocks = re.split(r"RELOCATION RECORDS FOR \[", subprocess.run(["objdump", "-r", obj], capture_output=True, text=True).stdout)
idx = sum(1 for i, n in names if int(i) < sec - 1 and n == target[0])
cands = [b for b in blocks if b.startswith(target[0] + "]")]
blk = cands[idx] if idx < len(cands) else cands[-1]
rel = sorted((int(m.group(1), 16), m.group(2)) for m in re.finditer(r"^([0-9a-f]{8})\s+\S+\s+(\S+)$", blk, re.M))
got = [s for o, s in rel]
want = [l.strip() for l in open(exp) if l.strip()]
print("vtable slots emitted", len(got), "expected", len(want))
bad = 0
for i, (g, w) in enumerate(zip(got, want)):
    if w != "-" and g != w:
        bad += 1
        if bad <= 40: print(f"  slot {i}: got {g}  want {w}")
print("mismatches", bad + abs(len(got) - len(want)))
