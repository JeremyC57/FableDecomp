# mkoff334.py (from NF332's mkoff332.py): NF334 stay-off lists -> work/offlimits.txt (with a reason per address in work/offlimits_why.tsv)
# Sources: standing brief held rows; Codex pass 26 / 27 / 28 RESERVED (still reserved); every T3_PORT_QUEUE address (col 2 + col 3);
# B's claims (COORDINATION_CLAIMS.tsv newest line per item, account B, not done / released) and pending/2026-11-24_acctB_b24 rows;
# NF330's ENTRIES.txt; the CScriptThing unit rows (NF326 / NF328 / NF331 hand-back rows.txt).
import re, glob, os, collections
A = '/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad/w22/agentNF334'
S = os.path.dirname(os.path.dirname(A)); R = '/home/user/FableDecomp'; M = R + '/rebuild/candidates/manual'
H8 = r'(?<![0-9A-Fa-f])(?:0x)?([0-9A-Fa-f]{8})(?![0-9A-Fa-f])'
why = collections.defaultdict(set)
def add(a, r): why[a.lower()].add(r)
for h in '00B4D370 00BD3070 00890250 0091C080 0052C7E0 00A254A0 00ADC190 00ADC250 00ADC4E0 00ADCB90 00ADCE60 00BE8E60 00BFBD80 00BE6090 00BFB590'.split(): add(h, 'held(brief)')
for p in ('2026-11-21_codex_pass26', '2026-11-22_codex_pass27', '2026-11-22_codex_pass28'):
    for l in list(open('%s/pending/%s/RESERVED.tsv' % (M, p)))[1:]:
        q = l.rstrip('\n').split('\t')
        if 'no longer reserved' not in l and re.match(r'[0-9A-Fa-f]{8}$', q[0]): add(q[0], 'codex:' + p[-6:])
for l in open(M + '/naming/T3_PORT_QUEUE.tsv'):
    q = l.rstrip('\n').split('\t')
    if len(q) > 1 and q[0] != 'source':
        m = re.match(r'0x([0-9A-Fa-f]{8})', q[1])
        if m: add(m.group(1), 'T3:' + q[0])
        if len(q) > 2:
            for x in re.findall(H8, q[2]): add(x, 'T3unit:' + q[0])
newest = {}
for l in list(open(M + '/COORDINATION_CLAIMS.tsv'))[1:]:
    q = l.rstrip('\n').split('\t')
    if len(q) >= 5: newest[q[2]] = q
for it, q in newest.items():
    if q[1] == 'B' and q[4] not in ('done', 'released'):
        for x in re.findall(H8, it): add(x, 'B-claim')
for f in glob.glob(M + '/pending/2026-11-24_acctB_b24/rows/*'):
    m = re.match(r'([0-9a-fA-F]{8})', os.path.basename(f))
    if m: add(m.group(1), 'acctB_b24')
for l in list(open(M + '/pending/2026-11-24_acctB_b24/STATUS.tsv'))[1:]:
    m = re.match(r'([0-9a-fA-F]{8})', l)
    if m: add(m.group(1), 'acctB_b24')
# NF334: agent NF333 runs at the same time: every target of its ENTRIES.tsv is off limits (its witnesses go to other_agents_witnesses.txt)
oe = set(); ow = set()
for l in list(open(S + '/w22/agentNF333/ENTRIES.tsv'))[1:]:
    q = l.rstrip('\n').split('\t'); oe.add(q[0].lower()); add(q[0], 'NF333E')
    for par in re.findall(r'\(([^)]*)\)', q[11] if len(q) > 11 else ''):
        for x in re.findall(r'[0-9a-fA-F]{8}', par): ow.add(x.lower())
for x in ow: add(x, 'NF333W')
open(A + '/work/other_agents_entries.txt', 'w').write('\n'.join(sorted(oe)) + '\n')
open(A + '/work/other_agents_witnesses.txt', 'w').write('\n'.join(sorted(ow)) + '\n')
for ag in ('nf326', 'nf328', 'nf331'):
    for f in glob.glob(M + '/pending/2026-11-2*_%s_handback/rows.txt' % ag):
        for x in open(f).read().split():
            if re.match(r'(0x)?[0-9a-fA-F]{8}$', x): add(x.replace('0x', ''), 'CScriptThing-unit:' + ag)
open(A + '/work/offlimits.txt', 'w').write('\n'.join(sorted(why)) + '\n')
open(A + '/work/offlimits_why.tsv', 'w').write(''.join('%s\t%s\n' % (a, ','.join(sorted(r))) for a, r in sorted(why.items())))
c = collections.Counter(r.split(':')[0] for v in why.values() for r in v); print(len(why), dict(c))
