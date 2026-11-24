# mkoff338.py (from NF334's mkoff334.py): NF338 stay-off lists -> work/offlimits.txt (with a reason per address in work/offlimits_why.tsv)
# Sources: standing brief held rows; Codex pass 26 / 27 / 28 RESERVED (still reserved); every T3_PORT_QUEUE address (col 2 + col 3);
# B's claims (COORDINATION_CLAIMS.tsv newest line per item, account B, not done / released) and pending/2026-11-24_acctB_b24 rows;
# NF330's ENTRIES.txt; the CScriptThing unit rows (NF326 / NF328 / NF331 hand-back rows.txt).
import re, glob, os, collections
A = '/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad/w22/agentNF338'
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
# NF338: agents NF336 (CV_HauntedHouseScript object 0x00E93D50..0x00E96E10 + 0x00E95380 + its ENTRIES.txt) and NF337 (its ENTRIES.tsv
# Copy rows + their operator= rows: the jmp / call targets of each Copy body, read from the exe) run at the same time.
import struct
oe = set(); ow = set()
for x in open(S + '/w22/agentNF336/ENTRIES.txt').read().split():
    if re.match(r'[0-9a-fA-F]{8}$', x): oe.add(x.lower()); add(x, 'NF336E')
oe.add('00e95380'); add('00e95380', 'NF336E')
for l in list(open(S + '/w22/agentNF337/ENTRIES.tsv'))[1:]:
    q = l.rstrip('\n').split('\t'); oe.add(q[0].lower()); add(q[0], 'NF337E')
# the NF337 Copy rows' operator= targets: first E8 / E9 rel32 in the first 64 bytes of each Copy body
d = open(os.environ['FABLE_EXE'], 'rb').read(); pe = struct.unpack_from('<I', d, 0x3C)[0]; ns = struct.unpack_from('<H', d, pe+6)[0]
osz = struct.unpack_from('<H', d, pe+20)[0]; base = struct.unpack_from('<I', d, pe+24+28)[0]
secs = []
for i in range(ns):
    o = pe+24+osz+40*i; vs, va, rs, rp = struct.unpack_from('<IIII', d, o+8); secs.append((base+va, rs, rp))
def rd(v, n):
    for va, rs, rp in secs:
        if va <= v < va+rs: return d[rp+v-va: rp+v-va+n]
    return b''
for a in [x for x in oe if x not in ()]:
    pass
for l in list(open(S + '/w22/agentNF337/ENTRIES.tsv'))[1:]:
    q = l.rstrip('\n').split('\t'); v = int(q[0], 16); b_ = rd(v, 64)
    for k in range(len(b_)-4):
        if b_[k] in (0xE8, 0xE9):
            t = (v + k + 5 + struct.unpack_from('<i', b_, k+1)[0]) & 0xffffffff
            if 0x401000 <= t < 0x1100000: ow.add('%08x' % t); add('%08x' % t, 'NF337op='); break
import csv; csv.field_size_limit(10**9)
for r in csv.DictReader(open(R + '/rebuild/oracles/auto-re-candidates.tsv', encoding='utf-8-sig'), delimiter='\t'):
    v = int(r['address'].replace('0x', ''), 16)
    if 0x00E93D50 <= v <= 0x00E96E10: oe.add('%08x' % v); add('%08x' % v, 'NF336range')
open(A + '/work/other_agents_entries.txt', 'w').write('\n'.join(sorted(oe)) + '\n')
open(A + '/work/other_agents_witnesses.txt', 'w').write('\n'.join(sorted(ow)) + '\n')
for ag in ('nf326', 'nf328', 'nf331'):
    for f in glob.glob(M + '/pending/2026-11-2*_%s_handback/rows.txt' % ag):
        for x in open(f).read().split():
            if re.match(r'(0x)?[0-9a-fA-F]{8}$', x): add(x.replace('0x', ''), 'CScriptThing-unit:' + ag)
open(A + '/work/offlimits.txt', 'w').write('\n'.join(sorted(why)) + '\n')
open(A + '/work/offlimits_why.tsv', 'w').write(''.join('%s\t%s\n' % (a, ','.join(sorted(r))) for a, r in sorted(why.items())))
c = collections.Counter(r.split(':')[0] for v in why.values() for r in v); print(len(why), dict(c))
