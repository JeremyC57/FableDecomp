# census2.py (NF328) <out.tsv> <obj...>: object census of the ~CScriptThing / copy-constructor unit. Per object (named H_<addr>.obj or
# <addr>.obj): for ??1CScriptThing@@UAE@XZ and ??0CScriptThing@@QAE@ABV0@@Z the state DEF/sel=<n> (1 strong, 2 pick-any) / UND / -;
# whether the object defines ??_7CScriptThing@@6B@ and with how many slots (relocations in the vftable section); and for every emitted
# member COMDAT its comparison with Fable.exe at the member's address (BYTES_EQUAL / BYTES_DIFF, relocation fields zeroed) plus the
# relocation list "offset:retail target=symbol" (BYTES_EQUAL copies only; a differing copy lists its relocation symbols only, since
# decoding retail at its offsets would print retail bytes). No bytes are printed.
import sys, io, contextlib
from pathlib import Path
sys.path.insert(0, __file__.rsplit('/', 1)[0]); import nf, objcensus
MEM = [('??1CScriptThing@@UAE@XZ', '004aa840'), ('??0CScriptThing@@QAE@ABV0@@Z', '004abe90')]
out = open(sys.argv[1], 'w')
out.write('obj\tdtor\tdtor_retail\tdtor_relocs\tcopy\tcopy_retail\tcopy_relocs\tvftable_slots\n')
for o in sys.argv[2:]:
    secs, sl = nf.coff(o); sel = objcensus.comdat_info(o); defs = {}; und = set()
    for i, n, v, sec, t, scl in sl:
        if scl != 2: continue
        if sec > 0: defs[n] = sec
        else: und.add(n)
    cols = []
    for s, ad in MEM:
        if s in defs:
            st = 'DEF/sel=%s' % sel.get(defs[s], 'plain')
            buf = io.StringIO()
            with contextlib.redirect_stdout(buf): objcensus.retail(s, ad, [o])
            p = buf.getvalue().strip().split('\t'); rel = p[3] if len(p) > 3 else ''
            if p[1] != 'BYTES_EQUAL':   # never print values decoded from retail bytes at a differing copy's offsets: symbols only
                rel = ' '.join(x.split('=', 1)[-1] for x in rel.split())
            cols += [st, p[1], rel]
        else: cols += ['UND' if s in und else '-', '', '']
    vt = '-'
    if '??_7CScriptThing@@6B@' in defs: vt = str(len(secs[defs['??_7CScriptThing@@6B@'] - 1]['rel']))
    out.write('\t'.join([Path(o).name] + cols + [vt]) + '\n')
out.close()
