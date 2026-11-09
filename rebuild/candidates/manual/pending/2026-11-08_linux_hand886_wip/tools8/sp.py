#!/usr/bin/env python3
"""sp.py <tag>: print the shook.py result (sorted top list + first choices) of t8/fr/<tag>.json."""
import json, sys
d = json.load(open(f'/tmp/claude-0/-home-user-FableDecomp/5221bbec-fb18-575f-8649-a32d96bb1c2d/scratchpad/w22/agent886/t8/fr/{sys.argv[1]}.json'))
R = {1: 'eax', 2: 'ecx', 3: 'edx', 4: 'ebx', 6: 'ebp', 7: 'esi', 8: 'edi'}
for e in d['events']:
    if e[0] == 'sorted':
        print('sorted (class', e[1], '):', ' '.join(f'#{c[0]}:{c[5] or c[4]}={c[1]}/k{c[2]}' for c in e[2][:12]))
    elif e[0] == 'chosen':
        c = e[1]; print(f'chosen #{c[0]} {c[5] or c[4]} prio {c[1]} -> {R.get(e[2], e[2])} costs ' + str({R.get(i, i): v for i, v in enumerate(e[3]) if v}))
    elif e[0] == 'err': print('ERR', e[1][-300:])
