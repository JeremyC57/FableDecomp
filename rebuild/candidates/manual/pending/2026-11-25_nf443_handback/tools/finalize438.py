#!/usr/bin/env python3
"""finalize438.py <batch> <tag> <unit> [accepted batches...]: write the lander files of a gated caller-respell batch.
renames.tsv columns: address, old, new, target. Verdict per row ACCEPT when objgate ALL_EQ, vland HEAD/new same grade + PASS (one path),
no LITERAL, no new WARN kind, pk OK, link PASS; else HOLD. TABLE_CHANGES (against the current reloc_bindings.tsv):
  - (new, target) has no line -> add (witness = first batch row binding it);
  - (old, target): every landed row still naming the old identifier outside ALL the given accepted batches (and whose retail bytes
    reference the target) keeps it -> rewitness to one of them when the current witness moves; none left -> delete.
Writes RESULTS.tsv, CHECKS.tsv, UNITS.tsv, TABLE_CHANGES.tsv, head/payload.json (+ oracle kept in work/ only)."""
import sys, os, re, csv, glob, json, shutil, collections, struct
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__))); import nf
W = str(nf.A); R = '/home/user/FableDecomp'
b, tag, unit = sys.argv[1:4]; acc = sys.argv[4:] or [b]; B = W + '/batches/' + b
rows = open(B + '/rows.txt').read().split()
ren = [r for r in csv.reader(open(B + '/renames.tsv'), delimiter='\t') if r]
moved = set()
renamed = set()
for x in acc:
    moved |= set(open(W + '/batches/%s/rows.txt' % x).read().split())
    renamed |= {tuple(r[:2] + r[3:4]) for r in csv.reader(open(W + '/batches/%s/renames.tsv' % x), delimiter='\t') if r}
og = {l.split('\t')[0]: l.rstrip('\n').split('\t') for l in open(B + '/evidence/objgate_summary.tsv')} if os.path.exists(B + '/evidence/objgate_summary.tsv') else {}
pk = {l.split('\t')[0]: l.rstrip('\n').split('\t') for l in open(B + '/evidence/pk_summary.txt') if l[:8].strip()}
lk = open(B + '/evidence/linkgate.txt').read().strip()
ng = {l.split('\t')[0]: l.rstrip('\n').split('\t')[-1] for l in open(B + '/evidence/namegate_summary.tsv')} if os.path.exists(B + '/evidence/namegate_summary.tsv') else {}
ruling = open(B + '/ruling.txt').read().strip() if os.path.exists(B + '/ruling.txt') else ''
RNOTE = {'r1': '; coordinator ruling r1 (2026-11-25): a callee-only respell may edit a row T3 names in col 2 for its own self debt; the T3 line stays open for that self debt',
         'r3': '; coordinator rulings r2/r3 (2026-11-25): bodiless local class declaration (no layout) for the rebound member call through reinterpret_cast; a new SHARED_HEADER_AVAILABLE warn is header debt (precedent NF412, NF417, NF423 a2/a3), booked as one T3 header-debt line',
         'r2': '; coordinator ruling r2 (2026-11-25): bodiless local class declaration for the qualified destructor call; the new SHARED_HEADER_AVAILABLE warn is header debt (precedent NF412, NF417), booked as one T3 header-debt line'}.get(ruling, '')
def grades(k):
    d = {}
    for l in open('%s/work/vland_%s%s.log' % (W, tag, k), errors='replace'):
        m = re.match(r'^([0-9a-f]{8})\s+(MATCH|RELOCATION_MATCH|DIFFER\S*|COMPILE\S*|FAIL\S*)\s+(\S+)', l)
        if m: d[m.group(1)] = (m.group(2), m.group(3))
    return d
gh, gn = grades('h'), grades('n')
lit = open('%s/work/vland_%sn.log' % (W, tag), errors='replace').read().count('LITERAL')
wc = os.popen('python3 %s/tools/warncmp.py %s' % (W, tag)).read()
tab = collections.defaultdict(list)
for r in csv.DictReader(open(os.environ.get('TABLE', R + '/rebuild/oracles/reloc_bindings.tsv')), delimiter='\t'): tab[(r['symbol'], r['target'])].append(r['address'])
files = {}
for f in glob.glob(R + '/rebuild/src/compiled/*/*/*.cpp'):
    m = re.search(r'_([0-9a-f]{8})\.cpp$', f)
    if m: files[m.group(1)] = f
cat = nf.catalog()
def refs(a, t):
    if a not in cat: return False
    bb = bytes.fromhex(cat[a]['bytes']); va = int(a, 16)
    for i in range(len(bb) - 4):
        if bb[i] in (0xE8, 0xE9) and '%08x' % ((va + i + 5 + struct.unpack_from('<i', bb, i + 1)[0]) & 0xffffffff) == t: return True
    return any('%08x' % struct.unpack_from('<I', bb, i)[0] == t for i in range(len(bb) - 3))
res = []; checks = []; tc = []; done_old = set(); done_new = set(); selfs = []
SS = {}; SSL = {}
for l in open(R + '/rebuild/oracles/self_symbols.tsv', encoding='utf-8'):
    q = l.rstrip('\n').split('\t')
    if len(q) > 1 and q[0] != 'address':
        SSL[q[0]] = q
        if q[1]: SS[q[0]] = q[1]
def leafof(a):
    m = re.search(r'^// Leaf:\s*(\S+)', open(B + '/rows/%s.cpp' % a, encoding='utf-8', errors='replace').read(), re.M)
    return m.group(1) if m else ''
def selfsym(a):
    m = re.search(r'^// Leaf:\s*(\S+)', open(B + '/rows/%s.cpp' % a, encoding='utf-8', errors='replace').read(), re.M)
    return SS.get(a) or (m.group(1) if m else '')
# rows still binding an old symbol: the NF438 libclang locator (work/ast.jsonl, mangled names of every main-file reference)
binders = collections.defaultdict(set)
for l in open(W + '/work/ast.jsonl'):
    j = json.loads(l)
    for h in j.get('hits', []): binders[h['sym']].add(j['row'])
for a in rows:
    rr = [r for r in ren if r[0] == a]
    v = 'ACCEPT'; why = []
    o = og.get(a, ['', 'MISSING'])[1]
    if o != 'ALL_EQ': v = 'HOLD'; why.append('objgate ' + o)
    if a not in gh or a not in gn or gh[a] != gn[a] or gn[a][1] != 'PASS': v = 'HOLD'; why.append('vland %s -> %s' % (gh.get(a), gn.get(a)))
    if lit: v = 'HOLD'; why.append('LITERAL warns in batch')
    nw = re.search(r'^%s NEW WARN KINDS (.*)$' % a, wc, re.M)
    if nw and not (ruling in ('r2', 'r3') and set(re.findall(r"'(\w+)'", nw.group(1))) <= {'SHARED_HEADER_AVAILABLE'}): v = 'HOLD'; why.append('new vland warn kind ' + nw.group(1))
    ngv = ng.get(a, 'MISSING')
    if ngv != 'NO_WORSE': v = 'HOLD'; why.append('name_gate ' + ngv)
    p = pk.get(a, ['', '', 'MISSING'])
    if p[2] != 'OK': v = 'HOLD'; why.append('pk ' + p[2])
    if '; PASS' not in lk and not lk.endswith('PASS'): v = 'HOLD'; why.append('link gate')
    if True:
        ss = selfsym(a); selfs.append([a, ss, gn.get(a, ('?',))[0], (SSL[a][3] if a in SSL and len(SSL[a]) > 3 and SSL[a][3] else leafof(a)), ' | '.join(SSL.get(a, []))]); t = ', 0x'.join(r[3] for r in rr)
        cr = ' || '.join('%s -> %s' % (r[1], r[2]) for r in rr)
        res.append([a, v, ss, ss, ('caller-only respell (NF443), self symbol unchanged; callee(s) %s; leaf bytes and relocation offsets unchanged, only the callee symbol renamed; target(s) 0x%s; '
                    'name: the T1 tracker PDB spelling (name set) = the landed self row Leaf; gates objgate ALL_EQ, vland %s PASS both (one path), pk %s, link PASS, name_gate no worse than HEAD; '
                    'HEAD test unchanged; leaf bytes equal to HEAD, so no mutation control (NF419 r1)' % (cr, t.upper(), gn.get(a, ('?',))[0], p[1].strip()) + RNOTE) if v == 'ACCEPT' else '; '.join(why)])
    checks.append([a, o, '%s/%s' % (gh.get(a, ('?', '?'))[0], gn.get(a, ('?', '?'))[0]), p[1].strip() if len(p) > 1 else '-', p[2] if len(p) > 2 else '-', lk.split(';')[-1].strip()])
    if v != 'ACCEPT': continue
    for _, old, new, t in rr:
        if (new, t) not in tab and (new, t) not in done_new:
            tc.append(['add', new, t, a, 'NF443: the respelt caller binds the PDB spelling at this target; no line bound it before']); done_new.add((new, t))
        if (old, t) in tab and (old, t) not in done_old:
            done_old.add((old, t))
            left = sorted(x for x in binders.get(old, ()) if (x, old, t) not in renamed and refs(x, t))
            wit = tab[(old, t)]
            if not left: tc.append(['delete', old, t, wit[0], 'NF443: no landed row binds the stand-in at this target after the respell (census of the identifier + retail call targets)'])
            elif (wit[0], old, t) in renamed: tc.append(['rewitness', old, t, left[0], 'NF443: the witness moved to the PDB spelling; %d landed row(s) still bind the stand-in (e.g. %s)' % (len(left), left[0])])
os.makedirs(B + '/head', exist_ok=True)
shutil.copy('%s/work/p_%sh/payload.json' % (W, tag), B + '/head/payload.json')
def wt(n, hdr, L): open(B + '/' + n, 'w').write('\t'.join(hdr) + '\n' + ''.join('\t'.join(x) + '\n' for x in L))
wt('RESULTS.tsv', ['address', 'verdict', 'old', 'new', 'reason'], res)
wt('CHECKS.tsv', ['address', 'objgate', 'vland_head/new', 'pk', 'pk_flag', 'link'], checks)
wt('UNITS.tsv', ['address', 'unit'], [[a, unit] for a in rows])
wt('SELF_SYMBOLS.tsv', ['address', 'symbol', 'status', 'leaf_given', 'current_line'], [x for x in selfs if x[0] in {r[0] for r in res if r[1] == 'ACCEPT'}])
wt('HEADERS.tsv', ['header', 'change'], [])
for a in rows:
    if os.path.exists(B + '/orig/tests/%s_test.cpp' % a) and not os.path.exists(B + '/tests/%s_test.cpp' % a): shutil.copy(B + '/orig/tests/%s_test.cpp' % a, B + '/tests/%s_test.cpp' % a)
wt('TABLE_CHANGES.tsv', ['action', 'symbol', 'target', 'witness', 'note'], tc)
print(b, collections.Counter(r[1] for r in res), 'table', collections.Counter(x[0] for x in tc))
