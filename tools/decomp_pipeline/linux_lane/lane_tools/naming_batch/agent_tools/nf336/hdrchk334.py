# hdrchk334.py: NF299 Q4 second clause. For every census skipped-port entry (work/st_script.tsv), each reachable binder's
# `#include "engine/<X>.h"` headers are grepped for the PDB member name (the first name of the PDB spelling); hits -> work/hdrchk.tsv
import os, re, sys, collections
A = os.path.dirname(os.path.dirname(os.path.abspath(__file__))); W = A + '/work'; R = '/home/user/FableDecomp/rebuild/'
sys.path.insert(0, A + '/tools'); import nf
L = collections.defaultdict(list)
for l in open(W + '/tri_b.tsv'):
    q = l.rstrip('\n').split('\t'); L[q[0]].append(q)
out = open(W + '/hdrchk.tsv', 'w'); n = 0; hits = 0
for l in open(W + '/st_script.tsv'):
    q = l.rstrip('\n').split('\t')
    if q[4] != 'skipped-port': continue
    pdb = q[2]; m = re.match(r'\?\?_?[0-9A-Z]([^@]+)@', pdb) or re.match(r'\?([^@?]+)@', pdb)
    if pdb.startswith('??0') or pdb.startswith('??1'): mem = pdb[3:].split('@')[0]; mem = ('~' if pdb.startswith('??1') else '') + mem
    else: mem = m.group(1) if m else pdb
    cls = pdb.split('@')[1] if '@' in pdb else ''
    for b in sorted(set(x[2] for x in L[q[0]] if x[3] == '-')):
        s = nf.src_of(b)
        if not s: continue
        n += 1
        for h in re.findall(r'#include\s+"(engine/[^"]+\.h)"', open(s, encoding='utf-8', errors='replace').read()):
            hp = R + 'include/' + h
            if not os.path.exists(hp): continue
            t = open(hp, encoding='utf-8', errors='replace').read()
            pat = (r'~' + re.escape(mem[1:]) if mem.startswith('~') else r'\b' + re.escape(mem)) + r'\s*\('
            if re.search(pat, t):
                hits += 1; out.write('\t'.join([q[0], q[1], b, h, mem, cls]) + '\n')
print('binders', n, 'hits', hits)
