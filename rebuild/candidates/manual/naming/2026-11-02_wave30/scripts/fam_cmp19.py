# fam_cmp19.py <ov> <rows.txt> [tag]: CBasicString<char>::Compare -- the PDB member is the protected static ?Compare@?$CBasicString@D@@KIJPBD0@Z
# (Ego_r / FableWin: `static long __fastcall Compare(const char*, const char*)`, called by the inline CBasicString operators). The landed
# rows declare a free __fastcall stand-in (Compare_00411570 / Compare / CompareStrings ...) and call it from the inline comparison
# operators of their row-local string class (CCharString). Name-only respell, for the rows whose every call of the stand-in sits in an
# inline member of one base-less string class S: S's data members and the members that call the stand-in move into
# `template <class T> class CBasicString` (protected static Compare), S derives from CBasicString<char> and keeps its other members
# (its retail-bound constructors / destructor keep their CCharString mangling). check: the stand-in is gone, Compare is named, nothing else.
import sys, re, os
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import famlib2 as F, inputs
from memlib import depth_at
ov, rf = sys.argv[1], sys.argv[2]; TAG = sys.argv[3] if len(sys.argv) > 3 else 'cmp'
T = '00411570'; FIN = '?Compare@?$CBasicString@D@@KIJPBD0@Z'
BT, BS = inputs.bindings(); B = set(BT.get(T, ()))
IXU = {}
import json
for l in open(inputs.W + '/idx/full.jsonl'):
    r = json.loads(l); IXU[r['a']] = (set(r.get('u', [])), set(r.get('d', [])))
DECLRE = re.compile(r'(?m)^[ \t]*(?:extern[ \t]+)?(?:long|int)[ \t]+__fastcall[ \t]+(\w+)[ \t]*\([ \t]*const[ \t]+char[ \t]*\*[ \t]*\w*[ \t]*,[ \t]*const[ \t]+char[ \t]*\*[ \t]*\w*[ \t]*\)[ \t]*;[^\n]*\n')
def members(c, b, e):
    # top-level statements of a class body: list of (start, end, is_func, text)
    out = []; i = b + 1; dep = 0; st = None
    while i < e:
        ch = c[i]
        if st is None and not ch.isspace(): st = i
        if ch == '{': dep += 1
        elif ch == '}':
            dep -= 1
            if dep == 0 and st is not None:
                # inline body ends; an optional ';'
                j = i + 1
                while j < e and c[j] in ' \t': j += 1
                if j < e and c[j] == ';': i = j
                out.append((st, i + 1, True)); st = None
        elif ch == ';' and dep == 0 and st is not None:
            out.append((st, i + 1, '(' in c[st:i])); st = None
        elif ch == ':' and dep == 0 and st is not None and re.match(r'(public|protected|private)\s*$', c[st:i]):
            out.append((st, i + 1, 'label')); st = None
        i += 1
    return out
def tr(t, a):
    u, d = IXU.get(a, (set(), set()))
    st = sorted(s for s in u if s in B and s != FIN)
    if not st: return None, 'no stand-in named'
    if any(s in d for s in st): return None, 'the row defines the stand-in'
    c = F.code_only(t)
    dm = list(DECLRE.finditer(c))
    if len(dm) != 1: return None, '%d free Compare-shaped declarations' % len(dm)
    N = dm[0].group(1)
    calls = [m for m in re.finditer(r'(?<![\w$])' + re.escape(N) + r'\s*\(', c) if m.start() != dm[0].start() and not c[dm[0].start():dm[0].end()].__contains__(c[m.start():m.end()]) or m.start() < dm[0].start() or m.start() > dm[0].end()]
    calls = [m for m in calls if not (dm[0].start() <= m.start() < dm[0].end())]
    if not calls: return None, 'no calls of %s' % N
    # the class whose inline members hold every call
    owner = None
    for m in re.finditer(r'\b(class|struct)\s+(\w+)\b(\s*:[^;{]*)?\s*\{', c):
        b = m.end() - 1; e = F.blk_end(c, b)
        if all(b < x.start() < e for x in calls):
            if m.group(3): return None, '%s already derives (%s)' % (m.group(2), m.group(3).strip()[:40])
            if depth_at(c, m.start()): return None, '%s nested in a scope' % m.group(2)
            owner = (m.group(1), m.group(2), m.start(), b, e); break
    if not owner: return None, 'calls of %s outside one class (free helper / free operator shape)' % N
    kw, S, s0, b, e = owner
    if F.class_defs(c, 'CBasicString'): return None, 'CBasicString already defined'
    ms = members(c, b, e)
    data = []; movers = []; keep = []; acc = 'private' if kw == 'class' else 'public'
    for ms_, me_, kind in ms:
        txt = t[ms_:me_]; ctxt = c[ms_:me_]
        if kind == 'label': acc = ctxt.split(':')[0].strip(); keep.append((ms_, me_)); continue
        if kind is True or kind == True:
            if re.search(r'(?<![\w$])' + re.escape(N) + r'\s*\(', ctxt): movers.append((ms_, me_, acc)); continue
            keep.append((ms_, me_)); continue
        # a declaration statement: data member when there is no '(' (arrays / bitfields allowed), else a member function declaration
        if '(' not in ctxt and not re.match(r'\s*(typedef|friend|using|enum|static)\b', ctxt): data.append((ms_, me_, acc)); continue
        keep.append((ms_, me_))
    if not movers: return None, 'no inline member of %s calls %s' % (S, N)
    if any(acc_ != 'public' for _, _, acc_ in data): return None, 'non-public data member'
    # build the template base
    def retext(x):
        x = re.sub(r'(?<![\w$])' + re.escape(N) + r'\s*\(', 'Compare(', x)
        x = re.sub(r'(?<![\w$])' + re.escape(S) + r'(?![\w$])', 'CBasicString', x)
        return x
    ind = '    '
    base = ['template <class T> class CBasicString { public:                       // Ego_r.pdb: the string base of %s (data and inline comparisons)' % S]
    for ms_, me_, _ in data: base.append(ind + t[ms_:me_].strip())
    base.append('protected:')
    base.append(ind + 'static long __fastcall Compare(const T* a, const T* b);   // 0x00411570 (Ego_r.pdb ?Compare@?$CBasicString@D@@KIJPBD0@Z, protected static)')
    base.append('public:')
    for ms_, me_, acc_ in movers:
        body = t[ms_:me_]
        lead = re.match(r'[ \t]*', t[t.rfind('\n', 0, ms_) + 1:ms_]).group(0)
        base.append(ind + retext(body).replace('\n' + lead, '\n' + ind) if lead else ind + retext(body))
    base.append('};')
    basetxt = '\n'.join(base) + '\n'
    # rebuild S without the moved pieces, deriving from CBasicString<char>
    rm = sorted([(ms_, me_) for ms_, me_, _ in data] + [(ms_, me_) for ms_, me_, _ in movers])
    body = ''; last = b + 1
    for ms_, me_ in rm:
        seg = t[last:ms_]
        body += seg; last = me_
        # drop the rest of the line when only whitespace / comment follows
        nl = t.find('\n', last)
        if nl >= 0 and not F.code_only(t[last:nl]).strip(): last = nl + 1
        else: pass
        # and the leading indentation of the removed statement
        ls = body.rfind('\n') + 1
        if not body[ls:].strip(): body = body[:ls]
    body += t[last:e]
    hdr = t[s0:b]
    hdr2 = re.sub(r'\b' + re.escape(S) + r'\b', S + ' : public CBasicString<char>', hdr, count=1)
    newS = hdr2 + '{' + body + '}'
    t2 = t[:dm[0].start()] + basetxt + t[dm[0].end():s0] + newS + t[e + 1:]
    # the decl may sit after the class: handle order (decl before class is the usual shape)
    if dm[0].start() > s0: return None, 'stand-in declared after %s' % S
    t2 = re.sub(r'(?<![\w$])' + re.escape(N) + r'(?![\w$])', 'CBasicString<char>::Compare', t2)   # comments
    return t2, 'CBasicString<char> base of %s: %d data member(s), %d member(s) moved' % (S, len(data), len(movers))
def ck(D0, U0, D1, U1, a):
    st = {s for s in U0 if s in B and s != FIN}
    if st & U1: return 'stand-in still named'
    if FIN not in U1: return 'final not named'
    hz = lambda s: '?A0x' in s
    rem = [s for s in U0 - U1 if s not in st and not hz(s)]
    add = [s for s in U1 - U0 if s != FIN and not hz(s)]
    if rem or add: return 'diff -%s +%s' % (rem[:3], add[:3])
    return None
F.run(ov, [l.split()[0] for l in open(rf) if l.strip()], tr, ck, TAG)
