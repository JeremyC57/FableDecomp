# w7_targets.py OUT TARGET... : per retail target, every spelling the index binds (callers) or defines (definer), with PDB membership
# (Ego_r / FableWin name sets) and the catalog name; one line per (target, symbol). Plan phase 7 wave 1 survey.
import sys,os,collections
sys.path.insert(0,os.path.dirname(os.path.abspath(__file__))); import wavelib as L
OUT=sys.argv[1]; T=sys.argv[2:]
if len(T)==1 and T[0].startswith('@'): T=[l.split()[0] for l in open(T[0][1:]) if l.strip()]
T=[t.lower().replace('0x','') for t in T]
E,F=L.pdb(); binds,callers=L.index_binds(); cat=L.cat()
with open(OUT,'w') as o:
    o.write('target\tcatalog\tsymbol\tpdb\tn\tcallers\n')
    for t in T:
        d=L.cache().get(t,{}).get('sym')
        syms=dict(binds.get(t,{}))
        if d: syms.setdefault(d,0)
        for s,n in sorted(syms.items(),key=lambda kv:-kv[1]):
            pd=('E' if s in E else '')+('F' if s in F else '')
            cs=sorted(callers.get((t,s),()))
            o.write('\t'.join([t,cat.get(t,['','',''])[1] if t in cat else '',s,pd or '-',str(n)+('+def' if s==d else ''),' '.join(cs)])+'\n')
print('wrote',OUT)
