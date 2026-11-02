# fam_mem.py <family> <ov> <rows.txt>: a bound stand-in member (any class / name, possibly in a row namespace) that is really
# the PDB member FIN of class CLS: the stand-in declaration is removed; CLS is modelled at global scope (a minimal
# `class CLS { public: DECL };` inserted before the top-level construct that held the stand-in, or DECL added to a global
# CLS the row already defines) and every call `E->N(a, b)` / `E.N(a, b)` / `N(a, b)` becomes `((CLS*)(E))->M(a, ARGF(b))`
# (no pointer adjustment: CLS is unrelated or single-inheritance at offset 0; the recheck proves the bytes).
#   FAM[f] = (target, final, CLS, M, DECL, virtual?, argfmt per parameter index ({} = expression))
import sys, re, os
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import famlib2 as F, inputs
FAM = {
 'sai': ('00833a10', '?SuspendAI@CThingAICreature@@QAEX_NPAX@Z', 'CThingAICreature', 'SuspendAI',
         'void SuspendAI(bool suspend, void* suspender);   // 0x00833A10 (Ego_r.pdb CThingAICreature::SuspendAI)', False, {1: 'ptr'}),
}
fam = sys.argv[1]; T, FIN, CLS, M, DECL, VIRT, AF = FAM[fam]
BT, BS = inputs.bindings(); B = BT[T]
E, IX = inputs.index()
MEM = re.compile(r'^\?(\w+)@((?:\w+@)+)@(?:Q|I|A|U|M|E)(?:A|B)E')
def stand(a): return sorted(s for s in IX.get(a, {}).get('u', []) if s in B and s != FIN)
def back_expr(c, i):
    # c[i] is the char right before '->' or '.': scan back over a postfix expression
    j = i
    while j >= 0:
        ch = c[j]
        if ch in ')]':
            d = 0; op = '(' if ch == ')' else '['
            while j >= 0:
                if c[j] in ')]': d += 1
                elif c[j] in '([': d -= 1
                if d == 0: break
                j -= 1
            j -= 1
            # a cast / call: identifier before '(' belongs to it
            while j >= 0 and (c[j].isalnum() or c[j] == '_'): j -= 1
        elif ch.isalnum() or ch == '_':
            while j >= 0 and (c[j].isalnum() or c[j] == '_'): j -= 1
        else: break
        k = j
        while k >= 0 and c[k] in ' \t': k -= 1
        if k >= 1 and c[k - 1:k + 1] == '->': j = k - 2; continue
        if k >= 0 and c[k] == '.': j = k - 1; continue
        if k >= 1 and c[k - 1:k + 1] == '::': j = k - 2; continue
        break
    return j + 1
def split_args(s):
    out = []; d = 0; cur = ''
    for ch in s:
        if ch in '([': d += 1
        elif ch in ')]': d -= 1
        if ch == ',' and d == 0: out.append(cur.strip()); cur = ''
        else: cur += ch
    if cur.strip(): out.append(cur.strip())
    return out
def depth_at(c, pos):
    d = 0
    for ch in c[:pos]:
        if ch == '{': d += 1
        elif ch == '}': d -= 1
    return d
def top_start(c, pos):
    # start of the line of the top-level (depth 0) construct containing pos
    d = 0; last0 = 0
    for k, ch in enumerate(c[:pos]):
        if ch == '{':
            if d == 0: pass
            d += 1
        elif ch == '}': d -= 1
        elif ch == ';' and d == 0: last0 = k + 1
        if ch == '}' and d == 0: last0 = k + 1
    p = c.find('\n', last0)
    while p >= 0 and p < pos and not c[p + 1:c.find('\n', p + 1)].strip(): p = c.find('\n', p + 1)
    return (p + 1) if 0 <= p < pos else last0
def tr(t, a):
    st = stand(a)
    if not st: return None, 'no stand-in named'
    msgs = []; NS_ = []
    for s in st:
        m = MEM.match(s)
        if not m: return None, 'shape %s' % s
        N = m.group(1); scope = m.group(2).rstrip('@').split('@'); SC = scope[0]
        c = F.code_only(t)
        dm = [x for x in re.finditer(r'(?m)^[ \t]*(?:virtual\s+)?(?:__forceinline\s+|inline\s+)?[\w:<>*&\s]*?\b' + N + r'\s*\([^;{}()]*(?:\([^()]*\)[^;{}()]*)*\)\s*(?:const\s*)?;[^\n]*\n', c)]
        dm = [x for x in dm if depth_at(c, x.start()) > 0]
        if len(dm) != 1: return None, '%d declarations of %s' % (len(dm), N)
        if re.search(r'\b' + SC + r'\s*::\s*' + N + r'\s*\(', c): return None, '%s::%s defined here' % (SC, N)
        if re.search(r'(?<![\w.>:])' + N + r'\s*\([^;{}]*\)\s*(?:const\s*)?\{', c): return None, '%s defined inline' % N
        ins = top_start(c, dm[0].start())
        t = t[:dm[0].start()] + t[dm[0].end():]
        c = F.code_only(t)
        # calls
        out = []; last = 0; n = 0
        for cm in re.finditer(r'(?<![\w$])' + N + r'\s*\(', c):
            if cm.start() < last: continue
            j = cm.end() - 1; d = 0; k = j
            while k < len(c):
                if c[k] == '(': d += 1
                elif c[k] == ')':
                    d -= 1
                    if d == 0: break
                k += 1
            args = split_args(t[j + 1:k])
            pre = cm.start() - 1
            while pre >= 0 and c[pre] in ' \t': pre -= 1
            if c[pre - 1:pre + 1] == '->':
                es = back_expr(c, pre - 2); expr = t[es:pre - 1].strip(); obj = '(%s)' % expr if not re.match(r'^\w+$', expr) else expr; start = es
            elif c[pre] == '.':
                es = back_expr(c, pre - 1); expr = t[es:pre].strip(); obj = '&(%s)' % expr; start = es
            elif c[pre - 1:pre + 1] == '::': return None, 'qualified call of %s' % N
            else:
                obj = 'this'; start = cm.start()
            if len(args) != 2: return None, 'call with %d args' % len(args)
            a1 = args[1]
            if AF.get(1) == 'ptr' and a1 not in ('0', 'NULL', 'this'): a1 = '(void*)' + ('(%s)' % a1 if not re.match(r'^[\w]+$', a1) else a1)
            out.append(t[last:start]); out.append('((%s*)%s)->%s(%s, %s)' % (CLS, obj, M, args[0], a1)); last = k + 1; n += 1
        out.append(t[last:]); t = ''.join(out)
        if n == 0: return None, 'no calls of %s' % N
        NS_.append(N)
        c = F.code_only(t)
        dd = F.class_defs(c, CLS)
        if dd:
            if len(dd) != 1 or depth_at(c, dd[0][1]) != 0: return None, '%s defined in a nested scope' % CLS
            if not re.search(r'\b' + M + r'\s*\(', c[dd[0][2]:dd[0][3]]):
                b = dd[0][2] + 1
                t = t[:b] + '\npublic:\n    ' + DECL + '\n' + ('private:' if dd[0][0] == 'class' and not re.match(r'\s*public\s*:', c[b:b + 40]) else 'public:') + t[b:]
                msgs.append('%s->%s (decl added to %s)' % (N, M, CLS))
            else: msgs.append('%s->%s' % (N, M))
        else:
            if ins > len(t): ins = len(t)
            if not re.search(r'\bclass\s+' + CLS + r'\b', F.code_only(t[:ins])):
                t = t[:ins] + 'class %s { public:\n    %s\n};\n' % (CLS, DECL) + t[ins:]
            msgs.append('%s::%s -> %s::%s x%d' % (SC, N, CLS, M, n))
    for N in NS_:
        # comments: the stand-in's qualified name (and a distinctive bare name) follow the respell
        def fixc(mm):
            s2 = re.sub(r'\b(?:\w+::)+' + N + r'\b', CLS + '::' + M, mm.group(0))
            if len(N) >= 10: s2 = re.sub(r'(?<![\w:])' + N + r'\b', M, s2)
            return s2
        t = re.sub(r'//[^\n]*', fixc, t)
    return t, ' '.join(msgs)
def ck(D0, U0, D1, U1, a):
    st = set(stand(a))
    if st & U1: return 'stand-in still named'
    if FIN not in U1: return 'final not named'
    hz = lambda s: '?A0x' in s
    rem = [s for s in U0 - U1 if s not in st and not hz(s)]
    add = [s for s in U1 - U0 if s != FIN and not hz(s)]
    if rem or add: return 'diff -%s +%s' % (rem[:3], add[:3])
    return None
if __name__ == '__main__':
    F.run(sys.argv[2], [l.split()[0] for l in open(sys.argv[3]) if l.strip()], tr, ck, 'm' + fam)
