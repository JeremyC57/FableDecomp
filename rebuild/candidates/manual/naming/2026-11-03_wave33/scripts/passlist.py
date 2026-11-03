# passlist.py <fam> <log...>: rows graded MATCH/RELOCATION_MATCH + PASS without a RELOC_NAME / leaf / conflict warning (last log wins)
import sys, re
fam=sys.argv[1]; st={}; warn={}
for lg in sys.argv[2:]:
    cur={}
    for l in open(lg):
        m=re.match(r'^([0-9a-f]{8})\s+(\S+)\s+(\S+)?',l)
        if not m: continue
        a,g,b=m.groups()
        if g=='WARN':
            w=l.split('WARN',1)[1].strip()
            if not w.startswith('SHARED_HEADER_AVAILABLE'): cur.setdefault(a,[]).append(w)
            continue
        st[a]=(g,b); warn[a]=[]
    for a,w in cur.items(): warn[a]=w
for l in open(sys.argv[2]): pass
for a,(g,b) in sorted(st.items()):
    ok=re.match(r'^(RELOCATION_)?MATCH(\[[^\]]*\])?$',g) and b=='PASS' and not warn.get(a)
    print('%s\t%s\t%s\t%s'%('PASS' if ok else 'FAIL',fam,a,g+' '+str(b)+(' | '+'; '.join(warn[a]) if warn.get(a) else '')))
