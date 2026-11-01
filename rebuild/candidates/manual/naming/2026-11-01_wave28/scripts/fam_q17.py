# fam_q.py <ov> <target> <final> <rows.txt> [tag]: generic caller respell of one queue target. Each bound stand-in spelling a
# row names (u) is a member declaration (any class, any name) or a one-argument free __fastcall stand-in; the PDB member FIN
# (demangled with llvm-undname) replaces it:
#  in place  -- the stand-in is FIN's own class and name (only access / const / return / parameter spelling differ): the
#               declaration is rewritten to FIN's signature under FIN's access;
#  moved     -- otherwise the declaration is removed and FIN's class is modelled at global scope (or in FIN's namespace) with a
#               minimal `class CLS { public: DECL };` (or DECL added to a global CLS the row defines); calls name CLS::M on
#               `((CLS*)object)` (static: `CLS::M(...)`).
# Call arguments follow FIN's parameter types (pointer <-> reference by * / &, other pointers by a C cast); a pointer result
# whose type changed is cast back to the row's old type at the call. Comments that name the stand-in follow. check: the
# [wave 17] derived mode (non-public FIN): when the stand-in's class S is the row's own class and derives from FIN's class CLS at
#   offset 0 (ghidra_out/rtti_bases.tsv), the declaration moves into CLS (defined in the row, or modelled as S's new base: S's first
#   virtual declaration copied so the vptr / slot order stay, FIN under its PDB access) and the calls keep their plain form (the
#   access is the inheritance retail has).
# stand-ins are gone, FIN is named, nothing else changes; the recheck proves the bytes.
import sys, re, os, json
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import famlib2 as F, inputs, und
from memlib import back_expr, split_args, depth_at, top_start
ov, T, FIN, rf = sys.argv[1], inputs.na(sys.argv[2]), sys.argv[3], sys.argv[4]; TAG = sys.argv[5] if len(sys.argv) > 5 else 'q'
BT, BS = inputs.bindings(); B = BT.get(T, set())
IXU = {}
for l in open(inputs.W + '/idx/full.jsonl'):
    r = json.loads(l); IXU[r['a']] = set(r.get('u', []))
RB = {}
for _l in open('/home/user/FableDecomp/ghidra_out/rtti_bases.tsv'):
    _q = _l.rstrip('\n').split('\t')
    if len(_q) >= 5: RB[_q[1]] = set(_q[4].split(';'))
CATN = {}
for _l in open('/home/user/FableDecomp/rebuild/oracles/auto-re-candidates.tsv'):
    _q = _l.split('\t', 2)
    if len(_q) >= 2: CATN[_q[0].lower()] = _q[1]
def derives(sc, a):
    # the row's graded function is a member of sc (catalog) and RTTI puts CLS at offset 0 of sc
    cn = CATN.get(a, '')
    if not cn.startswith(sc + '::'): return False
    return (CLS + '@0') in RB.get(sc, set())
def text_derives(c, sc, depth=0):
    m = re.search(r'\b(?:class|struct)\s+' + re.escape(sc) + r'\s*:([^;{]*)\{', c)
    if not m or depth > 3: return False
    bases = [re.sub(r'\b(public|protected|private|virtual)\b', '', b).strip() for b in m.group(1).split(',')]
    return CLS in bases or any(text_derives(c, b, depth + 1) for b in bases if b)
DEM = re.compile(r'^(public|protected|private): (static |virtual )?(.*?) ?(__thiscall|__fastcall|__cdecl|__stdcall) (.+?)\((.*)\)( const)?\s*$')
FREE = re.compile(r'^(.*?) ?(__thiscall|__fastcall|__cdecl|__stdcall) ([\w:]+)\((.*)\)\s*$')
def splitq(q):
    out = []; d = 0; cur = ''; i = 0
    while i < len(q):
        if q[i] == '<': d += 1
        elif q[i] == '>': d -= 1
        if d == 0 and q.startswith('::', i): out.append(cur); cur = ''; i += 2; continue
        cur += q[i]; i += 1
    out.append(cur); return out
def typ(s):
    s = re.sub(r'\b(class|struct|enum|union) ', '', s.strip())
    s = re.sub(r'\s*([*&])', r'\1', s); s = re.sub(r'([*&])(?=\w)', r'\1 ', s)
    return s.replace('(void)', '()').strip()
def parse(sym):
    d = und.get([sym])[sym]
    m = DEM.match(d)
    if m:
        q = splitq(m.group(5)); ps = [] if m.group(6) in ('void', '') else split_args(m.group(6))
        return dict(kind='mem', acc=m.group(1), static=(m.group(2) or '').strip() == 'static', virtual=(m.group(2) or '').strip() == 'virtual',
                    ret=m.group(3), cc=m.group(4), scope=q[:-1], name=q[-1], params=ps, const=bool(m.group(7)), dem=d)
    m = FREE.match(d)
    if m:
        q = splitq(m.group(3)); ps = [] if m.group(4) in ('void', '') else split_args(m.group(4))
        return dict(kind='free', ret=m.group(1), cc=m.group(2), scope=q[:-1], name=q[-1], params=ps, dem=d)
    mm = re.match(r'^@(\w+)@(\d+)$', sym)
    if mm: return dict(kind='free', ret='?', cc='__fastcall', scope=[], name=mm.group(1), params=['?'] * (int(mm.group(2)) // 4), dem=sym)
    return None
NF = parse(FIN); assert NF and NF['kind'] == 'mem', (FIN, NF)
CLSQ = NF['scope']; CLS = CLSQ[-1]; M = NF['name']; CLSFULL = '::'.join(CLSQ)
def decl_text(pnames=None):
    ps = [typ(p) for p in NF['params']]
    pn = ', '.join('%s a%d' % (p, i) if not p.endswith(('*', '&')) else '%sa%d' % (p, i) for i, p in enumerate(ps))
    pn = re.sub(r'([*&])a', r'\1 a', pn) if False else pn
    cc = '' if NF['cc'] == '__thiscall' else NF['cc'] + ' '
    r = typ(NF['ret'])
    return '%s%s%s%s%s(%s)%s;' % ('static ' if NF['static'] else '', 'virtual ' if NF['virtual'] else '', (r + ' ') if r else '', cc, M, pn, ' const' if NF['const'] else '')
DECL = decl_text()
NOTE = '   // 0x%s (Ego_r.pdb %s)' % (T.upper(), FIN)
def fwd_types(t):
    # forward declarations FIN's parameter / return types need when the row does not declare them
    out = []
    c = F.code_only(t)
    for kw, nm in re.findall(r'\b(class|struct|enum|union) ([\w:]+)(?!<)', ' '.join(NF['params'] + [NF['ret']])):
        parts = nm.split('::'); base = parts[-1]
        if re.search(r'\b(class|struct|enum|union)\s+' + base + r'\b', c): continue
        if kw == 'enum': return None
        decl = '%s %s;' % (kw, base)
        for n in reversed(parts[:-1]): decl = 'namespace %s { %s }' % (n, decl)
        out.append(decl)
    return out
def stand(a): return sorted(s for s in IXU.get(a, ()) if s in B and s != FIN)
def acc_set(t, s, e, want):
    import fam_dtor_helpers as H
    return H.set_stmt(t, s, e, want)
def class_body(c, name):
    d = F.class_defs(c, name)
    return d
def conv_arg(arg, oldT, newT):
    if oldT is None or typ(oldT) == typ(newT): return arg
    n = typ(newT); o = typ(oldT); w = arg if re.match(r'^[\w.]+$', arg) else '(' + arg + ')'
    if n.endswith('&') and o.endswith('*'): return '*' + w
    if n.endswith('*') and o.endswith('&'): return '&' + w
    if n.endswith('*') and arg not in ('0', 'NULL'): return '(%s)%s' % (n, w)
    return arg
def tr(t, a):
    st = stand(a)
    if not st: return None, 'no stand-in named'
    msgs = []; renames = []
    for s in st:
        O = parse(s)
        if not O: return None, 'unparsed %s' % s
        N = O['name']; c = F.code_only(t)
        oldparams = O['params']; oldret = O['ret']
        derived = False
        if O['kind'] == 'free':
            if len(oldparams) != 1 or NF['static'] or NF['params']: return None, 'free stand-in %s not a one-argument this-call' % N
            dm = [x for x in re.finditer(r'(?m)^[^\n;{}]*\b' + re.escape(N) + r'\s*\([^;{}()]*\)\s*;[^\n]*\n', c) if depth_at(c, x.start()) == 0]
            if len(dm) != 1: return None, '%d declarations of free %s' % (len(dm), N)
            if re.search(r'\b' + re.escape(N) + r'\s*\([^;{}()]*\)\s*\{', c): return None, '%s defined here' % N
            ins = dm[0].start(); t = t[:dm[0].start()] + t[dm[0].end():]
            mode = 'free'
        else:
            SC = O['scope'][-1] if O['scope'] else None
            defs = F.class_defs(c, SC) if SC else []
            if len(defs) != 1: return None, '%d definitions of %s' % (len(defs), SC)
            kw, s0, b, e = defs[0]
            dd = []
            for x in re.finditer(r'(?:\b(?:virtual|static|inline|__forceinline)\s+)*[\w:<>,\s*&]*?\b' + re.escape(N) + r'\s*\(', c[b + 1:e]):
                pos = b + 1 + x.start(); seg = c[b + 1:pos]
                if seg.count('{') - seg.count('}') == 0: dd.append(pos)
            if len(dd) != 1: return None, '%d %s::%s declarations' % (len(dd), SC, N)
            ds = dd[0]
            while c[ds] in ' \t\r\n': ds += 1
            k = c.find('(', ds + c[ds:].find(N)); dep = 0
            while k < e:
                if c[k] == '(': dep += 1
                elif c[k] == ')':
                    dep -= 1
                    if dep == 0: break
                k += 1
            m2 = re.match(r'\s*(?:const\s*)?(?:=\s*0\s*)?([;{])', c[k + 1:])
            if not m2 or m2.group(1) == '{': return None, '%s::%s defined inline' % (SC, N)
            de = k + 1 + m2.end()
            # statement start: back to the previous ; { } or access label
            j = ds
            while j > b + 1 and c[j - 1] not in ';{}:': j -= 1
            while j < ds and c[j] in ' \t\n': j += 1
            ds = j
            if re.search(r'\b' + re.escape(SC) + r'\s*::\s*' + re.escape(N) + r'\s*\(', c): return None, '%s::%s defined out of line' % (SC, N)
            inplace = (O['scope'] == CLSQ and N == M)
            derived = False
            if not inplace and NF['acc'] != 'public' and SC and derives(SC, a) and len(CLSQ) == 1:
                if text_derives(c, SC): derived = 'text'
                elif not F.class_defs(c, CLS) and not re.match(r'\s*:', c[s0:b].split(SC, 1)[1]): derived = 'base'
                else: return None, 'non-public %s: %s does not derive from %s in the row' % (M, SC, CLS)
            elif not inplace and NF['acc'] != 'public' and SC:
                return None, 'non-public %s: %s is not an RTTI-derived class of the row (%s)' % (M, SC, CATN.get(a, '')[:60])
            if inplace:
                import fam_dtor_set as FS
                t = FS.set_stmt(t, c, kw, b, ds, de, DECL, NF['acc'])
                mode = 'inplace'
            else:
                ins = top_start(c, ds)
                lsx = t.rfind('\n', 0, ds) + 1; lex = t.find('\n', de); lex = len(t) if lex < 0 else lex
                if not t[lsx:ds].strip() and not F.code_only(t[de:lex]).strip(): t = t[:lsx] + t[lex + 1:]
                else: t = t[:ds] + t[de:]
                mode = 'moved'
                if derived == 'base':
                    c2 = F.code_only(t); d2 = F.class_defs(c2, SC)
                    if len(d2) != 1: return None, 'base: %d defs of %s' % (len(d2), SC)
                    kw2, s2, b2, e2 = d2[0]
                    body2 = c2[b2 + 1:e2]
                    fv = None
                    for vm in re.finditer(r'\bvirtual\b[^;{}]*?(?:\([^;{}]*\))[^;{}]*?;', body2):
                        seg = body2[:vm.start()]
                        if seg.count('{') == seg.count('}'): fv = vm.group(0); break
                    if fv is not None:
                        fv = re.sub(r'~\s*' + re.escape(SC) + r'\b', '~' + CLS, fv)
                    bt = 'class %s {                         // Ego_r / RTTI base of %s (offset 0)\n' % (CLS, SC)
                    if fv: bt += 'public:\n    %s\n' % fv
                    bt += '%s:\n    %s%s\n};\n' % (NF['acc'], DECL, NOTE)
                    hdr_end = b2
                    t = t[:s2] + bt + t[s2:hdr_end].rstrip() + ' : public %s ' % CLS + t[hdr_end:]
        c = F.code_only(t)
        # calls of N
        if mode != 'free' and len(re.findall(r'(?<![\w$])' + re.escape(N) + r'\s*\(', c)) and mode == 'inplace':
            nd = len([x for x in re.finditer(r'(?<![\w$.>:])' + re.escape(N) + r'\s*\([^;{}()]*(?:\([^()]*\)[^;{}()]*)*\)\s*(?:const\s*)?;', c)])
        out = []; last = 0; n = 0
        for cm in re.finditer(r'(?<![\w$~])' + re.escape(N) + r'\s*\(', c):
            if cm.start() < last: continue
            # skip declarations / definitions: preceded by a type token on the same statement
            pre = cm.start() - 1
            while pre >= 0 and c[pre] in ' \t': pre -= 1
            if mode == 'inplace' and (c[pre].isalnum() or c[pre] in '_*&>') and not c[pre - 1:pre + 1] == '->' and c[pre] != '.' and c[pre - 1:pre + 1] != '::':
                if not re.search(r'(\breturn|[=(,!?:&|]|\bif|\bwhile)\s*$', c[max(0, pre - 10):pre + 1]): continue
            j = cm.end() - 1; d = 0; k = j
            while k < len(c):
                if c[k] == '(': d += 1
                elif c[k] == ')':
                    d -= 1
                    if d == 0: break
                k += 1
            args = split_args(t[j + 1:k])
            if pre >= 1 and c[pre - 1:pre + 1] == '->':
                es = back_expr(c, pre - 2); expr = t[es:pre - 1].strip(); objp = expr if re.match(r'^\w+$', expr) else '(%s)' % expr; start = es; acc = '->'
            elif pre >= 0 and c[pre] == '.':
                es = back_expr(c, pre - 1); expr = t[es:pre].strip(); objp = '&' + (expr if re.match(r'^\w+$', expr) else '(%s)' % expr); start = es; acc = '.'
            elif pre >= 1 and c[pre - 1:pre + 1] == '::':
                if mode != 'inplace': return None, 'qualified call of %s' % N
                objp = None; start = cm.start(); acc = '::'
            else: objp = 'this'; start = cm.start(); acc = ''
            if mode == 'free':
                if len(args) != 1: continue
                objp = args[0] if re.match(r'^\w+$', args[0]) else '(%s)' % args[0]; args = []; start = cm.start()
            if len(args) != len(NF['params']): return None, 'call of %s with %d args' % (N, len(args))
            na = [conv_arg(x, oldparams[i] if i < len(oldparams) and oldparams[i] != '?' else None, NF['params'][i]) for i, x in enumerate(args)]
            if mode == 'inplace' or (mode == 'moved' and derived):
                call = t[start:cm.start()] + M + '(' + ', '.join(na) + ')'
            elif NF['static']:
                call = '%s::%s(%s)' % (CLSFULL, M, ', '.join(na))
            else:
                call = '((%s*)%s)->%s(%s)' % (CLSFULL, objp, M, ', '.join(na))
            if oldret not in ('?',) and typ(oldret) != typ(NF['ret']) and typ(oldret).endswith('*'):
                call = '((%s)%s)' % (typ(oldret), call)
            out.append(t[last:start]); out.append(call); last = k + 1; n += 1
        out.append(t[last:]); t = ''.join(out)
        if n == 0 and mode != 'inplace': return None, 'no calls of %s' % N
        if mode in ('moved', 'free'):
            c = F.code_only(t); dd = F.class_defs(c, CLS)
            if dd:
                if len(dd) != 1 or depth_at(c, dd[0][1]) != len(CLSQ) - 1: return None, '%s defined in another scope' % CLS
                if not re.search(r'\b' + re.escape(M) + r'\s*\(', c[dd[0][2]:dd[0][3]]):
                    bb = dd[0][2] + 1; acc0 = 'private' if dd[0][0] == 'class' else 'public'
                    t = t[:bb] + '\n%s:\n    %s%s\n%s:' % (NF['acc'], DECL, NOTE, acc0) + t[bb:]
            else:
                fw = fwd_types(t[:ins] if ins <= len(t) else t)
                if fw is None: return None, 'enum type of %s not declared before use' % M
                body = 'class %s { %s:\n    %s%s\n};' % (CLS, NF['acc'], DECL, NOTE)
                for nsn in reversed(CLSQ[:-1]): body = 'namespace %s { %s }' % (nsn, body)
                if NF['acc'] != 'public' and not derived: return None, 'non-public %s needs an access path' % M
                t = t[:ins] + ''.join(x + '\n' for x in fw) + body + '\n' + t[ins:]
        renames.append((O['scope'], N))
        msgs.append('%s %s::%s -> %s x%d' % (mode, '::'.join(O['scope']), N, M, n))
    for sc, N in renames:
        def fixc(mm):
            s2 = mm.group(0)
            if sc: s2 = re.sub(r'\b' + re.escape('::'.join(sc)) + r'::' + re.escape(N) + r'\b', CLSFULL + '::' + M, s2)
            return s2
        t = re.sub(r'//[^\n]*', fixc, t)
    return t, ' '.join(msgs)
def ck(D0, U0, D1, U1, a):
    st = set(stand(a))
    if st & U1: return 'stand-in still named'
    if FIN not in U1 and FIN not in D1: return 'final not named'
    hz = lambda s: '?A0x' in s
    rem = [s for s in U0 - U1 if s not in st and not hz(s)]
    add = [s for s in U1 - U0 if s != FIN and not hz(s)]
    if rem or add: return 'diff -%s +%s' % (rem[:3], add[:3])
    return None
if __name__ == '__main__':
    F.run(ov, [l.split()[0] for l in open(rf) if l.strip()], tr, ck, TAG)
