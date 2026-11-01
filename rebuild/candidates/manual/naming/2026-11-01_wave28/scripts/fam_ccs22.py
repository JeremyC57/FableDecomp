# fam_ccs22.py <ov> <rows.txt> [tag]: naming wave 22, family ccs -- the mega family ~CCharString (0x0099EAE0 ??1CCharString@@QAE@XZ): rows whose
# string member / base is the CCharString under an invented class name (CMember_0099eae0, CBase_0099eae0, CCharString_Retail) are respelled
# name-only: the stand-in class -> `class CCharString { public:` (Ego_r.pdb class key V). Rows file: "<row> <StandIn>". Kept only when the
# object compiles and every symbol it gains is an Ego_r / FableWin or bound CCharString spelling (or the class rename of a removed callee that
# takes the string / an inline constructor), and every symbol it loses names the stand-in.
import sys, os, re
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import famlib as F, inputs
ov, rf = sys.argv[1], sys.argv[2]; tag = sys.argv[3] if len(sys.argv) > 3 else 'ccs'
BT, BS = inputs.bindings(); NM = inputs.names(); PDB = NM['Ego_r.pdb'] | NM['FableWin.pdb']
GOOD = set(s for s in PDB | set(BS) if re.search(r'(?:@|\?\?[0-9]|\?\?_[A-Z])CCharString@@', s) and (s in PDB or BS[s][0] >= '0099e000' and BS[s][0] < '009a0000'))
ROWS = {}
for l in open(rf):
    q = l.split()
    if q and not q[0].startswith('#'): ROWS[q[0]] = q[1].split(',')
SIGIL = r'(?:\?\?[0-9]|\?\?_[A-Z]|[@?UV])'
def standin_sym(x, st): return any(re.search(SIGIL + re.escape(c) + r'@', x) for c in st)
def renamed(x, rem, st):
    if 'CCharString@@' not in x: return False
    for c in st:
        for k in ('U', 'V'):
            if x.replace('VCCharString@@', k + c + '@@') in rem: return True
        m = re.match(r'^\?\?0CCharString@@(.*)$', x)
        if m and ('??0%s@@%s' % (c, m.group(1))) in rem: return True
    return False
def transform(t, a):
    t2 = t
    for c in ROWS[a]:
        if re.search(r'\b(?:struct|class)\s+CCharString\b', t2): return None, 'CCharString already declared'
        t2 = re.sub(r'\b' + c + r'\b', 'CCharString', t2)
        t2 = re.sub(r'\bstruct(\s+CCharString\s*)\{(?=\s*public:)', r'class\1{', t2)
        t2 = re.sub(r'\bstruct(\s+CCharString\s*)\{', r'class\1{ public:', t2)
        t2 = re.sub(r'\bstruct(\s+CCharString\s*:)', r'class\1 public', t2) if re.search(r'\bstruct\s+CCharString\s*:', t2) else t2
        t2 = re.sub(r'\bstruct(\s+CCharString\b)(?!\s*[{:])', r'class\1', t2)
    if t2 != t and 'naming wave 22 (ccs)' not in t2:
        m = re.search(r'^// Leaf: [^\n]*\n', t2, re.M); i = m.end() if m else t2.index('\n') + 1
        t2 = t2[:i] + '// Naming (naming wave 22, ccs): the string type is the PDB CCharString (was %s); name-only.\n' % ' / '.join(ROWS[a]) + t2[i:]
    return t2, ','.join(ROWS[a]) + ' -> CCharString'
def check(S0, S1, a):
    st = ROWS[a]; rem0 = S0 - S1
    rem = [s for s in rem0 if not F.hashy(s) and not standin_sym(s, st) and not s.startswith('??_C@')]
    add = [s for s in S1 - S0 if not F.hashy(s) and s not in GOOD and not s.startswith('??_C@') and not renamed(s, rem0, st)]
    if rem or add: return 'diff: -%s +%s' % (rem[:4], add[:4])
    return None
F.run(ov, list(ROWS), transform, check, tag)
