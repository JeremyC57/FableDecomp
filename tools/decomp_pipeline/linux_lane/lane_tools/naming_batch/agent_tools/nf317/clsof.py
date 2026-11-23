# clsof.py <file> <method>: the local class / struct definitions (head line + virtual count + first members) that declare <method>,
# with the declaration line (NF317 w22 triage of `virtual` edits: base clause, leading vptr stand-in, existing virtuals)
import sys, re
src = open(sys.argv[1], encoding='utf-8', errors='replace').read(); meth = sys.argv[2]
for m in re.finditer(r'^[ \t]*(class|struct)\s+(\w+)\b([^;{()]*)\{', src, re.M):
    i = m.end(); d = 1
    while d and i < len(src):
        d += {'{': 1, '}': -1}.get(src[i], 0); i += 1
    body = src[m.end():i - 1]
    if not re.search(r'\b%s\s*\(' % re.escape(meth), body): continue
    code = [l for l in body.split('\n') if l.strip() and not l.strip().startswith('//')]
    nv = sum(1 for l in code if re.search(r'\bvirtual\b', l))
    print('CLASS %s %s%s  virtuals=%d lines=%d' % (m.group(1), m.group(2), m.group(3).rstrip(), nv, len(code)))
    for l in code[:4]: print('   | ' + l.rstrip()[:150])
    for l in code:
        if re.search(r'\b%s\s*\(' % re.escape(meth), l): print('   > ' + l.rstrip()[:150])
