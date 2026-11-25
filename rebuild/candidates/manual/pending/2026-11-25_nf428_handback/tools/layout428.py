#!/usr/bin/env python3
"""layout428.py: an independent second source for each Reset row's T. At every retail call site of the row (evidence/xref.json), the
`this` passed in ecx is read from the instructions just before the call (`lea ecx,[r+disp]`, or `lea r2,[r+disp]` ... `mov ecx,r2`);
the calling function's PDB name gives its class C; Ego_r.pdb's struct layout (ghidra_out/struct_layouts_egor.tsv) gives the member of C
at disp. LAYOUT_OK when that member's type is CCountedPointer<T> of the row's own T (written in C++), LAYOUT_OTHER when a different type,
NO_SITE when no call site has a readable `this` / class. -> evidence/layout.tsv"""
import sys, os, json, glob, re, subprocess, collections
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__))); from pe import PE
A = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
x = json.load(open(A + '/evidence/xref.json')); p = PE()
L = collections.defaultdict(dict)
for l in open('/home/user/FableDecomp/ghidra_out/struct_layouts_egor.tsv', errors='replace'):
    f = l.rstrip('\n').split('\t')
    if len(f) >= 5 and f[2].isdigit(): L[f[0]][int(f[2])] = (f[3], f[4])
def dis(s, n=24):
    b = p.read(s - n, n + 5); open(A + '/work/c.bin', 'wb').write(b)
    o = subprocess.run(['objdump', '-D', '-b', 'binary', '-mi386', '-Mintel', '--adjust-vma=0x%x' % (s - n), A + '/work/c.bin'], capture_output=True, text=True).stdout
    return [ln.split('\t')[-1].strip() for ln in o.splitlines() if re.match(r'^\s+[0-9a-f]+:\t', ln)]
def cls_of(name):
    m = re.match(r'^\?\?[01]([A-Za-z_]\w*)@@', name) or re.match(r'^\?\w+@([A-Za-z_]\w*)@@', name)
    return m.group(1) if m else None
out = open(A + '/evidence/layout.tsv', 'w'); C = collections.Counter()
for f in sorted(glob.glob(A + '/runs/r*/PLAN.tsv')):
    for ln in open(f):
        pp = ln.rstrip('\n').split('\t'); a, ct = pp[0], pp[2]
        want = 'CCountedPointer<%s>' % ct
        res = []
        for c in x.get(a, []):
            if c[1] != 'call' or not c[4]: continue
            k = cls_of(c[4])
            ins = dis(int(c[0], 16))[:-1]
            disp = None; reg = None
            for i in range(len(ins) - 1, -1, -1):
                m = re.match(r'lea\s+ecx,\[(\w+)\+0x([0-9a-f]+)\]', ins[i])
                if m: disp = int(m.group(2), 16); break
                m = re.match(r'mov\s+ecx,(e\w\w)$', ins[i])
                if m: reg = m.group(1); continue
                if reg:
                    m = re.match(r'lea\s+%s,\[(\w+)\+0x([0-9a-f]+)\]' % reg, ins[i])
                    if m: disp = int(m.group(2), 16); break
            if disp is None or not k: continue
            mem = L.get(k, {}).get(disp)
            res.append((c[2], k, disp, mem))
        oks = [r for r in res if r[3] and r[3][0].replace(' ', '') == want.replace(' ', '')]
        bad = [r for r in res if r[3] and r[3][0].replace(' ', '') != want.replace(' ', '')]
        v = 'LAYOUT_OK' if oks and not bad else ('LAYOUT_CONFLICT' if oks else ('LAYOUT_OTHER' if bad else 'NO_SITE'))
        C[v] += 1
        print(a, ct, v, '; '.join('%s %s+0x%x %s' % (r[0], r[1], r[2], r[3]) for r in res)[:400], sep='\t', file=out)
print(C)
