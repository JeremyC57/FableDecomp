#!/usr/bin/env python3
"""skel.py <src.cpp> <sym-substr>: micro-compile (mc.py) and print a block skeleton: calls (by reloc symbol), jumps, ret."""
import sys, subprocess, re, os
A = os.environ["S"] + "/w22/agent893"
src, pat = sys.argv[1], sys.argv[2]
r = subprocess.run(["timeout", "120", "python3", f"{A}/tools/mc.py", src, pat] + sys.argv[3:], capture_output=True, text=True)
obj = f"{A}/wk/mc/m.obj"
if not os.path.exists(obj): print(r.stdout[-2000:]); sys.exit(1)
d = subprocess.run(["objdump", "-dr", "-M", "intel", "--no-show-raw-insn", obj], capture_output=True, text=True).stdout
show = False; out = []; last = None
for l in d.splitlines():
    m = re.match(r"^[0-9a-f]+ <(.+)>:$", l)
    if m: show = pat in m.group(1); continue
    if not show: continue
    m = re.match(r"^\s+([0-9a-f]+):\s+(.*)$", l)
    if m:
        last = (int(m.group(1), 16), m.group(2).strip()); ins = last[1]
        if ins.startswith("j") or ins.startswith("ret"): out.append(f"{last[0]:x} " + re.sub(r"\s*<[^>]*>", "", re.sub(r"\s+", " ", ins)))
        continue
    m = re.match(r"^\s+[0-9a-f]+: (?:dir32|DISP32|REL32|IMAGE_REL_I386_\w+)\s+(\S+)", l)
    if m and last and (last[1].startswith("call") or last[1].startswith("push")):
        sym = m.group(1)
        sm = re.match(r"\?(\w+)@", sym); out.append(f"{last[0]:x} {last[1].split()[0][0]}:{sm.group(1) if sm else sym}")
print(" | ".join(out))
