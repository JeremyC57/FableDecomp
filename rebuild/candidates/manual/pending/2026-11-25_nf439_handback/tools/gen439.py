#!/usr/bin/env python3
"""gen439.py <out_prefix> [pairs.tsv]: NF439 declaration respell generator for B-list T1 targets.
Input pairs (row, target, wrong symbol) default work/pairs.tsv. Shapes:
  LOCAL  the row's own local model class (same scope as the PDB spelling) declares the callee: the declaration is re-spelt to the PDB
         prototype (name / parameter types / const / access), calls renamed, arguments shimmed by cast (ptr->ref deref, ptr / int / enum casts).
  NSFUNC a free __fastcall stand-in -> the PDB namespace function: declaration moved into the namespace, calls qualified and shimmed.
Writes <out>_rows/<a>.cpp, <out>_PLAN.tsv (row, shape, old, new, target), <out>_RESIDUE.tsv (target, wrong, row, reason)."""
import sys, os, re, csv, collections
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__))); import nf, und439 as U
csv.field_size_limit(10**9)
R = '/home/user/FableDecomp'; A = str(nf.A); S = str(nf.S)
outp = sys.argv[1]; pairs = sys.argv[2] if len(sys.argv) > 2 else A + '/work/pairs.tsv'
B = {r['target']: r for r in csv.DictReader(open(S + '/t1split/T1_B.tsv'), delimiter='\t')}
t3 = collections.defaultdict(list)
for l in open(R + '/rebuild/candidates/manual/naming/T3_PORT_QUEUE.tsv').read().splitlines()[1:]:
    q = l.split('\t')
    for x in re.findall(r'0x([0-9a-fA-F]{8})', ' '.join(q[1:3])): t3[x.lower()].append(q[0])
SKIP = set(open(A + '/work/skip_rows.txt').read().split()) if os.path.exists(A + '/work/skip_rows.txt') else set()
INTS = {'bool', 'char', 'signed char', 'unsigned char', 'short', 'unsigned short', 'int', 'unsigned int', 'long', 'unsigned long', '__int64', 'unsigned __int64', 'wchar_t', 'float', 'double'}
STDH = {'vector': 'vector', 'list': 'list', 'pair': 'utility', 'map': 'map', 'set': 'set', 'allocator': 'memory', 'basic_string': 'string'}
class Res(Exception): pass
def strip_kw(t): return re.sub(r'\b(class|struct|union|enum) ', '', t)
def norm(t): return re.sub(r'\s+', ' ', strip_kw(t)).strip()
def kind(t):
    t = t.strip()
    if t.endswith('&'): return 'ref'
    if t.endswith('*') or t.endswith('* const'): return 'ptr'
    if t.startswith('enum '): return 'enum'
    if t in INTS: return 'int'
    return 'val'
def base(t): return re.sub(r'\s*[&*]\s*(const)?\s*$', '', t.strip())
def nocv(t): return norm(re.sub(r'\bconst\b', '', t))
def types_in(t):   # elaborated names in an undname type: (keyword, qualified name, is_template)
    return [(k, q, bool(lt)) for k, q, lt in re.findall(r'\b(class|struct|union|enum) ([A-Za-z_]\w*(?:::[A-Za-z_]\w*)*)(<?)', t)]
def lock_ok(a):
    p = S + '/w22/nflocks/%s/owner' % a
    return (not os.path.exists(p)) or open(p).read().strip() in ('released', 'NF439')
def find_body(x, cls):
    ms = [m for m in re.finditer(r'\b(class|struct)\s+%s\b[^;{()]*\{' % re.escape(cls), x)]
    if len(ms) != 1: raise Res('LOCAL: %d definitions of local class %s in the row' % (len(ms), cls))
    m = ms[0]; i = m.end(); d = 1
    while d and i < len(x):
        if x[i] == '{': d += 1
        elif x[i] == '}': d -= 1
        i += 1
    return m.group(1), m.end(), i - 1
def access_at(x, key, b0, pos):
    labs = [(m.start(), m.group(1)) for m in re.finditer(r'\b(public|protected|private)\s*:(?!:)', x[b0:pos])]
    return labs[-1][1] if labs else ('public' if key == 'struct' else 'private')
def match_paren(x, i):   # x[i] == '(' -> index of matching ')'
    d = 0
    for j in range(i, len(x)):
        if x[j] == '(': d += 1
        elif x[j] == ')':
            d -= 1
            if d == 0: return j
    raise Res('unbalanced call parentheses')
def expr_start(x, i):   # start of the postfix expression whose member name begins at i
    j = i
    while True:
        k = j
        while k > 0 and x[k - 1] in ' \t\n': k -= 1
        if x[k - 2:k] == '->': k -= 2
        elif k > 0 and x[k - 1] == '.' and not x[k - 2:k - 1].isdigit(): k -= 1
        elif x[k - 2:k] == '::': k -= 2
        else: return j
        while k > 0 and x[k - 1] in ' \t\n': k -= 1
        if k > 0 and x[k - 1] in ')]':
            op = {')': '(', ']': '['}[x[k - 1]]; cl = x[k - 1]; d = 0
            while k > 0:
                k -= 1
                if x[k] == cl: d += 1
                elif x[k] == op:
                    d -= 1
                    if d == 0: break
        while k > 0 and (x[k - 1].isalnum() or x[k - 1] == '_'): k -= 1
        j = k
def pnames(ptext, n):
    ps = U.split_top(ptext) if ptext.strip() and ptext.strip() != 'void' else []
    out = []
    for p in ps:
        m = re.search(r'[\s*&](\w+)\s*$', p)
        out.append(m.group(1) if m and m.group(1) not in ('const', 'long', 'int', 'short', 'char', 'bool', 'float', 'double', 'unsigned', 'signed') and len(p.split()) + p.count('*') + p.count('&') > 1 else '')
    return out if len(out) == n else [''] * n
def P2(arg): return arg if re.match(r'^[\w]+$', arg.strip()) else '(%s)' % arg
def shim_arg(arg, o, n):
    if norm(o) == norm(n): return arg
    ko, kn = kind(o), kind(n)
    N = strip_kw(n)
    if kn == 'ref':
        bn = strip_kw(base(n))
        if ko == 'ptr': return '*' + P2(arg) if nocv(base(o)) == nocv(base(n)) else '*(%s*)%s' % (bn, P2(arg))
        if ko == 'ref': return '*(%s*)&(%s)' % (bn, arg)
        if ko == 'val' and nocv(o) == nocv(base(n)): return arg
        raise Res('argument %s -> %s' % (o, n))
    if kn == 'ptr':
        if ko == 'ptr' or ko == 'int': return '(%s)%s' % (N, P2(arg))
        if ko == 'ref': return '(%s)&(%s)' % (N, arg)
        raise Res('argument %s -> %s' % (o, n))
    if kn in ('enum', 'int') and ko in ('enum', 'int'): return '(%s)%s' % (N, P2(arg))
    raise Res('argument %s -> %s (value / class type)' % (o, n))
def shim_ret(e, o, n):
    if norm(o) == norm(n) or o == 'void': return e, False
    ko, kn = kind(o), kind(n); Oo = strip_kw(o)
    if n == 'void': raise Res('return void <- %s' % o)
    if ko in ('ptr', 'int', 'enum') and kn in ('ptr', 'int', 'enum'): return '((%s)(%s))' % (Oo, e), True
    if ko == 'ref' and kn == 'ref': return '(*(%s*)&(%s))' % (strip_kw(base(o)), e), True
    if ko == 'ptr' and kn == 'ref': return '((%s)&(%s))' % (Oo, e), True
    if ko == 'ref' and kn == 'ptr': return '(*(%s*)(%s))' % (strip_kw(base(o)), e), True
    raise Res('return %s -> %s' % (o, n))
def stmt_context(x, s, e):   # call expression x[s:e] used as a whole statement
    a = x[:s].rstrip(); b = x[e:].lstrip()
    return (not a or a[-1] in ';{}') and b.startswith(';')
def uncomment(x): return re.sub(r'/\*.*?\*/', ' ', re.sub(r'//[^\n]*', ' ', x), flags=re.S)
def declared(x, last):
    return bool(re.search(r'\b(class|struct|union|enum)\s+%s\b' % re.escape(last), x) or re.search(r'\btypedef\b[^;]*\b%s\s*;' % re.escape(last), x)
                or re.search(r'#include\s+"engine/(\w+_)?%s\.h"' % re.escape(last), x))
def fwd_decls(types, x, extra_ok):
    out = []; inc = set(); x = uncomment(x)
    for t in types:
        for kw, q, tmpl in types_in(t):
            parts = q.split('::')
            if parts[0] == 'std':
                if len(parts) == 2 and parts[1] in STDH: inc.add(STDH[parts[1]]); continue
                raise Res('TYPE: std type %s' % q)
            last = parts[-1]
            if declared(x, last) or last in extra_ok: continue
            if tmpl: raise Res('TYPE: template %s not declared in the row' % q)
            ns = parts[:-1]
            if any(not re.match(r'^N[A-Z]', p) for p in ns): raise Res('TYPE: %s nested in a class' % q)
            if kw == 'enum' and re.search(r'\b%s\b' % re.escape(last), x): raise Res('TYPE: enum %s named in the row but not declared there (header?)' % q)
            d = ('enum %s {};   // PDB enum type (enumerators not modelled)' if kw == 'enum' else '%s %s;' % (kw, '%s')) % last
            for p in reversed(ns): d = 'namespace %s { %s }' % (p, d)
            if d not in out: out.append(d)
    return out, sorted(inc)
def gen_row(a, x, items):
    """items: list of (s, t, P). Returns new text + shapes."""
    shapes = []; pre_lines = []; pre_inc = set()
    for s, t, P in items:
        o = U.parse(s); n = U.parse(P)
        if not o or not n: raise Res('SPECIAL: undname parse (%s | %s)' % (s[:40], P[:40]))
        if n['sv'] == 'virtual' or o['sv'] == 'virtual': raise Res('VIRTUAL: virtual callee (vptr / dispatch)')
        if len(o['params']) != len(n['params']): raise Res('SIG: parameter count %d -> %d' % (len(o['params']), len(n['params'])))
        oname, nname = o['name'], n['name']
        if not re.match(r'^\w+$', oname) or not re.match(r'^\w+$', nname): raise Res('SPECIAL: operator / template name')
        if re.search(r'\b%s\s*::\s*%s\s*\([^;{]*\)[^;{]*\{' % (re.escape(o['scope'][-1]) if o['scope'] else 'XX__', re.escape(oname)), x): raise Res('row defines the stand-in body')
        same_scope = o['scope'] == n['scope']
        if o['scope'] and same_scope and o['cc'] == '__thiscall' and n['cc'] == '__thiscall' and not o['sv'] and not n['sv']:
            key, b0, b1 = find_body(x, o['scope'][-1]); body = x[b0:b1]
            ds = list(re.finditer(r'^([ \t]*)((?:inline\s+|FABLE_\w+\s+)*)([^;{}()\n]*?)\b%s\s*\(([^;{}]*)\)\s*(const)?\s*;[^\n]*$' % re.escape(oname), body, re.M))
            if len(ds) != 1: raise Res('LOCAL: %d declarations of %s in class %s' % (len(ds), oname, o['scope'][-1]))
            dm = ds[0]
            if dm.group(2).strip(): raise Res('LOCAL: declaration has specifiers %s' % dm.group(2))
            if bool(dm.group(5)) != o['const']: raise Res('LOCAL: declaration const does not match the stand-in')
            cur = access_at(x, key, b0, b0 + dm.start())
            if cur != (o['access'] or 'public'): raise Res('LOCAL: access in effect %s v stand-in %s' % (cur, o['access']))
            pn = pnames(dm.group(4), len(n['params']))
            nd = '%s %s(%s)%s;' % (strip_kw(n['ret']), nname, ', '.join((strip_kw(p) + (' ' + q if q else '')).replace('* ', '*').replace(' *', '*').replace(' &', '&') for p, q in zip(n['params'], pn)), ' const' if n['const'] else '')
            if n['access'] != cur: nd = '%s: %s %s:' % (n['access'], nd, cur)
            com = '%s// Naming (NF439 gen, 2026-11-25): callee 0x%s binds its PDB spelling %s (T1 target; was %s).\n' % (dm.group(1), t.upper(), P, s)
            new_body = body[:dm.start()] + com + dm.group(1) + nd + body[dm.end():]
            x = x[:b0] + new_body + x[b1:]
            shape = 'LOCAL'; qual = None
            fd, inc = fwd_decls([n['ret']] + n['params'], x.replace(nd, ''), set()); pre_lines += [d for d in fd if d not in pre_lines]; pre_inc |= set(inc)
        elif not o['scope'] and n['scope'] and o['cc'] == '__fastcall' and n['cc'] == '__fastcall' and not n['access'] and all(re.match(r'^N[A-Z]\w*$', p) for p in n['scope']):
            ds = list(re.finditer(r'^([ \t]*)((?:extern\s+)?)([^;{}()\n/]*?)\b(?:__fastcall|FABLE_FASTCALL)\s+%s\s*\(([^;{}]*)\)\s*;[^\n]*\n' % re.escape(oname), x, re.M))
            if len(ds) != 1: raise Res('NSFUNC: %d free __fastcall declarations of %s' % (len(ds), oname))
            dm = ds[0]
            if dm.group(1): raise Res('NSFUNC: declaration not at file level')
            if any(re.search(r'\b%s\b' % re.escape(p), x) for p in n['scope']) and re.search(r'\bnamespace\s+%s\b' % re.escape(n['scope'][0]), x) is None and False: pass
            pn = pnames(dm.group(4), len(n['params']))
            nd = '%s __fastcall %s(%s);' % (strip_kw(n['ret']), nname, ', '.join((strip_kw(p) + (' ' + q if q else '')).replace(' *', '*').replace(' &', '&') for p, q in zip(n['params'], pn)))
            for p in reversed(n['scope']): nd = 'namespace %s { %s }' % (p, nd)
            com = '// Naming (NF439 gen, 2026-11-25): callee 0x%s binds its PDB spelling %s (T1 target; was the free stand-in %s).\n' % (t.upper(), P, s)
            fd, inc = fwd_decls([n['ret']] + n['params'], x[:dm.start()] + x[dm.end():], set())
            x = x[:dm.start()] + ''.join(d + '\n' for d in fd) + com + nd + '\n' + x[dm.end():]
            shape = 'NSFUNC'; qual = '::'.join(n['scope']) + '::'; pre_inc |= set(inc)
        else:
            raise Res('SHAPE: %s %s -> %s %s %s (scope move / static / calling convention)' % (o['cc'], '::'.join(o['scope']) or '-', n['cc'], '::'.join(n['scope']) or '-', n['sv'] or n['access'] or ''))
        # call sites
        decl_pos = x.find(nd)
        calls = [m for m in re.finditer(r'\b%s\b' % re.escape(oname), x) if not (decl_pos <= m.start() < decl_pos + len(nd))]
        calls = [m for m in calls if not x[:m.start()].split('\n')[-1].lstrip().startswith('//') and '//' not in x[:m.start()].split('\n')[-1]]
        if not calls: raise Res('no call of %s left outside the declaration' % oname)
        for m in reversed(calls):
            i = m.start(); j = m.end()
            while j < len(x) and x[j] in ' \t': j += 1
            if j >= len(x) or x[j] != '(': raise Res('%s used other than as a call' % oname)
            if qual and (x[:i].rstrip().endswith('->') or x[:i].rstrip().endswith('.') or x[:i].rstrip().endswith('::')): raise Res('NSFUNC: qualified / member use of the stand-in')
            k = match_paren(x, j)
            args = U.split_top(x[j + 1:k]) if x[j + 1:k].strip() else []
            if len(args) != len(o['params']): raise Res('call with %d args v %d params (default args?)' % (len(args), len(o['params'])))
            na = [shim_arg(g, op, np_) for g, op, np_ in zip(args, o['params'], n['params'])]
            call = (qual or '') + nname + '(' + ', '.join(na) + ')'
            st = expr_start(x, i) if not qual else i
            whole = x[st:i] + call
            if not stmt_context(x, st, k + 1):
                whole, _ = shim_ret(whole, o['ret'], n['ret'])
            x = x[:st] + whole + x[k + 1:]
        shapes.append(shape)
    if pre_lines or pre_inc:
        ins = ''.join('#include <%s>\n' % h for h in sorted(pre_inc) if '#include <%s>' % h not in x) + ''.join(d + '\n' for d in pre_lines)
        if ins:
            ms = list(re.finditer(r'^#(include|define|pragma)[^\n]*\n', x, re.M))
            p = ms[-1].end() if ms else 0
            x = x[:p] + ins + x[p:]
    return x, shapes
P_ = collections.defaultdict(list)
for l in open(pairs):
    a, t, s = l.rstrip('\n').split('\t')[:3]
    P_[a].append((s, t, B[t]['pdb_spelling']))
plan = []; res = []
for a in sorted(P_):
    items = sorted(set(P_[a]))
    why = None
    if a in SKIP: why = 'skip list'
    elif not lock_ok(a): why = 'LOCKED by %s' % open(S + '/w22/nflocks/%s/owner' % a).read().strip()
    elif a in t3: why = 'T3_PORT_QUEUE col 2/3 (%s)' % ','.join(t3[a][:3])
    elif not nf.src_of(a) or not nf.src_of(a).exists(): why = 'no source'
    if why:
        for s, t, P in items: res.append((t, s, a, why))
        continue
    x = nf.src_of(a).read_text(encoding='utf-8')
    try:
        y, sh = gen_row(a, x, items)
        plan.append((a, '+'.join(sorted(set(sh))), items, y))
    except Res as e:
        for s, t, P in items: res.append((t, s, a, str(e)))
os.makedirs(outp + '_rows', exist_ok=True)
with open(outp + '_PLAN.tsv', 'w') as f:
    for a, sh, items, y in plan:
        for s, t, P in items: f.write('\t'.join([a, sh, s, P, t]) + '\n')
        open(outp + '_rows/%s.cpp' % a, 'w', encoding='utf-8').write(y)
with open(outp + '_RESIDUE.tsv', 'w') as f:
    f.write('target\twrong_symbol\trow\treason\n'); f.write(''.join('\t'.join(r) + '\n' for r in res))
print('rows planned', len(plan), collections.Counter(p[1] for p in plan), 'residue', len(res))
print(collections.Counter(r[3].split(':')[0][:50] for r in res).most_common(20))
