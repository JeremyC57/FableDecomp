# check364.py <addr...>: per-row NF364 gate summary from the gate objects (work/obj/H_<a>.obj v work/obj/<a>.obj) and pk files (evidence/pk_gate):
#  objsec (evidence/objsec/<a>.txt): exactly ONE CHANGED line = the Reset<CComponent> COMDAT, 62 -> 59, no ADDED / DROPPED / SYM lines, leaf=same;
#  owner compare (cmpcomdat semantics): the N copy bytes + relocations == the owner object's (work/obj/H_00429c6f.obj);
#  grades H / N from work/pass_*.log; pk_all verdict + reach of Reset 0x00429C6F / ReleaseData 0x004291DE / DeleteData 0x00428B10 H -> N.
import sys, os, re, subprocess, glob
A = '/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad/w22/agentNF364'
sys.path.insert(0, A + '/tools'); import nf
RS = '?Reset@?$CCountedPointer@VCComponent@NUISystem@@@@QAEXPAVCComponent@NUISystem@@@Z'
def sec_of(obj, sym):
    secs, sl = nf.coff(obj)
    for i, n, v, sec, t, scl in sl:
        if n == sym and sec > 0 and scl == 2: s = secs[sec - 1]; return s['data'], s['rel']
grades = {}
for f in glob.glob(A + '/work/pass_*.log'):
    for l in open(f):
        p = l.split()
        if len(p) > 2 and p[0] in ('H', 'N'): grades[(p[0], p[1])] = p[2]
def pk(path):
    d = {}
    if not os.path.exists(path): return d
    for l in open(path):
        if l.startswith('#') or not l.strip(): continue
        p = l.rstrip('\n').split('\t')
        if len(p) >= 10: d[p[2]] = (p[0], p[1], p[7], p[8], p[9])   # verdict(a), verdict(b?), leafref, reach, symbol
    return d
own = sec_of(A + '/work/obj/H_00429c6f.obj', RS); bad = 0
os.makedirs(A + '/evidence/objsec', exist_ok=True)
for a in sys.argv[1:]:
    lf = re.search(r'^// Leaf:\s*(\S+)', open(A + '/rows/%s.cpp' % a).read(), re.M).group(1)
    o = subprocess.run(['python3', A + '/tools/objsec352.py', A + '/work/obj/H_%s.obj' % a, A + '/work/obj/%s.obj' % a, lf], capture_output=True, text=True).stdout
    open(A + '/evidence/objsec/%s.txt' % a, 'w').write(o)
    L = [l for l in o.splitlines() if l.strip()]
    ch = [l for l in L if not l.startswith('VERDICT')]
    one = len(ch) == 1 and ch[0].startswith("CHANGED ('COMDAT', '%s', '.text') size 62 -> 59" % RS)
    leafsame = L[-1].endswith('leaf=same')
    x = sec_of(A + '/work/obj/%s.obj' % a, RS); oeq = x is not None and (x[0], x[1]) == (own[0], own[1])
    gh, gn = grades.get(('H', a)), grades.get(('N', a))
    H = pk(A + '/evidence/pk_gate/H_%s.pkall.tsv' % a); N = pk(A + '/evidence/pk_gate/N_%s.pkall.tsv' % a)
    cp = ' '.join('%s:%s/%s->%s/%s' % (k, (H.get(k) or ('-',) * 4)[0], (H.get(k) or ('-',) * 4)[3], (N.get(k) or ('-',) * 4)[0], (N.get(k) or ('-',) * 4)[3]) for k in ('00429c6f', '004291de', '00428b10'))
    ok = one and leafsame and oeq and gh == gn and gh in ('MATCH', 'RELOCATION_MATCH')
    bad += not ok
    print('%s %s objsec_one_comdat=%s leaf=%s owner_eq=%s grade %s -> %s | %s' % (a, 'OK' if ok else 'FAIL', one, 'same' if leafsame else 'DIFF', oeq, gh, gn, cp))
print('CHECK364', 'ALL_OK' if not bad else 'FAIL=%d' % bad)
