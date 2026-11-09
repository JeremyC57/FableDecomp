#!/usr/bin/env python3
"""late.py <tag> [gmin]: the CCountedPointer / CAction() / ConvertVector inline decisions after g >= gmin in tools/inl/<tag>.json"""
import json, sys
t = sys.argv[1]; gmin = int(sys.argv[2]) if len(sys.argv) > 2 else 7000
ev = json.load(open('/tmp/claude-0/-home-user-FableDecomp/5221bbec-fb18-575f-8649-a32d96bb1c2d/scratchpad/w22/agent882/tools/inl/%s.json' % t)); pend = []; groups = []
for e in ev:
    if e[0] == 'd': pend.append(e[1])
    elif e[0] == 'pipe': groups.append((e[1], pend)); pend = []
ds = [ds for n, ds in groups if n and 'ConstructQuestListPC' in n][0]
for i, d in enumerate(ds):
    n = d['n']
    if any(k in n for k in ('??0?$CCountedPointer@VCAction', '??0?$CCountedPointer@VCComponent@NUISystem@@@@QAE@PAV', '??0CAction@', 'ConvertVector')) and d['g'] >= gmin:
        f = (d['fhi'] * 2**32 + d['flo']) / 1e8
        print('  %4d r=%d sz=%d a=%d g=%d pen=%d c1=%d f=%.2f sc=%s %s' % (i, d['r'], d['sz'], d['a'], d['g'], d['pen'], d['c1'], f, d.get('sc'), n[:45]))
