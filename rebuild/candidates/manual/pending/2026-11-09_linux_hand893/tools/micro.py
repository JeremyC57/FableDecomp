#!/usr/bin/env python3
"""micro.py <bodies.txt> : each function in the file is `== NAME` followed by its body lines (a function body using
extern int a(),b(),...; int g; bool p(...)). Builds one TU, compiles it with the lane cl (BASE_FLAGS), prints a compact
skeleton per function: call targets by name, jumps (as relative labels), ret, xor/mov eax results."""
import sys, os, re, subprocess
from pathlib import Path
sys.path.insert(0, "/home/user/FableDecomp/tools/decomp_pipeline")
import verify_and_land as V
S = Path(os.environ["S"]); A = S/"w22"/"agent893"
txt = Path(sys.argv[1]).read_text()
extra = sys.argv[2:]
pre, funcs = [], []
cur = None
for l in txt.splitlines():
    m = re.match(r"^== (\w+)\s*$", l)
    if m: cur = [m.group(1), []]; funcs.append(cur); continue
    (cur[1] if cur else pre).append(l)
src = "\n".join(pre) + "\n"
for n, b in funcs: src += "\n".join(b) + "\n"
work = A/"wk"/"micro"; work.mkdir(parents=True, exist_ok=True)
sp = work/"m.cpp"; sp.write_text(V.vc71(src)); obj = work/"m.obj"; obj.unlink(missing_ok=True)
cp = V.cl(["/nologo", "/c"] + V.BASE_FLAGS + extra + [f"/Fo{obj}", str(sp)], V.env())
if not obj.exists(): print(cp.stdout[-3000:]); sys.exit(1)
d = subprocess.run(["objdump", "-dr", "-M", "intel", "--no-show-raw-insn", str(obj)], capture_output=True, text=True).stdout
cur = None; out = {}; last = None
for l in d.splitlines():
    m = re.match(r"^([0-9a-f]+) <(.+)>:$", l)
    if m:
        nm = m.group(2); cur = None
        for n, _ in funcs:
            if re.search(r"\b" + n + r"\b|\?" + n + "@", nm): cur = n
        if cur: out[cur] = []
        continue
    if not cur: continue
    m = re.match(r"^\s+[0-9a-f]+: (?:dir32|DISP32|REL32|IMAGE_REL_I386_\w+)\s+(\S+)", l)
    if m:
        if out[cur]:
            sym = m.group(1); sm = re.match(r"_?\??(\w+)", sym); out[cur][-1][1] += " =" + (sm.group(1) if sm else sym)
        continue
    m = re.match(r"^\s+([0-9a-f]+):\s+(.*)$", l)
    if m:
        a = int(m.group(1), 16); ins = re.sub(r"\s+", " ", m.group(2).strip()); ins = re.sub(r" <[^>]*>", "", ins)
        out[cur].append([a, ins]); continue
for n, _ in funcs:
    ins = out.get(n, [])
    tg = {}
    for a, s in ins:
        m = re.match(r"j\w+ ([0-9a-f]+)$", s)
        if m: tg.setdefault(int(m.group(1), 16), f"L{len(tg)}")
    parts = []
    for a, s in ins:
        if a in tg: parts.append(tg[a] + ":")
        if s.startswith("jmp") and "=" in s: parts.append("tail=" + s.split("=")[1]); continue
        m = re.match(r"(j\w+) ([0-9a-f]+)", s)
        if m: parts.append(f"{m.group(1)} {tg[int(m.group(2),16)]}"); continue
        if s.startswith("call"): parts.append("call" + (s.split("=")[1] if "=" in s else "?")); continue
        if s.startswith("ret") or "eax,eax" in s.replace(" ", "") and s.startswith("xor") or s.startswith("xor al") or re.match(r"mov (eax|al),0x", s) or s.startswith("sete") or s.startswith("setne"): parts.append(s.replace(" ", "")); continue
    print(f"{n:6} " + " ".join(parts))
