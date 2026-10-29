# fam_aia9.py <ov> <rows.txt>: CAIStateGroupBase protected AI actions returning EAIActionReturn (Ego_r.pdb + FableWin.pdb):
#   ?AIAction_DoNothing@CAIStateGroupBase@@IAE?AW4EAIActionReturn@@XZ 0x0097E200
#   ?AIAction_PerformActionAndWait@CAIStateGroupBase@@IAE?AW4EAIActionReturn@@ABVCCreatureActionBase@@_N@Z 0x0097DB70
# Rows declared them public returning long (QAEJXZ / QAEJPAVCCreatureActionBase@@_N@Z). Transform: return type
# EAIActionReturn (`enum EAIActionReturn { };` added ahead of the class when the row has none), the action parameter
# becomes `const CCreatureActionBase&` and each call passes the action object (`&x` -> `x`, `p` -> `*p`), the declaration
# moves under `protected:` (acclib9.respell). An enum return and a const& parameter generate the same code as long / a
# pointer. Kept only when the object names the PDB spellings and neither old one.
import sys, os, re
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import famlib9, acclib9, und
F_DN = '?AIAction_DoNothing@CAIStateGroupBase@@IAE?AW4EAIActionReturn@@XZ'
F_PW = '?AIAction_PerformActionAndWait@CAIStateGroupBase@@IAE?AW4EAIActionReturn@@ABVCCreatureActionBase@@_N@Z'
O_DN = '?AIAction_DoNothing@CAIStateGroupBase@@QAEJXZ'
O_PW = '?AIAction_PerformActionAndWait@CAIStateGroupBase@@QAEJPAVCCreatureActionBase@@_N@Z'
U = und.get([F_DN, F_PW])
def first_arg(t, i):
    d = 0
    for j in range(i, len(t)):
        c = t[j]
        if c in '([': d += 1
        elif c in ')]':
            if d == 0: return j
            d -= 1
        elif c == ',' and d == 0: return j
    return None
def tf(t, a):
    t0 = t; did = []
    if re.search(r'\blong\s+AIAction_DoNothing\s*\(\s*\)\s*;', t):
        t = re.sub(r'\blong(\s+AIAction_DoNothing\s*\(\s*\)\s*;)', r'EAIActionReturn\1', t, count=1); did.append('dn')
    m = re.search(r'\blong(\s+AIAction_PerformActionAndWait\s*\(\s*)(?:class\s+)?CCreatureActionBase\s*\*\s*(\w*)', t)
    if m:
        t = t[:m.start()] + 'EAIActionReturn' + m.group(1) + 'const CCreatureActionBase& ' + m.group(2) + t[m.end():]
        out = []; pos = 0
        for c in re.finditer(r'(?<![\w:])AIAction_PerformActionAndWait\s*\(', t):
            if t[max(0, c.start() - 40):c.start()].rstrip().endswith('EAIActionReturn'): continue
            ls = t.rfind('\n', 0, c.start()) + 1
            if '//' in t[ls:c.start()]: continue
            s = c.end(); e = first_arg(t, s)
            if e is None: continue
            arg = t[s:e]; x = arg.strip(); lead = arg[:len(arg) - len(arg.lstrip())]
            if x.startswith('&'): new = x[1:].lstrip()
            elif re.fullmatch(r'[\w.\->]+', x): new = '*' + x
            else: new = '*(' + x + ')'
            out.append(t[pos:s] + lead + new); pos = e
        t = ''.join(out) + t[pos:]; did.append('pw')
    if not did: return None, 'no long-returning declaration'
    cm = re.search(r'\b(class|struct)\s+CAIStateGroupBase\b[^;{]*\{', t)
    if not cm: return None, 'no CAIStateGroupBase class'
    if not re.search(r'\benum\s+EAIActionReturn\b', t):
        ls = t.rfind('\n', 0, cm.start()) + 1; t = t[:ls] + 'enum EAIActionReturn { };\n' + t[ls:]
    for k, f in (('dn', F_DN), ('pw', F_PW)):
        if k in did:
            t2, why = acclib9.respell(t, U[f].replace('protected:', 'public:', 1), U[f], 'CAIStateGroupBase', und.qname(U[f]).split('::')[-1])
            if t2 is None: return None, why
            t = t2
    return t, '+'.join(did)
def ck(S0, S1, a):
    for o, f in ((O_DN, F_DN), (O_PW, F_PW)):
        if o in S1: return 'old %s still emitted' % o[:30]
        if o in S0 and f not in S1: return 'final %s not emitted' % f[:30]
    return None
famlib9.run(sys.argv[1], [l.strip() for l in open(sys.argv[2]) if l.strip()], tf, ck, 'aia9')
