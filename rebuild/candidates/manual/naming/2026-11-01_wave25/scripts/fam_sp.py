# fam_sp.py <ov> <rows.txt>: naming wave 16, family sp -- the CStringParser constructor 0x00404720 is
# ??0CStringParser@@QAE@ABVCCharString@@P6IX0@Z0J@Z (Ego_r: the error callback is `void (__fastcall*)(const CCharString&)`, c674);
# the landed rows declare it with `void (*)(CCharString*)` (or a typedef of it). The callback parameter of every CStringParser
# constructor declaration (and the own row's typedef) is respelled; callers pass 0 (one pushed word: name-only).
import sys, os, re
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import famlib as F
FIN = '??0CStringParser@@QAE@ABVCCharString@@P6IX0@Z0J@Z'
NEWT = 'void (__fastcall*%s)(const CCharString&)'
ov, rf = sys.argv[1:3]
def tr(t, a):
    c = t; n = 0
    # own row: the callback typedef
    m = re.search(r'typedef\s+void\s*\(\s*\*\s*(\w+)\s*\)\s*\(\s*CCharString\s*\*\s*\w*\s*\)\s*;', t)
    tdefs = {}
    out = []; last = 0
    for cm in re.finditer(r'\bCStringParser\s*\(\s*const\s+CCharString\s*&\s*\w*\s*,\s*', t):
        s = cm.end(); d = 0; k = s
        while k < len(t):
            ch = t[k]
            if ch == '(': d += 1
            elif ch == ')':
                if d == 0: break
                d -= 1
            elif ch == ',' and d == 0: break
            k += 1
        p = t[s:k].strip()
        mm = re.match(r'^void\s*\(\s*\*\s*(\w*)\s*\)\s*\(\s*C(?:CharString|StringParser)\s*\*\s*\w*\s*\)$', p)
        if mm: new = NEWT % ((' ' + mm.group(1)) if mm.group(1) else '')
        else:
            mt = re.match(r'^(\w+)\s+(\w+)$', p) or re.match(r'^(\w+)$', p)
            if not mt: continue
            ty = mt.group(1); nm = mt.group(2) if mt.lastindex and mt.lastindex >= 2 else ''
            if not re.search(r'typedef\s+void\s*\(\s*\*\s*' + ty + r'\s*\)\s*\(\s*CCharString\s*\*\s*\w*\s*\)\s*;', t): continue
            if m and m.group(1) == ty and a == '00404720':
                continue   # own row: the typedef itself is respelled below
            new = NEWT % ((' ' + nm) if nm else '')
        # only declarations / the definition head: the next token after the parameter list is `;`, `{` or `:` (not a call)
        out.append(t[last:s]); out.append(new); last = s + len(t[s:k]) - len(t[s:k].lstrip()) + len(p) if False else k; n += 1
    out.append(t[last:]); t2 = ''.join(out)
    if a == '00404720' and m:
        t2 = t2.replace(m.group(0), 'typedef void (__fastcall *%s)(const CCharString& message);' % m.group(1)); n += 1
        # the out-of-line Init it forwards to is Ego_r ?Init@CStringParser@@QAEXPBDP6IXABVCCharString@@@Z1J@Z (const char*; bound 0x009B9530)
        t2 = t2.replace('    void Init(char* text, StringParserErrorCallback errorCallback,', '    void Init(const char* text, StringParserErrorCallback errorCallback,')
        t2 = t2.replace('// 0x0099EFB0, CStringParser::Init(char*, callback, const CCharString&, long) 0x009B9530.',
                        '// 0x0099EFB0, CStringParser::Init(const char*, callback, const CCharString&, long) 0x009B9530.\n'
                        '// Naming wave 16 (c674): Ego_r ??0CStringParser@@QAE@ABVCCharString@@P6IX0@Z0J@Z -- the error callback is\n'
                        '//   `void (__fastcall*)(const CCharString&)` (and Init takes const char*, Ego_r / bound 0x009B9530); name-only.')
    if n == 0: return None, 'no callback parameter found'
    return t2, 'callback parameter respelled x%d' % n
def ck(S0, S1, a):
    old = [s for s in S0 - S1 if s.startswith('??0CStringParser@@') or (a == '00404720' and s.startswith('?Init@CStringParser@@'))]
    rem = [s for s in S0 - S1 if s not in old and '?A0x' not in s]
    add = [s for s in S1 - S0 if s != FIN and '?A0x' not in s and not (a == '00404720' and s == '?Init@CStringParser@@QAEXPBDP6IXABVCCharString@@@Z1J@Z')]
    if FIN not in S1: return 'final not named'
    if rem or add: return 'diff -%s +%s' % (rem[:3], add[:3])
    return None
F.run(ov, F.rows_from(rf), tr, ck, 'sp')
