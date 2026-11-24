#!/usr/bin/env python3
"""zsall336.py <tag> <file>: nf.py zs (VC7.1 cl /nologo /Zs BASE_FLAGS, header overlay from FABLE_HDR_OVERLAY) of one source, but every
error / warning line is kept, with each error's line mapped back to its compose part (<file minus .cpp>.map from the flattener):
-> work/zsall_<tag>.txt. Evidence only (compose probe); the work dir goes through clean.py."""
import sys, re, subprocess, bisect
sys.path.insert(0, '/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad/w22/agentNF336/tools')
import nf
from pathlib import Path
import verify_and_land as V
tag, f = sys.argv[1], sys.argv[2]
mp = [(int(a), b) for a, b in (l.split('\t') for l in open(f[:-4] + '.map').read().split('\n') if l)]
starts = [a for a, b in mp]
wd = nf.W / 'zs' / tag; wd.mkdir(parents=True, exist_ok=True)
sp = wd / (tag + '.cpp'); sp.write_text(V.vc71(Path(f).read_text(encoding='utf-8')), encoding='utf-8')
cp = V.cl(['/nologo', '/Zs'] + V.BASE_FLAGS + [str(sp)], V.env()); t = (cp.stdout or '') + (cp.stderr or '')
out = open(nf.W / ('zsall_%s.txt' % tag), 'w')
print('rc', cp.returncode, 'flags', ' '.join(V.BASE_FLAGS), file=out)
for l in t.splitlines():
    m = re.search(r'([^\\/]+)\((\d+)\) : (error|warning|fatal error) (C\d+): (.*)', l)
    if not m: continue
    fn = m.group(1); n = int(m.group(2))
    if fn.lower() == sp.name.lower():   # a line of the composed TU itself: map it back to its part
        k = bisect.bisect_right(starts, n) - 1; part = mp[k][1] if k >= 0 else '-'; where = 'line %d (+%d)' % (n, n - (mp[k][0] if k >= 0 else 0))
    else:                               # a header line: report the header, not a part
        part = 'header:' + fn; where = 'line %d' % n
    print('%s\t%s\t%s\t%s\t%s' % (m.group(3), m.group(4), part, where, m.group(5)[:300]), file=out)
out.close()
subprocess.run(['python3', str(nf.R / 'tools/decomp_pipeline/linux_lane/lane_tools/naming_batch/clean.py'), str(nf.A), str(wd)], capture_output=True)
