# fam_eps18.py <ov> <rows.txt> [tag]: naming wave 18, family eps -- the TUs that spell the shared const float 1e-4f at 0x0129BA3C
# as EPSILON now spell it DELTA_NOTIONAL_ZERO (ESTABLISHED_SPELLINGS 2026-11-01, chunk 680: raw Ego_r public + typed data record,
# Ego assert text "< DELTA_NOTIONAL_ZERO"). Name-only: `extern const float DELTA_NOTIONAL_ZERO;` (?DELTA_NOTIONAL_ZERO@@3MB).
import sys, re, os, json
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import famlib2 as F
ov, rf = sys.argv[1], sys.argv[2]; TAG = sys.argv[3] if len(sys.argv) > 3 else 'eps'
OLD, NEW = '?EPSILON@@3MB', '?DELTA_NOTIONAL_ZERO@@3MB'
def tr(t, a):
    c = F.code_only(t)
    if not re.search(r'(?<![\w$])EPSILON(?![\w$])', c): return None, 'EPSILON not named'
    if re.search(r'\bDELTA_NOTIONAL_ZERO\b', c): return None, 'DELTA_NOTIONAL_ZERO already named'
    ext = re.findall(r'(?m)^[ \t]*extern\s+const\s+float\s+EPSILON\s*;', c)
    if len(ext) != 1: return None, '%d extern declarations' % len(ext)
    if re.search(r'(?<![\w$])EPSILON\s*=', c): return None, 'EPSILON defined'
    n = len(re.findall(r'(?<![\w$])EPSILON(?![\w$])', c))
    t = F.rename_code(t, 'EPSILON', 'DELTA_NOTIONAL_ZERO')
    t = re.sub(r'//[^\n]*', lambda mm: re.sub(r'\bEPSILON\b', 'DELTA_NOTIONAL_ZERO', mm.group(0)), t)
    L = t.split('\n'); li = [i for i, x in enumerate(L) if x.startswith('// Leaf: ')]
    at = li[0] + 1 if li else (1 if L[0].startswith('//') else 0)
    L.insert(at, '// Naming wave 18: the shared const float 1e-4f at 0x0129BA3C is ?DELTA_NOTIONAL_ZERO@@3MB (raw Ego_r public + typed data record; was EPSILON).')
    return '\n'.join(L), 'EPSILON x%d' % n
def ck(D0, U0, D1, U1, a):
    if OLD in U1: return 'EPSILON still named'
    if NEW not in U1: return 'final not named'
    rem = [s for s in U0 - U1 if s != OLD]; add = [s for s in U1 - U0 if s != NEW]
    nd = [s for s in D1 - D0 if not s.startswith(('??_C@', '__real@'))]
    if rem or add or nd: return 'diff -%s +%s d%s' % (rem[:3], add[:3], nd[:3])
    return None
if __name__ == '__main__':
    F.run(ov, [l.split()[0] for l in open(rf) if l.strip()], tr, ck, TAG)
