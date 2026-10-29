# fam_xfl9.py <ov> <rows.txt>: CPersistContext::Transfer<long> (Ego_r.pdb + FableWin.pdb ??$Transfer@J@CPersistContext@@QAEXPBDAAJABJ@Z,
# 0x00410BE0; Ego_r has no non-template Transfer(const char*, long&, const long&)). Rows declared the non-template overload
# (?Transfer@CPersistContext@@QAEXPBDAAJABJ@Z). Transform: that declaration becomes the member template
# `template <class T> void Transfer(const char* name, T& value, const T& defaultValue);` (or is dropped when the class
# already declares it); calls whose default argument does not deduce long (literal 0, enum) get an explicit `<long>`
# (compile-driven: only on lines the compiler rejects). Kept only when the object names the template and not the overload.
import sys, os, re
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import famlib9
DECL = re.compile(r'(?m)^([ \t]*)void\s+Transfer\s*\(\s*const\s+char\s*\*\s*(\w*)\s*,\s*long\s*&\s*(\w*)\s*,\s*const\s+long\s*&\s*(\w*)\s*\)\s*;[^\n]*')
TPL = re.compile(r'template\s*<\s*(class|typename)\s+\w+\s*>\s*void\s+Transfer\s*\(\s*const\s+char\s*\*\s*\w*\s*,\s*\w+\s*&\s*\w*\s*,\s*const\s+\w+\s*&')
def tf(t, a):
    ms = list(DECL.finditer(t))
    if len(ms) != 1: return None, 'long Transfer declared %d times' % len(ms)
    m = ms[0]
    rep = '' if TPL.search(t) else m.group(1) + 'template <class T> void Transfer(const char* name, T& value, const T& defaultValue);   // 0x00410BE0 <long>'
    t = t[:m.start()] + rep + t[m.end():]
    p = famlib9.os.path.join(famlib9.W, 'pw', 'xfl9', a + '.cpp'); os.makedirs(os.path.dirname(p), exist_ok=True)
    for it in range(12):
        famlib9.wr(p, t); S, err = famlib9.objsyms(a, p, 'xfl9')
        if S is not None: return t, 'ok (%d explicit)' % it
        lines = sorted({int(x) for e in err for x in re.findall(r'\((\d+)\) : error', e)})
        if not lines: return None, 'compile: %s' % err[:1]
        L = t.split('\n'); ch = False
        for ln in lines:
            s = L[ln - 1]
            n = re.sub(r'(\.|->)Transfer\s*\(', r'\1Transfer<long>(', s)
            if n != s: L[ln - 1] = n; ch = True
        if not ch: return None, 'compile line %s: %s' % (lines[:3], err[0][-120:])
        t = '\n'.join(L)
    return None, 'cap'
def ck(S0, S1, a):
    if '?Transfer@CPersistContext@@QAEXPBDAAJABJ@Z' in S1: return 'overload still emitted'
    if '?Transfer@CPersistContext@@QAEXPBDAAJABJ@Z' in S0 and '??$Transfer@J@CPersistContext@@QAEXPBDAAJABJ@Z' not in S1: return 'template not emitted'
    return None
famlib9.run(sys.argv[1], [l.strip() for l in open(sys.argv[2]) if l.strip()], tf, ck, 'xfl9')
