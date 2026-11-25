#!/usr/bin/env python3
"""rebind438.py <out_prefix> [rows...]: NF438 AST-driven T1 caller rebind generator (ruling r2/r3 form).
For each row with locator hits (work/ast.jsonl) on a wrong spelling s of a T1_A target t (PDB spelling P), the row is re-parsed with
libclang and every call of s is rebound to P without touching the row's own model:
  MEMBER  P a __thiscall method of a class Q in the global / N-namespace scope: a bodiless `class Q { public: [virtual] RET M(PARAMS)
          [const]; };` (namespaces wrapped) is inserted before the top-level declaration holding the first call, and each call's callee
          `B->M` / `B.M` / `M` becomes `reinterpret_cast<[const ]Q*>(B / &(B) / this)->[Q::]M` (qualified when P is virtual).
  STATIC  P a static method: same class declaration with `static`, callee -> `Q::M`.
  FREE    P a free function: callee renamed; a prototype `RET __cc G(PARAMS);` replaces the stand-in's declaration when that is a pure
          top-level declaration of the row, else is inserted.
  Arguments: a parameter that is `T*` in s and `T&` in P gets `*(arg)` (or `&x` -> `x`).
Screens (row): lock free/released/NF438, not T3 col 2/3, newest COORDINATION_CLAIMS not B, not in a pending RESERVED.tsv, row != t,
locator parse clean. (symbol): every hit is a call or a callee of a call (FREE: address-of allowed), P not protected/private, not a
special member / operator / member template / template or std scope / class-nested scope, Q not defined in the row, the stand-in is
not defined (bodied) in the row. One row = all its rebinds. Writes <out>_rows/<a>.cpp, <out>_PLAN.tsv (row, mode, s, P, t),
<out>_RESIDUE.tsv (t, s, row, reason)."""
import sys, os, re, json, csv, collections, subprocess, glob
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__))); import nf
sys.path.insert(0, os.path.join(os.path.dirname(os.path.abspath(__file__)), '..', 'pylib'))
import clang.cindex as ci
ci.Config.set_library_file('/usr/lib/llvm-18/lib/libclang-18.so.1')
R = '/home/user/FableDecomp'; S_ = str(nf.S); A = str(nf.A)
outp = sys.argv[1]; only = set(sys.argv[2:])
CONSTAWAY = set(open(os.environ['CONSTAWAY']).read().split()) if os.environ.get('CONSTAWAY') else set()
SKIPR = set(open(os.environ['SKIPROWS']).read().split()) if os.environ.get('SKIPROWS') else set()
op = {r['target']: r for r in csv.DictReader(open(A + '/tracker/OPEN.tsv'), delimiter='\t')}
cen = collections.defaultdict(dict)
for l in open(A + '/work/census_f.tsv'):
    a, t, s, P, n = l.rstrip('\n').split('\t'); cen[a].setdefault(s, set()).add(t)
t3 = set()
for l in open(R + '/rebuild/candidates/manual/naming/T3_PORT_QUEUE.tsv').read().splitlines()[1:]:
    q = l.split('\t'); t3 |= {x.lower() for x in re.findall(r'0x([0-9a-fA-F]{8})', ' '.join(q[1:3]))}
claims = collections.defaultdict(list)
for l in open(R + '/rebuild/candidates/manual/COORDINATION_CLAIMS.tsv').read().splitlines()[1:]:
    q = l.split('\t')
    for x in set(re.findall(r'(?:0x)?([0-9a-fA-F]{8})\b', l)): claims[x.lower()].append(q[1] if len(q) > 1 else '?')
reserved = set()
for f in glob.glob(R + '/rebuild/candidates/manual/pending/*/RESERVED.tsv'):
    reserved |= {x.lower() for x in re.findall(r'\b(?:0x)?([0-9a-fA-F]{8})\b', open(f).read())}
def lockfree(a):
    p = S_ + '/w22/nflocks/%s/owner' % a
    return (not os.path.exists(p)) or open(p).read().strip() in ('released', 'NF438')
def undname(xs):
    out = subprocess.run(['llvm-undname'], input='\n'.join(xs), capture_output=True, text=True).stdout; d = {}
    for b in out.strip().split('\n\n'):
        l = b.split('\n')
        if len(l) >= 2: d[l[0]] = l[1]
    return d
def split_top(s):   # split on top-level commas
    out, dep, cur = [], 0, ''
    for ch in s:
        if ch in '<(': dep += 1
        elif ch in '>)': dep -= 1
        if ch == ',' and dep == 0: out.append(cur.strip()); cur = ''
        else: cur += ch
    if cur.strip(): out.append(cur.strip())
    return out
def cty(t): return re.sub(r'\b(class|struct|union|enum) ', '', t).replace(' *', '*').replace(' &', '&').strip()
def parse_dem(d):
    """-> dict(access, storage, ret, cc, scope list, name, params list, const) or None"""
    m = re.match(r'^(?:(public|protected|private): )?(?:(virtual|static) )?(.*?)\b(__thiscall|__fastcall|__cdecl|__stdcall) (.*)$', d)
    if not m: return None
    acc, sto, ret, cc, rest = m.groups()
    # name(params)[ const]
    dep = 0; cut = None
    for i, ch in enumerate(rest):
        if ch == '<': dep += 1
        elif ch == '>': dep -= 1
        elif ch == '(' and dep == 0: cut = i; break
    if cut is None: return None
    qn = rest[:cut]; tail = rest[cut:]
    mt = re.match(r'^\((.*)\)( const)?$', tail)
    if not mt: return None
    parts = re.split(r'::(?![^<]*>)', qn)
    return dict(access=acc, storage=sto, ret=ret.strip(), cc=cc, scope=parts[:-1], name=parts[-1], params=[] if mt.group(1) == 'void' else split_top(mt.group(1)), const=bool(mt.group(2)))
ARGS = ['-x', 'c++', '--target=i686-pc-windows-msvc', '-fms-extensions', '-fms-compatibility', '-fms-compatibility-version=13.10', '-w', '-Xclang', '-fno-wchar', '-I' + R + '/rebuild/include', '-isystem', S_ + '/wine/vc71/include']
idx = ci.Index.create()
hitrows = {}
for l in open(A + '/work/ast.jsonl'):
    j = json.loads(l)
    if j.get('hits'): hitrows[j['row']] = j
allsyms = set(); [allsyms.update(cen[a]) for a in hitrows]
D = undname(sorted(allsyms | {op[t]['pdb_spelling'] for t in op}))
plan, res = [], []
def residue(t, s, a, why): res.append((t, s, a, why))
ns_of = {}
for a in sorted(hitrows):
    if only and a not in only: continue
    if a in SKIPR: continue
    j = hitrows[a]; src = j['src']
    syms = sorted({h['sym'] for h in j['hits']})
    def rr(why):
        for s in syms: residue(','.join(sorted(cen[a].get(s, ()))), s, a, why)
    if j.get('errs'): rr('row: libclang parse errors at HEAD'); continue
    if not lockfree(a): rr('row: locked by another agent'); continue
    if a in t3: rr('row: named in T3_PORT_QUEUE col 2/3'); continue
    if claims.get(a) and claims[a][-1] == 'B': rr('row: newest COORDINATION_CLAIMS line is Account B'); continue
    if a in reserved: rr('row: in a pending RESERVED.tsv'); continue
    data = open(src, 'rb').read(); text = data.decode('utf-8', 'replace')
    CAST = (lambda q, o: 'const_cast<%s*>(reinterpret_cast<const %s*>(%s))' % (q, q, o)) if a in CONSTAWAY else (lambda q, o: 'reinterpret_cast<%s*>(%s)' % (q, o))
    tu = idx.parse(src, args=ARGS)
    tops = [(c.extent.start.offset, c.extent.end.offset) for c in tu.cursor.get_children() if c.location.file and c.location.file.name == src]
    qdefs = []   # (name, ns path, (body open, body close offsets), [(method, mangled, decl ext, has body)])
    def nspath(c):
        out = []; p_ = c.semantic_parent
        while p_ is not None and p_.kind != ci.CursorKind.TRANSLATION_UNIT:
            out.append((p_.kind.name, p_.spelling)); p_ = p_.semantic_parent
        return out[::-1]
    for c in tu.cursor.walk_preorder():
        if c.kind in (ci.CursorKind.CLASS_DECL, ci.CursorKind.STRUCT_DECL) and c.is_definition() and c.location.file:
            np_ = nspath(c)
            if any(k != 'NAMESPACE' for k, _ in np_): continue
            ms = []
            for m in c.get_children():
                if m.kind == ci.CursorKind.CXX_METHOD:
                    try: mn = m.mangled_name
                    except Exception: mn = None
                    ms.append((m.spelling, mn, (m.extent.start.offset, m.extent.end.offset), m.is_definition()))
            qdefs.append((c.spelling, [n for _, n in np_], (c.extent.start.offset, c.extent.end.offset), ms, c.location.file.name == src))
    # re-collect hits with cursors (offsets from the locator are the same parse)
    fwd = []; edits = []; classes = collections.OrderedDict(); protos = []; done_syms = []; first_hit = None; decl_kill = []
    for s in syms:
        ts = cen[a].get(s, set())
        if len(ts) != 1: residue(','.join(sorted(ts)), s, a, 'symbol: bound to more than one census target'); continue
        t = next(iter(ts)); P = op[t]['pdb_spelling']
        if a == t: residue(t, s, a, 'row is the target self row'); continue
        hs = [h for h in j['hits'] if h['sym'] == s]
        dp, ds = D.get(P), D.get(s)
        pp = parse_dem(dp) if dp else None; ps = parse_dem(ds) if ds else None
        why = None
        if P.startswith('??') or s.startswith('??'): why = 'special member / operator (ctor, dtor, operator, ??_G): needs a model edit'
        elif not pp: why = 'PDB spelling not parsed (%s)' % (dp or P)[:60]
        elif not ps: why = 'wrong spelling not parsed'
        elif pp['access'] in ('protected', 'private'): why = 'PDB target is %s: a bodiless public declaration would not mangle it (ruling needed)' % pp['access']
        elif any('<' in x for x in pp['scope']) or '<' in pp['name']: why = 'template scope or member template (shared-header route)'
        elif pp['scope'] and pp['scope'][0] == 'std': why = 'std scope'
        elif any(x.startswith('`') for x in pp['scope']): why = 'anonymous-namespace scope'
        elif len(pp['params']) != len(ps['params']): why = 'parameter count differs (identity research)'
        if why: residue(t, s, a, why); continue
        calls = [h for h in hs if h['kind'] == 'CALL_EXPR']
        inner = set()
        for h in calls:
            if h.get('callee_ext'): inner.add(tuple(h['callee_ext']))
        loose = [h for h in hs if h['kind'] != 'CALL_EXPR' and not any(c['ext'][0] <= h['ext'][0] and h['ext'][1] <= c['ext'][1] for c in calls)]
        mode = 'FREE' if not pp['scope'] else ('STATIC' if pp['storage'] == 'static' else 'MEMBER')
        if mode == 'FREE' and pp['cc'] == '__thiscall': mode = None
        if mode is None: residue(t, s, a, 'unexpected calling convention'); continue
        if (mode != 'FREE' and pp['scope'] != ps['scope']) and ps['storage'] != pp['storage'] and (ps['storage'] == 'static' or pp['storage'] == 'static'): residue(t, s, a, 'static v non-static mismatch'); continue
        if loose and mode != 'FREE': residue(t, s, a, 'non-call reference to the member (address taken / member pointer)'); continue
        if not calls and mode != 'FREE': residue(t, s, a, 'no call site found'); continue
        decl_ext = next((h['decl_ext'] for h in hs if h.get('decl_ext')), None)
        if decl_ext and '{' in data[decl_ext[0]:decl_ext[1]].decode('utf-8', 'replace'): residue(t, s, a, 'stand-in has a body in the row (inline model, not a callee-only rebind)'); continue
        bad = False; my = []; qinfo = None; inplace = False; hdrq = False
        Q = pp['scope'][-1] if pp['scope'] else None; nss = pp['scope'][:-1]
        if Q:
            if any(not re.match(r'^N[A-Z]\w*$|^[A-Z]\w*$', n) for n in nss): residue(t, s, a, 'scope path has non-identifier parts'); continue
            qd = [c for c in qdefs if c[0] == Q]
            hq = [c for c in qd if not c[4]]
            if hq:
                if any(m[1] == P for c in hq for m in c[3]) and not any(c[4] for c in qd): hdrq = True
                else: residue(t, s, a, 'class %s is defined in an included header without the PDB method (header change: _hdr batch)' % Q); continue
            elif qd:
                if len(qd) != 1 or qd[0][1] != list(nss): residue(t, s, a, 'class %s defined in the row more than once / in another scope' % Q); continue
                qinfo = qd[0]
                if pp['storage'] == 'virtual': residue(t, s, a, 'virtual PDB method on the row\'s own model of %s (layout)' % Q); continue
                same = [m for m in qinfo[3] if m[0] == pp['name']]
                if any(m[1] == P for m in same): residue(t, s, a, 'model %s already declares the PDB method' % Q); continue
                stand = [m for m in same if m[1] == s]
                if same and not (len(same) == 1 and stand): residue(t, s, a, 'model %s has other overloads of %s' % (Q, pp['name'])); continue
                inplace = bool(stand)
                if inplace and stand[0][3]: residue(t, s, a, 'stand-in method has a body in the model'); continue
                if inplace and ps['storage'] != pp['storage']: residue(t, s, a, 'storage differs on the in-place model method'); continue
            elif re.search(r'\b(class|struct)\s+%s\b\s*[:{]' % re.escape(Q), text) or re.search(r'\bstruct\s+%s\s*;' % re.escape(Q), text) or re.search(r'\btypedef\b[^;]*\b%s\s*;' % re.escape(Q), text):
                residue(t, s, a, 'class %s named as struct / typedef / defined in a way the locator does not see' % Q); continue
            # nested-class check: an outer scope that the row or headers define as a class
            if nss and any(re.search(r'\b(class|struct)\s+%s\b' % re.escape(n), text) for n in nss): residue(t, s, a, 'outer scope is a class (nested)'); continue
        full = '::'.join(pp['scope'] + [pp['name']])
        qual = '::'.join(pp['scope'])
        # pointer -> reference argument fix-ups
        deref = [i for i, (x, y) in enumerate(zip(ps['params'], pp['params'])) if x.endswith('*') and y.endswith('&') and cty(x)[:-1].replace('const ', '').replace(' const', '') == cty(y)[:-1].replace('const ', '').replace(' const', '')]
        refptr = [i for i, (x, y) in enumerate(zip(ps['params'], pp['params'])) if x.endswith('&') and y.endswith('*')]
        if refptr: residue(t, s, a, 'reference -> pointer parameter'); continue
        for h in calls:
            ce = h.get('callee_ext'); ck = h.get('callee_kind')
            if not ce: bad = 'call without callee extent'; break
            ctext = data[ce[0]:ce[1]].decode('utf-8', 'replace')
            if mode == 'MEMBER' and inplace and ps['scope'] == pp['scope']:
                pass
            elif mode == 'MEMBER':
                if ck != 'MEMBER_REF_EXPR': bad = 'member call through %s' % ck; break
                qt = ('const ' if pp['const'] else '') + qual
                be = h.get('base_ext')
                if be:
                    btxt = data[be[0]:be[1]].decode('utf-8', 'replace'); rest = data[be[1]:ce[1]].decode('utf-8', 'replace')
                    mm = re.match(r'^\s*(->|\.)\s*(?:\w+::)*%s$' % re.escape(ps['name']), rest)
                    if not mm: bad = 'callee text not B->M / B.M (%r)' % rest[:30]; break
                    obj = btxt if mm.group(1) == '->' else '&(%s)' % btxt
                    if btxt.strip() == 'this' or re.match(r'^\w+$', btxt.strip()) and mm.group(1) == '->': obj = btxt.strip()
                    new = CAST(qt, obj) + '->%s' % ( (qual + '::' if pp['storage'] == 'virtual' else '') + pp['name'])
                else:
                    if not re.match(r'^(?:\w+::)*%s$' % re.escape(ps['name']), ctext.strip()): bad = 'implicit-this callee text %r' % ctext[:30]; break
                    new = CAST(qt, 'this') + '->%s' % ( (qual + '::' if pp['storage'] == 'virtual' else '') + pp['name'])
                my.append((ce[0], ce[1], new))
            else:
                if ck not in ('DECL_REF_EXPR',): bad = '%s call through %s' % (mode.lower(), ck); break
                my.append((ce[0], ce[1], full))
            args = h.get('args') or []
            for i, (po, pn) in enumerate(zip(ps['params'], pp['params'])):
                if i in deref or i >= len(args): continue
                co, cn = cty(po), cty(pn)
                if co == cn: continue
                at = data[args[i][0]:args[i][1]].decode('utf-8', 'replace')
                if co.endswith('*') and cn.endswith('*'): my.append((args[i][0], args[i][1], 'reinterpret_cast<%s>(%s)' % (cn, at)))
                elif co.endswith('*') and cn.endswith('&'): my.append((args[i][0], args[i][1], '*reinterpret_cast<%s*>(%s)' % (cn[:-1], at)))
                elif co.endswith('&') or cn.endswith('&') or co.endswith('*') or cn.endswith('*'): bad = 'parameter %d %s -> %s' % (i, co, cn); break
            if bad: break
            ro, rn = cty(ps['ret']), cty(pp['ret'])
            if ro != rn:
                ce_ = h['ext']
                if ro.endswith('*') and rn.endswith('*'): my += [(ce_[1], ce_[1], ')'), (ce_[0], ce_[0], 'reinterpret_cast<%s>(' % ro)]
                elif ro.endswith('*') and rn.endswith('&'):
                    my += [(ce_[1], ce_[1], ')'), (ce_[0], ce_[0], ('(&' if ro[:-1].replace(' const','') == rn[:-1].replace(' const','') else 'reinterpret_cast<%s>(&' % ro))]
                elif ro.endswith('&') and rn.endswith('&'): my += [(ce_[1], ce_[1], '))'), (ce_[0], ce_[0], '(*reinterpret_cast<%s*>(&' % ro[:-1])]
                elif ro.endswith(('*', '&')) or rn.endswith(('*', '&')): bad = 'return %s -> %s' % (ro, rn); break
            for i in deref:
                if i >= len(args): bad = 'argument count'; break
                at = data[args[i][0]:args[i][1]].decode('utf-8', 'replace')
                my.append((args[i][0], args[i][1], at[1:].strip() if re.match(r'^&\s*[\w.]+$', at) else '*(%s)' % at))
            if bad: break
        if bad: residue(t, s, a, 'call site: ' + bad); continue
        if mode == 'FREE':
            for h in loose:
                e = h['ext']; my.append((e[0], e[1], full))
        # macro check: every edit range must hold the identifier spelled out
        if any(ps['name'] not in data[x0:x1].decode('utf-8', 'replace') and not (x1 - x0 < 200) for x0, x1, _ in my): residue(t, s, a, 'edit inside a macro'); continue
        params = ', '.join(cty(x) for x in pp['params']); ret = cty(pp['ret'])
        code_ = re.sub(r'//[^\n]*|/\*.*?\*/', ' ', text, flags=re.S); toks_ = set(re.findall(r'\w+', code_)); fw_bad = None
        for kw, nm in re.findall(r'\b(class|struct|union|enum) ([\w:]+)(?![\w:<])', ' '.join([pp['ret']] + pp['params'])):
            parts_ = nm.split('::')
            if (parts_[-1] in toks_ and (len(parts_) == 1 or all(n in toks_ for n in parts_[:-1]))) or (parts_[-1] == Q and parts_[:-1] == list(nss)): continue
            if kw == 'enum': fw_bad = 'enum %s unknown to the row' % nm; break
            if any(not re.match(r'^N[A-Z]\w*$', n) for n in parts_[:-1]): fw_bad = 'unknown nested type %s' % nm; break
            fd = '%s %s;' % (kw, parts_[-1])
            for n in reversed(parts_[:-1]): fd = 'namespace %s { %s }' % (n, fd)
            if fd not in fwd: fwd.append(fd)
        if fw_bad: residue(t, s, a, 'type: ' + fw_bad); continue
        if mode == 'FREE':
            cc = {'__fastcall': '__fastcall ', '__cdecl': '__cdecl ', '__stdcall': '__stdcall '}[pp['cc']]
            protos.append(('%s %s%s(%s);' % (ret, cc, pp['name'], params), s, P, t, decl_ext))
        else:
            cc = '' if pp['cc'] == '__thiscall' else pp['cc'] + ' '
            line = '%s%s %s%s(%s)%s;' % ({'virtual': 'virtual ', 'static': 'static '}.get(pp['storage'], ''), ret, cc, pp['name'], params, ' const' if pp['const'] else '')
            if qinfo is not None and inplace:
                de = next(m[2] for m in qinfo[3] if m[1] == s)
                pre_ = data[de[0]:de[1]].decode('utf-8', 'replace')
                lead = 'virtual ' if pre_.lstrip().startswith('virtual') else ''
                my.append((de[0], de[1], lead + line.rstrip(';')))
            elif qinfo is not None:
                cl = qinfo[2][1] - 1   # the closing brace of the model body
                if data[cl:cl + 1] != b'}': residue(t, s, a, 'model body end not found'); continue
                my.append((cl, cl, ' public: ' + line + ' '))
                if decl_ext: decl_kill.append(decl_ext)
            elif hdrq:
                if decl_ext: decl_kill.append(decl_ext)
            else:
                classes.setdefault((tuple(nss), Q), []).append(line)
                if decl_ext: decl_kill.append(decl_ext)
        edits += my; done_syms.append((s, P, t, mode))
        first_hit = min([x[0] for x in my] + ([first_hit] if first_hit is not None else []))
    if not done_syms: continue
    # insertion point: start of the top-level declaration holding the first edit
    top = [tp for tp in tops if tp[0] <= first_hit < tp[1]]
    if not top: rr('no top-level declaration around the first call'); continue
    ins = top[0][0]
    # back up to the start of the line (and over a directly preceding comment block)
    while ins > 0 and data[ins - 1:ins] not in (b'\n',): ins -= 1
    lines = []
    lines.append('// Naming (NF438 gen, 2026-11-25): callee-only rebind of this row\'s calls to their PDB spellings (T1 tracker; the target\n'
                 '//   self rows\' Leaf / PDB name sets), through a bodiless declaration (no layout; rulings r2/r3):\n')
    for s, P, t, mode in done_syms: lines.append('//   0x%s %s (was %s)\n' % (t.upper(), P, s))
    for fd in fwd: lines.append(fd + '\n')
    for (nss, Q), L in classes.items():
        d = 'class %s { public: %s };' % (Q, ' '.join(L))
        for n in reversed(nss): d = 'namespace %s { %s }' % (n, d)
        lines.append(d + '\n')
    for proto, s, P, t, de in protos:
        same = parse_dem(D[s])['name'] == parse_dem(D[P])['name']
        if de and not same is None:
            pass
        lines.append(proto + '\n')
    block = ''.join(lines).encode()
    # FREE stand-in declarations with the same identifier must go (redeclaration with a new return type is an error); drop every
    # pure stand-in declaration line whose only content is the declaration (+ ';' + comment)
    kills = []
    for proto, s, P, t, de in protos:
        if de: kills.append(de)
    kills += decl_kill
    ed = list(edits)
    for k0, k1 in kills:
        ls = data.rfind(b'\n', 0, k0) + 1; le = data.find(b'\n', k1); le = len(data) if le < 0 else le
        before = data[ls:k0].strip(); after = data[k1:le].decode('utf-8', 'replace').strip()
        prefix_ok = re.fullmatch(rb'(extern\s+)?(virtual\s+|static\s+)?', before) is not None or before == b''
        if prefix_ok and re.fullmatch(r';\s*(//.*)?', after):
            ed.append((ls, le + 1, ''))
        elif any(p[1] == s for p in protos for s in [p[1]]):
            pass
    # apply (descending, non-overlapping)
    ed.sort(key=lambda x: (-x[0], -x[1])); last = None; okk = True
    for x0, x1, n_ in ed:
        if last is not None and x1 > last: okk = False; break
        last = x0
    if not okk: rr('overlapping edits'); continue
    if any(x0 < ins < x1 for x0, x1, _ in ed): rr('insertion point inside an edit'); continue
    new = data
    for x0, x1, n_ in ed:
        new = new[:x0] + n_.encode() + new[x1:]
        if x0 < ins: pass
    # recompute insertion offset after edits before it
    shift = sum(len(n_.encode()) - (x1 - x0) for x0, x1, n_ in ed if x1 <= ins)
    p = ins + shift
    new = new[:p] + block + new[p:]
    os.makedirs(outp + '_rows', exist_ok=True)
    open(outp + '_rows/%s.cpp' % a, 'wb').write(new)
    for s, P, t, mode in done_syms: plan.append((a, mode, s, P, t))
with open(outp + '_PLAN.tsv', 'w') as f:
    for x in plan: f.write('\t'.join(x) + '\n')
with open(outp + '_RESIDUE.tsv', 'w') as f:
    f.write('target\twrong_symbol\trow\treason\n')
    for x in res: f.write('\t'.join(x) + '\n')
print('rows', len({x[0] for x in plan}), 'rebinds', len(plan), collections.Counter(x[1] for x in plan))
print(collections.Counter(x[3].split('(')[0][:70] for x in res).most_common(30))
