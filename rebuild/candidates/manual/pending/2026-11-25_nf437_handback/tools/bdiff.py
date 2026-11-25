#!/usr/bin/env python3
# bdiff.py <addr> <obj> [leafsubstr]: disassemble the leaf function from obj and retail catalog bytes, print differing lines (relocs masked)
import sys, subprocess, re, os
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__))); import nf
a, o = sys.argv[1], sys.argv[2]; sub = sys.argv[3] if len(sys.argv) > 3 else None
secs, sl = nf.coff(o)
fn = [(len(secs[sec-1]['data']), nm, sec) for i, nm, v, sec, t, scl in sl if scl == 2 and sec > 0 and secs[sec-1]['name'].startswith('.text') and t == 0x20]
if sub: fn = [x for x in fn if sub in x[1]]
_, nm, sec = max(fn); d = secs[sec-1]['data']
r = bytes.fromhex(nf.catalog()[a]['bytes'])
open('/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad/w22/agentNF437/tmp/o.bin','wb').write(d)
open('/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad/w22/agentNF437/tmp/r.bin','wb').write(r)
def dis(p):
    t = subprocess.run(['objdump','-D','-b','binary','-mi386','-Mintel',p],capture_output=True,text=True).stdout
    return [re.sub(r'0x[0-9a-f]+','X',l.split('\t')[-1]).strip() for l in t.splitlines() if re.match(r'^\s+[0-9a-f]+:\t',l)]
x, y = dis(os.path.dirname(os.path.dirname(os.path.abspath(__file__)))+'/tmp/o.bin'), dis(os.path.dirname(os.path.dirname(os.path.abspath(__file__)))+'/tmp/r.bin')
print(nm, len(d), len(r))
import difflib
for l in difflib.unified_diff(y, x, 'retail', 'obj', n=2, lineterm=''): print(l)
