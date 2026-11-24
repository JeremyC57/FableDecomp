# screen346.py <addr file>: NF349 stay-off screen per candidate row. Reasons: lane lock owned by another agent (nflocks), T3_PORT_QUEUE
# column 2 / column-3 0x address (NF316 Q3), COORDINATION_CLAIMS.tsv (any B line, or an A line not done/released; newest per item),
# pending/2026-11-24_acctB_b24* (any file text naming the address: B staged), UNMATCHED_ROWS.tsv, codex pass RESERVED.tsv files and
# pending/*codex_pass*_open dir names, the standing held list. Prints addr<TAB>clear | reasons.
import sys, os, re, glob
M = '/home/user/FableDecomp/rebuild/candidates/manual/'; S = os.environ['S']
addrs = [l.split()[0].lower().replace('0x', '') for l in open(sys.argv[1]) if l.strip() and not l.startswith('#')]
HELD = set('00b4d370 00bd3070 00890250 0091c080 0052c7e0 00a254a0 00adc190 00adc250 00adc4e0 00adcb90 00adce60 00be8e60 00bfbd80 00be6090 00bfb590'.split())
t3c2, t3c3 = {}, {}
for l in open(M + 'naming/T3_PORT_QUEUE.tsv', encoding='utf-8', errors='replace').read().splitlines()[1:]:
    f = l.split('\t')
    if len(f) > 1: t3c2.setdefault(f[1].lower().replace('0x', ''), []).append(f[0])
    if len(f) > 2:
        for x in re.findall(r'0x([0-9a-fA-F]{8})', f[2]): t3c3.setdefault(x.lower(), []).append(f[0])
newest = {}
for l in open(M + 'COORDINATION_CLAIMS.tsv', encoding='utf-8', errors='replace').read().splitlines()[1:]:
    f = l.split('\t')
    if len(f) > 4: newest[(f[1], f[2])] = f
claims = {}
for (acct, item), f in newest.items():
    for a in re.findall(r'(?:0x)?([0-9a-fA-F]{8})', item):
        if acct == 'B' or f[4] not in ('done', 'released'): claims.setdefault(a.lower(), []).append('%s:%s' % (acct, f[4]))
btext = ''
for d in glob.glob(M + 'pending/2026-11-24_acctB_b24*'):
    for p in glob.glob(d + '/**', recursive=True):
        if os.path.isfile(p) and os.path.getsize(p) < 20_000_000:
            try: btext += open(p, encoding='utf-8', errors='replace').read().lower() + '\n' + p.lower() + '\n'
            except Exception: pass
unm = {}
for l in open(M + 'UNMATCHED_ROWS.tsv', encoding='utf-8', errors='replace').read().splitlines():
    m = re.match(r'(?:0x)?([0-9a-fA-F]{8})\t', l)
    if m: unm[m.group(1).lower()] = l.split('\t')[4] if len(l.split('\t')) > 4 else '?'
res = ''
for p in glob.glob(M + 'pending/*codex_pass*/RESERVED.tsv'): res += open(p, encoding='utf-8', errors='replace').read().lower()
opendirs = set(os.path.basename(p).lower() for p in glob.glob(M + 'pending/*codex_pass*_open/*'))
for a in addrs:
    why = []
    o = open(f'{S}/w22/nflocks/{a}/owner').read().strip() if os.path.exists(f'{S}/w22/nflocks/{a}/owner') else 'released'
    if o not in ('released', 'NF349'): why.append('lock ' + o)
    if a in HELD: why.append('held-list')
    if a in t3c2: why.append('T3col2 ' + ','.join(t3c2[a]))
    if a in t3c3: why.append('T3col3 ' + ','.join(t3c3[a]))
    if a in claims: why.append('claim ' + ','.join(claims[a]))
    if a in btext: why.append('acctB_b24-staged/mentioned')
    if a in unm: why.append('unmatched:' + unm[a])
    if a in res: why.append('codex-RESERVED')
    if a in opendirs: why.append('codex-open')
    print('%s\t%s' % (a, '; '.join(why) or 'clear'))
