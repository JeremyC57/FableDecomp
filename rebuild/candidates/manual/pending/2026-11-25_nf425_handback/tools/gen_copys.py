#!/usr/bin/env python3
"""gen_copys.py <hold batch> <out batch>: Option S thin rows for COPY_JMP rows whose new self is already defined strongly by the
operator= owner row's TU (the jmp target). Checks per row: both PDB object lists (egor/fablewin_obj_functions.tsv) put ?Copy@C and
??4C in one .obj; the thin row is `#include "../src/compiled/<owner>.cpp"` with its own Leaf; the HEAD test is kept (leaf bytes and
relocation targets unchanged: NF419 r1). Writes batches/<out>/rows, tests (HEAD copy), orig, selfmap, rows.txt, units (kind S)."""
import sys, os, csv, shutil, re, collections
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__))); import lib425 as L
hb, ob = sys.argv[1:3]; H = L.A + '/batches/' + hb; O = L.A + '/batches/' + ob
for d in ('rows', 'tests', 'orig/rows', 'orig/tests', 'evidence'): os.makedirs(O + '/' + d, exist_ok=True)
OBJ = collections.defaultdict(dict)
for pdb, f in (('Ego_r', 'egor_obj_functions.tsv'), ('FableWin', 'fablewin_obj_functions.tsv')):
    for l in open(L.R + '/ghidra_out/' + f, errors='replace'):
        x = l.rstrip('\n').split('\t')
        if len(x) >= 7 and x[6]: OBJ[pdb].setdefault(x[6], (x[0], x[1]))
g = {r[0]: r for r in csv.reader(open(L.A + '/work/gcj_all.tsv'), delimiter='\t')}
rows = []; sm = []; un = []; bad = []
for a in open(H + '/rows.txt').read().split():
    r = L.WL[a]; C = r['klass']; leaf = r['rtti']; T = g[a][5]; tn = g[a][6]
    osrc = L.nf.src_of(T); rel = os.path.relpath(str(osrc), L.R + '/rebuild/src/compiled')
    ev = []
    for pdb in ('Ego_r', 'FableWin'):
        c, o = OBJ[pdb].get(leaf), OBJ[pdb].get(tn)
        ev.append('%s %s' % (pdb, ('%s (%s idx %s, %s idx %s)' % (c[0].split('\\')[-1], 'Copy', c[1], 'operator=', o[1])) if c and o and c[0] == o[0] else 'NOT ONE OBJ %s / %s' % (c, o)))
    if any('NOT ONE' in e for e in ev): bad.append((a, ev)); continue
    hd = ['// %s::Copy(const CDefClassBase* other) -- copy every member of another %s: `*this = *static_cast<const %s*>(other)` (tail call of the operator=)' % (C, C, C),
          '// Leaf: %s' % leaf,
          '// Retail: Fable.exe (TLC, VC7.1) @ 0x%s, 5 bytes: a thiscall tail call, `jmp` to %s::operator= 0x%s (the next body).' % (a.upper(), C, T.upper())]
    hd += L.wrap('// Identity (NF425, 2026-11-25):', g[a][9] + '.')
    hd += L.wrap('// Option S unit (NF425):', 'this function\'s one body lives in its retail TU-mate\'s row 0x%s (%s), which already defines it (to emit the operator= it jumps to); both PDBs put the two in one object: %s. The row includes that source (no code of its own), as the EH funclet rows include their parent; that object\'s Copy grades against this address. Before the unit this row carried a second definition under the stand-in %s.' % (T.upper(), tn, '; '.join(ev), r['self_leaf']))
    src = '\n'.join(hd) + '\n#include "../src/compiled/%s"\n' % rel
    open(O + '/rows/%s.cpp' % a, 'w').write(src)
    for s, d in (('orig/rows/%s.cpp', 'orig/rows/%s.cpp'), ('orig/tests/%s_test.cpp', 'orig/tests/%s_test.cpp'), ('orig/tests/%s_test.cpp', 'tests/%s_test.cpp')):
        if os.path.exists(H + '/' + s % a): shutil.copy(H + '/' + s % a, O + '/' + d % a)
    rows.append(a); sm.append('%s\t%s\t%s' % (a, r['self_leaf'], leaf)); un.append('%s\t%s\tS' % (a, T))
open(O + '/rows.txt', 'w').write('\n'.join(rows) + '\n'); open(O + '/selfmap.tsv', 'w').write('\n'.join(sm) + '\n')
open(O + '/units_draft.tsv', 'w').write('\n'.join(un) + '\n')
open(O + '/expect.tsv', 'w').write(''.join('%s\t*\t%s\n' % (a, g[a][6]) for a in rows))
print(ob, len(rows), 'bad', bad)
