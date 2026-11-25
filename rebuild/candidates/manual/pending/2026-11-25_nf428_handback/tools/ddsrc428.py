#!/usr/bin/env python3
"""ddsrc428.py: independent check of source 2 per Reset row. The deleter's own retail body (catalog oracle bytes) is scanned for direct
calls (E8/E9); each target's tracker name is read. Class: CALLS_T_DTOR (a callee named ??1<T> / ??_G<T> / <T>:: of the row's T),
VIRTUAL (no direct call to a T-named function: deletes through the vtable, T-independent), OTHER. -> evidence/ddsrc.tsv"""
import sys, os, json, glob, re
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__))); import nf
A = str(nf.A); cat = nf.catalog(); r = json.load(open(A + '/status/rec.json'))
out = open(A + '/evidence/ddsrc.tsv', 'w'); import collections; C = collections.Counter()
for f in sorted(glob.glob(A + '/runs/r*/PLAN.tsv')):
    for l in open(f):
        p = l.rstrip('\n').split('\t'); a, d = p[0], p[5]
        T = p[4].split('?$CCountedPointer@')[1].split('@@@@')[0]; cls = re.sub(r'^\$\$CB', '', T)[1:]
        b = bytes.fromhex(cat[d]['bytes']); base = int(d, 16); calls = []
        for i in range(len(b) - 4):
            if b[i] in (0xE8, 0xE9):
                t = '%08x' % ((base + i + 5 + int.from_bytes(b[i + 1:i + 5], 'little', signed=True)) & 0xffffffff)
                if t in cat:
                    e = r.get(t, {}); calls.append((t, e.get('pdb_spelling') or e.get('self_leaf') or (e.get('names') or [''])[0] or cat[t]['name']))
        hit = [c for c in calls if ('@' + cls + '@@') in c[1] and (c[1].startswith('??1') or c[1].startswith('??_G'))]
        k = 'CALLS_T_DTOR' if hit else ('VIRTUAL' if not calls or all('delete' in c[1].lower() or c[0] in ('00bfe9bc',) for c in calls) else 'OTHER')
        C[k] += 1
        print(a, d, k, len(b), '; '.join('%s=%s' % c for c in calls)[:300], sep='\t', file=out)
print(C)
