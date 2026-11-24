# whose.py <addr..>: which NF314 / NF315 ENTRIES lines name the address (as target or inside a spellings witness), plus their rows.txt / locks
import sys, re, os
A = os.path.dirname(os.path.dirname(os.path.abspath(__file__))); S = os.path.dirname(os.path.dirname(A))
for a in sys.argv[1:]:
    a = a.lower().replace('0x', ''); hits = []
    for ag in ('agentNF324', 'agentNF326'):
        L = open(S + '/w22/%s/ENTRIES.tsv' % ag).read().split('\n')[1:]
        for i, l in enumerate(L, 1):
            p = l.split('\t')
            if not p[0]: continue
            if p[0].lower() == a: hits.append('%s#%d:target' % (ag[5:], i))
            elif len(p) > 11 and a in p[11].lower(): hits.append('%s#%d:witness(of %s)' % (ag[5:], i, p[0]))
        rt = S + '/w22/%s/rows.txt' % ag
        if os.path.exists(rt) and a in open(rt).read().lower(): hits.append('%s:rows.txt' % ag[5:])
    o = S + '/w22/nflocks/%s/owner' % a
    lk = open(o).read().strip() if os.path.exists(o) else 'none'
    print(a, 'lock=' + lk, ' '.join(hits) or '-')
