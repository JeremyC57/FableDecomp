#!/usr/bin/env python3
"""census428.py <srclist.json> <out.tsv>: compile each landed source (HEAD, its catalog row's flags, VC7.1) and list every external
CCountedPointer / CCPPointerInfo symbol it DEFines or references (UND) -> <out.tsv>: source address kind symbol. Objects kept in work/cen/."""
import sys, os, re, json, shutil
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__))); import nf
L = json.load(open(sys.argv[1])); out = open(sys.argv[2], 'w')
rev = {}
for m in re.finditer(r"\[pscustomobject\]@\{(.*?)\n\s*\}", nf.PS, re.S):
    a = re.search(r"\bAddress = '([0-9A-Fa-f]{8})'", m.group(1)); s = re.search(r"\bSource = '([^']*)'", m.group(1))
    if a and s: rev.setdefault(s.group(1).replace('\\', '/'), a.group(1).lower())
os.makedirs(nf.W / 'cen', exist_ok=True)
for src in sorted(L):
    a = rev.get(src)
    if not a: print(src, 'NOADDR', file=out, sep='\t', flush=True); continue
    o = nf.W / 'cen' / (a + '.obj')
    if not o.exists():
        try:
            st, leaf, fl, obj, wd = nf.compile_one(a, nf.R / 'rebuild/src/compiled' / src, 'cen')
            if obj.exists(): shutil.copy(obj, o)
        except Exception as e:
            print(src, a, 'ERR', str(e)[:200], file=out, sep='\t', flush=True); continue
    if not o.exists(): print(src, a, 'NOOBJ', '', file=out, sep='\t', flush=True); continue
    for s in sorted(nf.ext_syms(str(o))):
        k, n = s.split(' ', 1)
        if 'CCountedPointer' in n or 'CCPPointerInfo' in n: print(src, a, k, n, file=out, sep='\t', flush=True)
print('CENSUS_DONE', file=out, flush=True)
