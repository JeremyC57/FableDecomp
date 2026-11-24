# objsec346.py <H.obj> <N.obj> <pkdir> <addr> [leaf]: objsec HEAD v new for the REREAD port, every non-debug section keyed by the external
# symbol a COMDAT defines (else (name, ordinal); associative COMDATs by the keyed section before them), $L<n> labels by (section, value).
# Each difference is classified:
#   LEAF            the row's leaf section differs                      -> BAD
#   GETDEF_EQ       a ??$GetDef@ COMDAT changes and is EQ in N (pk_all)  -> allowed
#   GETDEF_NEQ      a ??$GetDef@ COMDAT changes and is not EQ in N       -> BAD
#   SUBDEF_EQ/NEQ   a ??$GetSubDef@ COMDAT changes                       -> its own class (not on the brief's list; ruling)
#   HELPER_BODY     a ??$SetDynamicCast@ COMDAT of CDefPointer changes, NOADDR in pk_all  -> allowed
#   HELPER_DROP     a NOADDR ??$SetDynamicCast@ / ?Assign@?$CDefPointer@ COMDAT dropped (and its DEF symbol)  -> allowed
#   OTHER           anything else (added section / symbol, other body, symbol-table change)  -> BAD
# Verdict: SAME (objects equal outside debug), OK (allowed only), SUBDEF (allowed + GetSubDef class), BAD. No bytes are printed.
import sys, os, re
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__))); import nf, objcensus
sys.path.insert(0, '/home/user/FableDecomp/tools/decomp_pipeline')
import contextlib, io
hp, np_, pkd, a = sys.argv[1:5]
def pkmap(p):
    d = {}
    if os.path.exists(p):
        for l in open(p):
            f = l.rstrip('\n').split('\t')
            if l.startswith('#') or len(f) < 10: continue
            d.setdefault(f[9], []).append((f[0], f[1], f[2]))
    return d
PH, PN = pkmap('%s/H_%s.pkall.tsv' % (pkd, a)), pkmap('%s/N_%s.pkall.tsv' % (pkd, a))
def noaddr(s, P): return s in P and all(x[1] == 'NOADDR' for x in P[s])
def eqn(s): return s in PN and all(x[0].startswith('EQ') for x in PN[s])
def symtab(p):
    secs, sl = nf.coff(p); sel = objcensus.comdat_info(p); d = {}
    for i, n, v, sec, t, scl in sl:
        if scl == 2: d[n] = ('DEF', sel.get(sec)) if sec > 0 else ('UND', None)
    return d
def keyed(p):
    secs, sl = nf.coff(p)
    lab = {nm: (sec, v) for i, nm, v, sec, t, scl in sl if re.match(r'^\$L\d+$', nm)}
    for x in secs: x['rel'] = [(o, ('$L@%s' % (lab[nm],)) if nm in lab else nm, t) for o, nm, t in x['rel']]
    first = {}
    for i, nm, v, sec, t, scl in sl:
        if scl == 2 and sec > 0 and sec not in first: first[sec] = nm
    out, cnt, last = {}, {}, None
    for k, x in enumerate(secs, 1):
        if x['name'].startswith('.debug'): continue
        if x['ch'] & 0x1000 and k in first: key = ('COMDAT', first[k], x['name']); last = key
        elif x['ch'] & 0x1000: key = ('ASSOC', last, x['name'])
        else: cnt[x['name']] = cnt.get(x['name'], 0) + 1; key = ('SEC', x['name'], cnt[x['name']]); last = key
        out[key] = x
    return out, first, sl
leaf = sys.argv[5] if len(sys.argv) > 5 else None
import verify_and_land as V
with contextlib.redirect_stderr(io.StringIO()):
    leafsym = V.obj_text(np_, leaf)[2]
    leafsym_h = V.obj_text(hp, leaf)[2]
KH, fh, slh = keyed(hp); KN, fn, sln = keyed(np_)
def leafkey(K, sl, sym):
    for i, nm, v, sec, t, scl in sl:
        if nm == sym and sec > 0:
            for k in K:
                pass
            return sec
lsec_h, lsec_n = leafkey(KH, slh, leafsym_h), leafkey(KN, sln, leafsym)
def secno(K, key, p):
    return list(K).index(key)
items = []
if leafsym != leafsym_h: items.append(('LEAF', 'leaf symbol differs %s v %s' % (leafsym_h, leafsym)))
# section index -> key, to identify the leaf's section
def idxkey(p, K):
    secs, sl = nf.coff(p); m = {}
    keys = list(K)
    # rebuild the k -> key map
    first = {}
    for i, nm, v, sec, t, scl in sl:
        if scl == 2 and sec > 0 and sec not in first: first[sec] = nm
    cnt, last = {}, None
    for k, x in enumerate(secs, 1):
        if x['name'].startswith('.debug'): continue
        if x['ch'] & 0x1000 and k in first: key = ('COMDAT', first[k], x['name']); last = key
        elif x['ch'] & 0x1000: key = ('ASSOC', last, x['name'])
        else: cnt[x['name']] = cnt.get(x['name'], 0) + 1; key = ('SEC', x['name'], cnt[x['name']]); last = key
        m[k] = key
    return m
LK = idxkey(np_, KN).get(lsec_n); LKH = idxkey(hp, KH).get(lsec_h)
if LK != LKH: items.append(('LEAF', 'leaf section key differs'))
def cls(sym, how):
    if sym.startswith('??$GetDef@'): return 'GETDEF_EQ' if eqn(sym) else 'GETDEF_NEQ'
    if sym.startswith('??$GetSubDef@'): return 'SUBDEF_EQ' if eqn(sym) else 'SUBDEF_NEQ'
    if how == 'body' and sym.startswith('??$SetDynamicCast@') and '?$CDefPointer@' in sym and (noaddr(sym, PN) or noaddr(sym, PH)): return 'HELPER_BODY'
    if how == 'drop' and (sym.startswith('??$SetDynamicCast@') or sym.startswith('?Assign@?$CDefPointer@')) and noaddr(sym, PH): return 'HELPER_DROP'
    return 'OTHER'
for key in sorted(set(KH) | set(KN), key=str):
    x, y = KH.get(key), KN.get(key)
    sym = key[1] if key[0] == 'COMDAT' else (key[1][1] if key[0] == 'ASSOC' and key[1] and key[1][0] == 'COMDAT' else str(key))
    if x is not None and y is not None:
        if (x['name'], x['ch'], x['data'], x['rel']) == (y['name'], y['ch'], y['data'], y['rel']): continue
        if key == LK: items.append(('LEAF', 'leaf section %s differs' % str(key)[:120])); continue
        c = cls(sym, 'body') if key[0] == 'COMDAT' else ('OTHER' if key[0] == 'SEC' else cls(sym, 'body'))
        items.append((c, 'changed %s %s %d->%d B' % (key[0], sym[:150], len(x['data']), len(y['data']))))
    elif y is None:
        c = cls(sym, 'drop') if key[0] in ('COMDAT', 'ASSOC') else 'OTHER'
        items.append((c, 'dropped %s %s' % (key[0], sym[:150])))
    else:
        items.append(('OTHER', 'added %s %s' % (key[0], sym[:150])))
th, tn = symtab(hp), symtab(np_)
for s in sorted(set(th) | set(tn)):
    if th.get(s) == tn.get(s): continue
    if s in th and s not in tn and th[s][0] == 'DEF' and cls(s, 'drop') == 'HELPER_DROP': continue   # its section is listed above
    items.append(('OTHER', 'symbol %s %s -> %s' % (s[:150], th.get(s), tn.get(s))))
bad = [i for i in items if i[0] in ('LEAF', 'GETDEF_NEQ', 'OTHER')]
sub = [i for i in items if i[0].startswith('SUBDEF')]
v = 'SAME' if not items else 'BAD' if bad else 'SUBDEF' if sub else 'OK'
n = {}
for c, _ in items: n[c] = n.get(c, 0) + 1
print('%s %s %s leaf=%s' % (a, v, ' '.join('%s=%d' % kv for kv in sorted(n.items())), leafsym[:100]))
for c, t in items: print('    %s :: %s' % (c, t))
