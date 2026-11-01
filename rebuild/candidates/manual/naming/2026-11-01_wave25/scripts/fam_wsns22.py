# fam_ccsns22.py <ov> <rows.txt> [tag]: naming wave 22, family wsns -- chunk-690 queue row 0x0099B510 ??1CWideString@@QAE@XZ, rows whose string class is declared
# inside a namespace (std::CWideString, NScreen*::CWideString, X_*::CWideString, ...: the mangled ??1CWideString@<ns>@@QAE@XZ): the class block is
# hoisted to global scope (the namespaces closed and reopened around it, as naming wave 10 did), struct -> `class CWideString { public:`.
# Kept only when the object compiles and every symbol it gains is an Ego_r / FableWin or bound CWideString spelling (or the class rename of a
# removed callee that takes the string), and every symbol it loses names the namespaced class.
import sys, os, re
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import famlib as F, inputs
sys.path.insert(0, os.path.join(os.path.dirname(os.path.abspath(__file__)), 'hand22'))
import he
ov, rf = sys.argv[1], sys.argv[2]; tag = sys.argv[3] if len(sys.argv) > 3 else 'wsns'
BT, BS = inputs.bindings(); NM = inputs.names(); PDB = NM['Ego_r.pdb'] | NM['FableWin.pdb']
GOOD = set(s for s in PDB | set(BS) if re.search(r'(?:@|\?\?[0-9]|\?\?_[A-Z])CWideString@@', s) and (s in PDB or s in BS))
ROWS = {}
for l in open(rf):
    q = l.split()
    if q and not q[0].startswith('#'): ROWS[q[0]] = q[1]   # the namespace path as mangled, e.g. NScreen005fc2d0 or FableVector_006fc130
def nsyms(x, ns):
    return ('CWideString@%s@@' % ns) in x or ('@%s@@' % ns) in x and 'CWideString' in x
def renamed(x, rem, ns):
    if 'CWideString@@' not in x: return False
    # a callee that takes / returns the string (the namespaced class may be a back-reference in the old spelling): same qualified name
    q = re.match(r'^(\?[^?@][^@]*@(?:[^@?$]+@)*@)', x)
    if q and any(y.startswith(q.group(1)) and 'CWideString' in y for y in rem): return True
    h = re.match(r'^(\?\?[0-9_][A-Z]?\?\$\w+@|\?\w+@[\w@]*?@\?\$\w+@)', x)   # a template instantiation over the string (pair<CWideString, T> ...)
    if h and any(y.startswith(h.group(1)) and 'CWideString' in y for y in rem): return True
    h = re.match(r'^(\?\?[0-9A-Z_]\w+@)', x)   # an operator / ctor / dtor of a sibling class that takes the string
    if h and any(y.startswith(h.group(1)) and 'CWideString' in y for y in rem): return True
    for y in rem:
        if y.replace('CWideString@%s@@' % ns, 'CWideString@@').replace('UCWideString@', 'VCWideString@') == x.replace('UCWideString@', 'VCWideString@'): return True
        if re.sub(r'(?:U|V)CWideString@[\w@]*?' + ns + r'@@', 'VCWideString@@', y) == x: return True
    return False
def transform(t, a):
    m = re.search(r'(?:struct|class)\s+CWideString\s*(?::[^{;]*)?\{', t)
    if not m: return None, 'no CWideString block'
    ch = he.ns_chain(t, m.start())
    if not ch: return None, 'not in a namespace'
    if None in ch: return None, 'nested in a class'
    if re.search(r'\w+::CWideString\b', t): return None, 'qualified uses'
    s = m.start(); e = he.block_end(t, m.start()); body = t[s:e]
    body = re.sub(r'^struct(\s+CWideString\s*)\{(?=\s*public:)', r'class\1{', body)
    body = re.sub(r'^struct(\s+CWideString\s*)\{', r'class\1{ public:', body)
    close = '}' * len(ch); reopen = ' '.join('namespace %s {' % n for n in ch)
    t2 = t[:s] + close + ' // (hoisted to global scope: naming wave 22, the PDB CWideString)\n' + body + '\n' + reopen + t[e:]
    # forward declarations of the string inside the namespace would shadow the global class
    return t2, '%s::CWideString -> ::CWideString' % '::'.join(ch)
def check(S0, S1, a):
    ns = ROWS[a]; rem0 = S0 - S1
    rem = [s for s in rem0 if not F.hashy(s) and not ('CWideString' in s and ns in s) and not s.startswith('??_C@')]
    add = [s for s in S1 - S0 if not F.hashy(s) and s not in GOOD and not s.startswith('??_C@') and not renamed(s, rem0, ns)]
    if rem or add: return 'diff: -%s +%s' % (rem[:4], add[:4])
    return None
F.run(ov, list(ROWS), transform, check, tag)
