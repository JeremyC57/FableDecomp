#!/usr/bin/env python3
"""CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound (0x0040F020 / const 0x004365B0): wave 5, the stand-in models.

Wave 5 (naming/w5) = wave 4B's fam_lb4.py (unchanged recipe, below) plus:
  * MODELS is the full list of stand-in model names the deferred rows use (CTCMap_Retail, ComponentMap, CTCMap, ...);
  * inline member functions inside the model body (the inlined `Find`) are kept in the specialization; their
    LowerBound calls, entry member uses and const results are rewritten like the file-scope ones;
  * a cast call argument may be a qualified name (`(unsigned long)T::INTERFACE_TYPE`).
fam_lb4.py's description:

DECISIONS.tsv (proven, STLport 4.6 spelling; Ego_r.pdb has the same class):
  0x0040F020 ?LowerBound@?$CVectorMap@W4ETCInterfaceType@@PAVCTCBase@@V?$CKeyPairCompareLess@W4ETCInterfaceType@@PAVCTCBase@@@@@@QAEPAU?$pair@W4ETCInterfaceType@@PAVCTCBase@@@std@@ABW4ETCInterfaceType@@@Z
  0x004365B0 ...@@QBEPBU?$pair@W4ETCInterfaceType@@PAVCTCBase@@@std@@ABW4ETCInterfaceType@@@Z
Wave 2B's fam_lowerbound.py (committed, 2026-10-29_lowerbound) handles the canonical CInterfaceMap model only. This
script generalises it (source rows only, tests are not touched):
  * the model is any file-scope class/struct in MODELS (CInterfaceMap, CInterfaceMap_Retail, CTCMap, ...) whose
    body holds only data members, LowerBound declarations and the entry type: a nested `struct E { K k; V* v; };`
    or a file-scope one defined before the model (K in long/int/unsigned long/unsigned int/fable_i32, V any class);
  * the model becomes the explicit specialization `template <> class CVectorMap<ETCInterfaceType, CTCBase*,
    CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > { public: ... };` plus `typedef ... <Model>;`, the entry a
    typedef of `std::pair<ETCInterfaceType, CTCBase*>` (same 8-byte layout);
  * each LowerBound takes `const ETCInterfaceType&`; a const LowerBound returns `const E*` (the PDB form; the
    non-const one `E*`); locals that receive a const LowerBound result get `const`;
  * calls: `LowerBound(&x)` (pointer parameter) -> `LowerBound(x)`; a reference-parameter call keeps its argument,
    a cast argument `(long)e` becomes `(ETCInterfaceType)e`; the named local `K x = e;` becomes
    `ETCInterfaceType x = (ETCInterfaceType)(e);` (same 4-byte slot);
  * entry member uses `->k` / `.k` -> `first`, `->v` / `.v` -> `second`; V != CTCBase gets `class CTCBase;`.
The prelude (`enum ETCInterfaceType {}`, `std::pair`, `CKeyPairCompareLess`, `CVectorMap` declarations) skips what
the file already declares in the same form. Byte-neutral by construction; the recheck is the proof. Idempotent.
usage: fam_lb5.py [--report] <file>...  (fam_lb4.py's usage line; a `_test.cpp` in the list is edited too)
"""
import sys, re, os
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__))); import lib, famlib

SPEC = 'CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> >'
PAIR = 'std::pair<ETCInterfaceType, CTCBase*>'
MODELS = os.environ.get('LB5_MODELS', 'BlockSet2,CCameraComponentMap,CCreatureInterfaceMap,CIceMap,CInterfaceMap,CInterfaceMapB,CInterfaceMap_Retail,CRetailInterfaceMap,CTCInterfaceMap,CTCMap,CTCMapPair,CTCMapRemaining,CTCMap_Retail,CTrapInterfaceMap,CTrapResetInterfaceMap,CVectorMapLong,ComponentMap,Container,DoorMap,IFaceMap,InterfaceMap,IsPCReady_Map,Map,Map896e60,Map898b30,Map8a12a0,TCMap').split(',')
KEYS = r'(?:long|int|unsigned\s+long|unsigned\s+int|unsigned|fable_i32|signed\s+long)'
EKEYS = r'(?:long|int|unsigned\s+long|unsigned\s+int|unsigned|fable_i32|signed\s+long|ETCInterfaceType)'   # entry key types
PAIR_DEF = re.compile(r'template\s*<\s*class\s+(\w+)\s*,\s*class\s+(\w+)\s*>\s*struct\s+pair\s*\{\s*\1\s+first\s*;\s*\2\s+second\s*;\s*\}\s*;')


class Edits(list):
    """edits[off] = (len, new) appends; apply() right to left, a replacement before an insertion at one offset."""
    def __setitem__(self, off, v):
        self.append((off, v[0], v[1]))
    def apply(self, text):
        rep = sorted(e for e in self if e[1])
        for x, y in zip(rep, rep[1:]):
            if x[0] + x[1] > y[0]: raise ValueError('overlap %r %r' % (x[:2], y[:2]))
        for off, ln, new in sorted(set(self), key=lambda e: (e[0], e[1] != 0), reverse=True):
            text = text[:off] + new + text[off + ln:]
        return text


def expr_start(m, pos):
    """Start of the postfix expression whose member access (`->` / `.`) begins at pos."""
    k = pos
    while True:
        j = k - 1
        while j >= 0 and m[j] in ' \t\r\n': j -= 1
        if j < 0: return None
        if m[j] in ')]':
            o = '(' if m[j] == ')' else '['
            d = 0; q = j
            while q >= 0:
                if m[q] in ')]': d += 1
                elif m[q] in '([':
                    d -= 1
                    if d == 0: break
                q -= 1
            if q < 0: return None
            k = q
            # a call `f(...)` / index `a[...]`: keep the callee name
            j2 = k - 1
            while j2 >= 0 and m[j2] in ' \t': j2 -= 1
            if j2 >= 0 and (m[j2].isalnum() or m[j2] == '_'):
                w = re.search(r'(\w+)$', m[:j2 + 1]).group(1)
                if w in ('return', 'if', 'while', 'switch', 'case', 'else', 'do', 'throw', 'new', 'delete', 'sizeof', 'and', 'or', 'not'):
                    return k
                continue
            if j2 >= 0 and m[j2] == ']':
                continue
            return k
        elif m[j].isalnum() or m[j] == '_':
            q = j
            while q >= 0 and (m[q].isalnum() or m[q] == '_'): q -= 1
            k = q + 1
            j2 = q
            while j2 >= 0 and m[j2] in ' \t': j2 -= 1
            if j2 >= 1 and m[j2] == '>' and m[j2 - 1] == '-': k = j2 - 1; continue
            if j2 >= 0 and m[j2] == '.' : k = j2; continue
            if j2 >= 1 and m[j2] == ':' and m[j2 - 1] == ':': k = j2 - 1; continue
            return k
        else:
            return None


# rows whose single non-const LowerBound declaration is bound to the const member 0x004365B0 (base recheck pairs):
# the declaration becomes the const member (the call then resolves to it; the result local gets const)
FORCE_CONST = {'0092b800', '009643d0', '00965520',
               '00857cb0', '00899c10', '0089a210', '0089b330', '006cdbd0', '006cdc70', '006cdd00'}   # w5: from the lbdef_base2 recheck pairs (QAE bound to 0x004365B0)


def tnorm(t):
    t = re.sub(r'\s+', ' ', t.strip())
    return {'unsigned': 'unsigned int', 'signed long': 'long', 'fable_i32': 'int', 'signed int': 'int', 'signed': 'int'}.get(t, t)


def fix(text, row=None, ool_ok=False):
    # w5: an address-suffixed stand-in method name (`LowerBound_004365b0`, both overloads) is the LowerBound itself
    if re.search(r'\bLowerBound_004365b0\b', text) and not re.search(r'\bLowerBound\b', lib.mask(text)):
        t2, info = fix(re.sub(r'\bLowerBound_004365b0\b', 'LowerBound', text), row, ool_ok)
        return (t2, info) if 'FIX' in info else (text, info)
    m = lib.mask(text); info = []
    # a non-template `struct CVectorMap { ... };` stand-in is itself the model: it becomes the specialization and its
    # other uses (`CVectorMap m_44;`) are spelled as the specialization (no typedef: the template owns the name)
    cvm = False
    if re.search(r'\bCVectorMap\b|\bCKeyPairCompareLess\b', m):
        cb = lib.class_bodies(m, 'CVectorMap')
        if (len(cb) == 1 and not re.search(r'\bCKeyPairCompareLess\b|\btemplate\b[^;{]*\bCVectorMap\b|\bCVectorMap\s*<', m)
                and not re.search(r'\btemplate\s*<[^>]*>\s*$', m[:cb[0][3]])):
            cvm = True
        else:
            return text, ['BLOCK CVectorMap present']
    if re.search(r'#\s*include\s*<(?!cstdio|stdio\.h|string\.h|cstring|stdlib\.h|cstdlib|math\.h|cmath|stddef\.h|cstddef|new>|windows\.h|stdarg\.h|cstdarg)', text):
        return text, ['BLOCK stl-include']
    if re.search(r'\bstruct\s+CTCBase\b', m): return text, ['BLOCK struct CTCBase']
    if re.search(r'^\s*#\s*define\b[^\n]*\bLowerBound\b', text, re.M): return text, ['BLOCK macro']
    hits = []
    for name in (['CVectorMap'] if cvm else MODELS):
        for b in lib.class_bodies(m, name):
            if re.search(r'\bLowerBound\s*\(', m[b[1]:b[2]]): hits.append((name,) + b)
    if len(hits) != 1: return text, ['BLOCK bodies=%d' % len(hits)]
    name, key, lb, rb, hs = hits[0]
    if lib.in_ns(m, hs): return text, ['BLOCK namespaced']
    # out-of-line LowerBound definitions: only in a test (a stub body; the source rows that define it in their own TU
    # would emit the retail-named definition, so they stay blocked)
    oolp = re.compile(r'(?P<rc>const\s+)?(?:%s\s*::\s*)?(?P<e>\w+)\s*\*\s*%s\s*::\s*LowerBound\s*\(\s*(?:const\s+)?(?:%s)\s*(?P<pk>[*&])\s*(?P<n>\w+)\s*\)\s*(?P<c>const\s*)?\{' % (name, name, EKEYS))
    oold = list(oolp.finditer(m))
    if re.search(r'\b%s\s*::\s*LowerBound\b' % name, m) and (not ool_ok or len(oold) != len(re.findall(r'\b%s\s*::\s*LowerBound\b' % name, m))):
        return text, ['BLOCK out-of-line LowerBound']
    if re.search(r'\btemplate\s*<[^>]*>\s*$', m[:hs]): return text, ['BLOCK template model']
    if not re.match(r'\s*;', m[rb + 1:]): return text, ['BLOCK body-end']
    if re.search(r'\b(?:class|struct)\s+%s\s*:' % name, m[hs:lb + 1]): return text, ['BLOCK model-base']
    # depth-1 statements of the body
    stm = []
    i = lb + 1
    while i < rb:
        j = i; d = 0
        while j < rb:
            if m[j] == '{': d += 1
            elif m[j] == '}':
                d -= 1
                if d == 0 and not re.match(r'\s*;', m[j + 1:]) and re.search(r'\)\s*(?:const\s*)?\{', m[i:j]): break
            elif m[j] == ';' and d == 0: break
            j += 1
        if m[i:j].strip(): stm.append((i, j + 1))
        i = j + 1
    entry = None; decls = []; data = []; inl = []
    for a, b in stm:
        s = re.sub(r'^\s*(?:(?:public|private|protected)\s*:\s*)+', '', m[a:b])
        if not s.strip(): continue
        e = re.match(r'\s*struct\s+(\w+)\s*\{\s*(%s)\s+(\w+)\s*;\s*(?:class\s+|struct\s+)?(\w+)\s*\*\s*(\w+)\s*;\s*\}\s*;' % EKEYS, s)
        if e:
            if entry: return text, ['BLOCK two-entries']
            entry = ('nested', a + m[a:b].index('struct'), b) + e.groups()
            continue
        d = re.match(r'\s*(?P<rc>const\s+)?(?P<ret>\w+)\s*\*\s*LowerBound\s*\(\s*(?P<pc>const\s+)?(?P<pt>%s)\s*(?P<pk>[*&])\s*(?P<n>\w*)\s*\)\s*(?P<c>const\s*)?;\s*$' % EKEYS, s)
        if d:
            decls.append((a + (len(m[a:b]) - len(s.lstrip())), b, d)); continue
        if re.match(r'\s*(?:(?:inline|__forceinline)\s+)?(?:const\s+)?\w+\s*\*?\s*\w+\s*\([^(){};]*\)\s*(?:const\s*)?\{', s) and not re.search(r'\b(?:operator|typedef|friend|static|virtual|template)\b', s):
            inl.append((a, b)); continue
        if '(' in s or '{' in s or 'operator' in s or 'typedef' in s or 'friend' in s or 'static' in s or 'virtual' in s:
            return text, ['BLOCK extra-members']
        data.append((a, b))
    if not decls: return text, ['BLOCK no-decl']
    # positions the rewrite must not treat as uses: the LowerBound declarations and the nested entry struct
    skip = [(a, b) for a, b, _ in decls] + ([(entry[1], entry[2])] if entry else [])
    def skipped(p): return any(a <= p < b for a, b in skip)
    if len([c for c in re.finditer(r'\bLowerBound\s*\(', m[lb:rb]) if not skipped(lb + c.start())]) != \
       len([c for a, b in inl for c in re.finditer(r'\bLowerBound\s*\(', m[a:b])]): return text, ['BLOCK decl-count']
    ename = entry[3] if entry else decls[0][2].group('ret')
    for _, _, d in decls:
        if d.group('ret') != ename: return text, ['BLOCK ret-type %s' % d.group('ret')]
        pass   # w5: a non-const pointer key (`int* key`) is tried too; the recheck decides
    if not entry:
        # file-scope entry struct before the model
        es = [x for x in lib.class_bodies(m, ename) if x[3] < hs and not lib.in_ns(m, x[3])]
        if len(es) != 1 or len(lib.class_bodies(m, ename)) != 1: return text, ['BLOCK entry-bodies']
        ek, elb, erb, ehs = es[0]
        e = re.match(r'(?:struct|class)\s+%s\s*\{\s*(?:public\s*:\s*)?(%s)\s+(\w+)\s*;\s*(?:class\s+|struct\s+)?(\w+)\s*\*\s*(\w+)\s*;\s*\}\s*;' % (ename, EKEYS), m[ehs:])
        if not e: return text, ['BLOCK entry-shape']
        entry = ('file', ehs, ehs + e.end(), ename) + e.groups()
    _, es_, ee_, ename, ktype, kname, vtype, vname = entry
    if kname == vname: return text, ['BLOCK entry-names']
    edits = Edits()
    # ---- prelude -------------------------------------------------------------------------------
    pre = []
    if not re.search(r'\benum\s+ETCInterfaceType\b', m):
        if re.search(r'\bETCInterfaceType\b', m): return text, ['BLOCK ETCInterfaceType other']
        pre.append('enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)')
    elif re.search(r'\benum\s+ETCInterfaceType\b', m[hs:]) or lib.in_ns(m, re.search(r'\benum\s+ETCInterfaceType\b', m).start()):
        return text, ['BLOCK ETCInterfaceType placement']
    nsr = [(n.start(), n.end() - 1, lib.match(m, n.end() - 1, '{', '}')) for n in re.finditer(r'\bnamespace\s+std\s*\{', m)]
    has_pair = False
    for s0, a, b in nsr:
        if re.search(r'\bpair\b', m[a:b]):
            if PAIR_DEF.search(m[a:b]) and s0 < min(hs, es_): has_pair = True
            else: return text, ['BLOCK std::pair other']
    if not has_pair:
        if re.search(r'\bpair\b', m): return text, ['BLOCK pair token']
        pre.append('namespace std { template <class T1, class T2> struct pair { T1 first; T2 second; }; }   // STLport 4.6 <utility>')
    pre.append('template <class K, class V> class CKeyPairCompareLess;')
    pre.append('template <class K, class V, class TCompare> class CVectorMap;')
    ctc = re.search(r'\bclass\s+CTCBase\s*[;{:]', m)
    if not ctc or ctc.start() > min(hs, es_):
        if re.search(r'\bCTCBase\b', m[:min(hs, es_)]): return text, ['BLOCK CTCBase order']
        pre.insert(0, 'class CTCBase;')
    first = min(hs, es_) if entry[0] == 'file' else hs
    ls = text.rfind('\n', 0, first) + 1
    if m[ls:first].strip(): return text, ['BLOCK model-line']
    edits[ls] = (0, '\n'.join(pre) + '\n')
    # ---- the entry -----------------------------------------------------------------------------
    if entry[0] == 'nested':
        edits[es_] = (ee_ - es_, 'typedef %s %s;' % (PAIR, ename))
    else:
        edits[es_] = (ee_ - es_, 'typedef %s %s;' % (PAIR, ename))
        if re.search(r'\b(?:struct|class)\s+%s\s*;' % ename, m): return text, ['BLOCK entry-fwd']
    # ---- the model head / tail -----------------------------------------------------------------
    edits[hs] = (lb + 1 - hs, 'template <> class %s { public:' % SPEC)
    se = rb + 1 + re.match(r'\s*;', m[rb + 1:]).end()
    if cvm:
        for u in re.finditer(r'\bCVectorMap\b', m):
            if u.start() == m.index('CVectorMap', hs) and hs <= u.start() < lb: continue   # the model head itself
            if lb < u.start() < rb: return text, ['BLOCK CVectorMap inside the model']
            if any(o.start() <= u.start() < o.end() for o in oold): continue      # an out-of-line head (rewritten below)
            edits[u.start()] = (len('CVectorMap'), SPEC)
    else:
        edits[se] = (0, '\ntypedef %s %s;' % (SPEC, name))
    constfix = False
    force = row in FORCE_CONST and len(decls) == 1 and not decls[0][2].group('c')
    # w5: a FORCE_CONST row with a const and a non-const declaration (every call binds 0x004365B0): the non-const one
    # goes, so the calls resolve to the const member
    drop = row in FORCE_CONST and len(decls) == 2 and sorted(bool(d.group('c')) for _, _, d in decls) == [False, True]
    for a, b, d in decls:
        if drop and not d.group('c'):
            ls_ = text.rfind('\n', 0, a) + 1; le_ = text.find('\n', b); le_ = len(text) if le_ < 0 else le_ + 1
            if m[ls_:a].strip() or m[b:le_].strip(): return text, ['BLOCK drop-line']
            edits[ls_] = (le_ - ls_, ''); continue
        cq = bool(d.group('c')) or force
        if force: constfix = True
        ret = ('const %s*' if cq else '%s*') % ename
        if cq and not d.group('rc'): constfix = True
        if not cq and d.group('rc'): return text, ['BLOCK const-ret-nonconst']
        n = d.group('n') or 'key'
        tail = ' const;' if cq else ';'
        edits[a] = (b - a, '%s LowerBound(const ETCInterfaceType& %s)%s' % (ret, n, tail))
    pk = decls[0][2].group('pk')
    if any(d.group('pk') != pk for _, _, d in decls): return text, ['BLOCK mixed-params']
    # ---- calls ---------------------------------------------------------------------------------
    sc = famlib.scopes(m)
    def fbody(p):
        f = None
        s_ = famlib.innermost(sc, p)
        while s_ is not None and s_.kind == 'block':
            f = s_; s_ = s_.parent
        return f
    for o in oold:
        h0 = o.start()
        hb = o.end() - 1
        if o.group('pk') != pk or bool(o.group('c')) != bool(decls[0][2].group('c')) or len(decls) != 1:
            return text, ['BLOCK out-of-line shape']
        cq = bool(o.group('c'))
        n = o.group('n')
        qn = SPEC if cvm else name
        eq = ('%s::%s' % (qn, ename)) if entry[0] == 'nested' else ename
        edits[h0] = (hb - h0, '%s%s* %s::LowerBound(const ETCInterfaceType& %s)%s\n' % ('const ' if cq else '', eq, qn, n, ' const' if cq else ''))
        skip.append((h0, hb))
        bb = lib.match(m, hb, '{', '}')
        if pk == '*':
            uses = list(re.finditer(r'(?<![\w.>])%s\b' % re.escape(n), m[hb:bb]))
            for u in uses:
                st = hb + u.start()
                k = st - 1
                while m[k] in ' \t': k -= 1
                if m[k] != '*': return text, ['BLOCK out-of-line key use']
                edits[k] = (st - k, '')
    ncall = 0
    retyped = set()
    for c in re.finditer(r'\bLowerBound\s*\(', m):
        if skipped(c.start()): continue
        lp = c.end() - 1; rp = lib.match(m, lp, '(', ')')
        arg = m[lp + 1:rp].strip()
        a0 = lp + 1 + m[lp + 1:rp].index(arg)
        v = None
        if pk == '*':
            am = re.fullmatch(r'&\s*(\w+)', arg)
            if not am: return text, ['BLOCK call-arg %s' % arg[:30]]
            v = am.group(1)
        else:
            if re.fullmatch(r'\w+', arg) and not re.fullmatch(r'\d\w*|[A-Z_][A-Z0-9_]*', arg):
                v = arg
            else:
                cm = re.fullmatch(r'\(\s*%s\s*\)\s*(\w+(?:\s*::\s*\w+)*)' % EKEYS, arg)
                if cm:
                    cs = m.index('(', lp + 1); ce = lib.match(m, cs, '(', ')')
                    edits[cs] = (ce + 1 - cs, '(ETCInterfaceType)')
                elif re.fullmatch(r'[A-Z_][A-Z0-9_]*|0x[0-9a-fA-F]+|\d+', arg):
                    edits[a0] = (len(arg), '(ETCInterfaceType)%s' % arg)
                else:
                    return text, ['BLOCK call-arg %s' % arg[:30]]
        if v:
            fb = fbody(c.start())
            if fb is None: return text, ['BLOCK call-scope']
            decl = None
            for dd in re.finditer(r'(?<![\w.>:])(?:const\s+)?(?P<t>%s|ETCInterfaceType)\s+%s\s*(?:=\s*|;)' % (KEYS, re.escape(v)), m[:c.start()]):
                if dd.start() > fb.lb: decl = dd
            if decl and decl.group(0).rstrip().endswith(';'): return text, ['BLOCK uninit-decl ' + v]
            pt = tnorm(decls[0][2].group('pt'))
            if decl:
                vt = tnorm(decl.group('t'))
            else:
                hs0 = max(m.rfind(';', 0, fb.lb), m.rfind('}', 0, fb.lb), m.rfind('{', 0, fb.lb))
                pm = re.search(r'[(,]\s*(?:const\s+)?(%s|ETCInterfaceType)\s*&?\s*%s\s*[,)]' % (KEYS, re.escape(v)), m[hs0 + 1:fb.lb])
                if not pm: return text, ['BLOCK call-arg-decl ' + v]
                vt = tnorm(pm.group(1))
            if pk == '*' and vt != pt: return text, ['BLOCK ptr-arg-type ' + v]
            if pk == '&' and vt != pt:
                # the old `const long&` key bound a converted TEMPORARY: keep one by spelling the old conversion
                # (`(ETCInterfaceType)(long)key`; a bare `(ETCInterfaceType)key` of an enum lvalue binds the variable
                # itself in VC7.1 and the compare then reloads it); the variable keeps its type
                edits[a0] = (len(arg), '(ETCInterfaceType)(%s)%s' % (pt, v))
            elif vt == 'ETCInterfaceType':
                if pk == '*': edits[a0] = (len(arg), v)                     # already the key type: binds directly
            elif decl:
                if pk == '*': edits[a0] = (len(arg), v)
                if (decl.start(), v) not in retyped:
                    semi = m.index(';', decl.end())
                    if ',' in m[decl.end():semi] and '(' not in m[decl.end():semi]: return text, ['BLOCK multi-decl ' + v]
                    expr = text[decl.end():semi].strip()
                    cq_ = 'const ' if decl.group(0).lstrip().startswith('const') else ''   # a const key stays const (folds the compare)
                    edits[decl.start()] = (semi - decl.start(), '%sETCInterfaceType %s = (ETCInterfaceType)(%s)' % (cq_, v, expr))
                    retyped.add((decl.start(), v))
            else:
                edits[a0] = (len(arg), '(const ETCInterfaceType&)%s' % v)   # a parameter: same address, no temporary
        ncall += 1
        info.append('CALL')
    # ---- const results -------------------------------------------------------------------------
    if constfix:
        dpat = r'(?<![\w:])(?P<c>const\s+)?(?P<q>(?:%s\s*::\s*)?%s\s*\*\s*)(?P<v>\w+)\s*(?P<e>=|;|,|\))' % (re.escape(name), re.escape(ename))
        decls_ = [(d_, fbody(d_.start())) for d_ in re.finditer(dpat, m) if not skipped(d_.start())]
        cv = set()
        made = set()
        def mkconst(d_, f_):
            if not d_.group('c') and d_.start('q') not in made:
                edits[d_.start('q')] = (0, 'const '); made.add(d_.start('q'))
            cv.add((id(f_), d_.group('v')))
        cfun = set()
        def fhead(f_):
            hs0 = max(m.rfind(';', 0, f_.lb), m.rfind('}', 0, f_.lb), m.rfind('{', 0, f_.lb))
            return hs0 + 1
        def hits(rhs, f_):
            if re.search(r'\bLowerBound\s*\(', rhs): return True
            if any(re.search(r'(?<![\w.>:])%s\s*\(' % re.escape(fn), rhs) for fn in cfun): return True
            return any(re.search(r'(?<![\w.>])%s(?!\w)' % re.escape(v_), rhs) for (fi, v_) in cv if fi == id(f_))
        changed = True
        while changed:
            changed = False
            for rr in re.finditer(r'\breturn\s+(?P<r>[^;]*);', m):
                f_ = fbody(rr.start())
                if f_ is None or not hits(rr.group('r'), f_): continue
                h0 = fhead(f_)
                hd = re.match(r'\s*(?:(?:inline|static|__forceinline)\s+)*(?P<c>const\s+)?(?P<q>(?:%s\s*::\s*)?%s\s*\*)\s*(?:__\w+\s+)?(?P<n>\w+)\s*\(' % (re.escape(name), re.escape(ename)), m[h0:f_.lb])
                if not hd: continue
                if hd.group('n') in cfun: continue
                if not hd.group('c'):
                    edits[h0 + hd.start('q')] = (0, 'const ')
                cfun.add(hd.group('n')); changed = True
            for d_, f_ in decls_:
                if (id(f_), d_.group('v')) in cv: continue
                if d_.group('e') == '=':
                    semi = m.index(';', d_.end())
                    rhs = m[d_.end():semi]
                    if hits(rhs, f_):
                        mkconst(d_, f_); changed = True; continue
                # plain assignments `v = rhs;` in the same function
                for r in re.finditer(r'(?<![\w.>:])%s\s*=(?!=)\s*(?P<r>[^;]*);' % re.escape(d_.group('v')), m):
                    if fbody(r.start()) is not f_ or r.start() < d_.start(): continue
                    rhs = r.group('r')
                    if hits(rhs, f_):
                        mkconst(d_, f_); changed = True; break
    # ---- members -------------------------------------------------------------------------------
    wrap = vtype != 'CTCBase'
    if wrap and re.search(r'\b(?:class|struct)\s+%s\s*:' % re.escape(vtype), m): return text, ['BLOCK value-derived']
    eids = set(re.findall(r'\b%s\s*(?:const\s*)?[*&]\s*(?:const\s+)?(\w+)' % re.escape(ename), m))
    eids |= set(re.findall(r'\b%s\s+(\w+)\s*[;=\[(,)]' % re.escape(ename), m))
    for r in re.finditer(r'(->|\.)(\s*)(%s|%s)\b' % (re.escape(kname), re.escape(vname)), m):
        if skipped(r.start()): continue
        if lb < r.start() < rb and not any(a <= r.start() < b for a, b in inl): continue
        if r.start() > 0 and m[r.start() - 1] == '.': continue
        if entry[0] == 'file' and es_ < r.start() < ee_: continue
        st0 = expr_start(m, r.start())
        if st0 is None: return text, ['BLOCK member-expr']
        obj = m[st0:r.start()]
        if not ('LowerBound' in obj or any(re.search(r'(?<![\w])%s(?!\w)' % re.escape(x), obj) for x in eids)):
            continue
        isv = r.group(3) == vname
        edits[r.start(3)] = (len(r.group(3)), 'second' if isv else 'first')
        ka = re.match(r'\s*=(?!=)\s*', m[r.end(3):])
        if not isv and ka and not m[r.end(3) + ka.end():].startswith('(ETCInterfaceType)'):
            edits[r.end(3) + ka.end()] = (0, '(ETCInterfaceType)')      # a key store: int -> enum needs the cast
        if isv and wrap:
            asg = re.match(r'\s*=(?!=)', m[r.end(3):])
            if asg:                                          # a store: cast the stored value instead (no cast lvalue)
                edits[r.end(3) + asg.end()] = (0, ' (CTCBase*)')
            else:
                edits[st0] = (0, '((%s*)' % vtype)
                edits[r.end(3)] = (0, ')')
    # tests: aggregate entry tables `E e[N] = { { 0x6d, (CTCBase*)&x }, ... };` -> cast each key / value
    if ool_ok:
        for ag in re.finditer(r'\b%s\s+\w+\s*(?:\[[^\]]*\])?\s*=\s*\{' % re.escape(ename), m):
            ob = ag.end() - 1; cb = lib.match(m, ob, '{', '}')
            for pr in re.finditer(r'\{([^{}]*)\}', m[ob + 1:cb]):
                parts = pr.group(1).split(',')
                if len(parts) != 2: return text, ['BLOCK test aggregate']
                p0 = ob + 1 + pr.start(1); p1 = p0 + len(parts[0]) + 1
                k0 = p0 + len(parts[0]) - len(parts[0].lstrip())
                edits[k0] = (0, '(ETCInterfaceType)')
                v0 = p1 + len(parts[1]) - len(parts[1].lstrip())
                if parts[1].strip() != '0' and not parts[1].strip().startswith('(CTCBase*)'):
                    edits[v0] = (0, '(CTCBase*)')
    outside = [c for c in re.finditer(r'\bLowerBound\s*\(', m) if not skipped(c.start())]
    if len(outside) != ncall: return text, ['BLOCK other-calls']
    info.append('FIX')
    return edits.apply(text), info


def row_of(f):
    mm = re.search(r'_([0-9a-f]{8})\.cpp$', os.path.basename(f))
    return mm.group(1) if mm else None


# a test (`*_test.cpp`) is edited only when it is in the file list (the tests a source edit broke: they define the old
# LowerBound stub out of line); out-of-line definitions are rewritten in tests only

if __name__ == '__main__':
    rep = '--report' in sys.argv
    for f in [a for a in sys.argv[1:] if a != '--report']:
        if rep:
            print(f, fix(open(f, encoding='utf-8', errors='surrogateescape').read(), row_of(f), '_test.cpp' in f)[1]); continue
        ch, info = lib.rw(f, lambda t: fix(t, row_of(f), '_test.cpp' in f))
        if ch: print('edited', f, info.count('CALL'))
