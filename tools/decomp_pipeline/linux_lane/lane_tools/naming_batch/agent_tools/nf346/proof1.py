# proof1.py <tag> <addr...>: item-1 objsec, HEAD (work/obj/H_<a>.obj) v new (work/obj/<a>.obj), both compiled at one path:
#  1. external symbol tables equal (DEF / UND names and COMDAT selection);
#  2. every non-debug section equal (name, flags, bytes, relocation list), except the COMDAT that defines GetDef<CMaterialDef>, which may
#     change bytes (and length) but must keep the same relocation symbols;
#  3. that COMDAT in the new object equals Fable.exe 0x0066C340 (pickany.py: bytes with relocation fields zeroed; relocation targets in
#     the table) and the landed owner row's object (work/hobj/0066c340.obj: bytes and relocation symbols).
# Compiler $L<n> labels are compared by (section, value). -> evidence/proof1_<tag>.txt (ALL_EQ / FIXED_COPY_ONLY / NOT_EQ per row).
import sys, os, re, subprocess
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__))); import nf, objcensus
A = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
SYM = '??$GetDef@VCMaterialDef@@@CDefinitionManager@@QBE_NJPAV?$CDefPointer@$$CBVCMaterialDef@@@@@Z'
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
def norm(secs, sl):
    lab = {nm: (sec, v) for i, nm, v, sec, t, scl in sl if re.match(r'^\$L\d+$', nm)}
    for x in secs: x['rel'] = [(o, ('$L@%s' % (lab[nm],)) if nm in lab else nm, t) for o, nm, t in x['rel']]
    return secs
own = A + '/work/hobj/0066c340.obj'
osec = nf.coff(own)[0][defsec(own, SYM) - 1]
tag = sys.argv[1]; out = open(A + '/evidence/proof1_%s.txt' % tag, 'w')
for a in sys.argv[2:]:
    h, n = A + '/work/obj/H_%s.obj' % a, A + '/work/obj/%s.obj' % a
    if not (os.path.exists(h) and os.path.exists(n)): print('%s MISSING_OBJ' % a, file=out); continue
    bad, notes = [], []
    th, tn = symtab(h), symtab(n)
    if th != tn: bad.append('symbol tables differ: %s' % sorted(set(th.items()) ^ set(tn.items()))[:4])
    sh, lh = nf.coff(h); sn, ln = nf.coff(n); sh = norm(sh, lh); sn = norm(sn, ln)
    k_n = defsec(n, SYM); k_h = defsec(h, SYM)
    # key every section: a COMDAT section by the external symbol it defines (first DEF with that section number), else by (name, ordinal
    # among the non-COMDAT sections of that name); then compare matched sections and list the unmatched ones by key
    def keyed(secs, sl):
        first = {}
        for i, nm, v, sec, t, scl in sl:
            if scl == 2 and sec > 0 and sec not in first: first[sec] = nm
        out, cnt = {}, {}
        for k, x in enumerate(secs, 1):
            if DEBUG.match(x['name']): continue
            if x['ch'] & 0x1000 and k in first: key = ('COMDAT', first[k], x['name'])
            elif x['ch'] & 0x1000:
                # an associative COMDAT (no external symbol of its own): key by the section it follows
                key = ('ASSOC', out and list(out)[-1], x['name'])
            else:
                cnt[x['name']] = cnt.get(x['name'], 0) + 1; key = ('SEC', x['name'], cnt[x['name']])
            out[key] = (k, x)
        return out
    KH, KN = keyed(sh, lh), keyed(sn, ln)
    for key in sorted(set(KH) | set(KN), key=str):
        if key not in KN: bad.append('dropped %s %s' % (key[0], str(key[1])[:110])); continue
        if key not in KH: bad.append('added %s %s' % (key[0], str(key[1])[:110])); continue
        (kh, x), (kn, y) = KH[key], KN[key]
        if x['name'] == y['name'] and x['ch'] == y['ch'] and x['data'] == y['data'] and x['rel'] == y['rel']: continue
        if kn == k_n and kh == k_h and [r[1:] for r in x['rel']] == [r[1:] for r in y['rel']]:
            notes.append('GetDef<CMaterialDef> copy changed %d -> %d B' % (len(x['data']), len(y['data']))); continue
        bad.append('section %s differs' % (str(key)[:140],))
    if k_n:
        r = subprocess.run(['python3', A + '/tools/pickany.py', n, SYM, '0066c340'], capture_output=True, text=True).stdout
        eqr = 'bytes_equal True' in r and 'NOT_IN_TABLE' not in r
        y = sn[k_n - 1]; eqo = (y['data'], [x[1:] for x in y['rel']]) == (osec['data'], [x[1:] for x in osec['rel']])
        notes.append('copy v retail 0x0066C340 %s, v owner object %s' % ('EQ' if eqr else 'NEQ', 'EQ' if eqo else 'NEQ'))
        if not eqr: bad.append('GetDef<CMaterialDef> copy still differs from retail')
    # the default SetDynamicCast form's inline helpers for T = CMaterialDef, unreferenced COMDATs no longer instantiated once the copy is fixed
    OKDROP = ('??$SetDynamicCast@$$CBVCDefClassBase@@@?$CDefPointer@$$CBVCMaterialDef@@@@QAEXABV?$CDefPointer@$$CBVCDefClassBase@@@@@Z',
              '?Assign@?$CDefPointer@$$CBVCMaterialDef@@@@AAEXPAVCDefPointeeBase@@@Z')
    fixed = any(s.startswith('GetDef<') for s in notes)
    drops = [b for b in bad if b.startswith('dropped COMDAT') or b.startswith('symbol tables differ')]
    rest = [b for b in bad if b not in drops]
    only_ok = fixed and not rest and all(any(o[:100] in b for o in OKDROP) for b in drops if b.startswith('dropped')) and \
              set(x[0] for x in (set(th.items()) ^ set(tn.items()))) <= set(OKDROP) and all(k not in tn for k in OKDROP)
    v = ('FIXED_COPY_PLUS_DROPPED_HELPERS' if only_ok and drops else 'FIXED_COPY_ONLY' if fixed and not bad else 'NOT_EQ' if bad else 'ALL_EQ')
    print('%s %s %s' % (a, v, '; '.join(bad + notes)), file=out); out.flush()
out.close(); print(open(A + '/evidence/proof1_%s.txt' % tag).read())
