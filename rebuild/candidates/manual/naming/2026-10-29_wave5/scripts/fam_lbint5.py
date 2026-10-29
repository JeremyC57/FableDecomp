#!/usr/bin/env python3
"""CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound (0x0040F020): the `int*` entry stand-in of the CTC FindInterface
leaves (0x0042C43F .. 0x0042C8F4, wave 5).

The rows model the thing's component map as `struct M { int* begin; int* end; int* LowerBound(const int* key); };` and
read the entry as `*entry` (key) / `entry[1]` (component). Final symbol (DECISIONS.tsv, proven):
  ?LowerBound@?$CVectorMap@W4ETCInterfaceType@@PAVCTCBase@@V?$CKeyPairCompareLess@W4ETCInterfaceType@@PAVCTCBase@@@@@@QAEPAU?$pair@W4ETCInterfaceType@@PAVCTCBase@@@std@@ABW4ETCInterfaceType@@@Z
Edit (fam_lb4/fam_lb5 recipe, same 8-byte entry): the model becomes the explicit specialization with
`typedef std::pair<ETCInterfaceType, CTCBase*> SEntry;` plus `typedef ... M;`; the key local passed by address
`int k = X;` becomes `ETCInterfaceType k = (ETCInterfaceType)(X);` and is passed by reference; `int* entry` / `int* end`
become `M::SEntry*`; `*entry` -> `entry->first`, `entry[1]` -> `(int)entry->second`. Works inside the rows'
#define bodies (plain text). Byte-neutral by construction; the recheck is the proof. Idempotent (the model regex no
longer matches after the edit). usage: fam_lbint5.py [--report] <file>...
"""
import sys, re, os
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__))); import lib

SPEC = 'CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> >'
MODEL = re.compile(r'struct\s+(\w+)\s*\{\s*int\s*\*\s*begin\s*;\s*int\s*\*\s*end\s*;\s*int\s*\*\s*LowerBound\s*\(\s*const\s+int\s*\*\s*key\s*\)\s*;\s*\}\s*;')
PRE = ('class CTCBase;\n'
       'enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)\n'
       'namespace std { template <class T1, class T2> struct pair { T1 first; T2 second; }; }   // STLport 4.6 <utility>\n'
       'template <class K, class V> class CKeyPairCompareLess;\n'
       'template <class K, class V, class TCompare> class CVectorMap;\n')


def fix(text):
    m = lib.mask(text)
    ms = list(MODEL.finditer(m))
    if len(ms) != 1: return text, ['BLOCK model=%d' % len(ms)]
    if re.search(r'\b(?:ETCInterfaceType|CTCBase|CVectorMap|pair)\b', m): return text, ['BLOCK prelude names present']
    mo = ms[0]; name = mo.group(1)
    if len(re.findall(r'\bLowerBound\s*\(', m)) - 1 != len(re.findall(r'\bLowerBound\s*\(\s*&\s*\w+\s*\)', m)):
        return text, ['BLOCK call shape']
    keys = set(re.findall(r'\bLowerBound\s*\(\s*&\s*(\w+)\s*\)', m))
    ed = {}
    ls = text.rfind('\n', 0, mo.start()) + 1
    if m[ls:mo.start()].strip(): return text, ['BLOCK model-line']
    ed[mo.start()] = (mo.end() - mo.start(),
        PRE + 'template <> class %s { public: typedef std::pair<ETCInterfaceType, CTCBase*> SEntry; SEntry* begin; SEntry* end; '
              'SEntry* LowerBound(const ETCInterfaceType& key); };\ntypedef %s %s;' % (SPEC, SPEC, name))
    n = 0
    for c in re.finditer(r'\bLowerBound\s*\(\s*&\s*(\w+)\s*\)', m):
        ed[c.start()] = (c.end() - c.start(), 'LowerBound(%s)' % c.group(1)); n += 1
    for k in keys:
        ds = list(re.finditer(r'(?<!register )(?<![\w])int\s+%s\s*=\s*([^;]+);' % re.escape(k), m))
        if not ds: return text, ['BLOCK key-decl ' + k]
        for d in ds:
            ed[d.start()] = (d.end() - d.start(), 'ETCInterfaceType %s = (ETCInterfaceType)(%s);' % (k, text[d.start(1):d.end(1)].strip()))
    for d in re.finditer(r'\bint\s*\*\s*(entry|end)\s*=', m):
        ed[d.start()] = (d.end() - d.start(), '%s::SEntry* %s =' % (name, d.group(1)))
    for d in re.finditer(r'\*\s*entry\b(?!\s*=[^=])', m):
        if m[d.start() - 1:d.start()] in ('t', ')'): continue
        ed[d.start()] = (d.end() - d.start(), 'entry->first')
    for d in re.finditer(r'\bentry\s*\[\s*1\s*\]', m):
        ed[d.start()] = (d.end() - d.start(), '(int)entry->second')
    return lib.apply(text, ed), ['CALL'] * n + ['FIX']


if __name__ == '__main__':
    rep = '--report' in sys.argv
    for f in [a for a in sys.argv[1:] if a != '--report']:
        if '_test.cpp' in f: continue
        if rep:
            print(f, fix(open(f, encoding='utf-8', errors='surrogateescape').read())[1]); continue
        ch, info = lib.rw(f, fix)
        if ch: print('edited', f, info.count('CALL'))
