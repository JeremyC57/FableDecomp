# census2.py (NF328 form, generalised by NF337) <out.tsv> <addr>:<leaf>:<obj> ...: object census of the def-family operator= rows. Per object:
# the row's own leaf (its ??4C) state DEF/sel=<n> (1 = no-duplicates / strong, 2 = pick-any) / UND / -; that COMDAT v Fable.exe at the row's
# address (BYTES_EQUAL / BYTES_DIFF with relocation fields zeroed) and its relocation symbols; and every other ?Copy@ / ??4 symbol the
# object DEFINES with its selection (scaffolding / forcing devices). No bytes are printed.
import sys, io, contextlib
sys.path.insert(0, __file__.rsplit('/', 1)[0]); import nf, objcensus
out = open(sys.argv[1], 'w')
out.write('address\tleaf\tleaf_state\tretail\trelocs\tother_defs\n')
for spec in sys.argv[2:]:
    ad, leaf, o = spec.split(':', 2)
    secs, sl = nf.coff(o); sel = objcensus.comdat_info(o); defs = {}; und = set()
    for i, n, v, sec, t, scl in sl:
        if scl != 2: continue
        if sec > 0: defs[n] = sec
        else: und.add(n)
    if leaf in defs:
        st = 'DEF/sel=%s' % sel.get(defs[leaf], 'plain')
        buf = io.StringIO()
        with contextlib.redirect_stdout(buf): objcensus.retail(leaf, ad, [o])
        p = buf.getvalue().strip().split('\t'); rel = ' '.join(x.split('=', 1)[-1] for x in (p[3] if len(p) > 3 else '').split())
        cols = [st, p[1], rel]
    else: cols = ['UND' if leaf in und else '-', '', '']
    other = ['%s(sel=%s)' % (n, sel.get(s, 'plain')) for n, s in sorted(defs.items()) if n != leaf and (n.startswith('?Copy@') or n.startswith('??4'))]
    out.write('\t'.join([ad, leaf] + cols + [' '.join(other) or '-']) + '\n')
out.close()
