# mkpairs14.py: access/const-only auto pairs (target final old) for fam_auto.py from QUEUE_W14.tsv: every bound spelling of a
# queue target that some indexed row names and that differs from the queue's PDB spelling only in the access letter and/or
# the const qualifier (same name, class, signature). Also lists, per queue row, how many rows name a non-final spelling.
import sys, os, re, collections
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import inputs
MEM = re.compile(r'^\?(\w+)@(\w+)@@([AIQ])([AB])E(.*)$')
BT, BS = inputs.bindings(); E, IX = inputs.index(); NM = inputs.names(); PDB = NM['Ego_r.pdb'] | NM['FableWin.pdb']
out = open(sys.argv[1], 'w'); rep = open(sys.argv[2], 'w')
for q in inputs.queue():
    ch, a, fin = q[0], inputs.na(q[1]), q[2].strip()
    if not fin.startswith('?'): continue
    alts = [s for s in BT.get(a, ()) if s != fin and E.get(s)]
    n = sum(len(E[s]) for s in alts)
    rep.write('%s\t%s\t%s\t%d rows\t%d spellings\t%s\n' % (ch, a, fin, n, len(alts), ' | '.join(alts)[:600]))
    mf = MEM.match(fin)
    if not mf: continue
    for s in alts:
        mo = MEM.match(s)
        if mo and mo.group(1) == mf.group(1) and mo.group(2) == mf.group(2) and mo.group(5) == mf.group(5):
            out.write('%s\t%s\t%s\n' % (a, fin, s))
