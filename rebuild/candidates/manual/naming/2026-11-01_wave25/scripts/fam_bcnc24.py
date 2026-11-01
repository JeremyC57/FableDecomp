# fam_bcnc24.py <overlay> <rows.txt>: naming wave 24, family bcnc -- ~CBaseClassNonCopyable 0x0099A300 is protected virtual in Ego_r.pdb /
# FableWin.pdb (??1CBaseClassNonCopyable@@MAE@XZ, engine/CBaseClassNonCopyable.h; DECISIONS_W23_RESOLVED item 6). Rows whose local
# CBaseClassNonCopyable declares (or defines) the virtual destructor public (UAE) get it under `protected:`; kept only when the object then
# names / defines MAE and no longer UAE, with every other symbol unchanged.
import sys, os, re
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import famlib2 as F
ov, rf = sys.argv[1:3]
rows = [l.split()[0] for l in open(rf) if l.strip()]
UAE, MAE = '??1CBaseClassNonCopyable@@UAE@XZ', '??1CBaseClassNonCopyable@@MAE@XZ'
NOTE = '// Naming (naming wave 24): ~CBaseClassNonCopyable 0x0099A300 is protected virtual (Ego_r.pdb / FableWin.pdb ??1CBaseClassNonCopyable@@MAE@XZ, engine/CBaseClassNonCopyable.h).'
def transform(t, a):
    c = F.code_only(t)
    defs = F.class_defs(c, 'CBaseClassNonCopyable')
    if len(defs) != 1: return None, 'defs%d' % len(defs)
    kw, s, b, e = defs[0]
    body = t[b:e]; cb = c[b:e]
    m = re.search(r'\n([ \t]*)(virtual\s+~CBaseClassNonCopyable\s*\([^\n]*)', cb)
    if not m:
        # one-line class bodies (`{virtual ~CBaseClassNonCopyable();...}`) and classes with no declared destructor (the implicit one is
        # public virtual through CBaseClass): declare it protected in place, inline-empty when it was implicit
        m1 = re.search(r'virtual\s+~CBaseClassNonCopyable\s*\(\s*\)\s*(;|\{\s*\})', cb)
        if m1:
            body2 = body[:m1.start()] + 'protected: ' + body[m1.start():m1.end()] + ' public:' + body[m1.end():]
            why = 'protected (one-line)'
        elif '~CBaseClassNonCopyable' not in cb:
            body2 = body[:1] + ' protected: virtual ~CBaseClassNonCopyable() {} public:' + body[1:]
            why = 'protected (was implicit)'
        else:
            return None, 'nodecl'
        t2 = t[:b] + body2 + t[e:]
        mm = re.search(r'^// Leaf: [^\n]*\n', t2, re.M); i = mm.end() if mm else t2.index('\n') + 1
        return t2[:i] + NOTE + '\n' + t2[i:], why
    if re.search(r'\bprotected\s*:', cb[:m.start()]) and not re.search(r'\bpublic\s*:', cb[cb.rfind('protected', 0, m.start()):m.start()]):
        return None, 'already protected'
    ind = m.group(1)
    line_end = body.find('\n', m.end())
    if line_end < 0: line_end = len(body)
    line = body[m.start() + 1:line_end]
    new = '\n' + ind + 'protected:\n' + line + '\n' + ind + 'public:'
    body2 = body[:m.start()] + new + body[line_end:]
    t2 = t[:b] + body2 + t[e:]
    mm = re.search(r'^// Leaf: [^\n]*\n', t2, re.M); i = mm.end() if mm else t2.index('\n') + 1
    return t2[:i] + NOTE + '\n' + t2[i:], 'protected'
def check(D0, U0, D1, U1, a):
    S0 = (D0 | U0); S1 = (D1 | U1)
    if UAE in S1: return 'UAE still named'
    if MAE not in S1: return 'MAE not named'
    G0, G1 = '??_GCBaseClassNonCopyable@@UAEPAXI@Z', '??_GCBaseClassNonCopyable@@MAEPAXI@Z'   # FableWin ??_G...MAE (protected virtual)
    if (S0 - {UAE, G0}) != (S1 - {MAE, G1}) or (G0 in S0) != (G1 in S1): return 'other symbols changed: +%s -%s' % (sorted(S1 - S0 - {MAE})[:3], sorted(S0 - S1 - {UAE})[:3])
    return None
F.run(ov, rows, transform, check, tag='bcnc24')
