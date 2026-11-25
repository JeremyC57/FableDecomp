#!/usr/bin/env python3
"""forcehold439.py <batch> <reason> <addr...>: mark rows HOLD in RESULTS.tsv (reason appended) so split423.py moves them to <batch>_hold."""
import sys
W = '/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad/w22/agentNF439'
b, why, rows = sys.argv[1], sys.argv[2], set(sys.argv[3:]); p = W + '/batches/%s/RESULTS.tsv' % b
L = [l.rstrip('\n').split('\t') for l in open(p)]
for r in L[1:]:
    if r[0] in rows: r[1] = 'HOLD'; r[4] = why
open(p, 'w').write(''.join('\t'.join(r) + '\n' for r in L))
