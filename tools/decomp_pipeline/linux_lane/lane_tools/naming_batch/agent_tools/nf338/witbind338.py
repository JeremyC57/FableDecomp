# witbind338.py (NF338): for every compiled witness (work/hobj/<w>.obj) and each ENTRIES target it witnesses (work/wit_map.tsv):
# what the HEAD object binds at the target (nf.py xr), against the PDB spelling and the table's spelling for that witness.
# -> work/witbind.tsv: idx target witness table_spelling bound PDB? TABLE?
import sys, os, io, contextlib
A = os.path.dirname(os.path.dirname(os.path.abspath(__file__))); R = '/home/user/FableDecomp'
sys.path.insert(0, A + '/tools'); import nf
E = nf.entries()
T = {}
for l in open(R + '/rebuild/oracles/reloc_bindings.tsv'):
    p = l.rstrip('\n').split('\t')
    if len(p) == 3: T.setdefault((p[1].lower().replace('0x', ''), p[2].lower().replace('0x', '')), []).append(p[0])
o = open(A + '/work/witbind.tsv', 'w')
for l in open(A + '/work/wit_map.tsv'):
    w, rest = l.rstrip('\n').split('\t')
    ob = A + '/work/hobj/' + w + '.obj'
    for it in rest.split():
        i, t = it.split(':'); pdb = E[int(i) - 1][6]
        if not os.path.exists(ob): o.write('%s\t%s\t%s\tNOOBJ\n' % (i, t, w)); continue
        out = io.StringIO()
        with contextlib.redirect_stdout(out):
            try: nf.cmd_xr(w, ob)
            except Exception as ex: print('XR_ERR', ex)
        b = sorted(set(x.split('\t')[1] for x in out.getvalue().splitlines() if x.startswith(t)))
        tab = T.get((t, w), [])
        o.write('%s\t%s\t%s\t%s\t%s\t%s\t%s\n' % (i, t, w, ' | '.join(tab), ' | '.join(b) or '-', 'PDB' if pdb in b else 'notPDB', 'TABLE' if any(x in b for x in tab) else 'notTABLE'))
