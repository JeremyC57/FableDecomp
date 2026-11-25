#!/usr/bin/env python3
"""objgate437.py <batch> <tag>: LowerBound-split object gate, HEAD (work/wd_<tag>h) v new (work/wd_<tag>n) at one path.
Sections keyed by COMDAT symbol (first external DEF in the section; ?A0x hashes normalised), plain sections by name+ordinal.
Rename: every table spelling at 0x0040F020 / 0x004365B0 -> the established spelling of that target. Per row:
  PAIRED sections must have equal data, characteristics and relocations under the rename;
  DROPPED sections may only be stand-in map members (LowerBound bodies / inline helpers of the removed local map model);
  ADDED sections may only be shared-header pick-any copies (CVectorMap LowerBound / Find, STLport vector begin / end) or the row's
  own forceinline helpers over the real map; each shared-header copy is compared byte-and-reloc with the same-named COMDAT of
  landed HEAD objects (work/hobj/*, compiled from rows that already use the typedef / GetTC form): R10 tier 1."""
import sys, os, glob, re, csv, collections
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__))); import nf
W = os.path.dirname(os.path.dirname(os.path.abspath(__file__))); R = '/home/user/FableDecomp'
b, tag = sys.argv[1:3]; B = W + '/batches/' + b
rows = open(B + '/rows.txt').read().split()
tab = {}
for r in csv.DictReader(open(R + '/rebuild/oracles/reloc_bindings.tsv'), delimiter='\t'):
    if r['target'] in ('0040f020', '004365b0'): tab[r['symbol']] = r['target']
EST = {t: s for s, t in tab.items() if 'CVectorMap@W4ETCInterfaceType' in s}
REN = {s: EST[t] for s, t in tab.items() if s not in EST.values()}
hx = lambda s: re.sub(r'\?A0x[0-9a-f]{8}', '?A0x', s)
def obj(k, a): return glob.glob('%s/work/wd_%s%s/*/*/%s/%s.obj' % (W, tag, k, a, a))[0]
def load(p, mp):
    secs, sl = nf.coff(p); first = {}
    for i, nm, v, sec, t, scl in sl:
        if scl == 2 and sec > 0 and sec not in first: first[sec] = nm
    key = {}; cnt = {}; last = None
    for k, x in enumerate(secs, 1):
        if x['ch'] & 0x1000 and k in first: key[k] = ('COMDAT', hx(mp.get(first[k], first[k])), x['name']); last = key[k][1]
        elif x['ch'] & 0x1000 and last: key[k] = ('ASSOC', last, x['name'])
        else: cnt[x['name']] = cnt.get(x['name'], 0) + 1; key[k] = ('SEC', x['name'], cnt[x['name']])
    out = {}
    for k, x in enumerate(secs, 1):
        if x['name'].startswith('.debug'): continue
        kk = key[k]
        while kk in out: kk = kk + ('dup',)
        out[kk] = (x['data'], [(o, hx(mp.get(s, s)), t) for o, s, t in x['rel'] if not s.startswith('$')], x['ch'])
    return out
SHARED = re.compile(r'^\?(LowerBound|Find)@\?\$CVectorMap@W4ETCInterfaceType@@PAVCTCBase@@|^\?(begin|end)@\?\$vector@U\?\$pair@W4ETCInterfaceType@@PAVCTCBase@@')
HELPER_NEW = re.compile(r'^(\?\?\$MazeLookup|\?\?\$MazeLookupConst|\?FindTC@@|\?FindTCPick@@|\?FindTC@@|\?GetComponents@CThing@@|\?ComponentMap@CThingAICreatureComponentAccess@@)')
HELPER_OLD = re.compile(r'(MazeTable|CTCMap_Retail|CTCMap@@|Map@NFactory008385b0|Map@NCondition00935160|Map@Row00868b40|CInterfaceMap|ComponentMap|GetComponents@CThing@@|ComponentMap@CThingAICreatureComponentAccess)')
# landed reference copies of the shared-header COMDATs
ref = {}
for o in sorted(glob.glob(W + '/work/hobj/*.obj')):
    a = os.path.basename(o)[:8]
    if a in rows: continue
    for k, v in load(o, {}).items():
        if k[0] == 'COMDAT' and SHARED.match(k[1]) and k[1] not in ref: ref[k[1]] = (v, a)
DRREF = {}
for o in sorted(glob.glob(W + '/work/hobj/*.obj')):
    a = os.path.basename(o)[:8]
    if a in rows: continue
    for x in nf.coff(o)[0]:
        if x['name'] == '.drectve' and b'libcp' in x['data']: DRREF.setdefault('/DEFAULTLIB:"libcp"', a)
rep = []; summ = []
for a in rows:
    h = load(obj('h', a), REN); n = load(obj('n', a), {}); bad = []; notes = []
    for k in sorted(set(h) | set(n), key=str):
        if k in h and k in n:
            x, y = h[k], n[k]
            if k[0] == 'COMDAT' and k[1] in EST.values() and (x[0] != y[0] or x[1] != y[1]):
                r = ref.get(k[1])
                if r and r[0][0] == y[0] and r[0][1] == y[1]: notes.append('stand-in LowerBound body %s replaced by the template body = landed %s (R10)' % (k[1][:40], r[1]))
                else: bad.append('LB_NEQ_REF %s' % (k,))
            elif k == ('SEC', '.drectve', 1) and x[0] != y[0]:
                dx, dy = set(x[0].decode('latin1').split()), set(y[0].decode('latin1').split())
                if dy - dx == {'/DEFAULTLIB:"libcp"'} and not dx - dy and '/DEFAULTLIB:"libcp"' in DRREF: notes.append('drectve +/DEFAULTLIB:"libcp" (as landed CVectorMap.h includers, e.g. %s)' % DRREF['/DEFAULTLIB:"libcp"'])
                else: bad.append('DRECTVE -%s +%s' % (sorted(dx - dy), sorted(dy - dx)))
            elif x[0] != y[0] or x[2] != y[2]: bad.append('DATA %s' % (k,))
            elif x[1] != y[1]: bad.append('RELOC %s' % (k,))
        elif k in n:
            nm = k[1]
            if k[0] == 'COMDAT' and SHARED.match(nm):
                r = ref.get(nm)
                if not r: bad.append('ADDED_NOREF %s' % nm[:90])
                elif r[0][0] != n[k][0] or r[0][1] != n[k][1]: bad.append('ADDED_NEQ_REF %s (ref %s)' % (nm[:90], r[1]))
                else: notes.append('added shared %s = landed %s' % (nm[:60], r[1]))
            elif k[0] == 'COMDAT' and HELPER_NEW.match(nm): notes.append('added helper %s' % nm[:70])
            elif k[0] == 'ASSOC' and (SHARED.match(nm) or HELPER_NEW.match(nm)): pass
            else: bad.append('ADDED %s' % (k,))
        else:
            nm = k[1]
            if k[0] in ('COMDAT', 'ASSOC') and (nm in EST.values() or HELPER_OLD.search(nm)): notes.append('dropped %s %s' % (k[0], nm[:70]))
            else: bad.append('DROPPED %s' % (k,))
    v = 'EQ_UNDER_RENAME' if not bad else 'NOT_EQ'
    rep.append('##### %s %s\n%s\n%s' % (a, v, '\n'.join('  BAD ' + x for x in bad), '\n'.join('  ' + x for x in notes)))
    summ.append('%s\t%s\t%s' % (a, v, '; '.join(bad)[:300]))
os.makedirs(B + '/evidence', exist_ok=True)
open(B + '/evidence/objgate.txt', 'w').write('\n'.join(rep) + '\n'); open(B + '/evidence/objgate_summary.tsv', 'w').write('\n'.join(summ) + '\n')
print('\n'.join(summ)); print('refs', len(ref))
