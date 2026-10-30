# fam_gmp2.py <ov> <rows.txt>: the rows fam_gmp left for 0x00449970 (?GetMainPlayer@CPlayerManager@@QAEAAVCPlayer@@XZ, Ego_r
# 0x004450F0, FableWin 0x01A596C0) whose stand-in is NOT a CPlayerManager_Retail class:
#  F  a free __fastcall helper `RET __fastcall N(T* p);` -> class CPlayerManager { public: CPlayer& GetMainPlayer(); } and each
#     call `N(arg)` -> `(&((CPlayerManager*)(arg))->GetMainPlayer())` (ecx = the manager either way: same code);
#  M  a member `RET N();` of a row-local class X -> the declaration goes and each call `chain->N()` becomes
#     `((CPlayerManager*)chain)->GetMainPlayer()` (same ecx); X == CPlayerManager: N is renamed to the real getter (CPlayer&).
#  (an inline forwarder was tried first: VC7.1 emitted it out of line as a COMDAT, so the row still called the stand-in)
# CPlayer becomes a class (PDB key V). Rows that include engine/CPlayerManager.h (generated header, no methods) are skipped.
import sys, re, os, json
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import famlib as F, inputs, und
W = F.W
FIN = '?GetMainPlayer@CPlayerManager@@QAEAAVCPlayer@@XZ'
BT, BS = inputs.bindings(); B70 = BT['00449970']
# the row's bound 0x00449970 spelling(s), snapshotted from the post-wave-10 index (runs/gmp2_spell.tsv)
SP = {l.split('\t')[0]: set(l.rstrip('\n').split('\t')[1].split()) for l in open(os.path.join(W, 'runs/gmp2_spell.tsv'))}
def bal(t, i):            # t[i] == '(' -> index after the matching ')'
    d = 0
    for j in range(i, len(t)):
        d += {'(': 1, ')': -1}.get(t[j], 0)
        if d == 0: return j + 1
    return -1
def body(t, name):
    m = re.search(r'\b(?:struct|class)\s+' + re.escape(name) + r'\b[^;{()]*\{', t)
    if not m: return None
    d = 0
    for j in range(m.end() - 1, len(t)):
        d += {'{': 1, '}': -1}.get(t[j], 0)
        if d == 0: return m, j
    return None
def fix_player(t, at):
    if re.search(r'#\s*define\s+CPlayer\b', t): return None
    t = re.sub(r'\bstruct(\s+CPlayer\b)([^;{(]*)\{', r'class\1\2{ public:', t)
    t = re.sub(r'\bstruct(\s+CPlayer\s*);', r'class\1;', t)
    if not re.search(r'\bclass\s+CPlayer\b', t[:at]):
        t = t[:at] + 'class CPlayer;\n' + t[at:]
    return t
def ret_expr(ret, e):
    r = re.sub(r'\s+', '', ret)
    return ('&' + e) if r in ('CPlayer*', 'classCPlayer*') else '(%s)&%s' % (ret.strip(), e)
MGR = 'class CPlayerManager { public: CPlayer& GetMainPlayer(); }; /* 0x00449970 CPlayerManager::GetMainPlayer (Ego_r 0x004450F0) */\n'
def tr(t, a):
    if 'engine/CPlayerManager.h' in t: return None, 'includes the generated CPlayerManager header (no methods)'
    ss = set(SP.get(a, ())) & B70
    if len(ss) != 1: return None, 'bound spellings %d' % len(ss)
    s = ss.pop(); d = und.get([s])[s]
    code = re.sub(r'//[^\n]*', '', t)
    if s.startswith('@') or '__fastcall' in d:
        n = s[1:].split('@')[0] if s.startswith('@') else re.search(r'__fastcall (\w+)\(', d).group(1)
        dm = list(re.finditer(r'^([^\n;{}]*?)\b__fastcall\s+' + n + r'\s*\(([^()]*)\)\s*;[^\n]*\n', t, re.M))
        if len(dm) != 1: return None, 'free %s: %d declarations' % (n, len(dm))
        if re.search(r'\b' + n + r'\s*\([^()]*\)\s*\{', code): return None, 'free %s is defined here' % n
        if re.search(r'\bCPlayerManager\b', code): return None, 'file already names CPlayerManager'
        ret = re.sub(r'\bextern\b(\s*"C")?\s*', '', dm[0].group(1)).strip()
        if not ret: return None, 'no return type'
        st = dm[0].start(); t = t[:st] + MGR + t[dm[0].end():]
        out = []; i = 0; n_calls = 0
        for m in re.finditer(r'\b' + n + r'\s*\(', t):
            if m.start() < i: continue
            j = bal(t, m.end() - 1)
            arg = t[m.end():j - 1].strip()
            if not arg or ',' in arg: return None, 'call with %r' % arg
            e = '((CPlayerManager*)%s)->GetMainPlayer()' % (arg if re.match(r'^[\w>.\-]+$', arg) and '->' in arg or re.match(r'^\w+$', arg) else '(' + arg + ')')
            out.append(t[i:m.start()] + '(' + ret_expr(ret, e) + ')'); i = j; n_calls += 1
        out.append(t[i:]); t = ''.join(out)
        if n_calls == 0: return None, 'no calls of %s' % n
        t = fix_player(t, t.index(MGR))
        if t is None: return None, '#define CPlayer'
        return t, 'free %s -> CPlayerManager::GetMainPlayer (%d calls)' % (n, n_calls)
    m = re.match(r'public: (.*?) __thiscall (\w+)::(\w+)\(void\)$', d)
    if not m: return None, 'member shape %s' % d
    ret, x, n = m.groups()
    ret = ret.replace('class ', '').replace('struct ', '').replace(' *', '*')
    b = body(t, x)
    if not b: return None, 'no body of %s' % x
    bm, bj = b; bt = t[bm.end():bj]
    dm = list(re.finditer(r'(?:(?<=[;{}\n])|^)([^\n;{}]*?)\b' + n + r'\s*\(\s*(?:void)?\s*\)\s*;', bt))
    if len(dm) != 1: return None, '%s::%s: %d declarations' % (x, n, len(dm))
    lab, rtxt = re.match(r'^(\s*(?:(?:public|private|protected)\s*:\s*)?)(.*)$', dm[0].group(1), re.S).groups()
    rtxt = rtxt.strip()
    if not rtxt: return None, 'no return text'
    if re.search(r'\b' + n + r'\s*\([^()]*\)\s*(const\s*)?\{', code): return None, '%s is defined here' % n
    if x == 'CPlayerManager':
        if re.search(r'\bGetMainPlayer\b', code): return None, 'already has GetMainPlayer'
        if re.sub(r'\s+', '', rtxt) != 'CPlayer*': return None, 'CPlayerManager::%s returns %s' % (n, rtxt)
        # the local class is the manager: the member is the real getter (CPlayer&); calls follow (fam_gmp rules)
        nb = bt[:dm[0].start()] + lab + 'CPlayer& GetMainPlayer();   /* 0x00449970 */' + bt[dm[0].end():]
        t = t[:bm.end()] + nb + t[bj:]
        nc = len(re.findall(r'\b' + n + r'\s*\(\s*\)', t))
        if nc == 0: return None, 'no calls of %s' % n
        t = re.sub(r'\b' + n + r'\s*\(\s*\)', 'GetMainPlayer()', t)
        t = re.sub(r'GetMainPlayer\(\)\s*->', 'GetMainPlayer().', t)
        def amp(mm):
            pre = t[max(0, mm.start() - 2):mm.start()]
            return mm.group(0) if pre.endswith('&') else '(&' + mm.group(0) + ')'
        t = re.sub(r'((?:[A-Za-z_]\w*(?:\(\))?\s*(?:->|\.)\s*)+)GetMainPlayer\(\)(?!\s*\.)', amp, t)
        t = re.sub(r'\bstruct(\s+CPlayerManager\s*)\{', r'class\1{ public:', t)
        t = re.sub(r'\bstruct(\s+CPlayerManager\s*);', r'class\1;', t)
        t = fix_player(t, bm.start())
        if t is None: return None, '#define CPlayer'
        return t, 'CPlayerManager::%s -> GetMainPlayer (%d calls)' % (n, nc)
    if re.search(r'\b(?:struct|class)\s+CPlayerManager\b[^;]*\{', code): return None, 'row defines CPlayerManager elsewhere'
    # the member of the row-local class X goes; each call `chain->N()` calls CPlayerManager::GetMainPlayer on the same object
    calls = list(re.finditer(r'(?<![\w>.])((?:[A-Za-z_]\w*(?:\(\))?\s*(?:->|\.)\s*)+)' + n + r'\s*\(\s*\)(\s*->)?', t))
    if not calls or len(calls) != len(re.findall(r'\b' + n + r'\s*\(', code)) - 1: return None, '%s: call shapes (%d simple)' % (n, len(calls))
    cp = re.sub(r'\s+', '', rtxt) in ('CPlayer*',)
    out = []; i = 0
    for c in calls:
        ch = re.sub(r'\s+', '', c.group(1))
        ex = ch[:-2] if ch.endswith('->') else '&' + ch[:-1]
        g = '((CPlayerManager*)%s)->GetMainPlayer()' % ex
        if c.group(2):
            r = (g + '.') if cp else '((%s)&%s)->' % (rtxt, g)
        else:
            r = ('(&%s)' % g) if cp else '((%s)&%s)' % (rtxt, g)
        out.append(t[i:c.start()] + r); i = c.end()
    out.append(t[i:]); t = ''.join(out)
    b = body(t, x); bm, bj = b; bt = t[bm.end():bj]
    dm = list(re.finditer(r'(?:(?<=[;{}\n])|^)([^\n;{}]*?)\b' + n + r'\s*\(\s*(?:void)?\s*\)\s*;', bt))
    nb = bt[:dm[0].start()] + (lab if lab.strip() else '') + bt[dm[0].end():]
    t = t[:bm.end()] + nb + t[bj:]
    t = t[:bm.start()] + MGR + t[bm.start():]
    t = fix_player(t, t.index(MGR))
    if t is None: return None, '#define CPlayer'
    return t, '%s::%s calls -> CPlayerManager::GetMainPlayer (%d)' % (x, n, len(calls))
def ck(S0, S1, a):
    if not (S0 & B70): return 'emits no 0x00449970 spelling before'
    ren = lambda s: s.replace('UCPlayer@@', 'VCPlayer@@').replace('UCPlayerManager@@', 'VCPlayerManager@@')
    R0 = set(ren(s) for s in S0)
    left = (S1 & B70) - {FIN}
    if left: return 'still names %s' % sorted(left)[:2]
    return F.diffcheck(S0, S1, lambda s: s in B70 or ren(s) in S1, lambda s: s == FIN or (s in R0 and s not in B70), [FIN])
if __name__ == '__main__': F.run(sys.argv[1], F.rows_from(sys.argv[2]), tr, ck, 'gmp2')
