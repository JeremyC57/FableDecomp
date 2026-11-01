# fam_bitset18.py <ov> <rows.txt> [tag]: naming wave 18, family bitset -- the rows that call ?test@?$bitset@$0BBC@@std@@QBE_NI@Z
# (0x00410DE0, std::bitset<274>::test const: the CThing interface-presence bitset) through a row-local stand-in class
# (CPresenceBits / CTCBitsRemaining / ... with a Test / test member). The stand-in class definition becomes the STLport 4.6
# <bitset> shape (9 words), the class is renamed std::bitset<274> and the call names test(). Name-only: the recheck proves the bytes.
import sys, re, os, json
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import famlib2 as F, inputs, und
from memlib import depth_at, top_start
ov, rf = sys.argv[1], sys.argv[2]; TAG = sys.argv[3] if len(sys.argv) > 3 else 'bitset'
T = '00410de0'; FIN = '?test@?$bitset@$0BBC@@std@@QBE_NI@Z'
BT, BS = inputs.bindings(); B = BT.get(T, set())
IXU = {}
for l in open(inputs.W + '/idx/full.jsonl'):
    r = json.loads(l); IXU[r['a']] = set(r.get('u', []))
MODEL = '''namespace std {                                                       // STLport 4.6 <bitset>: _Base_bitset<9> words + bitset<274>
template <unsigned int _Nb> class bitset { public:
    unsigned long _M_w[(_Nb + 31) / 32];
    bool test(unsigned int __pos) const;                            // 0x00410DE0 ?test@?$bitset@$0BBC@@std@@QBE_NI@Z
};
}
'''
NEW = 'std::bitset<274>'
def stand(a): return sorted(s for s in IXU.get(a, ()) if s in B and s != FIN)
def tr(t, a):
    st = stand(a)
    if not st: return None, 'no stand-in named'
    if len(st) != 1: return None, 'several stand-ins %s' % st
    m = re.match(r'^\?(\w+)@(\w+)@@[QIA]([AB])E', st[0])
    if not m: return None, 'unparsed %s' % st[0]
    N, S = m.group(1), m.group(2)
    c = F.code_only(t)
    if re.search(r'\bbitset\b', c): return None, 'bitset already named'
    d = F.class_defs(c, S)
    if len(d) != 1: return None, '%d definitions of %s' % (len(d), S)
    kw, s0, b, e = d[0]
    body = c[b + 1:e]
    # the stand-in holds the 9 words (36 bytes) and the member; nothing else
    mem = re.sub(r'\b(public|private|protected)\s*:', '', body)
    mem = [x.strip() for x in mem.split(';') if x.strip()]
    data = [x for x in mem if not re.search(r'\b' + N + r'\s*\(', x)]
    fn = [x for x in mem if re.search(r'\b' + N + r'\s*\(', x)]
    if len(fn) != 1: return None, '%d %s members' % (len(fn), N)
    if '{' in fn[0]: return None, '%s defined inline' % N
    ok = len(data) <= 1 and all(re.match(r'^(unsigned long|unsigned int|fable_u32|unsigned char|fable_u8|char)\s+\w+\s*\[\s*(9|36|0x24)\s*\]$', x) for x in data)
    if not ok: return None, 'stand-in layout %s' % (data[:2],)
    if re.search(r'\b' + S + r'\s*::\s*' + N + r'\s*\(', c): return None, '%s::%s defined out of line' % (S, N)
    if depth_at(c, s0) != 0: return None, 'stand-in nested'
    # the whole definition statement (to the ; after })
    e2 = e + 1
    while e2 < len(c) and c[e2] in ' \t': e2 += 1
    if c[e2:e2 + 1] != ';': return None, 'definition not followed by ;'
    ls = t.rfind('\n', 0, s0) + 1; le = t.find('\n', e2); le = len(t) if le < 0 else le
    head = t[ls:s0]
    if head.strip(): return None, 'definition shares its line'
    model = MODEL if data else MODEL.replace('    unsigned long _M_w[(_Nb + 31) / 32];\n', '    // the row lays the 9 words (36 bytes) out itself: the stand-in was an empty class\n')
    t = t[:ls] + model + t[le + 1:]
    # forward declarations of the stand-in go
    t = re.sub(r'(?m)^[ \t]*(class|struct)\s+' + S + r'\s*;[^\n]*\n', '', t)
    c = F.code_only(t)
    if re.search(r'\b(class|struct)\s+' + S + r'\b', c): return None, 'stand-in still declared'
    t = F.rename_code(t, S, NEW)
    # the call: .Test( / ->Test( -> test(
    c = F.code_only(t); out = []; last = 0; n = 0
    for cm in re.finditer(r'(?<=[.>])\s*' + N + r'(?=\s*\()', c):
        out.append(t[last:cm.start()]); out.append(cm.group(0).replace(N, 'test')); last = cm.end(); n += 1
    out.append(t[last:]); t = ''.join(out)
    if n == 0: return None, 'no call of %s' % N
    c = F.code_only(t)
    if re.search(r'(?<![\w$])' + N + r'\s*\(', c) and N != 'test': return None, 'other uses of %s' % N
    # comments naming the stand-in class
    t = re.sub(r'//[^\n]*', lambda mm: re.sub(r'\b' + S + r'\b', NEW, mm.group(0)), t)
    L = t.split('\n'); li = [i for i, x in enumerate(L) if x.startswith('// Leaf: ')]
    at = li[0] + 1 if li else (1 if L[0].startswith('//') else 0)
    L.insert(at, '// Naming wave 18: the interface-presence stand-in %s is std::bitset<274> (STLport 4.6); its %s is ?test@?$bitset@$0BBC@@std@@QBE_NI@Z (name-only).' % (S, N))
    return '\n'.join(L), '%s::%s -> %s::test x%d' % (S, N, NEW, n)
def ck(D0, U0, D1, U1, a):
    st = set(stand(a))
    if st & U1: return 'stand-in still named'
    if FIN not in U1: return 'final not named'
    rem = [s for s in U0 - U1 if s not in st and '?A0x' not in s]
    add = [s for s in U1 - U0 if s != FIN and '?A0x' not in s]
    nd = [s for s in D1 - D0 if not s.startswith(('??_C@', '__real@'))]
    if rem or add or nd: return 'diff -%s +%s d%s' % (rem[:3], add[:3], nd[:3])
    return None
if __name__ == '__main__':
    F.run(ov, [l.split()[0] for l in open(rf) if l.strip()], tr, ck, TAG)
