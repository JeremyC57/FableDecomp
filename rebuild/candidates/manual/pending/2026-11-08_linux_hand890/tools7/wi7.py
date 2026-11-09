#!/usr/bin/env python3
"""wi7.py <src.cpp> <tag> 'name=prio,...' ['name:old=new,...'] [watch,names]  (agent890 s7; from hand885 wi.py)
c2 what-if run on CTCBoastUI::Activate: force= every insert of that leaf name; forceif= only when its prio equals old.
Saves work/s7/wi/<tag>.obj/.json; prints forced events and the chosen registers of watched candidates."""
import sys, os, json, re
sys.path.insert(0, "/home/user/FableDecomp/tools/decomp_pipeline/linux_lane/lane_tools/c2probe")
import c2dbg
src, tag, spec = sys.argv[1], sys.argv[2], sys.argv[3]
spec2 = sys.argv[4] if len(sys.argv) > 4 else ''
watch = sys.argv[5].split(',') if len(sys.argv) > 5 else []
force = [[a.split('=')[0], int(a.split('=')[1])] for a in spec.split(',') if '=' in a]
forceif = []
for a in spec2.split(','):
    if ':' in a and '=' in a:
        n, r = a.rsplit(':', 1); o, p = r.split('='); forceif.append([n, int(o), int(p)])
W = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
obj = f'{W}/wi/{tag}.obj'
flags = []
m = re.search(r'^// Flags: (.+)$', open(src).read(), re.M)
if m and m.group(1).strip() != 'none': flags = m.group(1).split()
here = os.path.dirname(os.path.abspath(__file__))
tr, log, run = c2dbg.probe(src, os.path.join(here, 'wig7.py'), {'force': force, 'forceif': forceif, 'watch': watch,
                           'all': bool(os.environ.get('ALL')), 'loginsert': bool(os.environ.get('LOGINS')), 'loginsall': bool(os.environ.get('LOGALL'))},
                           flags=flags, timeout=1500, obj_out=obj)
import shutil; shutil.rmtree(run, ignore_errors=True)
if tr is None or tr.get('error'):
    print('ERROR', (log or '')[-1500:], (tr or {}).get('error')); sys.exit(1)
json.dump(tr['events'], open(f'{W}/wi/{tag}.json', 'w'))
for e in tr['events']:
    if e[0] == 'chosen': print('chosen #%d %s prio %d -> reg %d costs %s' % (e[1], e[2], e[3], e[4], {k: v for k, v in enumerate(e[5]) if v}))
    else: print(e)
