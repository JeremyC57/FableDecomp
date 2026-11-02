# fam_gren18.py <ov> <rows.txt> <tag> <old_sym> <new_sym> <old_ident> <new_ident> [note]: naming wave 18 -- a global whose landed
# spelling differs from the established one only by its name (same type): the extern declaration and every use are renamed.
# Name-only; the check: the old symbol gone, the new one named, nothing else differs; the recheck proves the bytes.
import sys, re, os
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import famlib2 as F
ov, rf, TAG, OLD, NEW, OI, NI = sys.argv[1:8]; NOTE = sys.argv[8] if len(sys.argv) > 8 else ''
def tr(t, a):
    c = F.code_only(t)
    if not re.search(r'(?<![\w$])' + OI + r'(?![\w$])', c): return None, '%s not named' % OI
    if re.search(r'(?<![\w$])' + NI + r'(?![\w$])', c): return None, '%s already named' % NI
    ext = re.findall(r'(?m)^[ \t]*extern\s+[^;\n]*?(?<![\w$])' + OI + r'\s*(?:\[[^\]]*\])?\s*;', c)
    if len(ext) != 1: return None, '%d extern declarations' % len(ext)
    if re.search(r'(?m)^[ \t]*(?!extern\b)[\w:<>]+[\s*&]+' + OI + r'\s*(?:\[[^\]]*\])?\s*(=[^=]|;)', c): return None, '%s defined' % OI
    n = len(re.findall(r'(?<![\w$])' + OI + r'(?![\w$])', c))
    t = F.rename_code(t, OI, NI)
    t = re.sub(r'//[^\n]*', lambda mm: re.sub(r'\b' + OI + r'\b', NI, mm.group(0)), t)
    L = t.split('\n'); li = [i for i, x in enumerate(L) if x.startswith('// Leaf: ')]
    at = li[0] + 1 if li else (1 if L[0].startswith('//') else 0)
    L.insert(at, '// Naming wave 18: %s is %s (%s)%s.' % (OI, NI, NEW, ('; ' + NOTE) if NOTE else ''))
    return '\n'.join(L), '%s x%d' % (OI, n)
def ck(D0, U0, D1, U1, a):
    if OLD in U1: return 'old still named'
    if NEW not in U1: return 'final not named'
    rem = [s for s in U0 - U1 if s != OLD]; add = [s for s in U1 - U0 if s != NEW]
    nd = [s for s in D1 - D0 if not s.startswith(('??_C@', '__real@'))]
    if rem or add or nd: return 'diff -%s +%s d%s' % (rem[:3], add[:3], nd[:3])
    return None
if __name__ == '__main__':
    F.run(ov, [l.split()[0] for l in open(rf) if l.strip()], tr, ck, TAG)
