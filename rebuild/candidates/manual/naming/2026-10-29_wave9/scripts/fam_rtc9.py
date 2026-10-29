# fam_rtc9.py <ov> <rows.txt>: CThing::RemoveTC takes ETCInterfaceType (Ego_r.pdb + FableWin.pdb
# ?RemoveTC@CThing@@QAEXW4ETCInterfaceType@@@Z, 0x004C9840). Rows declared `void RemoveTC(long)` (?RemoveTC@CThing@@QAEXJ@Z).
# Transform: the parameter type becomes ETCInterfaceType (`enum ETCInterfaceType { };` ahead of the class when the row has
# none); calls the compiler then rejects get `(ETCInterfaceType)` on their argument. An enum argument is passed like long.
import sys, os, re
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import famlib9
def tf(t, a):
    m = re.search(r'\bvoid(\s+RemoveTC\s*\(\s*)long\b', t)
    if not m: return None, 'no RemoveTC(long) declaration'
    t = t[:m.start()] + 'void' + m.group(1) + 'ETCInterfaceType' + t[m.end():]
    cm = re.search(r'\b(class|struct)\s+CThing\b[^;{]*\{', t)
    if not cm: return None, 'no CThing class'
    if not re.search(r'\benum\s+ETCInterfaceType\b', t):
        ls = t.rfind('\n', 0, cm.start()) + 1; t = t[:ls] + 'enum ETCInterfaceType { };\n' + t[ls:]
    p = os.path.join(famlib9.W, 'pw', 'rtc9', a + '.cpp'); os.makedirs(os.path.dirname(p), exist_ok=True)
    for it in range(10):
        famlib9.wr(p, t); S, err = famlib9.objsyms(a, p, 'rtc9')
        if S is not None: return t, 'ok (%d casts)' % it
        lines = sorted({int(x) for e in err for x in re.findall(r'\((\d+)\) : error', e)})
        L = t.split('\n'); ch = False
        for ln in lines:
            s = L[ln - 1]
            n = re.sub(r'((?:\.|->)\s*RemoveTC\s*\()(?!\s*\(ETCInterfaceType\))([^()]*(?:\([^()]*\))?[^()]*)\)', r'\1(ETCInterfaceType)(\2))', s, count=1)
            if n != s: L[ln - 1] = n; ch = True
        if not ch: return None, 'compile line %s: %s' % (lines[:3], err[0][-140:] if err else '')
        t = '\n'.join(L)
    return None, 'cap'
def ck(S0, S1, a):
    if '?RemoveTC@CThing@@QAEXJ@Z' in S1: return 'old still emitted'
    if '?RemoveTC@CThing@@QAEXW4ETCInterfaceType@@@Z' not in S1: return 'final not emitted'
    return None
famlib9.run(sys.argv[1], [l.strip() for l in open(sys.argv[2]) if l.strip()], tf, ck, 'rtc9')
