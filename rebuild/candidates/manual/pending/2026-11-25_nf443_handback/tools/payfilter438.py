#!/usr/bin/env python3
"""payfilter438.py <batch...>: keep only rows.txt addresses in batches/<b>/head/payload.json (after split423 moved HOLD rows out);
HOLD folders get their own head/payload.json with their rows. Prints the counts."""
import sys, json, os
W = '/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad/w22/agentNF443'
for b in sys.argv[1:]:
    p = W + '/batches/%s/head/payload.json' % b; full = json.load(open(W + '/work/p_%sh/payload.json' % b))
    for dst in (b, b + '_hold'):
        f = W + '/batches/%s/rows.txt' % dst
        if not os.path.exists(f): continue
        keep = set(open(f).read().split()); d = json.loads(json.dumps(full))
        d['result']['authored'] = [x for x in d['result']['authored'] if x['address'] in keep]
        os.makedirs(W + '/batches/%s/head' % dst, exist_ok=True); json.dump(d, open(W + '/batches/%s/head/payload.json' % dst, 'w'))
        print(dst, len(d['result']['authored']), 'of', len(keep))
