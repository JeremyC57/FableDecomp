# copyproof.py (NF326) <tag> <addr...>: objsec of a respelled copy row, HEAD v new (work/pobj, one work path). Pass conditions:
#   1. the external symbol tables are equal (DEF and UND names, same COMDAT selection per DEF);
#   2. every non-debug section is equal in name, flags, bytes and relocations EXCEPT the COMDAT sections that define a unit member's PDB
#      symbol (work/watch.txt), which may change bytes only (same relocation symbols);
#   3. each such member COMDAT in the new object equals Fable.exe at the self address (relocation fields zeroed) and the self draft's
#      own COMDAT (work/obj/<self>.obj: bytes and relocation list);
#   4. .drectve equal (it is a non-debug section, so covered by 2).
# Compiler-internal $L<n> labels are compared by (section, value): their numbers shift when an inline body adds labels.
# -> evidence/copyproof_<tag>.txt (OK / NOT_OK per row with the reasons)
import sys, io, contextlib, re, os
from pathlib import Path
A = Path(__file__).resolve().parent.parent
sys.path.insert(0, str(A / 'tools')); import nf, objcensus
SELF = {'GetDefName': '004aa8d0', 'GetDataString': '004aa900', 'SetDataString': '004aa930', 'GetPos': '004aa980', 'GetAngleXY': '004aaa40',
        'MsgIsHitBySpecialAbilityFrom': '004aad20', 'MsgIsHitByAnySpecialAbilityFrom': '004aad40',
        'MsgIsHitByAnyAggressiveSpecialAbilityFrom': '004aad60', 'IsBeingCarriedBy': '004aafe0', 'IsNull': '004ab150'}
W = [l.strip() for l in open(A / 'work/watch.txt') if l.strip() and not l.startswith('??')]
DEBUG = re.compile(r'^\.debug')
def symtab(p):
    secs, sl = nf.coff(p); sel = objcensus.comdat_info(p); d = {}
    for i, n, v, sec, t, scl in sl:
        if scl == 2: d[n] = ('DEF', sel.get(sec)) if sec > 0 else ('UND', None)
    return d
def defsec(p, sym):
    secs, sl = nf.coff(p)
    for i, n, v, sec, t, scl in sl:
        if n == sym and sec > 0 and scl == 2: return sec
tag = sys.argv[1]; out = open(A / 'evidence' / ('copyproof_%s.txt' % tag), 'w')
for a in sys.argv[2:]:
    h, n = str(A / os.environ.get('OBJDIR', 'work/pobj') / ('H_' + a + '.obj')), str(A / os.environ.get('OBJDIR', 'work/pobj') / (a + '.obj'))
    bad = []; notes = []
    th, tn = symtab(h), symtab(n)
    if th != tn: bad.append('symbol tables differ: %s' % sorted(set(th.items()) ^ set(tn.items()))[:4])
    sh, lh = nf.coff(h); sn, ln = nf.coff(n)
    # compiler-internal labels ($L<n>, jump tables / switch targets) are numbered per TU: compare them by (section, value), not by name
    vh = {nm: (sec, v) for i, nm, v, sec, t, scl in lh if re.match(r'^\$L\d+$', nm)}
    vn = {nm: (sec, v) for i, nm, v, sec, t, scl in ln if re.match(r'^\$L\d+$', nm)}
    for S_, V_ in ((sh, vh), (sn, vn)):
        for x in S_: x['rel'] = [(o, ('$L@%s' % (V_[nm],)) if nm in V_ else nm, t) for o, nm, t in x['rel']]
    members = {defsec(n, w): w for w in W if defsec(n, w)}
    if len(sh) != len(sn): bad.append('section count %d v %d' % (len(sh), len(sn)))
    for k, (x, y) in enumerate(zip(sh, sn), 1):
        if DEBUG.match(x['name']): continue
        same = x['name'] == y['name'] and x['ch'] == y['ch'] and x['data'] == y['data'] and x['rel'] == y['rel']
        if same: continue
        w = members.get(k)
        if w and x['name'] == y['name'] and x['ch'] == y['ch'] and [r[1:] for r in x['rel']] == [r[1:] for r in y['rel']]:
            notes.append('changed %s' % w); continue
        bad.append('section %d %s differs%s' % (k, x['name'], (' (member %s: relocation symbols changed)' % w) if w else ''))
    for k, w in members.items():
        m = w[1:].split('@')[0]; s = SELF[m]
        buf = io.StringIO()
        with contextlib.redirect_stdout(buf): objcensus.retail(w, s, [n])
        v = buf.getvalue().split('\t')[1]
        selfobj = str(A / 'work/obj' / (s + '.obj'))
        so = nf.coff(selfobj)[0][defsec(selfobj, w) - 1]; yo = sn[k - 1]
        eqself = (so['data'], so['rel']) == (yo['data'], yo['rel'])
        notes.append('%s v retail %s, v self %s' % (m, v, 'EQUAL' if eqself else 'DIFF(bytes=%s relocs=%s)' % (so['data'] == yo['data'], so['rel'] == yo['rel'])))
        if v != 'BYTES_EQUAL': bad.append('%s copy still differs from retail' % m)
    print('%s %s %s' % (a, 'OK' if not bad else 'NOT_OK', '; '.join(bad + notes)), file=out)
out.close(); print(open(A / 'evidence' / ('copyproof_%s.txt' % tag)).read())
