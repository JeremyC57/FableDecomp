#!/usr/bin/env python3
"""census439.py <rows file> <out.tsv>: compile each row's HEAD source (VC7.1, row flags), resolve the leaf's relocations against the retail
bytes, and write every (row, retail target, symbol) whose target is a B-list T1 target, with the PDB spelling and whether it agrees."""
import sys, os, csv, struct, shutil, collections
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__))); import nf
csv.field_size_limit(10**9)
S = str(nf.S)
B = {r['target']: r['pdb_spelling'] for r in csv.DictReader(open(S + '/t1split/T1_B.tsv'), delimiter='\t')}
rows = open(sys.argv[1]).read().split(); out = open(sys.argv[2], 'a')
for a in rows:
    try:
        st, leaf, fl, obj, wd = nf.compile_one(a, nf.src_of(a), 'C')
        if not obj.exists(): out.write('%s\t-\t-\tNOOBJ\t%s\n' % (a, st)); out.flush(); continue
        retail = bytes.fromhex(nf.catalog()[a]['bytes']); va0 = int(a, 16)
        secs, sl = nf.coff(obj)
        cand = [(i, n, v, sec) for i, n, v, sec, t, scl in sl if scl == 2 and sec > 0 and secs[sec - 1]['name'].startswith('.text') and leaf in n]
        if not cand:
            big = max((k for k, s in enumerate(secs) if s['name'].startswith('.text')), key=lambda k: len(secs[k]['data']))
            cand = [(0, '?', 0, big + 1)]
        i, n, v, sec = cand[0]; s = secs[sec - 1]; seen = set()
        for off, sym, typ in s['rel']:
            o = off - v
            if o < 0 or o + 4 > len(retail): continue
            x = struct.unpack_from('<I', retail, o)[0]
            tgt = '%08x' % (((va0 + o + 4 + x) & 0xffffffff) if typ == 0x14 else x)
            if tgt in B and (tgt, sym) not in seen:
                seen.add((tgt, sym)); out.write('%s\t%s\t%s\t%s\t%s\n' % (a, tgt, sym, 'AGREE' if sym == B[tgt] else 'WRONG', getattr(st, 'name', st)))
        out.flush()
        shutil.rmtree(wd, ignore_errors=True)
    except Exception as e:
        out.write('%s\t-\t-\tERR\t%s\n' % (a, str(e)[:100])); out.flush()
