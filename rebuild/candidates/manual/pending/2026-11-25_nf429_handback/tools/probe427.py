#!/usr/bin/env python3
"""probe427.py <tag> <class list file> [defs]: emitted-vftable probe (NF329 vprobe2 form) of each class's VIRTUAL_MODEL opt-in in hdr/
(FABLE_HDR_OVERLAY=hdr). Scratch TU: the model + Needs defines, `new X` / `new X(x)`; VC7.1 /O2 /Oy; then vtcmp.py against the retail
vftable (work/model_<X>.tsv). -> work/probe_<tag>.tsv: class, verdict (OK = emitted count == retail count and every slot SAME), counts."""
import sys, os, re, subprocess
from concurrent.futures import ThreadPoolExecutor
A = os.path.dirname(os.path.dirname(os.path.abspath(__file__))); W = A + '/work'
sys.path.insert(0, '/home/user/FableDecomp/tools/decomp_pipeline')
os.environ['FABLE_HDR_OVERLAY'] = A + '/hdr'
import verify_and_land as V
tag = sys.argv[1]; d = W + '/probe/' + tag; os.makedirs(d, exist_ok=True)
NEED = 'FABLE_RETAIL_CAISTATEGROUPBASE,FABLE_RETAIL_CAISTATEGROUPBASE_MEMBERS'
defs0 = (sys.argv[3] if len(sys.argv) > 3 else NEED).split(',')
def pure_chain(X):
    out = []; c = X
    while c:
        p = A + '/hdr/engine/%s.h' % c
        if not os.path.exists(p): p = '/home/user/FableDecomp/rebuild/include/engine/%s.h' % c
        if not os.path.exists(p): break
        ht = open(p).read(); sw = 'FABLE_RETAIL_%s_VIRTUAL_MODEL' % c.upper()
        i = ht.find('#ifdef ' + sw)
        if i < 0: break
        b = ht[i:ht.find('#else', i)]; out.append(b)
        m = re.search(r'^class %s : public (\w+)' % c, b, re.M); c = m.group(1) if m else None
    return out
def one(X):
    vt = open(W + '/model_%s.tsv' % X).readline().split('\t')[1]
    ht = open(A + '/hdr/engine/%s.h' % X).read(); import re as _re
    nd = _re.search(r'// Needs \(defined by the including row, above its first #include\): ([^.]*)\.', ht)
    dd = [x.strip() for x in nd.group(1).split(',')] if nd else defs0
    src = ''.join('#define %s\n' % x for x in ['FABLE_RETAIL_%s_VIRTUAL_MODEL' % X.upper()] + dd)
    obj = d + '/' + X + '.obj'
    # NF429 form 3 (abstract class): a scratch derived class overrides every pure slot of the chain; at /Od its implicit constructor
    # calls X's out-of-line implicit constructor, which stores X's vptr, so ??_7X is emitted with __purecall in the pure slots
    pures = []
    for blkc in pure_chain(X):
        for l in blkc.splitlines():
            mm = _re.match(r'\s*virtual (.*?) (\w+)\((.*?)\)( const)? = 0;', l)
            if mm and mm.group(2) not in [q[1] for q in pures]: pures.append(mm.groups())
    if not [q for q in pures if q[1] == 'Update']: pures.append(('void', 'Update', '', None))
    def body(q):
        ps = ', '.join('%s a%d' % (x.strip(), i) for i, x in enumerate([y for y in q[2].split(',') if y.strip()]))
        return '    %s %s(%s)%s { %s}\n' % (q[0], q[1], ps, q[3] or '', '' if q[0] == 'void' else 'return 0; ')
    form3 = 'struct NF429ProbeD_%s : %s {\n%s};\nvoid* NF325Probe_%s() { return new NF429ProbeD_%s; }\n' % (X, X, ''.join(body(q) for q in pures), X, X)
    for form in ('void* NF325Probe_%s() { return new %s; }\n' % (X, X), 'void* NF325Probe_%s(const %s& x) { return new %s(x); }\n' % (X, X, X), form3):
        sp = d + '/' + X + '.cpp'; open(sp, 'w').write(src + '#include "engine/%s.h"\n' % X + form)
        cp = V.cl(['/nologo', '/c'] + (['/Od'] if form is form3 else ['/O2', '/Oy']) + ['/W3', '/Fo' + obj, sp], V.env())
        t = (cp.stdout or '') + (cp.stderr or '')
        errs = [l for l in t.splitlines() if ' error ' in l or 'fatal error' in l]
        if not (cp.returncode or errs): break
    if cp.returncode or errs or not os.path.exists(obj):
        return [X, 'COMPILE_FAIL', ' | '.join(e[-160:] for e in errs[:3])]
    r = subprocess.run(['python3', A + '/tools/vtcmp.py', obj, '??_7%s@@6B@' % X, vt], capture_output=True, text=True, cwd='/home/user/FableDecomp')
    m = re.search(r'retail slots (\d+), emitted slots (\d+); SAME (\d+) DIFF (\d+) NOLABEL (\d+)', r.stdout)
    if not m: return [X, 'VTCMP_FAIL', r.stderr[-200:].replace('\n', ' ')]
    rs, es, sa, di, nl = map(int, m.groups())
    bad = [l.split()[1] + ':' + (l.split('ours=')[1].split() or ['?'])[0][:50] for l in r.stdout.splitlines() if (' DIFF ' in l or ' NOLABEL ' in l) and l.startswith('slot')]
    v = 'OK' if rs == es == sa else 'BAD'
    open(d + '/' + X + '.vtcmp', 'w').write(r.stdout)
    return [X, v, 'retail %d emitted %d SAME %d DIFF %d NOLABEL %d %s' % (rs, es, sa, di, nl, ' '.join(bad[:5]))]
cls = [l.strip() for l in open(sys.argv[2]) if l.strip()]
with ThreadPoolExecutor(4) as ex: res = list(ex.map(one, cls))
with open(W + '/probe_%s.tsv' % tag, 'w') as f:
    for x in res: f.write('\t'.join(x) + '\n')
import collections; print(collections.Counter(x[1] for x in res))
