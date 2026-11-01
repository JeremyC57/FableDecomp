# fam_vcs.py <ov> <rows.txt>: wave 14 family vcs -- the vector<CCharString> destructor 0x00414EA0 is the STLport instantiation
# ??1?$vector@VCCharString@@V?$allocator@VCCharString@@@std@@@std@@QAE@XZ (FableWin; DECISIONS_W13_RESOLVED). Two row shapes:
#  (a) a row-local 12-byte struct (CCharStringVector / SCharStringVector) with the destructor: the struct becomes the row-local
#      STLport model `template <> class vector<CCharString, allocator<CCharString> >` in namespace std (the struct's own members
#      kept as they were, so no inline member is added and no byte moves) and every use names `std::vector<CCharString>`;
#  (b) a one-parameter `template <class T> class vector` in namespace std: it takes STLport's allocator parameter
#      (`template <class T, class A = allocator<T> >`), so every instantiation it names gains `V?$allocator@...@std@@`.
# check: the row loses exactly the stand-in spellings and gains their allocator-qualified forms (leaf included, kept in step).
import sys, re, os
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import famlib as F, inputs, f2m
T = '00414ea0'; FIN = '??1?$vector@VCCharString@@V?$allocator@VCCharString@@@std@@@std@@QAE@XZ'
BT, BS = inputs.bindings(); B = BT[T]
E, IX = inputs.index()
def blk_end(c, i):
    d = 0
    for j in range(i, len(c)):
        if c[j] == '{': d += 1
        elif c[j] == '}':
            d -= 1
            if d == 0: return j
    return -1
def stand(a): return [s for s in IX.get(a, {}).get('u', []) + IX.get(a, {}).get('d', []) if s in B and s != FIN]
def tr(t, a):
    olds = stand(a)
    if len(olds) != 1: return None, 'stand-ins %r' % olds
    c = f2m.code_only(t)
    m = re.match(r'^\?\?1(\w+)@@QAE@XZ$', olds[0])
    if m:   # shape (a)
        cls = m.group(1)
        if re.search(r'\bstruct\s+CCharString\b', c): return None, 'CCharString is a struct model'
        fwdcs = '' if re.search(r'\bclass\s+CCharString\b', c[:m and c.find('struct ' + cls) or 0]) else 'class CCharString;\n'
        dm = list(re.finditer(r'\bstruct\s+' + cls + r'\s*(?::\s*(\w+)\s*)?\{', c))
        if len(dm) != 1: return None, '%d definitions of %s' % (len(dm), cls)
        if re.search(r'\bnamespace\s+\w+\s*\{', c[:dm[0].start()]) and not re.search(r'\bnamespace\s+std\b', c): pass
        b0 = dm[0].end() - 1; be = blk_end(c, b0)
        semi = c.index(';', be)
        body = t[b0 + 1:be]
        bc = f2m.code_only(body)
        out = []; last = 0
        for mm in re.finditer(r'\b' + cls + r'\b', bc):
            out.append(body[last:mm.start()]); out.append('vector'); last = mm.end()
        out.append(body[last:]); body = ''.join(out)
        # STLport size_type is unsigned int: FableWin ??0?$vector@...@QAE@I@Z / ?reserve@...@QAEXI@Z
        body = re.sub(r'\bexplicit\s+vector\s*\(\s*unsigned\s+long\s+(\w+)\s*\)', r'explicit vector(unsigned int \1)', body)
        body = re.sub(r'\bvoid\s+reserve\s*\(\s*unsigned\s+long\s+(\w+)\s*\)', r'void reserve(unsigned int \1)', body)
        hasvec = re.search(r'\bclass\s+vector\b|\bstruct\s+vector\b', c)
        hasalloc = re.search(r'\b(class|struct)\s+allocator\b', c)
        fwd = '' if hasvec else ('template <class _Tp> class allocator;\n' if not hasalloc else '') + \
              'template <class _Tp, class _Alloc = allocator<_Tp> > class vector;\n'
        if hasvec and not hasalloc: return None, 'vector template without allocator'
        cmt = t[dm[0].end():t.find('\n', dm[0].end())]
        spec = (fwdcs + 'namespace std {\n' + fwd + 'template <> class vector<CCharString, allocator<CCharString> >' + (' : public ' + dm[0].group(1) if dm[0].group(1) else '') + ' { public:' + cmt.rstrip()
                + ('   // STLport 4.6 vector<CCharString> (row-local members)' if not cmt.strip() else '') + body + '}')
        # the struct line comment moves onto the specialization line
        if cmt.strip(): spec = spec.replace(cmt.rstrip() + body, cmt.rstrip() + body[len(cmt):] if body.startswith(cmt) else body, 1)
        t2 = t[:dm[0].start()] + spec + t[be + 1:semi + 1] + '\n}' + t[semi + 1:]
        # uses outside the specialization
        c2 = f2m.code_only(t2); out = []; last = 0
        s0 = dm[0].start(); s1 = s0 + len(spec)
        for mm in re.finditer(r'\b' + cls + r'\b', c2):
            if s0 <= mm.start() < s1: continue
            out.append(t2[last:mm.start()]); out.append('vector' if t2[mm.start() - 1] == '~' else 'std::vector<CCharString>'); last = mm.end()
        out.append(t2[last:]); t2 = ''.join(out)
        return t2, 'vcs %s -> std::vector<CCharString>' % cls
    if olds[0].startswith('??1?$vector@VCCharString@@@std@@'):   # shape (b)
        vm = list(re.finditer(r'template\s*<\s*class\s+(\w+)\s*>\s*(class|struct)\s+vector\b', c))
        if len(vm) != 1: return None, '%d one-parameter vector templates' % len(vm)
        p = vm[0].group(1)
        hasalloc = re.search(r'\b(class|struct)\s+allocator\b', c)
        rep = ('' if hasalloc else 'template <class %s> class allocator;\n' % p) + 'template <class %s, class _Alloc = allocator<%s> > %s vector' % (p, p, vm[0].group(2))
        t2 = t[:vm[0].start()] + rep + t[vm[0].end():]
        t2 = re.sub(r'(?m)^(// Leaf: [^\n]*)$', lambda mm: mm.group(1).replace(A1, A2), t2)
        return t2, 'vcs vector<T> -> vector<T, allocator<T> >'
    return None, 'stand-in shape %s' % olds[0]
A1 = 'V?$vector@VCCharString@@@std@@'; A2 = 'V?$vector@VCCharString@@V?$allocator@VCCharString@@@std@@@std@@'
NM = inputs.names(); PDB = NM['Ego_r.pdb'] | NM['FableWin.pdb']
REPORT = []
VS = 'V?$vector@VCCharString@@V?$allocator@VCCharString@@@std@@@std@@'
def sub(s):
    for k in ('CCharStringVector', 'SCharStringVector'):
        s = s.replace('U' + k + '@@', VS).replace('??0' + k + '@@', '??0?$vector@VCCharString@@V?$allocator@VCCharString@@@std@@@std@@')
        s = s.replace('@' + k + '@@', '@?$vector@VCCharString@@V?$allocator@VCCharString@@@std@@@std@@')
    return s
def chk(S0, S1, a):
    olds = set(stand(a)); D0 = set(IX.get(a, {}).get('d', []))
    lost = S0 - S1; got = S1 - S0 - {FIN}
    if not olds <= lost: return 'stand-in still emitted'
    if FIN not in S1: return 'final not emitted'
    bad = []
    for s in sorted(lost - olds):
        if s in D0: continue                          # a row-local inline member / the leaf: follows the class spelling
        if s in B or s in BS:                         # a bound callee changes its spelling: only onto a PDB spelling
            n = sub(s).replace('@QAE@K@Z', '@QAE@I@Z').replace('?reserve@?$vector@VCCharString@@V?$allocator@VCCharString@@@std@@@std@@QAEXK@Z', '?reserve@?$vector@VCCharString@@V?$allocator@VCCharString@@@std@@@std@@QAEXI@Z')
            if n in got and n in PDB: REPORT.append((a, 'bound-respelled', s, n)); continue
            bad.append('bound ' + s + ' -> ' + n)
    for s in sorted(got):
        if s in PDB: REPORT.append((a, 'pdb', s))
    if bad: return 'bound callee respelled off-PDB: %r' % bad[:3]
    return None
if __name__ == '__main__':
    F.run(sys.argv[1], [l.strip() for l in open(sys.argv[2]) if l.strip()], tr, chk, tag='vcs')
    for r in REPORT: print('report', *r)
