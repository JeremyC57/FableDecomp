# fam_dtor.py <family> <ov> <rows.txt>: wave 15 base-destructor families. A row names the target destructor through a
# stand-in spelling; the shapes handled:
#  (a) ??1Y@@{Q,I,U}AE@XZ  -- the PDB class itself with the wrong access / virtual-ness: the ~Y declaration in the row's class Y
#      becomes `virtual ~Y();` under the PDB access; when Y was non-virtual, a leading placeholder virtual (v00 / b000 ...) is
#      the retail slot-0 destructor and is replaced by it (no slot moves); an explicit vptr member with no uses likewise.
#  (b) ??1X@@{Q,U}AE@XZ   -- a row-local stand-in class X: renamed Y in the code (struct -> class keeps public access), then (a).
#  (c) ??1?$CIntelligentPointer@VCThing@@@@UAE@XZ (bip only) -- the template's out-of-line `virtual ~CIntelligentPointer();`
#      is removed: the implicit destructor (inline) calls ~CBaseIntelligentPointer directly, as the 551 rows without it do.
# check: the stand-ins are gone, FIN is named, every other removed callee re-appears under the class rename / the derived
# destructor's new virtual spelling (UAE / MAE), every added undefined symbol is a PDB name or such a counterpart.
import sys, re, os, collections
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import famlib2 as F, inputs, acclib
FAM = {
 'bcn': ('0099a300', '??1CBaseClassNonCopyable@@MAE@XZ', 'CBaseClassNonCopyable', 'protected'),
 'bo':  ('0099a430', '??1CBaseObject@@MAE@XZ', 'CBaseObject', 'protected'),
 'tcb': ('00686830', '??1CTCBase@@UAE@XZ', 'CTCBase', 'public'),
 'bip': ('00a01c10', '??1CBaseIntelligentPointer@@UAE@XZ', 'CBaseIntelligentPointer', 'public'),
 'cab': ('00693ef0', '??1CCreatureActionBase@@UAE@XZ', 'CCreatureActionBase', 'public'),
 'cmb': ('0062e900', '??1CCombatActionBase@@UAE@XZ', 'CCombatActionBase', 'public'),
 'scb': ('00cbd510', '??1CScriptBase@@UAE@XZ', 'CScriptBase', 'public'),
}
fam = sys.argv[1]; T, FIN, Y, ACC = FAM[fam]
BT, BS = inputs.bindings(); B = BT[T]
E, IX = inputs.index()
PDB = F.pdb()
PH = re.compile(r'^(?:v|b|h|m|s|f|vf|vfn|vfunc|slot|Slot|V|_v|unk|Unk|Vfn|vt|base|Base)_?[0-9a-fA-F]{1,3}\w{0,6}$|^vfunc\w*$|^Slot\w*$|^Unknown\w*$|^Virtual\w*$|^__vf\w*$')
VPTR = re.compile(r'(?i)(vf|vtbl|vtable|vptr|vfptr)')
def stand(a): return sorted(s for s in IX.get(a, {}).get('u', []) if s in B and s != FIN)
REN = {}
PHN = {}; CUR = [None]
RT = {'??3@YAXPAX@Z', '??_V@YAXPAX@Z', '??2@YAPAXI@Z', '__purecall'}
def mren(s, X):
    for p in ('??0', '??1', '??_G', '??_E', '??_7', '??4', '??_R4', '??_R1'):
        s = s.replace(p + X + '@', p + Y + '@')
    return s.replace('U' + X + '@@', 'V' + Y + '@@').replace('V' + X + '@@', 'V' + Y + '@@').replace('@' + X + '@@', '@' + Y + '@@').replace('?' + X + '@@', '?' + Y + '@@')
def vfix(s): return re.sub(r'^(\?\?1\w+@@)Q(AE@XZ)$', r'\1U\2', re.sub(r'^(\?\?1\w+@@)I(AE@XZ)$', r'\1M\2', s))
def depth0(c, b, e, rx):
    out = []
    for m in re.finditer(rx, c[b + 1:e]):
        pos = b + 1 + m.start(); seg = c[b + 1:pos]
        if seg.count('{') - seg.count('}') == 0 and seg.count('(') - seg.count(')') == 0: out.append((pos, b + 1 + m.end(), m))
    return out
def line_span(t, s, e):
    ls = t.rfind('\n', 0, s) + 1; le = t.find('\n', e); le = len(t) if le < 0 else le
    return ls, le
def set_stmt(t, c, kw, b, s, e, new, want):
    """replace statement t[s:e] with `new` under access `want` (wrapping labels when the current access differs)."""
    cur = acclib.access_at(c, kw, b + 1, s)
    ls, le = line_span(t, s, e)
    pre = t[ls:s]; post = t[e:le]
    alone = not pre.strip() and not F.code_only(post).strip()
    if cur == want: return t[:s] + new + t[e:]
    if alone:
        ind = re.match(r'[ \t]*', pre).group(0)
        return t[:ls] + want + ':\n' + ind + new + post + '\n' + cur + ':' + t[le:]
    return t[:s] + want + ': ' + new + ' ' + cur + ':' + t[e:]
def del_stmt(t, s, e):
    ls, le = line_span(t, s, e)
    if not t[ls:s].strip() and not F.code_only(t[e:le]).strip(): return t[:ls] + t[le + 1:]
    return t[:s] + t[e:]
def fix_dtor(t, cls, want):
    c = F.code_only(t)
    defs = F.class_defs(c, cls)
    if len(defs) != 1: return None, '%d definitions of %s' % (len(defs), cls)
    kw, s0, b, e = defs[0]
    d = depth0(c, b, e, r'(?:\bvirtual\s+)?(?:__forceinline\s+|inline\s+)?~\s*' + re.escape(cls) + r'\s*\(\s*(?:void)?\s*\)\s*(?:throw\s*\(\s*\)\s*)?([;{])')
    if len(d) != 1: return None, '%d ~%s declarations' % (len(d), cls)
    ds, de, dm = d[0]
    if dm.group(1) == '{': return None, '~%s defined inline' % cls
    isv = c[ds:de].lstrip().startswith('virtual')
    new = 'virtual ~%s();' % cls
    if isv: return set_stmt(t, c, kw, b, ds, de, new, want), 'access'
    virts = depth0(c, b, e, r'\bvirtual\b[^;{}]*?(?:;|\{)')
    if virts and virts[0][0] < ds:
        vs, ve, vm = virts[0]
        pm = re.match(r'virtual\s+void\s+(\w+)\s*\(\s*(?:void)?\s*\)\s*;$', c[vs:ve])
        if pm and PH.match(pm.group(1)) and not re.search(r'(?:\.|->)\s*' + pm.group(1) + r'\s*\(', c):
            PHN.setdefault(CUR[0], set()).add(pm.group(1))
            # placeholder slot 0 -> the destructor (statement moved there, original removed; its comment kept)
            ls, le = line_span(t, ds, de); cm = re.search(r'//\s*(.*)$', t[de:le])
            keep = ('; ' + cm.group(1).strip()) if cm else ''
            t2 = del_stmt(t, ds, de)
            c2 = F.code_only(t2); defs2 = F.class_defs(c2, cls); kw2, _, b2, e2 = defs2[0]
            v2 = depth0(c2, b2, e2, r'\bvirtual\s+void\s+' + pm.group(1) + r'\s*\(\s*(?:void)?\s*\)\s*;')
            if len(v2) != 1: return None, 'placeholder lost'
            return set_stmt(t2, c2, kw2, b2, v2[0][0], v2[0][1], new + '   // slot 0 (retail scalar deleting dtor; was placeholder %s)%s' % (pm.group(1), keep), want), 'virtual dtor replaces placeholder %s' % pm.group(1)
        return None, 'first virtual %r is not a placeholder' % c[vs:ve][:50]
    if not virts:
        base = re.search(r'\b(class|struct)\s+' + re.escape(cls) + r'\s*:', c[s0:b + 1])
        if not base:
            vp = depth0(c, b, e, r'\b(?:void|unsigned\s+long|const\s+void|\w+)\s*\*+\s*(?:const\s*)?(\w+)\s*;')
            if vp and VPTR.search(vp[0][2].group(1)) and len(re.findall(r'\b' + vp[0][2].group(1) + r'\b', c)) == 1:
                t2 = set_stmt(t, c, kw, b, ds, de, new, want)
                c2 = F.code_only(t2); kw2, _, b2, e2 = F.class_defs(c2, cls)[0]
                vp2 = depth0(c2, b2, e2, r'\b(?:void|unsigned\s+long|const\s+void|\w+)\s*\*+\s*(?:const\s*)?' + vp[0][2].group(1) + r'\s*;')
                return del_stmt(t2, vp2[0][0], vp2[0][1]), 'virtual dtor replaces explicit vptr %s' % vp[0][2].group(1)
            pd = depth0(c, b, e, r'\b(?:unsigned\s+char|char|signed\s+char)\s+(\w+)\s*\[\s*(0x[0-9a-fA-F]+|\d+)\s*\]\s*;')
            first = depth0(c, b, e, r'(?<![\w:])(?!(?:public|protected|private)\s*:)[A-Za-z_][^;{}():]*?\b\w+\s*(?:\[[^\]]*\])?\s*;')
            if pd and first and pd[0][0] == first[0][0] and len(re.findall(r'\b' + pd[0][2].group(1) + r'\b', c[b:e])) == 1 and not re.search(r'(?:\.|->)\s*' + pd[0][2].group(1) + r'\b', c) and int(pd[0][2].group(2), 0) >= 4:
                n0 = int(pd[0][2].group(2), 0); n1 = n0 - 4
                t2 = set_stmt(t, c, kw, b, ds, de, new, want)
                c2 = F.code_only(t2); kw2, _, b2, e2 = F.class_defs(c2, cls)[0]
                pd2 = depth0(c2, b2, e2, r'\b(?:unsigned\s+char|char|signed\s+char)\s+' + pd[0][2].group(1) + r'\s*\[\s*' + re.escape(pd[0][2].group(2)) + r'\s*\]\s*;')
                if n1 == 0: return del_stmt(t2, pd2[0][0], pd2[0][1]), 'virtual dtor; vptr taken from the leading pad'
                sz = ('0x%x' % n1) if pd[0][2].group(2).lower().startswith('0x') else str(n1)
                s2 = t2[pd2[0][0]:pd2[0][1]]
                s3 = s2.replace('[' + pd[0][2].group(2), '[' + sz, 1) if ('[' + pd[0][2].group(2)) in s2 else re.sub(r'\[\s*' + re.escape(pd[0][2].group(2)) + r'\s*\]', '[' + sz + ']', s2)
                ls, le = line_span(t2, pd2[0][0], pd2[0][1])
                return t2[:pd2[0][0]] + s3 + t2[pd2[0][1]:le] + ('' if '+0x04' in t2[pd2[0][1]:le] else '   // +0x04 (vptr at +0x00)') + t2[le:], 'virtual dtor; vptr taken from the leading pad (%s -> %s)' % (pd[0][2].group(2), sz)
            return None, 'no vptr in %s model' % cls
    return set_stmt(t, c, kw, b, ds, de, new, want), 'virtual added'
def tr(t, a):
    st = stand(a)
    if not st: return None, 'no stand-in named'
    msgs = []; REN[a] = []; CUR[0] = a; PHN.pop(a, None)
    for s in st:
        if s == '??1?$CIntelligentPointer@VCThing@@@@UAE@XZ' and fam == 'bip':
            c = F.code_only(t)
            m = list(re.finditer(r'(?m)^[ \t]*virtual\s+~CIntelligentPointer\s*\(\s*\)\s*;[^\n]*\n', c))
            if len(m) != 1: return None, '%d out-of-line ~CIntelligentPointer declarations' % len(m)
            t = t[:m[0].start()] + t[m[0].end():]; msgs.append('tmpl dtor removed'); continue
        m = re.match(r'^\?\?1(\w+)@@([QIUM])AE@XZ$', s)
        if not m: return None, 'shape %s' % s
        X = m.group(1)
        if X != Y:
            c = F.code_only(t)
            if not F.class_defs(c, X): return None, 'no definition of %s' % X
            if F.class_defs(c, Y):
                if not derives(c, X, Y): return None, '%s and stand-in %s both declared (%s not derived from it)' % (Y, X, X)
                dd = depth0_cls(c, X, r'(?:\bvirtual\s+)?~\s*' + re.escape(X) + r'\s*\(\s*(?:void)?\s*\)\s*;')
                if len(dd) != 1 or re.search(r'\b' + X + r'\s*::\s*~\s*' + X + r'\b', c): return None, '~%s (derived from %s) not a lone declaration' % (X, Y)
                t = del_stmt(t, dd[0][0], dd[0][1]); msgs.append('~%s removed (implicit: calls ~%s)' % (X, Y)); DREM.setdefault(a, []).append(X)
                r = fix_dtor(t, Y, ACC)
                if r[0] is None: return None, r[1]
                t = r[0]; msgs.append(r[1]); continue
            if real(X) and s not in LISTED: return None, 'stand-in %s is a PDB class not listed as a wrong spelling of %s (needs a %s base model)' % (X, T, Y)
            t = F.rename_code(t, X, Y)
            t = re.sub(r'\bstruct(\s+' + Y + r'\b[^;{]*)\{', r'class\1{ public:', t)
            t = re.sub(r'\bstruct(\s+' + Y + r'\s*;)', r'class\1', t)
            REN[a].append(X); msgs.append('%s->%s' % (X, Y))
            c = F.code_only(t); dd = F.class_defs(c, Y)
            if len(dd) == 1:
                ls, le = line_span(t, dd[0][1], dd[0][2])
                note = '// %s (Ego_r.pdb %s; was stand-in %s, naming wave 15)' % (Y, FIN, X)
                k = t.find('//', dd[0][2], le)
                t = (t[:k] + note + t[le:]) if k >= 0 else (t[:le] + '   ' + note + t[le:])
        r = fix_dtor(t, Y, ACC)
        if r[0] is None: return None, r[1]
        t = r[0]; msgs.append(r[1])
    return t, ' '.join(msgs)
NEWB = []
DREM = {}
def derives(c, X, Y, depth=0):
    if depth > 6: return False
    for kw, s0, b, e in F.class_defs(c, X):
        m = re.search(r':([^{]*)\{', c[s0:b + 1])
        if not m: continue
        for bn in re.findall(r'(\w+)\s*(?:<[^>]*>)?\s*(?:,|$)', m.group(1).strip()):
            if bn == Y or derives(c, bn, Y, depth + 1): return True
    return False
def depth0_cls(c, X, rx):
    d = F.class_defs(c, X)
    if len(d) != 1: return []
    return depth0(c, d[0][2], d[0][3], rx)
_BLOB = None
def real(X):
    global _BLOB
    if _BLOB is None: _BLOB = '\n'.join(PDB)
    return any(k in _BLOB for k in ('@' + X + '@@', '??0' + X + '@@', '??1' + X + '@@', 'V' + X + '@@'))
LISTED = set()
for q in open(inputs.W + '/QUEUE_W15_input.tsv'):
    f = q.rstrip('\n').split('\t')
    if len(f) >= 5 and inputs.na(f[1]) == T: LISTED |= set(re.findall(r'\?\?1\w+@@[A-Z]AE@XZ', f[4]))
def rn(s, a):
    for X in REN.get(a, []): s = mren(s, X)
    return s
def ck(D0, U0, D1, U1, a):
    st = set(stand(a)); hz = lambda s: '?A0x' in s
    if st & U1: return 'stand-in still named: %s' % sorted(st & U1)[:2]
    if FIN not in U1: return 'final not named'
    bad = []
    lost = [r for r in U0 - U1 if r not in st and not hz(r)]
    cps = {}
    for r in lost:
        if any(r.startswith('?%s@' % p) for p in PHN.get(a, ())): continue
        n = rn(r, a); ok = [x for x in (n, vfix(n)) if x in U1 or x in D1]
        if ok: cps[ok[0]] = r
        else: bad.append('-' + r)
    for n in U1 - U0:
        if n == FIN or hz(n) or n in PDB or n in RT or n.startswith(('??_G', '??_E')): continue
        if n in cps:
            if cps[n] in BS: bad.append('rename moves bound %s off-PDB to %s' % (cps[n][:60], n[:60])); continue
            NEWB.append((a, n, cps[n])); continue
        bad.append('+' + n)
    if bad: return 'diff %s' % bad[:4]
    return None
def leafsym(t):
    m = re.search(r'(?m)^// Leaf: (\S+)', t); return m.group(1) if m else None
def post(t, D1, U1, a):
    L = leafsym(t)
    if not L or not L.startswith('?') or L in D1: return t
    for n in (rn(L, a), vfix(rn(L, a))):
        if n in D1: return t.replace('// Leaf: ' + L, '// Leaf: ' + n, 1)
    return t   # leaf line is not a mangled symbol of this object (vland resolves by name); recheck decides
if __name__ == '__main__':
    F.run(sys.argv[2], F.rows_from(sys.argv[3]) if hasattr(F, 'rows_from') else [l.split()[0] for l in open(sys.argv[3]) if l.strip()], tr, ck, 'd' + fam, post)
    for x in NEWB: print('newbind', *x)
