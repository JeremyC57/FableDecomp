# screen.py <addrfile>: stay-off screen for each address: lane lock owner, Codex pass 28 RESERVED.tsv, the standing held list, T3_PORT_QUEUE
# column 2 / column 3 (NF316 Q3), open COORDINATION_CLAIMS.tsv lines (newest line per item; B's or not done), UNMATCHED_ROWS.tsv status.
import sys, os, re
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__))); import nf
M = '/home/user/FableDecomp/rebuild/candidates/manual/'
addrs = [l.split()[0].lower().replace('0x', '') for l in open(sys.argv[1]) if l.strip()]
res = open(M + 'pending/2026-11-22_codex_pass28/RESERVED.tsv', encoding='utf-8', errors='replace').read().lower()
HELD = set('00b4d370 00bd3070 00890250 0091c080 0052c7e0 00a254a0 00adc190 00adc250 00adc4e0 00adcb90 00adce60 00be8e60 00bfbd80 00be6090 00bfb590'.split())
t3c2, t3c3 = set(), set()
for l in open(M + 'naming/T3_PORT_QUEUE.tsv', encoding='utf-8', errors='replace').read().splitlines()[1:]:
    f = l.split('\t')
    if len(f) > 1: t3c2.add(f[1].lower().replace('0x', ''))
    if len(f) > 2: t3c3.update(x.lower() for x in re.findall(r'0x([0-9a-fA-F]{8})', f[2]))
claims = {}
for l in open(M + 'COORDINATION_CLAIMS.tsv', encoding='utf-8', errors='replace').read().splitlines()[1:]:
    f = l.split('\t')
    if len(f) > 4:
        for a in re.findall(r'(?:0x)?([0-9a-fA-F]{8})', f[2]): claims[a.lower()] = (f[1], f[4], f[2][:60])
unm = {}
for l in open(M + 'UNMATCHED_ROWS.tsv', encoding='utf-8', errors='replace').read().splitlines():
    m = re.match(r'(?:0x)?([0-9a-fA-F]{8})\t', l)
    if m: unm[m.group(1).lower()] = l[:120]
for a in addrs:
    why = []
    o = nf.lock_owner(a)
    if o not in ('released', 'NF349'): why.append('lock ' + o)
    if a in res: why.append('codex28-reserved')
    if a in HELD: why.append('held-list')
    if a in t3c2: why.append('T3col2')
    if a in t3c3: why.append('T3col3')
    c = claims.get(a)
    if c and (c[0] == 'B' or c[1] not in ('done', 'released')): why.append('claim %s %s' % (c[0], c[1]))
    if a in unm: why.append('unmatched-row')
    print('%s\t%s' % (a, ','.join(why) or 'clear'))
