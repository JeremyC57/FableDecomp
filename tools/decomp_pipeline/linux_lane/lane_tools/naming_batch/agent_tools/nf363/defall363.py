# defall363.py <label=obj>...: every external symbol defined by two or more of the objects: per definer the COMDAT selection, size and a
# body key (bytes + relocation symbol names), the number of distinct bodies, and whether a strong (sel 0/1) definer sits beside another
# definer (an LNK2005). Differing pick-any bodies link silently under /FORCE (first wins), so they are listed as DIFF_BODIES. (NF363)
import sys, os, struct, hashlib, collections
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__))); import nf
defs = collections.defaultdict(list)
for spec in sys.argv[1:]:
    lab, obj = spec.split('=', 1)
    d = open(obj, 'rb').read(); psym, nsym = struct.unpack_from('<II', d, 8); sel = {}; i = 0
    while i < nsym:
        o = psym + 18 * i; v, sec, t, scl, na = struct.unpack_from('<IhHBB', d, o + 8)
        if scl == 3 and v == 0 and na == 1 and sec > 0 and sec not in sel: sel[sec] = d[o + 18 + 14]
        i += 1 + na
    secs, sl = nf.coff(obj)
    for i, n, v, sec, t, scl in sl:
        if scl == 2 and sec > 0:
            s = secs[sec - 1]; key = hashlib.sha1(s['data'] + repr(s['rel']).encode()).hexdigest()[:8]
            defs[n].append((lab, sel.get(sec, 0), len(s['data']), key))
for n, L in sorted(defs.items()):
    if len(L) < 2: continue
    bodies = len({k for _, _, _, k in L}); strong = sum(1 for _, s, _, _ in L if s in (0, 1))
    flag = ('LNK2005 ' if strong and len(L) > 1 else '') + ('DIFF_BODIES' if bodies > 1 else 'same_body')
    print('%s\t%s\t%s' % (n[:90], flag, ' | '.join('%s sel=%s %dB %s' % x for x in L)))
