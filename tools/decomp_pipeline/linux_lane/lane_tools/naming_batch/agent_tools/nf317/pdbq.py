# pdbq.py <Method@Class[@NS]>...: every Ego_r / FableWin PDB symbol starting `?<Method@Class...>@`, with which name set(s) carry it
import sys
G = '/home/user/FableDecomp/ghidra_out/'
def names(p):
    s = {}
    for l in open(G + p, encoding='utf-8', errors='replace'):
        f = l.rstrip('\n').split('\t')
        if len(f) > 1 and f[1].startswith('?'): s.setdefault(f[1], f[0])
    return s
eg, fw = names('egor_pdb_names.tsv'), names('fablewin_pdb_names.tsv')
for q in sys.argv[1:]:
    pre = '?' + q + '@'
    hits = sorted(set(k for k in eg if k.startswith(pre)) | set(k for k in fw if k.startswith(pre)))
    print('##', q, len(hits))
    for h in hits: print('   %s%s %s' % ('E' if h in eg else '-', 'F' if h in fw else '-', h))
