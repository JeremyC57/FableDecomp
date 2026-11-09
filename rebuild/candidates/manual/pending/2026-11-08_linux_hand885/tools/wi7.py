#!/usr/bin/env python3
"""wi.py <src.cpp> <tag> 'name=prio,...' [watch,names]: c2 what-if run (wig7.py); saves s7/wi_<tag>.obj/.json and prints
the first 40 instructions of RunCutsceneMacro plus the chosen registers of the forced / watched candidates."""
import sys, os, json, re, subprocess
sys.path.insert(0, "/home/user/FableDecomp/tools/decomp_pipeline/linux_lane/lane_tools/c2probe")
import c2dbg
src, tag, spec = sys.argv[1], sys.argv[2], sys.argv[3]
watch = sys.argv[4].split(',') if len(sys.argv) > 4 else []
force = [[a.split('=')[0], int(a.split('=')[1])] for a in spec.split(',') if '=' in a]
A = os.environ['A']
flags = []
m = re.search(r'^// Flags: (.+)$', open(src).read(), re.M)
if m: flags = m.group(1).split()
obj = f'{A}/s7/wi_{tag}.obj'
here = os.path.dirname(os.path.abspath(__file__))
tr, log, run = c2dbg.probe(src, os.path.join(here, 'wig7.py'), {'force': force, 'watch': watch, 'all': bool(os.environ.get('ALL')), 'blocks': bool(os.environ.get('BLOCKS')), 'blocks_once': bool(os.environ.get('BLOCKS')), 'penalties': bool(os.environ.get('PEN'))},
                           flags=flags, timeout=1500, obj_out=obj)
if tr is None or tr.get('error'):
    print('ERROR', (log or '')[-1500:], (tr or {}).get('error')); sys.exit(1)
json.dump(tr['events'], open(f'{A}/s7/wi_{tag}.json', 'w'))
for e in tr['events'][:60]:
    if e[0] == 'chosen':
        print('chosen #%d %s prio %d -> reg %d costs %s' % (e[1], e[2], e[3], e[4], {k: v for k, v in enumerate(e[5]) if v}))
        if len(e) > 6 and e[6]: print('    pen:', e[6][:12])
    elif e[0] == 'blk':
        print('blk wK %d w %d refl %d live %d' % (e[1], e[2], len(e[3]), len(e[4])))
        print('   refl:', ' '.join('#%d:%s:%x:%d' % tuple(c) for c in e[3]))
        print('   live:', ' '.join('#%d:%s:%x:%d' % tuple(c) for c in e[4]))
    else: print(e)
