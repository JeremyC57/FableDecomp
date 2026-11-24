# census_op.py (NF331, from NF328's census2.py) <out.tsv> <obj...>: object census of the CScriptThing::operator= unit. Per object (named
# H_<addr>.obj or <addr>.obj): for ??4CScriptThing@@QAEAAV0@ABV0@@Z the state DEF/sel=<n> (1 strong, 2 pick-any) / UND / -; for a DEF its
# comparison with Fable.exe at 0x008AB980 (BYTES_EQUAL / BYTES_DIFF, relocation fields zeroed), its length, and the relocation list
# "offset:retail target=symbol" (BYTES_EQUAL copies only; a differing copy lists its relocation symbols only, so no value decoded from
# retail bytes at a differing copy's offsets is printed); whether another section of the object references the symbol (called / slot);
# and the ??_7CScriptThing@@6B@ slot count when the object defines it. No bytes are printed.
import sys, io, contextlib
from pathlib import Path
sys.path.insert(0, __file__.rsplit('/', 1)[0]); import nf, objcensus
SYM, AD = '??4CScriptThing@@QAEAAV0@ABV0@@Z', '008ab980'
out = open(sys.argv[1], 'w')
out.write('obj\top_assign\tretail\tlen\trelocs\treferenced\tvftable_slots\n')
for o in sys.argv[2:]:
    secs, sl = nf.coff(o); sel = objcensus.comdat_info(o); defs = {}; und = set()
    for i, n, v, sec, t, scl in sl:
        if scl != 2: continue
        if sec > 0: defs[n] = sec
        else: und.add(n)
    ref = any((k + 1) != defs.get(SYM) and any(r[1] == SYM for r in s['rel']) for k, s in enumerate(secs) if not s['name'].startswith('.debug'))
    if SYM in defs:
        st = 'DEF/sel=%s' % sel.get(defs[SYM], 'plain')
        buf = io.StringIO()
        with contextlib.redirect_stdout(buf): objcensus.retail(SYM, AD, [o])
        p = buf.getvalue().strip().split('\t'); rel = p[3] if len(p) > 3 else ''
        if p[1] != 'BYTES_EQUAL': rel = ' '.join(x.split('=', 1)[-1] for x in rel.split())
        cols = [st, p[1], p[2].replace('len=', ''), rel]
    else: cols = ['UND' if SYM in und else '-', '', '', '']
    vt = '-'
    if '??_7CScriptThing@@6B@' in defs: vt = str(len(secs[defs['??_7CScriptThing@@6B@'] - 1]['rel']))
    out.write('\t'.join([Path(o).name] + cols + ['ref' if ref else '-', vt]) + '\n')
out.close()
