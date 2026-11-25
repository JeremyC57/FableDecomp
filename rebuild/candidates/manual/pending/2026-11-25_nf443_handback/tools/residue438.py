#!/usr/bin/env python3
"""residue438.py <gen RESIDUE.tsv> <out RESIDUE.tsv>: refine the generator's `shape` reasons into sub-codes (comparing the wrong spelling with
the target's PDB spelling) and append the HOLD rows of the batches. Codes:
  A0X_HASH        identical except the ?A0x<hash> of an anonymous namespace (path hash: a tracker normalisation issue, not a naming fix)
  C_NAME          C / extern "C" / @fastcall@N spellings (vendor-library prefixes such as zlib z_, or C stand-ins): needs a per-row decision
  SCOPE_ONLY      same function name and same parameter / qualifier tail, other class / namespace (stand-in class: needs the real class model)
  TAIL_ONLY       same name and scope, other parameter / return / access tail (signature: needs call-site type edits)
  SCOPE_AND_TAIL  same name, other scope and tail
  OTHER_NAME      another function name (identity differs: needs evidence per row)
  SPECIAL         ctor / dtor / operator spellings with a stand-in class or other form"""
import sys, csv, re, collections
A = '/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad/w22/agentNF443'
csv.field_size_limit(10**9)
P = {r['target']: r['pdb_spelling'] for r in csv.DictReader(open(A + '/tracker/OPEN.tsv'), delimiter='\t') if r['tier'] == 'T1'}
def split(m):
    x = re.match(r'^\?(\w+)@((?:[\w$?]+@)*?)@([A-Z][A-Z0-9_]*.*)$', m)
    return x.groups() if x else None
out = []; c = collections.Counter()
for t, s, w, why in list(csv.reader(open(sys.argv[1]), delimiter='\t'))[1:]:
    if why.startswith('shape'):
        p = P.get(t, ''); a = split(s); b = split(p)
        if re.sub(r'\?A0x[0-9a-f]{8}', '?A0x', s) == re.sub(r'\?A0x[0-9a-f]{8}', '?A0x', p): k = 'A0X_HASH'
        elif not s.startswith('?') or not p.startswith('?'): k = 'C_NAME'
        elif s.startswith('??') or p.startswith('??'): k = 'SPECIAL'
        elif not a or not b: k = 'SPECIAL'
        elif a[0] == b[0] and a[2] == b[2]: k = 'SCOPE_ONLY'
        elif a[0] == b[0] and a[1] == b[1]: k = 'TAIL_ONLY'
        elif a[0] == b[0]: k = 'SCOPE_AND_TAIL'
        else: k = 'OTHER_NAME'
        why = '%s: %s (PDB %s)' % (k, why, p)
    else:
        k = why.split(':')[0].split(' (')[0][:40]
    c[k] += 1; out.append((t, s, w, why))
for b in sys.argv[3:]:
    for r in list(csv.reader(open(A + '/batches/%s/RESULTS.tsv' % b), delimiter='\t'))[1:]:
        if r[1] != 'ACCEPT': out.append(('-', r[2], r[0], 'BATCH_HOLD %s: %s' % (b, r[4]))); c['BATCH_HOLD'] += 1
with open(sys.argv[2], 'w') as f:
    f.write('target\twrong_symbol\twitness_or_row\treason\n')
    for x in out: f.write('\t'.join(x) + '\n')
for k, v in c.most_common(): print(v, k)
