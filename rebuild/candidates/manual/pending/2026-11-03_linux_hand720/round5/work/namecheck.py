import sys,subprocess
eg=set(l.rstrip('\n').split('\t')[1] for l in open('ghidra_out/egor_pdb_names.tsv') if '\t' in l)
fw=set(l.rstrip('\n').split('\t')[1] for l in open('ghidra_out/fablewin_pdb_names.tsv') if '\t' in l)
bind={}
for l in open('rebuild/oracles/reloc_bindings.tsv'):
    p=l.rstrip('\n').split('\t')
    if len(p)>=3: bind.setdefault(p[1],set()).add(p[0])
symaddr={}
for l in open('rebuild/oracles/reloc_bindings.tsv'):
    p=l.rstrip('\n').split('\t')
    if len(p)>=3: symaddr.setdefault(p[0],set()).add(p[1])
for l in sys.stdin:
    a,s=l.rstrip('\n').split('\t')
    b=bind.get(a,set())
    other=symaddr.get(s,set())-{a}
    print('%s\tE%d F%d\tbound=%s\t%s%s'%(a,s in eg,s in fw,'Y' if s in b else ('N(%d others)'%len(b)),s,('\tALSO-BOUND-TO '+','.join(other)) if other else ''))
