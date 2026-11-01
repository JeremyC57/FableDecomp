# fam_leaf.py <ov> <queue rows tsv: target \t final> : the queue row's OWN landed source defines its function under a
# non-PDB spelling (catalog/manifest already carry the proven name). Handled shapes (one defined function of the row is the
# stand-in: same name in another class, or another name in FIN's class / a stand-in class):
#   class rename  -- a row-local stand-in class X for FIN's class (FIN's class not declared here): X -> CLS (struct -> class)
#   method rename -- N -> M on the declaration in the class and the out-of-line `CLS::N` definition (and `// Leaf:`)
#   const / access -- the declaration (and definition head) take FIN's const and access
# Return / parameter type changes are not attempted (deferred). check: FIN defined, the old spelling gone, every other
# changed symbol follows the class rename.
import sys, re, os, json
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import famlib2 as F, inputs, und, acclib
import fam_dtor_set as FS
from qlib import parse, typ, splitq
JOBS = {}
for l in open(sys.argv[2]):
    if l.strip(): t, fin = l.rstrip('\n').split('\t')[:2]; JOBS[t] = fin
IXD = {}
for l in open(inputs.W + '/idx/full.jsonl'):
    r = json.loads(l); IXD[r['a']] = set(r.get('d', []))
INFO = {}
REN2 = {}
_BLOB = None
def real(X):
    global _BLOB
    if _BLOB is None: _BLOB = '\n'.join(F.pdb())
    return any(k in _BLOB for k in ('@' + X + '@@', '??0' + X + '@@', '??1' + X + '@@', 'V' + X + '@@'))
def tr(t, a):
    FIN = JOBS[a]; NF = parse(FIN)
    if not NF or NF['kind'] != 'mem': return None, 'final not a member function'
    CLSQ = NF['scope']; CLS = CLSQ[-1]; M = NF['name']
    if len(CLSQ) > 1: return None, 'namespaced class %s' % '::'.join(CLSQ)
    if M.startswith('~') or M.startswith('operator') or '<' in M or '<' in CLS: return None, 'special member %s' % M
    if FIN in IXD.get(a, ()): return None, 'already defines the final'
    cands = []
    for s in IXD.get(a, ()):
        if not s.startswith('?') or s.startswith('??'): continue
        O = parse(s)
        if not O or O['kind'] != 'mem': continue
        if O['scope'] == CLSQ or O['name'] == M: cands.append((s, O))
    cands = [x for x in cands if x[0] != FIN]
    if not cands:
        # a stand-in class (no PDB class of that name) whose one defined non-special member is the row's function
        bycls = {}
        for s in IXD.get(a, ()):
            if not s.startswith('?') or s.startswith('??'): continue
            O = parse(s)
            if O and O['kind'] == 'mem' and len(O['scope']) == 1 and not real(O['scope'][0]): bycls.setdefault(O['scope'][0], []).append((s, O))
        one = [v[0] for v in bycls.values() if len(v) == 1]
        if len(bycls) == 1 and len(one) == 1: cands = one
    if len(cands) != 1: return None, '%d candidate own definitions %s' % (len(cands), [x[0][:50] for x in cands][:3])
    old, O = cands[0]
    if len(O['scope']) != 1: return None, 'old in a namespace %s' % O['scope']
    X = O['scope'][0]; N = O['name']
    INTS = ('long', 'unsigned long', 'int', 'unsigned int')
    PCH = {}
    if len(O['params']) != len(NF['params']): return None, 'parameter count differs'
    for i, (po, pn) in enumerate(zip(O['params'], NF['params'])):
        if typ(po) == typ(pn): continue
        if typ(po) in INTS and typ(pn) in INTS: PCH[i] = (typ(po), typ(pn)); continue
        return None, 'parameter types differ %s -> %s' % (O['params'], NF['params'])
    if typ(O['ret']) != typ(NF['ret']): return None, 'return type differs %s -> %s' % (O['ret'], NF['ret'])
    if O['static'] != NF['static'] or O['virtual'] != NF['virtual']: return None, 'static/virtual differ'
    msgs = []
    c = F.code_only(t)
    if X != CLS:
        if F.class_defs(c, CLS): return None, '%s and stand-in %s both declared' % (CLS, X)
        if not F.class_defs(c, X): return None, 'no definition of %s' % X
        t = F.rename_code(t, X, CLS)
        t = re.sub(r'\bstruct(\s+' + CLS + r'\b[^;{]*)\{', r'class\1{ public:', t)
        t = re.sub(r'\bstruct(\s+' + CLS + r'\s*;)', r'class\1', t)
        msgs.append('class %s->%s' % (X, CLS)); INFO[a] = X
    REN2[a] = (X, N, CLS, M, PCH)
    c = F.code_only(t)
    defs = F.class_defs(c, CLS)
    if len(defs) != 1: return None, '%d definitions of %s' % (len(defs), CLS)
    kw, s0, b, e = defs[0]
    dd = []
    for x in re.finditer(r'\b' + re.escape(N) + r'\s*\(', c[b + 1:e]):
        pos = b + 1 + x.start(); seg = c[b + 1:pos]
        if seg.count('{') - seg.count('}') == 0: dd.append(pos)
    if len(dd) != 1: return None, '%d declarations of %s in %s' % (len(dd), N, CLS)
    k = c.find('(', dd[0]); dep = 0
    while k < e:
        if c[k] == '(': dep += 1
        elif c[k] == ')':
            dep -= 1
            if dep == 0: break
        k += 1
    m2 = re.match(r'\s*(const\s*)?([;{])', c[k + 1:])
    if not m2: return None, 'unparsed declaration of %s' % N
    inline = m2.group(2) == '{'
    j = dd[0]
    while j > b + 1 and c[j - 1] not in ';{}:': j -= 1
    while c[j] in ' \t\r\n': j += 1
    head = fixparams(t[j:k + 1], PCH)
    if head is None: return None, 'parameter respell failed'
    newhead = re.sub(r'\b' + re.escape(N) + r'(\s*\()', M + r'\1', head)
    tail = (' const' if NF['const'] else '')
    if inline:
        if NF['acc'] != acclib.access_at(c, kw, b + 1, j): return None, 'inline definition with an access change'
        t = t[:j] + newhead + tail + t[k + 1 + len(m2.group(1) or ''):]
    else:
        de = k + 1 + m2.end()
        t = FS.set_stmt(t, c, kw, b, j, de, newhead + tail + ';', NF['acc'])
    if N != M: msgs.append('%s->%s' % (N, M))
    if NF['const'] != O['const']: msgs.append('const' if NF['const'] else 'non-const')
    if NF['acc'] != O['acc']: msgs.append(NF['acc'])
    c = F.code_only(t)
    od = list(re.finditer(r'\b' + re.escape(CLS) + r'\s*::\s*' + re.escape(N) + r'\s*\(', c))
    if len(od) > 1: return None, '%d definitions of %s::%s' % (len(od), CLS, N)
    if od:
        k = c.find('(', od[0].start()); dep = 0
        while k < len(c):
            if c[k] == '(': dep += 1
            elif c[k] == ')':
                dep -= 1
                if dep == 0: break
            k += 1
        m3 = re.match(r'(\s*const)?', c[k + 1:])
        ph = fixparams(t[od[0].start() + len(od[0].group(0)) - 1:k + 1], PCH)
        if ph is None: return None, 'definition parameter respell failed'
        t = t[:od[0].start()] + CLS + '::' + M + ph + tail + t[k + 1 + len(m3.group(0)):]
    def fixc(mm):
        s2 = re.sub(r'\b' + re.escape(X) + r'::' + re.escape(N) + r'\b', CLS + '::' + M, mm.group(0))
        s2 = re.sub(r'\b' + re.escape(CLS) + r'::' + re.escape(N) + r'\b', CLS + '::' + M, s2)
        if X != CLS: s2 = re.sub(r'\b' + re.escape(X) + r'\b', CLS, s2)
        return s2
    t = re.sub(r'//[^\n]*', fixc, t)
    t = re.sub(r'(?m)^// Leaf: .*$', lambda mm: '// Leaf: ' + FIN if ('@' in mm.group(0) or re.search(r'\b' + re.escape(N) + r'\b', mm.group(0))) else mm.group(0), t)
    return t, ' '.join(msgs)
def fixparams(h, PCH):
    if not PCH: return h
    i0 = h.find('('); inner = h[i0 + 1:-1]
    from memlib import split_args
    ps = split_args(inner)
    for i, (o, n) in PCH.items():
        if i >= len(ps): return None
        p2 = re.sub(r'^(\s*(?:const\s+)?)' + re.escape(o).replace('\\ ', '\\s+') + r'\b', lambda mm: mm.group(1) + n, ps[i])
        if p2 == ps[i]: return None
        ps[i] = p2
    return h[:i0 + 1] + ', '.join(ps) + ')'
def ck(D0, U0, D1, U1, a):
    FIN = JOBS[a]
    if FIN not in D1: return 'final not defined'
    X = INFO.get(a); C = parse(FIN)['scope'][-1]
    def rn(s):
        if not X: return s
        return s.replace('@' + X + '@@', '@' + C + '@@').replace('V' + X + '@@', 'V' + C + '@@').replace('U' + X + '@@', 'V' + C + '@@')
    hz = lambda s: '?A0x' in s
    R1 = D1 | U1; R0 = set(map(rn, D0 | U0))
    rem = [s for s in (D0 | U0) - R1 if not hz(s) and rn(s) not in R1]
    add = [s for s in R1 - (D0 | U0) if not hz(s) and s != FIN and s not in R0]
    if len(rem) > 1 or add: return 'diff -%s +%s' % (rem[:3], add[:3])
    return None
def post(t1, D1, U1, a):
    # the test links the source: a declaration of the old name (no body) follows the respell
    X, N, CLS, M, PCH = REN2[a]
    p = F.src_path(sys.argv[1], a, 'tst'); tt = F.rd(p); c = F.code_only(tt)
    if re.search(r'\b' + re.escape(N) + r'\s*\([^;{}]*\)\s*(?:const\s*)?;', c) and not re.search(r'\b' + re.escape(N) + r'\s*\([^;{}]*\)\s*(?:const\s*)?\{', c):
        t2 = F.rename_code(tt, N, M) if N != M else tt
        if X != CLS and re.search(r'\b(class|struct)\s+' + re.escape(X) + r'\b', c) and not re.search(r'\b(class|struct)\s+' + re.escape(CLS) + r'\b', c):
            t2 = F.rename_code(t2, X, CLS)
        if t2 != tt: F.wr(p, t2); print('test', a, 'renamed')
    return t1
if __name__ == '__main__':
    F.run(sys.argv[1], sorted(JOBS), tr, ck, 'leaf', post)
