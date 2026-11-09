#!/usr/bin/env python3
"""pk.py <src.cpp> <out.json> [--iset] [--func NAME]: run the c2 frame-packer probe (pkg.py) on a source, with the row's
// Flags: line, plus /FAs into <out>.asm. Needs FABLE_HDR_OVERLAY and `source $S/wine/env.sh`."""
import sys, os, json, re
sys.path.insert(0, "/home/user/FableDecomp/tools/decomp_pipeline/linux_lane/lane_tools/c2probe")
import c2dbg
args = sys.argv[1:]
iset = '--iset' in args; args = [a for a in args if a != '--iset']
func = 'RunCutsceneMacro'
if '--func' in args:
    i = args.index('--func'); func = args[i + 1]; del args[i:i + 2]
src, outp = args[0], args[1]
flags = []
m = re.search(r'^// Flags: (.+)$', open(src).read(), re.M)
if m: flags = m.group(1).split()
asm = os.path.abspath(outp[:-5] + '.asm' if outp.endswith('.json') else outp + '.asm')
flags += ['/FAs', '/Fa' + c2dbg.winpath(asm)]
here = os.path.dirname(os.path.abspath(__file__))
tr, log, run = c2dbg.probe(src, os.path.join(here, 'pkg.py'), {'iset': iset, 'func': func}, flags=flags, timeout=1500,
                           obj_out=os.environ.get('OBJ_OUT'))
if tr is None or tr.get('error'):
    print('ERROR', (log or '')[-2000:], (tr or {}).get('error')); sys.exit(1)
json.dump(tr['events'], open(outp, 'w'))
print('events', len(tr['events']), 'asm', asm)
