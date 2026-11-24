# objsec352.py <A.obj> <B.obj> [leaf]: object diff by STABLE section keys (leafrel.py's keying: a COMDAT by the first external symbol it
# defines, else (name, ordinal among same-named sections); an associative COMDAT (.debug$S / .xdata) after its key keeps that key plus its
# own name). $L labels resolve to (section key, value). Prints every non-debug section key that differs (data or relocations), is added or
# is dropped, and the external DEF / UND symbol deltas. No bytes. Last line: VERDICT SAME | DIFF n=<count> leaf=<same|DIFF|absent>.
import sys, os
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__))); import nf
def load(p):
    secs, sl = nf.coff(p); first = {}
    for i, nm, v, sec, t, scl in sl:
        if scl == 2 and sec > 0 and sec not in first: first[sec] = nm
    key = {}; cnt = {}; last = None
    for k, x in enumerate(secs, 1):
        if x['ch'] & 0x1000 and k in first: key[k] = ('COMDAT', first[k], x['name']); last = first[k]
        elif x['ch'] & 0x1000 and last: key[k] = ('ASSOC', last, x['name'])
        else: cnt[x['name']] = cnt.get(x['name'], 0) + 1; key[k] = ('SEC', x['name'], cnt[x['name']])
    lab = {nm: (key.get(sec), v) for i, nm, v, sec, t, scl in sl if nm.startswith('$L')}
    out = {}
    for k, x in enumerate(secs, 1):
        if x['name'].startswith('.debug'): continue
        kk = key[k]
        while kk in out: kk = kk + ('dup',)
        out[kk] = (x['data'], [(o, lab.get(s, s), t) for o, s, t in x['rel']], x['ch'])
    return out, nf.ext_syms(p)
a, sa = load(sys.argv[1]); b, sb = load(sys.argv[2]); leaf = sys.argv[3] if len(sys.argv) > 3 else None
n = 0; lv = 'absent'
for k in sorted(set(a) | set(b), key=str):
    if k not in b: print('DROPPED', k); n += 1; continue
    if k not in a: print('ADDED  ', k, 'size=%d' % len(b[k][0])); n += 1; continue
    x, y = a[k], b[k]
    if x != y:
        print('CHANGED', k, 'size %d -> %d' % (len(x[0]), len(y[0])), 'data_eq=%s relocs_eq=%s flags_eq=%s' % (x[0] == y[0], x[1] == y[1], x[2] == y[2])); n += 1
    if leaf and k[0] == 'COMDAT' and k[1] == leaf: lv = 'same' if x == y else 'DIFF'
for s in sorted(sa - sb): print('SYM-', s)
for s in sorted(sb - sa): print('SYM+', s)
print('VERDICT', 'SAME' if n == 0 and sa == sb else 'DIFF n=%d symdelta=%d' % (n, len(sa ^ sb)), 'leaf=%s' % lv)
