# objctx366.py <class-substring> ...: per PDB (FableWin / Ego_r module streams), the objects that hold a proc whose name contains the
# substring, and every CCountedPointer<...> DeleteData / Reset / ShareData / ctor proc in those objects (name sets as evidence only). NF366.
import sys, collections
R = '/home/user/FableDecomp/ghidra_out/'
for pdb in ('fablewin', 'egor'):
    objs = collections.defaultdict(list)
    for l in open(R + pdb + '_obj_functions.tsv', encoding='utf-8', errors='replace'):
        if l.startswith('#'): continue
        f = l.rstrip('\n').split('\t')
        if len(f) < 7: continue
        objs[f[0]].append(f)
    for sub in sys.argv[1:]:
        for o, L in objs.items():
            if any(sub in f[5] for f in L):
                cp = [f for f in L if 'CCountedPointer<' in f[5] and any(k in f[5] for k in ('DeleteData', '::Reset', 'ShareData', '::CCountedPointer'))]
                print('%s %s [%s] procs=%d countedptr=%d' % (pdb, sub, o.split('\\')[-1], len(L), len(cp)))
                for f in cp: print('    %s %s' % (f[1], f[6] or f[5]))
