#!/usr/bin/env python3
"""screen438.py <batch...>: T3_PORT_QUEUE col 2/3 and COORDINATION_CLAIMS screen of every row AND target of the batches.
Prints rows whose newest claim line is Account B (blocking), and every T3 line naming a target (resolved fully / partly by the batch:
partly when the tracker still lists another wrong spelling at that target after the batch's renames). Writes evidence/T3_RESOLVES.tsv."""
import sys, re, csv, collections
W = '/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad/w22/agentNF438'; R = '/home/user/FableDecomp'
t3 = collections.defaultdict(list)
for l in open(R + '/rebuild/candidates/manual/naming/T3_PORT_QUEUE.tsv').read().splitlines()[1:]:
    q = l.split('\t')
    for x in re.findall(r'0x([0-9a-fA-F]{8})', ' '.join(q[1:3])): t3[x.lower()].append((q[0], q[1], (q[2] if len(q) > 2 else '')[:80]))
claims = collections.defaultdict(list)
for l in open(R + '/rebuild/candidates/manual/COORDINATION_CLAIMS.tsv').read().splitlines()[1:]:
    q = l.split('\t')
    for x in set(re.findall(r'(?:0x)?([0-9a-fA-F]{8})\b', l)): claims[x.lower()].append(q[1] if len(q) > 1 else '?')
tab = collections.defaultdict(set)
for r in csv.DictReader(open(R + '/rebuild/oracles/reloc_bindings.tsv'), delimiter='\t'): tab[r['target']].add(r['symbol'])
out = ['batch\taddress\trole\tt3_line\tstatus']; blockB = []
for b in sys.argv[1:]:
    ren = [l.rstrip('\n').split('\t') for l in open(W + '/batches/%s/renames.tsv' % b) if l.strip()]
    tchanges = [l.rstrip('\n').split('\t') for l in open(W + '/batches/%s/TABLE_CHANGES.tsv' % b)][1:]
    for a, old, new, t in ren:
        for x, role in ((a, 'row'), (t, 'target')):
            if claims.get(x) and claims[x][-1] == 'B': blockB.append((b, x, role))
            for src, addr, unit in t3.get(x, []):
                left = (tab[t] - {new} - {s for act, s, tg, *_ in tchanges if act == 'delete' and tg == t})
                out.append('%s\t%s\t%s\t%s %s %s\t%s' % (b, x, role, src, addr, unit, 'partly (other spellings left: %d)' % len(left - {old}) if left - {old} else 'fully'))
open(W + '/evidence/T3_RESOLVES.tsv', 'w').write('\n'.join(sorted(set(out), key=lambda s: (s != out[0], s))) + '\n')
print('newest-claim-B', blockB); print('T3 lines touching batch rows/targets', len(set(out)) - 1)
