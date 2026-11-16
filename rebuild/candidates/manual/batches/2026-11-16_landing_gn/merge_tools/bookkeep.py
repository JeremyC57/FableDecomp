# Landing gn bookkeeping (run from the repo root after a successful --land):
#  1. UNMATCHED_ROWS.tsv: drop the landed rows and the two re-cut fragments; re-point the 19 PARKED rows at their
#     2026-11-16_acctC_open drafts and mark them parked (reservation released); 0x00BA50A0 back to parked with its lead.
#  2. NAME_FIXES_QUEUE.tsv: append the acctC folders' fixes not already queued (tag land1116gn); symbol-vs-symbol
#     disagreements with a queued spelling go to the landing's NAME_FIXES_CONFLICTS_WITH_QUEUE.tsv instead
#     (except 0x00435F70, whose fix resolves the DoRender binding conflict this landing would otherwise create).
#  3. ghidra_out/labels_landing_2026-11-16gn.tsv from labels_gn.tsv.
import csv, json, shutil, sys
L = '/tmp/claude-0/-home-user-FableDecomp/5221bbec-fb18-575f-8649-a32d96bb1c2d/scratchpad/lc2/'
B = 'rebuild/candidates/manual/batches/2026-11-16_landing_gn/'
landed = set(open(L + 'landed.txt').read().split())
st = {r['address']: r for r in csv.DictReader(open('rebuild/candidates/manual/pending/2026-11-13_acctC/STATUS.tsv'), delimiter='\t')}
OPEN = 'rebuild/candidates/manual/pending/2026-11-16_acctC_open/'

# 1
P = 'rebuild/candidates/manual/UNMATCHED_ROWS.tsv'
rows = [l.rstrip('\n').split('\t') for l in open(P, encoding='utf-8')]
out = [rows[0]]; rm = ch = 0
for r in rows[1:]:
    a = r[0].lower()
    if a in landed or a in ('00537e70', '00564d5d') and 'acctC' in r[4]:
        rm += 1; continue
    if 'acctC' in r[4]:
        s = st.get(a)
        if s and s['state'] == 'PARKED':
            batch = s['staged_in'].split('/')[-1]
            r[4] = 'parked'; r[5] = 'acctC round six: ' + s['agent_and_note'][:110]
            r[6] = str(int(r[6] or 0) + 1); r[7] = OPEN + batch + '/' + a + '.cpp'
        elif a == '00ba50a0':
            r[4] = 'parked'; r[5] = ('RELOCATION_MATCH inside the TU of the landed 0x00BA5260 (its source defines SetupStaticBlocks); '
                                     'needs its own row source and behaviour test')
            r[7] = 'rebuild/candidates/manual/pending/2026-11-15_acctC_wfB3p/src/00ba5260.cpp'
        else:
            sys.exit('unhandled acctC row ' + a)
        ch += 1
    out.append(r)
open(P, 'w', encoding='utf-8').write('\n'.join('\t'.join(r) for r in out) + '\n')
print('unmatched: removed', rm, 'changed', ch, 'left acctC', sum(1 for r in out if 'acctC' in r[4]))

# 2
nf = json.load(open(L + 'namefixes.json'))
confkeys = set()
held = []
for f, r in nf['conf']:
    a = r[0].lower().replace('0x', '')
    Q = [q for q in (l.rstrip('\n').split('\t') for l in open('rebuild/candidates/manual/naming/NAME_FIXES_QUEUE.tsv', encoding='utf-8'))
         if len(q) > 2 and q[1].lower().replace('0x', '') == a]
    other = [q[2] for q in Q if q[2].startswith('?') and q[2] != r[1]]
    if other and a != '00435f70':
        confkeys.add((a, r[1])); held.append([f.split('acctC_')[1], '0x' + a.upper(), r[1], r[2], ' | '.join(other)])
with open('rebuild/candidates/manual/naming/NAME_FIXES_QUEUE.tsv', 'a', encoding='utf-8') as q:
    n = 0
    for row in nf['rows']:
        a = row[1].lower().replace('0x', '')
        if (a, row[2]) in confkeys:
            continue
        if a == '00435f70':
            row[3] = 'PRIORITY binding conflict: ' + row[3]
        q.write('\t'.join(row) + '\n'); n += 1
print('queued', n, 'held', len(held))
with open(B + 'NAME_FIXES_CONFLICTS_WITH_QUEUE.tsv', 'w', encoding='utf-8') as f:
    f.write('acctC_batch\taddress\tacctC_symbol\tacctC_evidence\tqueued_symbols\n')
    for h in held:
        f.write('\t'.join(h) + '\n')

# 3
shutil.copy(L + 'labels_gn.tsv', 'ghidra_out/labels_landing_2026-11-16gn.tsv')
print('labels written')
