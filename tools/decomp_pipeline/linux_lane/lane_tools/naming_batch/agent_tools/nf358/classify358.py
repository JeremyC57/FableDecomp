# classify358.py: NF358 census tables. For every census row (work/cand.tsv) and each copy it is a candidate for, read the HEAD object
# (work/obj/H_<a>.obj), pk_all (work/pkc/H_<a>.pkall.tsv) and pickany_vs_retail (work/pkc/H_<a>.pvr.txt) and write
# evidence/CENSUS_<copy>.tsv: row legs compiled grade lock copy_present(DEF-pickany / DEF-strong / UND / none) symbol pk_all pvr leafref reach note
# Copy found BY ADDRESS in pk_all (column 3 string-equal), so stand-in spellings are seen; the PDB symbol's DEF / UND state from the object.
# Also lists every other DEF in the object whose name carries the copy's member + class tokens (stand-in / other spelling).
import sys, os, re, collections
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__))); import nf
W = nf.W; E = nf.A / 'evidence'
SYM = {'00429c6f': '?Reset@?$CCountedPointer@VCComponent@NUISystem@@@@QAEXPAVCComponent@NUISystem@@@Z',
       '0042b312': '??$GetDef@VCUIMiscThingsDef@NUISystem@@@CDefinitionManager@@QBE_NJPAV?$CDefPointer@$$CBVCUIMiscThingsDef@NUISystem@@@@@Z',
       '0042b040': '??$GetDef@VCThingObjectDef@@@CDefinitionManager@@QBE_NJPAV?$CDefPointer@$$CBVCThingObjectDef@@@@@Z'}
TOK = {'00429c6f': ('Reset', 'CComponent'), '0042b312': ('GetDef', 'CUIMiscThingsDef'), '0042b040': ('GetDef', 'CThingObjectDef')}
grades = {}
for f in W.glob('pass_*.log'):
    for l in open(f):
        p = l.split()
        if len(p) >= 3 and p[0] == 'H': grades[p[1]] = p[2]
lockpre = {}
if (W / 'lock_pre.tsv').exists():
    for l in open(W / 'lock_pre.tsv'):
        p = l.rstrip('\n').split('\t')
        if len(p) == 4: lockpre[p[0]] = p
cand = [l.rstrip('\n').split('\t') for l in open(W / 'cand.tsv')]
out = {c: [] for c in SYM}
for a, copies, legs, own in cand:
    lg = dict(x.split(':') for x in legs.split(';'))
    obj = W / 'obj' / ('H_%s.obj' % a); pk = W / 'pkc' / ('H_%s.pkall.tsv' % a); pv = W / 'pkc' / ('H_%s.pvr.txt' % a)
    for c in copies.split(','):
        sym = SYM[c]; row = [a, lg.get(c, '-')]
        if own == 'OWNER':
            out[c].append(row + ['owner', '-', '-', 'self(strong owner)', sym[:60], '-', '-', '-', '-', '-', 'the retail owner row itself; not an emitter']); continue
        if not obj.exists():
            lp = lockpre.get(a, [a, '-', '-', '-'])
            out[c].append(row + ['no', grades.get(a, '-'), lp[2], '-', '-', '-', '-', '-', '-', '-', 'not compiled: test=%s lock=%s src=%s' % (lp[1], lp[2], lp[3])]); continue
        _, sl = nf.coff(obj); st = '-'; others = []
        for i, n, v, sec, typ, scl in sl:
            if scl != 2: continue
            if n == sym: st = 'DEF' if sec > 0 else ('UND' if st == '-' else st)
            elif sec > 0 and TOK[c][0] in n and TOK[c][1] in n: others.append(n)
        pkl = [l.rstrip('\n').split('\t') for l in open(pk) if not l.startswith('#')] if pk.exists() else []
        hit = [p for p in pkl if len(p) >= 10 and p[2] == c]
        pvr = '-'
        if pv.exists():
            for l in open(pv):
                if (' %s ' % sym) in l or l.rstrip().endswith(' ' + sym): pvr = l.split()[0]; break
        if hit:
            p = hit[0]
            pres = 'DEF-%s' % p[4]
            out[c].append(row + ['yes', grades.get(a, '-'), 'NF358', pres, p[9][:70], p[0], pvr, p[7], p[8], '%s/%s' % (p[5], p[6]), ('also: ' + ' '.join(others)) if others else ''])
        else:
            pres = {'DEF': 'DEF(NOADDR?)', 'UND': 'UND(call, no copy)', '-': 'none'}[st]
            out[c].append(row + ['yes', grades.get(a, '-'), 'NF358', pres, sym[:70] if st != '-' else '-', '-', pvr, '-', '-', '-', ('other spellings DEF: ' + ' '.join(others)) if others else ''])
hdr = 'row\tlegs\tcompiled\tgrade_H\tlock\tcopy_present\tsymbol\tpk_all\tpvr\tleafref\treach\tsize_ours/retail\tnote'
for c, rows in out.items():
    with open(E / ('CENSUS_%s.tsv' % c), 'w') as o:
        o.write('# copy 0x%s %s\n%s\n' % (c.upper(), SYM[c], hdr))
        for r in rows: o.write('\t'.join(r) + '\n')
    cnt = collections.Counter((r[5], r[7], r[9]) for r in rows)
    print(c, len(rows), dict(cnt))
