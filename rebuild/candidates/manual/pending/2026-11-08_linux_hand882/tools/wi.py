#!/usr/bin/env python3
"""wi.py <src.cpp> <tag> 'id=prio,...' ['name=prio,...']: c2 what-if (wig.py) forcing candidate priorities by first-pass
candidate id (pinned to the first record seen with that id) or by leaf name; writes tools/wi/<tag>.obj/.json."""
import sys, os, json, re
sys.path.insert(0, "/home/user/FableDecomp/tools/decomp_pipeline/linux_lane/lane_tools/c2probe")
import c2dbg
src, tag, spec = sys.argv[1], sys.argv[2], sys.argv[3]
spec2 = sys.argv[4] if len(sys.argv) > 4 else ''
forceid = [[int(a.split('=')[0]), int(a.split('=')[1])] for a in spec.split(',') if '=' in a]
force = [[a.split('=')[0], int(a.split('=')[1])] for a in spec2.split(',') if '=' in a]
here = os.path.dirname(os.path.abspath(__file__))
flags = []
m = re.search(r'^// Flags: (.+)$', open(src).read(), re.M)
if m: flags = m.group(1).split()
obj = f'{here}/{tag}.obj'
tr, log, run = c2dbg.probe(src, os.path.join(here, 'wig.py'), {'force': force, 'forceid': forceid, 'watch': [], 'all': True},
                           flags=flags, timeout=1500, obj_out=obj)
if tr is None or tr.get('error'):
    print('ERROR', (log or '')[-1500:], (tr or {}).get('error')); sys.exit(1)
json.dump(tr['events'], open(f'{here}/{tag}.json', 'w'))
print('events', len(tr['events']), 'forced', sum(1 for e in tr['events'] if e[0] == 'forced'))
