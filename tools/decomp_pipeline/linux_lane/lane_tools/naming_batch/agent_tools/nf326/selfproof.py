# selfproof.py (NF326) <tag> <addr...>: objsec of a pick-any self draft, HEAD v new (work/pobj, one work path). Reports, per row:
#   - the HEAD self symbol (the stand-in) and its COMDAT selection; the new PDB symbol and its selection (2 = pick-any expected);
#   - the self function's bytes and relocation list (relocation symbols compared by name) HEAD v new: must be EQUAL;
#   - the new object's member COMDAT v Fable.exe at the address (relocation fields zeroed): must be BYTES_EQUAL;
#   - DEF set delta (expected: stand-in -> PDB symbol, plus the one scaffolding helper in the TU's ?A0x namespace);
#   - UND set delta (expected: none); .drectve equal; no ??_7 / ??_R in the new object; every other non-debug section equal.
# -> evidence/selfproof_<tag>.txt
import sys, io, contextlib, re, os
from pathlib import Path
A = Path(__file__).resolve().parent.parent
sys.path.insert(0, str(A / 'tools')); import nf, objcensus
LEAF = {}
for l in open(A / 'tools/mkpick.py').read().splitlines():
    m = re.match(r"\s*'([0-9a-f]{8})': \('(\w+)', '([^']+)'", l)
    if m: LEAF[m.group(1)] = m.group(3)
DEBUG = re.compile(r'^\.debug')
def funcs(p):
    secs, sl = nf.coff(p); sel = objcensus.comdat_info(p); d = {}
    for i, n, v, sec, t, scl in sl:
        if scl == 2 and sec > 0 and secs[sec - 1]['name'].startswith('.text'): d[n] = (sec, sel.get(sec))
    return secs, d
tag = sys.argv[1]; out = open(A / 'evidence' / ('selfproof_%s.txt' % tag), 'w')
for a in sys.argv[2:]:
    h, n = str(A / os.environ.get('OBJDIR', 'work/pobj') / ('H_' + a + '.obj')), str(A / os.environ.get('OBJDIR', 'work/pobj') / (a + '.obj'))
    leaf = LEAF[a]; bad = []
    sh, fh = funcs(h); sn, fn = funcs(n)
    old = [s for s in fh if s not in fn]
    if leaf not in fn: bad.append('new object does not define %s' % leaf)
    helper = [s for s in fn if s not in fh and s != leaf]
    if len(old) != 1: bad.append('HEAD self symbol not unique: %s' % old)
    if len(helper) != 1 or '?A0x' not in helper[0]: bad.append('unexpected new definitions: %s' % helper)
    if old and leaf in fn:
        x, y = sh[fh[old[0]][0] - 1], sn[fn[leaf][0] - 1]
        if x['data'] != y['data']: bad.append('self bytes differ')
        if [r[1:] for r in x['rel']] != [r[1:] for r in y['rel']] or [r[0] for r in x['rel']] != [r[0] for r in y['rel']]: bad.append('self relocations differ')
        if fn[leaf][1] != 2: bad.append('new selection %s, not pick-any' % fn[leaf][1])
    eh, en = nf.ext_syms(h), nf.ext_syms(n)
    uh = {s for s in eh if s.startswith('UND ')}; un = {s for s in en if s.startswith('UND ')}
    if uh != un: bad.append('UND delta %s' % sorted(uh ^ un))
    if any(s.startswith('DEF ??_7') or s.startswith('DEF ??_R') for s in en): bad.append('vftable / RTTI emitted')
    dh = [s for s in sh if s['name'] == '.drectve']; dn = [s for s in sn if s['name'] == '.drectve']
    if [s['data'] for s in dh] != [s['data'] for s in dn]: bad.append('.drectve differs')
    # every other non-debug, non-function-COMDAT section: compare as a multiset of (name, flags, bytes, relocs)
    skip_h = {fh[s][0] for s in old}; skip_n = {fn[leaf][0]} | {fn[s][0] for s in helper} if leaf in fn else set()
    rest = lambda S, skip: sorted((s['name'], s['ch'], s['data'], tuple(s['rel'])) for k, s in enumerate(S, 1) if k not in skip and not DEBUG.match(s['name']) and s['name'] != '.drectve')
    if rest(sh, skip_h) != rest(sn, skip_n): bad.append('other sections differ')
    buf = io.StringIO()
    with contextlib.redirect_stdout(buf): objcensus.retail(leaf, a, [n])
    v = buf.getvalue().split('\t')[1] if '\t' in buf.getvalue() else 'NODEF'
    if v != 'BYTES_EQUAL': bad.append('member COMDAT v retail %s' % v)
    print('%s %s HEAD=%s(sel %s) NEW=%s(sel %s) helper=%s retail=%s %s' % (a, 'OK' if not bad else 'NOT_OK', old[0] if old else '?',
          fh[old[0]][1] if old else '?', leaf, fn.get(leaf, (0, '?'))[1], helper[0] if helper else '-', v, '; '.join(bad)), file=out)
out.close(); print(open(A / 'evidence' / ('selfproof_%s.txt' % tag)).read())
