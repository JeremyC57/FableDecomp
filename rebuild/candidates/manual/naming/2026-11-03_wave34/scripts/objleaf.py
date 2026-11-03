# objleaf.py <rk dir> <rows file>: per row, the current // Leaf: and the obj's defined function symbols that complete it.
import sys, re, glob, subprocess
F, R = sys.argv[1], sys.argv[2]
for p in [l.strip() for l in open(R) if l.strip()]:
    a = re.search(r"_([0-9a-f]{8})\.cpp$", p).group(1)
    t = open(p).read(); m = re.search(r"^// Leaf: *(.*)$", t, re.M); leaf = m.group(1).strip() if m else ""
    objs = glob.glob(f"{F}/rk_part_0*/work/**/{a}.obj", recursive=True)
    if not objs: print(f"{a}\t{leaf}\tNOOBJ"); continue
    out = subprocess.run(["objdump", "-t", objs[0]], capture_output=True, text=True).stdout
    syms = []
    for l in out.splitlines():
        mm = re.search(r"\(sec\s+(\d+)\)\(fl 0x00\)\(ty\s+20\)\(scl\s+2\) \(nx \d+\) 0x[0-9a-f]+ (\S+)", l)
        if mm and int(mm.group(1)) > 0: syms.append(mm.group(2))
    exact = leaf in syms
    pref = [s for s in syms if leaf and s.startswith(leaf)]
    print(f"{a}\t{leaf}\t{'EXACT' if exact else ('PREFIX:' + ' | '.join(pref) if pref else 'NONE:' + ' | '.join(syms))}")
