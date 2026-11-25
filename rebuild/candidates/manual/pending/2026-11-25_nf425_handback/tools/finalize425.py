#!/usr/bin/env python3
"""finalize425.py <batch> <tag> <shape> [all ACCEPT batches...]: lander files for a gated NF425 batch.
Verdict ACCEPT when: objgate ALL_EQ (or only `.drectve +/DEFAULTLIB:"libcp"`), vland HEAD/new same grade + PASS, 0 LITERAL, new warn kinds
only SHARED_HEADER_AVAILABLE (header debt), pk OK, link gate PASS, mutant test FAIL; else HOLD (the caller moves HOLD rows out).
TABLE_CHANGES (current reloc_bindings.tsv): per callee rename old -> new at target T: add (new, T, row) when (new, T) has no line;
(old, T) lines witnessed by a row of the ACCEPT set: rewitness to another landed source still naming the old identifier whose retail
bytes reference T, else delete. Writes RESULTS.tsv, CHECKS.tsv, UNITS.tsv, TABLE_CHANGES.tsv, SELF_SYMBOLS.tsv."""
import sys, os, re, csv, glob, json, collections, struct
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__))); import lib425 as L
W = L.A; R = L.R
b, tag, shape = sys.argv[1:4]; acc = sys.argv[4:] or [b]; B = W + '/batches/' + b
rows = open(B + '/rows.txt').read().split()
moved = set()
for x in acc: moved |= set(open(W + '/batches/%s/rows.txt' % x).read().split())
sm = {l.split('\t')[0]: l.rstrip('\n').split('\t') for l in open(B + '/selfmap.tsv') if l.strip()}
og = {l.split('\t')[0]: l.rstrip('\n').split('\t') for l in open(B + '/evidence/objgate_summary.tsv') if l.strip()}
pk = {l.split('\t')[0]: l.rstrip('\n').split('\t') for l in open(B + '/evidence/pk_summary.txt') if l[:8].strip() and not l.startswith('BAD')}
lk = open(B + '/evidence/linkgate.txt').read().strip().splitlines()[0]
gen = {}
for f in ('gch_final.tsv', 'gcg_final.tsv', 'gco3_final.tsv', 'gcj_all.tsv', 'gco_final.tsv', 'gci_final.tsv'):
    if os.path.exists(W + '/work/' + f):
        for r in csv.reader(open(W + '/work/' + f), delimiter='\t'):
            if f not in ('gch_final.tsv', 'gcg_final.tsv') or r[1] == 'OK': gen.setdefault(r[0], r)
def grades(k):
    d = {}; p = '%s/work/vland_%s%s.log' % (W, tag, k)
    if k == 'n' and os.path.exists('%s/work/vland_%sn2.log' % (W, tag)): p = '%s/work/vland_%sn2.log' % (W, tag)
    if k == 'n' and os.path.exists('%s/work/vland_%sn3.log' % (W, tag)): p = '%s/work/vland_%sn3.log' % (W, tag)
    if not os.path.exists(p): return d
    for l in open(p, errors='replace'):
        m = re.match(r'^([0-9a-f]{8})\s+(MATCH|RELOCATION_MATCH|DIFFER\S*|COMPILE\S*|FAIL\S*)\s+(\S+)', l)
        if m: d[m.group(1)] = (m.group(2), m.group(3))
    return d
gh, gn, gm = grades('h'), grades('n'), grades('m')
lit = open('%s/work/vland_%sn.log' % (W, tag), errors='replace').read().count('LITERAL')
wc = open('%s/work/wc_%s.out' % (W, tag)).read()
whead = collections.defaultdict(list); wnew = collections.defaultdict(list)
for k, d in (('h', whead), ('n', wnew)):
    for l in open('%s/work/vland_%s%s.log' % (W, tag, k), errors='replace'):
        m = re.match(r'^([0-9a-f]{8})\s+WARN (\S+)(.*)$', l.rstrip())
        if m: d[m.group(1)].append(m.group(2).rstrip(':') + m.group(3)[:120])
tab = collections.defaultdict(list)
for r in csv.DictReader(open(R + '/rebuild/oracles/reloc_bindings.tsv'), delimiter='\t', quoting=csv.QUOTE_NONE):
    if r.get('target') and r.get('address'): tab[(r['symbol'], r['target'].lower())].append(r['address'].lower())
cat = L.nf.catalog()
def refs(a, t):
    if a not in cat: return False
    bb = bytes.fromhex(cat[a]['bytes']); va = int(a, 16)
    for i in range(len(bb) - 4):
        if bb[i] in (0xE8, 0xE9) and '%08x' % ((va + i + 5 + struct.unpack_from('<i', bb, i + 1)[0]) & 0xffffffff) == t: return True
    return False
SRCIDX = None
def namers(ident):
    global SRCIDX
    if SRCIDX is None:
        SRCIDX = {}
        for f in glob.glob(R + '/rebuild/src/compiled/*/*/*.cpp'):
            m = re.search(r'_([0-9a-f]{8})\.cpp$', f)
            if m: SRCIDX[f] = (m.group(1), open(f, errors='replace').read())
    return [a for f, (a, t) in SRCIDX.items() if ident in t]
SELF = {}
for l in open(R + '/rebuild/oracles/self_symbols.tsv'):
    f = l.rstrip('\n').split('\t')
    if len(f) >= 4: SELF[f[0]] = f
UNITS_S = {l.split('\t')[0]: l.rstrip('\n').split('\t')[1] for l in open(B + '/units_draft.tsv')} if os.path.exists(B + '/units_draft.tsv') else {}
SG = {l.split('\t')[0]: l.rstrip('\n').split('\t') for l in open(B + '/evidence/sgate.tsv')} if os.path.exists(B + '/evidence/sgate.tsv') else {}
RETG = {}; VTC = {}
if os.path.exists(B + '/evidence/retg.tsv'):
    for l in open(B + '/evidence/retg.tsv').read().splitlines()[1:]: f = l.split('\t'); RETG[f[0]] = f[4]
if os.path.exists(B + '/evidence/vtcmp.tsv'):
    for l in open(B + '/evidence/vtcmp.tsv').read().splitlines()[1:]:
        f = l.split('\t')
        if len(f) >= 5: VTC[f[0]] = [f[3].split()[0], f[4].split()[0]]
res = []; chk = []; tc = []; ss = []; seen = set()
for a in rows:
    old, new = sm[a][1], sm[a][2]
    why = []
    o = og.get(a, ['', 'MISSING', '', ''])
    ogok = o[1] == 'ALL_EQ' or (o[1] == 'NOT_EQ' and o[3].strip() == 'DRECTVE -[] +[\'/DEFAULTLIB:"libcp"\']')
    extra = ''
    if shape == 'COPY_S':
        sg = SG.get(a, [])
        ogok = len(sg) > 3 and sg[2] == 'OWNER_OBJ_EQ' and sg[3] == 'LEAF_EQ'
        extra = '; objgate (Option S, evidence/sgate.tsv): the thin row object equals its owner 0x%s HEAD object section by section, and the owner TU\'s Copy equals the HEAD leaf (relocations renamed %s)' % (UNITS_S.get(a, '?'), sg[4] if len(sg) > 4 else '-')
        if not ogok: why.append('sgate %s' % sg)
    elif shape == 'CLONE_INLINE' and not ogok:
        C_ = L.WL[a]['klass']
        items = [x.strip() for x in o[3].split(';') if x.strip()]
        allow = ["DATA ('COMDAT', '??_7%s@@6B@', '.rdata')" % C_, "RELOC ('COMDAT', '??_7%s@@6B@', '.rdata')" % C_, "ADDED ('COMDAT', '??_G%s@@UAEPAXI@Z', '.text')" % C_,
                 'DRECTVE -[] +[\'/DEFAULTLIB:"libcp"\']']
        rest = [x for x in items if x not in allow and not x.startswith("CH ('COMDAT', '??_7%s@@6B@'" % C_)]
        rg = RETG.get(a, '-'); vt = VTC.get(a, ['-', '-'])
        def ag(s): return int(s.split('/')[0]) if '/' in s else -1
        if not rest and rg == 'EQ' and ag(vt[1]) >= ag(vt[0]):
            ogok = True; extra = '; objgate: only the class vftable COMDAT (stand-in at HEAD: %s retail slots agree -> %s, nf176 stand-in data rule) and the added pick-any ??_G%s (EQ at retail vftable slot 0, evidence/retg.tsv) differ' % (vt[0], vt[1], C_)
        else: why.append('objgate rest=%s retg=%s vt=%s' % (rest[:3], rg, vt))
    elif not ogok: why.append('objgate ' + o[1] + ' ' + o[3][:200])
    if a not in gh or a not in gn or gh[a] != gn[a] or gn[a][1] != 'PASS': why.append('vland %s -> %s' % (gh.get(a), gn.get(a)))
    if shape != 'COPY_S' and gm.get(a, ('', ''))[1] != 'FAIL': why.append('mutant test %s' % (gm.get(a),))
    if lit: why.append('LITERAL warns in batch')
    nw = re.search(r'^%s NEW WARN KINDS (.*)$' % a, wc, re.M)
    if nw and set(re.findall(r"'(\w+)'", nw.group(1))) - {'SHARED_HEADER_AVAILABLE'}: why.append('new vland warn kind ' + nw.group(1))
    p = pk.get(a, ['', '', 'MISSING'])
    pkok = p[2] == 'OK'
    if not pkok and shape == 'COPY_S' and re.search(r'N copies=\d+ neq=0', p[1]) and 'NEW_NEQ' not in p[2]:
        pkok = True; extra += '; pk: the added copies are the owner TU\'s own pick-any copies (all EQ at their retail addresses, 0 NEQ; the owner object is already landed)'
    if not pkok and shape == 'CLONE_INLINE' and RETG.get(a) == 'EQ':
        hp = open(B + '/evidence/pk/H_%s.pkall.tsv' % a).read().splitlines(); npk = open(B + '/evidence/pk/N_%s.pkall.tsv' % a).read().splitlines()
        hs = {l.split('\t')[-1] for l in hp if not l.startswith('#')}; ns_ = {l.split('\t')[-1] for l in npk if not l.startswith('#')}
        rn = {x for x in hs}; addl = ns_ - hs
        if addl == {'??_G%s@@UAEPAXI@Z' % L.WL[a]['klass']} and 'NEW_NEQ' not in p[2]: pkok = True; extra += '; pk: the one added copy is that ??_G (NOADDR in pk_all: reached only through the vftable)'
    if not pkok: why.append('pk ' + p[2])
    if not lk.endswith('PASS'): why.append('link gate')
    v = 'HOLD' if why else 'ACCEPT'
    renames = [kv.split(' -> ') for kv in o[2].split(' | ') if ' -> ' in kv]
    callee = [(x, y) for x, y in renames if x != old]
    g = gen.get(a, [])
    ev = g[-1] if shape != 'CLONE_INLINE' else g[3] if len(g) > 3 else ''
    if shape == 'COPY_S': ev = 'Option S thin row of the unit owned by 0x%s (evidence/sgate.tsv)' % UNITS_S.get(a, '?')
    form = ((g[5] if shape == 'CLONE_INLINE' and len(g) > 5 and g[5] else g[2] + (' ' + g[3] if shape == 'CLONE_OOL' and len(g) > 3 and g[3].startswith('engine/') and g[2] == 'F1' else '')) if len(g) > 2 else '') if shape != 'COPY_S' else 'thin row (Option S)'
    if shape == 'COPY_S': extra += '; test: the HEAD test is kept (self-contained; leaf bytes and relocation targets unchanged: GOTCHAS NF419 r1, no mutation control)'
    shh = 'SHARED_HEADER_AVAILABLE' in (nw.group(1) if nw else '')
    reason = ('NF425 %s: self respelt onto the RTTI-placed member (leaf bytes and relocation offsets unchanged, %s); %s; form %s%s%s'
              % (shape, 'callee %s' % '; '.join('%s -> %s' % c for c in callee) if callee else 'no callee rename', ev, form,
                 '; new WARN SHARED_HEADER_AVAILABLE = header debt (local class while engine/%s.h exists; precedent NF417 / NF423 r2), T3 line' % L.WL[a]['klass'] if shh else '',
                 extra + (('; HOLD: ' + '; '.join(why)) if why else '')))
    res.append([a, v, old, new, reason])
    chk.append([a, '%s %s' % gh.get(a, ('-', '-')), '%s %s' % gn.get(a, ('-', '-')), '%s %s' % gm.get(a, ('-', '-')), o[1] + ('(libcp)' if 'libcp' in o[3] else ''),
                p[1] if len(p) > 1 else '-', p[2], lk.split(';')[-1].strip(), str(lit), ';'.join(sorted(set(w.split()[0] for w in wnew[a]) - set(w.split()[0] for w in whead[a]))) or '-'])
    cur = SELF.get(a)
    ss.append([a, new, gn.get(a, ('-',))[0], new, '|'.join(cur[1:4]) if cur else '-'])
    if v != 'ACCEPT': continue
    for x, y in callee:
        T = None
        for (s, t), ws in tab.items():
            if s == x and a in ws: T = t
        if not T:
            if shape in ('COPY_JMP', 'COPY_S'): T = g[5]
            elif shape == 'CLONE_INLINE': T = {'CCombatActionBase': '0062f3b0', 'CCreatureActionBase': '0062ebc0'}[g[2]]
            else: T = g[4]
        if (y, T) not in tab and (y, T) not in seen:
            tc.append(['add', y, T, a, 'NF425: the respelt row 0x%s binds %s at 0x%s (its leaf relocation; was %s)' % (a.upper(), y, T.upper(), x)]); seen.add((y, T))
        for w in tab.get((x, T), []):
            if w not in rows or (x, T, w) in seen: continue
            seen.add((x, T, w))
            ident = re.sub(r'^_|@.*$', '', x.lstrip('?')) if not x.startswith('??') else re.match(r'\?\?[0-9A-Z_]\??\$?(\w+)@', x).group(1)
            others = [z for z in namers(ident) if z not in moved and refs(z, T)]
            if others: tc.append(['rewitness', x, T, others[0], 'NF425: witness 0x%s moved to %s; 0x%s still names it' % (w.upper(), y, others[0].upper())])
            else: tc.append(['delete', x, T, w, 'NF425: the stand-in callee spelling of 0x%s; its witness 0x%s now binds %s and no other landed source names it' % (T.upper(), w.upper(), y)])
def wr(n, hdr, rows_):
    with open(B + '/' + n, 'w') as f:
        f.write('\t'.join(hdr) + '\n')
        for x in rows_: f.write('\t'.join(x) + '\n')
wr('RESULTS.tsv', ['address', 'verdict', 'old', 'new', 'reason'], res)
wr('CHECKS.tsv', ['address', 'head_grade', 'new_grade', 'mutant', 'objgate', 'pk_copies', 'pk', 'link', 'literal_warns', 'new_warn_kinds'], chk)
wr('UNITS.tsv', ['address', 'unit', 'kind'], [[a, UNITS_S[a], 'S'] if a in UNITS_S else [a, a, '-'] for a in rows])
wr('TABLE_CHANGES.tsv', ['action', 'symbol', 'target', 'witness', 'note'], tc)
wr('SELF_SYMBOLS.tsv', ['address', 'symbol', 'status', 'leaf_given', 'current_line'], ss)
print(b, collections.Counter(x[1] for x in res), 'table', collections.Counter(x[0] for x in tc))
for x in res:
    if x[1] != 'ACCEPT': print(' ', x[0], x[4][-300:])
