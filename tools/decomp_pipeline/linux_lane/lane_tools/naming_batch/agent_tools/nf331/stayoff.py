# stayoff.py (NF331) <addr-list-file>: classify each listed address against the stay-off sets.
#   codex:<pass>   in a Codex / hand RESERVED.tsv and not landed (census-only compile allowed, no lock, no edit)
#   acctB_claim    newest COORDINATION_CLAIMS.tsv line for the address is B's and not done / released
#   acctB_b24      a row of pending/2026-11-24_acctB_b24/rows
#   acctB_unm      UNMATCHED_ROWS.tsv marks it reserved / handed off to acctB
#   lock:<owner>   locked by another agent ($S/w22/nflocks)
#   held           in the standing brief's held list
#   t3             column 2 or 3 of a T3_PORT_QUEUE.tsv line other than NF331's own unit line (edit stay-off; census compile only)
#   ok
# -> stdout: addr <TAB> class[,class...]
import sys, re, glob, os
from pathlib import Path
R = Path('/home/user/FableDecomp'); M = R / 'rebuild/candidates/manual'
S = Path('/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad')
H8 = re.compile(r'\b(?:0x)?([0-9a-fA-F]{8})\b')
cls = {}
def add(a, c): cls.setdefault(a.lower(), set()).add(c)
for f in glob.glob(str(M / 'pending/*/RESERVED.tsv')):
    L = open(f, encoding='utf-8', errors='replace').read().splitlines()
    hdr = L[0].split('\t'); ai = hdr.index('address') if 'address' in hdr else 0
    si = hdr.index('status') if 'status' in hdr else None
    for l in L[1:]:
        p = l.split('\t')
        if len(p) <= ai: continue
        a = p[ai].strip().lower().replace('0x', '')
        if not re.fullmatch(r'[0-9a-f]{8}', a): continue
        st = p[si] if si is not None and si < len(p) else 'reserved'
        if st.startswith('landed') and 'no longer reserved' in st: continue
        add(a, 'codex:' + f.split('/')[-2])
newest = {}
for l in open(M / 'COORDINATION_CLAIMS.tsv', encoding='utf-8', errors='replace').read().splitlines()[1:]:
    p = l.split('\t')
    if len(p) < 5: continue
    for a in H8.findall(p[2]): newest[a.lower()] = (p[1], p[4])
for a, (acct, st) in newest.items():
    if acct == 'B' and st not in ('done', 'released'): add(a, 'acctB_claim')
for f in glob.glob(str(M / 'pending/2026-11-24_acctB_b24/rows/*.cpp')):
    m = re.match(r'([0-9a-f]{8})', os.path.basename(f))
    if m: add(m.group(1), 'acctB_b24')
try:
    for l in open(M / 'UNMATCHED_ROWS.tsv', encoding='utf-8', errors='replace'):
        if 'acctB' in l or 'handed off' in l:
            m = H8.search(l)
            if m: add(m.group(1), 'acctB_unm')
except OSError: pass
HELD = '00B4D370 00BD3070 00890250 0091C080 0052C7E0 00A254A0 00ADC190 00ADC250 00ADC4E0 00ADCB90 00ADCE60 00BE8E60 00BFBD80 00BE6090 00BFB590'
for a in HELD.split(): add(a, 'held')
for l in open(M / 'naming/T3_PORT_QUEUE.tsv', encoding='utf-8', errors='replace').read().splitlines():
    p = l.split('\t')
    if len(p) < 3 or p[0] == 'nf328' and p[1].upper() == '0X008AB980': continue
    for a in H8.findall(p[1] + ' ' + p[2]): add(a, 't3')
for d in (S / 'w22/nflocks').iterdir():
    o = d / 'owner'
    if o.exists():
        w = o.read_text().strip()
        if w not in ('released', 'NF331'): add(d.name, 'lock:' + w)
for a in open(sys.argv[1]).read().split():
    a = a.lower()
    print('%s\t%s' % (a, ','.join(sorted(cls.get(a, {'ok'})))))
