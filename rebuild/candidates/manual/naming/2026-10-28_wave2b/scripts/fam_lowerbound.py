#!/usr/bin/env python3
"""PILOT. CInterfaceMap::LowerBound (0x0040F020) / const LowerBound (0x004365B0) are CVectorMap template members.

DECISIONS.tsv (proven, STLport 4.6 spelling; Ego_r.pdb has the same class with Dinkumware iterators):
  0x0040F020 ?LowerBound@?$CVectorMap@W4ETCInterfaceType@@PAVCTCBase@@V?$CKeyPairCompareLess@W4ETCInterfaceType@@PAVCTCBase@@@@@@QAEPAU?$pair@W4ETCInterfaceType@@PAVCTCBase@@@std@@ABW4ETCInterfaceType@@@Z
  0x004365B0 ...@@QBEPBU?$pair@W4ETCInterfaceType@@PAVCTCBase@@@std@@ABW4ETCInterfaceType@@@Z
Landed rows model the map as `struct CInterfaceMap { struct SEntry { long type; CTCBase* component; }; SEntry* first;
SEntry* last; SEntry* LowerBound(const long* type); }` and call `map->LowerBound(&type)` on a `long type = ...;` local.
Edit (canonical model only; anything else is left alone and reported):
  * a prelude declares `enum ETCInterfaceType {}`, `std::pair<T1,T2> {first, second}`, `CKeyPairCompareLess<K,V>`,
    `CVectorMap<K,V,C>`; the model becomes the explicit specialization
    `CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> >`, its SEntry a typedef of
    `std::pair<ETCInterfaceType, CTCBase*>`, and `typedef ... CInterfaceMap;` keeps every other spelling;
  * LowerBound takes `const ETCInterfaceType&` (a reference is passed as the same pointer);
  * each `LowerBound(&x)` call becomes `LowerBound(x)` and the `long x = e;` local it names `ETCInterfaceType x =
    (ETCInterfaceType)(e);` (same 4-byte stack slot);
  * pair member uses `->type`/`.type` -> `first`, `->component`/`.component` -> `second`.
Byte-neutral by construction (same sizes, offsets and argument passing); the recheck is the proof. Idempotent.
usage: fam_lowerbound.py [--report] <file>...
"""
import sys, re, os
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__))); import lib

SPEC = 'CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> >'
PRELUDE = ('enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)\n'
           'namespace std { template <class T1, class T2> struct pair { T1 first; T2 second; }; }   // STLport 4.6 <utility>\n'
           'template <class K, class V> class CKeyPairCompareLess;\n'
           'template <class K, class V, class TCompare> class CVectorMap;\n')
ENTRY = re.compile(r'struct\s+SEntry\s*\{\s*long\s+type\s*;\s*CTCBase\s*\*\s*component\s*;\s*\}\s*;')
LBDECL = re.compile(r'(?P<ret>(?:const\s+)?SEntry\s*\*)\s*LowerBound\s*\(\s*const\s+long\s*\*\s*(?P<n>\w*)\s*\)\s*(?P<c>const\s*)?;')

def fix(text):
    m = lib.mask(text); info = []
    for bad in (r'\bETCInterfaceType\b', r'\bCVectorMap\b', r'\bCKeyPairCompareLess\b', r'\bnamespace\s+std\b',
                r'\bstruct\s+CTCBase\b', r'#\s*include\s*<(?!cstdio|stdio\.h|string\.h|cstring|stdlib\.h|cstdlib|math\.h|cmath)'):
        if re.search(bad, m): return text, ['BLOCK ' + bad]
    if re.search(r'\bCInterfaceMap\s*::\s*LowerBound\b', m): return text, ['BLOCK out-of-line LowerBound']
    bodies = lib.class_bodies(m, 'CInterfaceMap')
    if len(bodies) != 1: return text, ['BLOCK bodies=%d' % len(bodies)]
    key, lb, rb, hs = bodies[0]
    if lib.in_ns(m, hs): return text, ['BLOCK namespaced']
    if not re.match(r'\s*;', m[rb + 1:]): return text, ['BLOCK body-end']
    body = m[lb + 1:rb]
    e = ENTRY.search(m, lb, rb)
    if not e: return text, ['BLOCK entry-shape']
    decls = list(LBDECL.finditer(m, lb, rb))
    if not decls: return text, ['BLOCK no-decl']
    for d in decls:
        const_ret = d.group('ret').startswith('const'); cq = bool(d.group('c'))
        if const_ret != cq: return text, ['BLOCK const-shape']
    # other members must be plain SEntry*/long/pad data: no further functions
    rest = body
    for x in [e] + decls: rest = rest.replace(m[x.start():x.end()], '')
    if '(' in rest: return text, ['BLOCK extra-member-functions']
    edits = {}
    ls = text.rfind('\n', 0, hs) + 1
    # prelude on its own lines before the model's line, then the head `struct CInterfaceMap {` -> the specialization
    edits[ls] = (lb + 1 - ls, PRELUDE + text[ls:hs] + 'template <> class %s { public:' % SPEC)
    edits[e.start()] = (e.end() - e.start(), 'typedef std::pair<ETCInterfaceType, CTCBase*> SEntry;')
    for d in decls:
        lp = m.index('(', d.start()); rp = lib.match(m, lp, '(', ')')
        n = d.group('n') or 'key'
        edits[lp + 1] = (rp - lp - 1, 'const ETCInterfaceType& ' + n)
    se = rb + 1 + re.match(r'\s*;', m[rb + 1:]).end()
    edits[se] = (0, '\ntypedef %s CInterfaceMap;' % SPEC)
    # calls
    for c in re.finditer(r'(?:->|\.)\s*LowerBound\s*\(\s*&\s*(\w+)\s*\)', m):
        if lb < c.start() < rb: continue
        v = c.group(1)
        amp = m.index('&', c.start())
        edits[amp] = (c.end(1) - amp, v)
        decl = None
        for dd in re.finditer(r'\blong\s+%s\s*=\s*' % re.escape(v), m[:c.start()]): decl = dd
        if not decl: return text, ['BLOCK call-arg-decl ' + v]
        semi = m.index(';', decl.end())
        expr = text[decl.end():semi].strip()
        edits[decl.start()] = (semi - decl.start(), 'ETCInterfaceType %s = (ETCInterfaceType)(%s)' % (v, expr))
        info.append('CALL')
    if re.search(r'LowerBound\s*\(\s*(?!&)', m[:lb] + ' ' * (rb - lb) + m[rb:]) and 'CALL' not in info:
        pass
    for r in re.finditer(r'(->|\.)(\s*)(type|component)\b', m):
        if lb < r.start() < rb: continue
        edits[r.start(3)] = (len(r.group(3)), 'first' if r.group(3) == 'type' else 'second')
    # every LowerBound call outside the body must have been rewritten
    outside = [c for c in re.finditer(r'\bLowerBound\s*\(', m) if not (lb < c.start() < rb)]
    if len(outside) != info.count('CALL'): return text, ['BLOCK other-calls']
    info.append('FIX')
    return lib.apply(text, edits), info

if __name__ == '__main__':
    rep = '--report' in sys.argv
    for f in [a for a in sys.argv[1:] if a != '--report']:
        if rep:
            print(f, fix(open(f, encoding='utf-8', errors='surrogateescape').read())[1]); continue
        ch, info = lib.rw(f, fix)
        if ch: print('edited', f, info.count('CALL'))
