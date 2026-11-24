"""objcmp369.py A.obj B.obj: section-by-section compare of two COFF objects (NF369).
Key = (section name, defining external symbol, or the first named symbol for a static section, plus an ordinal for repeats).
Per section: raw bytes, relocation list (offset, target symbol name, type), characteristics, COMDAT selection (+ associated section key).
Prints one line per section (SAME / DIFF <what> / ONLY_A / ONLY_B), then .drectve tokens, DEF / UND set differences.
.debug$S / .debug$T sections are listed but flagged as debug (allowed to differ). Names and verdicts only, no bytes."""
import sys, struct


def load(p):
    d = open(p, 'rb').read()
    ns = struct.unpack_from('<H', d, 2)[0]
    psym, nsym = struct.unpack_from('<II', d, 8)
    oh = struct.unpack_from('<H', d, 16)[0]
    strt = d[psym + 18 * nsym:]

    def nm(raw):
        if raw[:4] == b'\0\0\0\0':
            o = struct.unpack_from('<I', raw, 4)[0]
            return strt[o:strt.index(b'\0', o)].decode('latin1')
        return raw.rstrip(b'\0').decode('latin1')

    syms = {}; sel = {}; assoc = {}; ext = {}; anyn = {}; und = set(); dfn = set(); i = 0
    while i < nsym:
        o = psym + 18 * i
        v, sec, t, scl, na = struct.unpack_from('<IhHBB', d, o + 8)
        n = nm(d[o:o + 8]); syms[i] = n
        if scl == 3 and v == 0 and na == 1 and sec > 0 and sec not in sel:
            aux = d[o + 18:o + 36]
            sel[sec] = aux[14]
            assoc[sec] = struct.unpack_from('<H', aux, 12)[0]
        if scl == 2 and sec > 0:
            ext.setdefault(sec, n); dfn.add(n)
        if scl == 2 and sec == 0:
            und.add(n)
        if sec > 0 and scl in (2, 3) and not n.startswith('.'):
            anyn.setdefault(sec, n)
        i += 1 + na
    secs = {}; order = []; seen = {}
    names = {}
    for k in range(ns):
        h = d[20 + oh + 40 * k:20 + oh + 40 * k + 40]
        name = h[:8].rstrip(b'\0').decode('latin1')
        if name.startswith('/'):
            off = int(name[1:]); name = strt[off:strt.index(b'\0', off)].decode('latin1')
        sz, ptr, rp, _, nr = struct.unpack_from('<IIIIH', h, 16)
        ch = struct.unpack_from('<I', h, 36)[0]
        data = d[ptr:ptr + sz] if ptr else b''
        rel = [(struct.unpack_from('<I', d, rp + j * 10)[0], syms.get(struct.unpack_from('<I', d, rp + j * 10 + 4)[0]),
                struct.unpack_from('<H', d, rp + j * 10 + 8)[0]) for j in range(nr)]
        base = (name, ext.get(k + 1, '(static) ' + anyn.get(k + 1, '-')))
        c = seen.get(base, 0); seen[base] = c + 1
        key = base + (c,)
        names[k + 1] = key
        secs[key] = [data, rel, ch, sel.get(k + 1), assoc.get(k + 1)]
        order.append(key)
    for key in secs:  # associated section -> its key, so the compare is index-free
        s = secs[key]
        if s[3] == 5 and s[4]:
            s[4] = names.get(s[4])
        else:
            s[4] = None
    return secs, order, und, dfn


A = load(sys.argv[1]); B = load(sys.argv[2])
keys = A[1] + [k for k in B[1] if k not in A[0]]
nondebug_diff = 0
for k in keys:
    a = A[0].get(k); b = B[0].get(k)
    dbg = k[0].startswith('.debug')
    tag = ' (debug)' if dbg else ''
    label = '%-9s %s%s' % (k[0], k[1][:100], ('#%d' % k[2]) if k[2] else '')
    if a is None or b is None:
        print('%s %s%s' % ('ONLY_B' if a is None else 'ONLY_A', label, tag))
        if not dbg: nondebug_diff += 1
        continue
    what = []
    if a[0] != b[0]: what.append('bytes(%d v %d)' % (len(a[0]), len(b[0])))
    if a[1] != b[1]: what.append('relocs(%d v %d)' % (len(a[1]), len(b[1])))
    if a[2] != b[2]: what.append('ch(%08x v %08x)' % (a[2], b[2]))
    if a[3] != b[3]: what.append('sel(%s v %s)' % (a[3], b[3]))
    if a[4] != b[4]: what.append('assoc')
    if what:
        print('DIFF   %s %s%s' % (label, ' '.join(what), tag))
        if not dbg: nondebug_diff += 1
    else:
        print('SAME   %s sel=%s size=%d nrel=%d%s' % (label, a[3], len(a[0]), len(a[1]), tag))


def drectve(X):
    return [X[0][k][0] for k in X[1] if k[0] == '.drectve']


print('DRECTVE_A', [x.decode('latin1').split() for x in drectve(A)])
print('DRECTVE_B', [x.decode('latin1').split() for x in drectve(B)])
print('UND only A', sorted(A[2] - B[2])); print('UND only B', sorted(B[2] - A[2]))
print('DEF only A', sorted(A[3] - B[3])); print('DEF only B', sorted(B[3] - A[3]))
print('VERDICT', 'ALL_NONDEBUG_SAME' if nondebug_diff == 0 else 'NONDEBUG_DIFF %d' % nondebug_diff)
