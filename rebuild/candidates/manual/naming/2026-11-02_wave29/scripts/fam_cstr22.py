# fam_cstr22.py <ov> <rows.txt> [tag]: naming wave 22, family cstr -- DECISIONS_W21_RESOLVED "CString for the libpk error stream": every row
# that reaches the libpk CString own rows (0x00C0DEC0 ??6CString@@QAIAAV0@PBD@Z, 0x00C0DF00 ??6CString@@QAIAAV0@J@Z, 0x00C0DF10
# ??6CString@@QAIAAV0@K@Z, 0x00C0BA40 ??1CString@@QAE@XZ) or the break temporary 0x00C16750 ??0CDebugBreak@@QAE@XZ under a stand-in class
# (CDebugStream, CString_<addr>, CLogLine_<addr>, CAssertString, CTrace[_<addr>], CAssertTrace, ...) is respelled name-only: the class ->
# `class CString { public:` (the own rows' class key V), Append / AppendNumber -> the own rows' stream operator<< (chains rewritten as
# `line << a << (long)n`), the trace temporaries -> CDebugBreak. Rows file: "<row> <StandIn>[,<StandIn>...]" (the stand-in classes).
# Kept only when the object compiles and every symbol it gains is an own-row-defined / bound CString / CDebugBreak spelling, and every
# symbol it loses is a stand-in spelling.
import sys, os, re, json
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import famlib as F, inputs
ov, rf = sys.argv[1], sys.argv[2]; tag = sys.argv[3] if len(sys.argv) > 3 else 'cstr'
BT, BS = inputs.bindings()
OWN = set()
for l in open(os.path.join(F.W, 'idx', 'full.jsonl')):
    r = json.loads(l)
    if '00c08000' <= r['a'] <= '00c10400': OWN |= set(s for s in r.get('d', []) if re.search(r'@CString@@|^\?\?[0-9_][A-Z]?CString@@', s))
GOOD = OWN | set(s for s in BS if re.search(r'@CString@@|^\?\?[0-9_][A-Z]?CString@@', s) and '00c08000' <= BS[s][0] <= '00c10400') | {'??0CDebugBreak@@QAE@XZ'}
ROWS = {}
for l in open(rf):
    q = l.split()
    if q and not q[0].startswith('#'): ROWS[q[0]] = q[1].split(',')
STR = r'"(?:[^"\\]|\\.)*"|\'(?:[^\'\\]|\\.)*\''
def args_end(t, i):
    # t[i] == '(' -> index after the matching ')', string-aware
    d = 0; j = i
    while j < len(t):
        c = t[j]
        if c in '"\'':
            m = re.compile(STR).match(t, j); j = m.end(); continue
        if c == '(': d += 1
        elif c == ')':
            d -= 1
            if d == 0: return j + 1
        j += 1
    raise ValueError('unbalanced')
SIMPLE = re.compile(r'^\s*(?:' + STR + r'|[\w.\[\]>-]+(?:\([^()]*\))?|[\w:]+)\s*$')
def wrap(a, kind, nkind):
    # Append(text) -> << text; AppendNumber(n) -> << n (an integer argument cannot reach operator<<(const char*), so overload
    # resolution between operator<<(long) / (unsigned long) is the one AppendNumber had); a literal keeps its type with a suffix
    a = a.strip()
    if kind == 'AppendNumber' and re.match(r'^\d+$', a): return a + ('UL' if nkind == 'K' else 'L')
    return a if (re.match(r'^' + STR + r'$', a) or SIMPLE.match(a)) else '(' + a + ')'
def rechain(t, cls, numkind):
    # X.Append(a)->Append(b)->AppendNumber(n)  ->  X << a << b << (long)n
    out = []; pos = 0
    pat = re.compile(r'(\.|->)(Append|AppendNumber)\(')
    while True:
        m = pat.search(t, pos)
        if not m: out.append(t[pos:]); break
        # receiver: the expression before the first '.Append' of the chain
        k = m.start(); 
        rm = re.search(r'(\*?\(?[\w\[\]\.]+(?:\([^()]*\))?\)?|\b' + cls + r'\([^()]*\))$', t[pos:k])
        if not rm: out.append(t[pos:m.end()]); pos = m.end(); continue
        recv = rm.group(0); rs = pos + rm.start()
        if recv == 'this' or m.group(1) == '->': 
            recv_expr = '(*%s)' % recv if m.group(1) == '->' else recv
        else: recv_expr = recv
        parts = []; j = m.start()
        while True:
            mm = pat.match(t, j)
            if not mm: break
            e = args_end(t, mm.end() - 1)
            parts.append((mm.group(2), t[mm.end():e - 1])); j = e
        out.append(t[pos:rs])
        deref = recv_expr.startswith('*')
        if deref: recv_expr = recv_expr[1:]
        ch = recv_expr + ''.join(' << ' + wrap(a, kd, numkind.get(kd, 'J')) for kd, a in parts)
        out.append('(' + ch + ')' if deref else ch)
        pos = j
    return ''.join(out)
def transform_text(t, standins, src=True):
    t2 = t
    for c in standins:
        trace = bool(re.search(r'\bstruct\s+' + c + r'\s*\{\s*(?://[^\n]*\n\s*)?' + c + r'\(\);\s*(?://[^\n]*)?\s*\}', t2)) or c in ('CTrace', 'CAssertTrace') or c.startswith('CTrace_')
        if trace:
            if re.search(r'\bCDebugBreak\b', t2) and re.search(r'\bstruct\s+' + c + r'\b', t2):
                # drop the stand-in struct (CDebugBreak already declared)
                t2 = re.sub(r'\nstruct\s+' + c + r'\s*\{.*?\n?\};[^\n]*', '', t2, count=1, flags=re.S)
            t2 = re.sub(r'\b' + c + r'\b', 'CDebugBreak', t2)
            continue
        if re.search(r'\bCString\b', t2) and c != 'CString':
            # a second CString class would collide: only allowed when the stand-in is the file's sole string class
            if re.search(r'\b(?:struct|class)\s+CString\b', t2): raise ValueError('CString already declared')
        nk = {}
        if re.search(r'AppendNumber\(\s*unsigned long', t2) and not re.search(r'AppendNumber\(\s*long', t2): nk['AppendNumber'] = 'K'
        t2 = re.sub(r'\b' + c + r'\b', 'CString', t2)
        t2 = re.sub(r'\bstruct(\s+CString\s*)\{(?=\s*public:)', r'class\1{', t2)
        t2 = re.sub(r'\bstruct(\s+CString\s*)\{', r'class\1{ public:', t2)
        t2 = re.sub(r'\bstruct(\s+CString\b)(?!\s*\{)', r'class\1', t2)
        # declarations
        t2 = re.sub(r'(?<!const )\bCString\s*&\s*operator\s*=\s*\(\s*const CString\s*&', 'const CString& operator=(const CString&', t2)
        t2 = re.sub(r'CString\s*\*\s*__fastcall\s+Append\(\s*const char\s*\*', 'CString& __fastcall operator<<(const char*', t2)
        t2 = re.sub(r'CString\s*\*\s*__fastcall\s+AppendNumber\(\s*(unsigned long|long)\b', lambda m: 'CString& __fastcall operator<<(' + m.group(1), t2)
        if re.search(r'(\.|->)(Append|AppendNumber)\(', re.sub(r'//[^\n]*', '', t2)):
            # rewrite code lines only (comments keep their text)
            L = t2.split('\n'); 
            for i, x in enumerate(L):
                code, sep, com = x.partition('//') if not re.search(STR, x) else (x, '', '')
                if re.search(r'(\.|->)(Append|AppendNumber)\(', code) and not x.lstrip().startswith('//'):
                    L[i] = rechain(code, 'CString', {'AppendNumber': nk.get('AppendNumber', 'J')}) + sep + com
            t2 = '\n'.join(L)
    return t2
SIGIL = r'(?:\?\?[0-9]|\?\?_[A-Z]|[@?UV])'
def standin_sym(x, standins):
    return any(re.search(SIGIL + re.escape(c) + r'@', x) for c in standins)
def renamed_ctor(x, rem, standins):
    # an inline (non-retail) CString / CDebugBreak constructor that is the class rename of a removed stand-in constructor
    m = re.match(r'^\?\?0(CString|CDebugBreak)@@(.*)$', x)
    return bool(m) and any(('??0%s@@%s' % (c, m.group(2))) in rem for c in standins)
def renamed_param(x, rem, standins):
    # a callee that takes / returns the string: the class rename of a removed symbol (the stand-in in a parameter / return
    # position, struct key U -> class key V); listed in BINDINGS as the respelled callee
    if 'VCString@@' not in x: return False
    return any(x.replace('VCString@@', 'U%s@@' % c) in rem or x.replace('VCString@@', 'V%s@@' % c) in rem for c in standins)
def check(S0, S1, a):
    st = ROWS[a]
    rem = [s for s in S0 - S1 if not F.hashy(s) and not standin_sym(s, st) and not s.startswith('??_C@')]
    add = [s for s in S1 - S0 if not F.hashy(s) and s not in GOOD and not s.startswith('??_C@') and not renamed_ctor(s, S0 - S1, st) and not renamed_param(s, S0 - S1, st)]
    if rem or add: return 'diff: -%s +%s' % (rem[:4], add[:4])
    return None
def transform(t, a):
    try: t2 = transform_text(t, ROWS[a])
    except ValueError as e: return None, str(e)
    return t2, ','.join(ROWS[a]) + ' -> CString / CDebugBreak'
for a in ROWS:
    if a not in F.R: print('skip', a, 'not a landed row'); continue
    p = F.src_path(ov, a); t0 = F.rd(p)
    S0, _ = F.objsyms(a, p, tag)
    if S0 is None: print('skip', a, 'compile fails before'); continue
    t1, why = transform(t0, a)
    if t1 is None: print('skip', a, why); continue
    if t1 == t0: print('already', a, why); continue
    F.wr(p, t1); S1, err = F.objsyms(a, p, tag)
    bad = ('compile: ' + ' | '.join(x[-170:] for x in err)) if S1 is None else check(S0, S1, a)
    if bad: F.wr(p, t0); print('skip', a, bad); sys.stdout.flush(); continue
    # the test source: identifier renames / chains only when it names a stand-in
    tp_rel = F.R[a].get('TestSource')
    if tp_rel:
        tsrc = os.path.join(F.ROOT, 'rebuild/tests', tp_rel)
        if os.path.exists(tsrc) and any(re.search(r'\b' + c + r'\b', F.rd(tsrc)) for c in ROWS[a]):
            tp = F.src_path(ov, a, 'tst'); tt = F.rd(tp)
            # the test is a standalone behavioural model: identifiers only (its own Append bodies stay)
            if re.search(r'\b(?:struct|class)\s+CString\b', tt) and any(not (c.startswith('CTrace') or c == 'CAssertTrace') for c in ROWS[a] if re.search(r'\b' + c + r'\b', tt) and c != 'CString'):
                print('note', a, 'test keeps its names (CString already declared there)')
            else:
                for c in ROWS[a]:
                    if c == 'CString': continue
                    tt = re.sub(r'\b' + c + r'\b', 'CDebugBreak' if (c.startswith('CTrace') or c == 'CAssertTrace') else 'CString', tt)
                F.wr(tp, tt)
    print('edited', a, why, '+', sorted(x for x in S1 - S0 if not x.startswith('??_C'))[:6], '-', sorted(x for x in S0 - S1 if not x.startswith('??_C'))[:6])
    sys.stdout.flush()
