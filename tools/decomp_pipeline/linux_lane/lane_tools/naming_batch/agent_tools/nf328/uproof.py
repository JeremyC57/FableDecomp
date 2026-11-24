# uproof.py (NF328) <tag> <addr...>: objsec HEAD v new (work/pobj/H_<a>.obj v work/pobj/<a>.obj, compiled at ONE work path by
# pairgrade.py) for the ~CScriptThing / copy-constructor unit. NF326's selfproof / copyproof assume a stand-in self name and no vftable;
# here the self keeps its (PDB) name and every object already emits the CScriptThing vftable, so the checks are:
#   - symbol tables: DEF / UND names equal, except (a) at most ONE new DEF, which must be a ?A0x anonymous-namespace helper, and
#     (b) the unit members' COMDAT selection may go 1 -> 2 (strong -> pick-any); every other selection equal;
#   - a unit member defined at HEAD must still be defined (dropping a COMDAT is a stop, NF318 Q5);
#   - every non-debug section (.drectve, ??_7 / ??_R / ??_G COMDATs included) equal in name, flags, bytes and relocations, as a multiset,
#     except the member sections (selection change allowed; bytes / relocations compared and reported) and the helper section;
#     $L<n> labels compared by (section, value);
#   - each unit member COMDAT in the new object v Fable.exe at the member's address (relocation fields zeroed; BYTES_EQUAL / DIFF)
#     and its relocation list "offset:retail target=symbol" (so a copy that binds a stand-in at a retail address shows up).
# -> evidence/uproof_<tag>.txt. No bytes are printed.
import sys, re, os, io, contextlib
from pathlib import Path
A = Path(__file__).resolve().parent.parent
sys.path.insert(0, str(A / 'tools')); import nf, objcensus
MEM = {'??1CScriptThing@@UAE@XZ': '004aa840', '??0CScriptThing@@QAE@ABV0@@Z': '004abe90'}
DEBUG = re.compile(r'^\.debug')
def load(p):
    secs, sl = nf.coff(p); sel = objcensus.comdat_info(p)
    lab = {nm: (sec, v) for i, nm, v, sec, t, scl in sl if re.match(r'^\$L\d+$', nm)}
    for x in secs: x['rel'] = [(o, ('$L@%s' % (lab[nm],)) if nm in lab else nm, t) for o, nm, t in x['rel']]
    tab = {}; secof = {}
    for i, n, v, sec, t, scl in sl:
        if scl == 2:
            tab[n] = ('DEF', sel.get(sec)) if sec > 0 else ('UND', None)
            if sec > 0: secof[n] = sec
    return secs, tab, secof
def retail(sym, addr, obj):
    buf = io.StringIO()
    with contextlib.redirect_stdout(buf): objcensus.retail(sym, addr, [obj])
    p = buf.getvalue().strip().split('\t')
    v, rel = (p[1] if len(p) > 1 else 'NODEF'), (p[3] if len(p) > 3 else '')
    if v != 'BYTES_EQUAL': rel = ' '.join(x.split('=', 1)[-1] for x in rel.split())   # symbols only: no values decoded from retail bytes
    return v, rel
def proof(a, objdir):
    h, n = str(objdir / ('H_' + a + '.obj')), str(objdir / (a + '.obj'))
    sh, th, ch = load(h); sn, tn, cn = load(n); bad = []; notes = []
    newdefs = [s for s in tn if s not in th]; gone = [s for s in th if s not in tn]
    helper = [s for s in newdefs if tn[s][0] == 'DEF' and '?A0x' in s]
    if gone: bad.append('symbols gone: %s' % gone[:4])
    if len(helper) > 1 or len(newdefs) != len(helper): bad.append('new symbols: %s' % newdefs[:4])
    for s in th:
        if s in tn and th[s] != tn[s]:
            if s in MEM and th[s] == ('DEF', 1) and tn[s] == ('DEF', 2): notes.append('%s sel 1->2' % s[:3])
            else: bad.append('%s %s -> %s' % (s, th[s], tn[s]))
    skip_h = {ch[s] for s in MEM if s in ch}; skip_n = {cn[s] for s in MEM if s in cn} | {cn[s] for s in helper}
    rest = lambda S, skip: sorted((s['name'], s['ch'], s['data'], tuple(s['rel'])) for k, s in enumerate(S, 1) if k not in skip and not DEBUG.match(s['name']))
    if rest(sh, skip_h) != rest(sn, skip_n): bad.append('other non-debug sections differ')
    for s, ad in MEM.items():
        if s in ch and s in cn:
            x, y = sh[ch[s] - 1], sn[cn[s] - 1]
            if (x['data'], x['rel']) != (y['data'], y['rel']): notes.append('%s bytes changed (len %d -> %d)' % (s[:3], len(x['data']), len(y['data'])))
        if s in cn:
            v, rel = retail(s, ad, n); notes.append('%s v retail %s [%s]' % (s[:3], v, rel))
    return ('OK' if not bad else 'NOT_OK'), bad, notes, helper
if __name__ == '__main__':
    tag = sys.argv[1]; od = A / os.environ.get('OBJDIR', 'work/pobj'); out = open(A / 'evidence' / ('uproof_%s.txt' % tag), 'w')
    for a in sys.argv[2:]:
        st, bad, notes, helper = proof(a, od)
        print('%s %s helper=%s %s' % (a, st, helper[0] if helper else '-', '; '.join(bad + notes)), file=out)
    out.close(); print(open(A / 'evidence' / ('uproof_%s.txt' % tag)).read())
