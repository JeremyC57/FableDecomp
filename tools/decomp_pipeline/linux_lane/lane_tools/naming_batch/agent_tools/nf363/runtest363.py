# runtest363.py <tag> <addr> <test.cpp> <pass pattern>: the lander's behaviour step (verify_and_land.behaviour_of: VC7.1 /Od /W3, a
# self-contained link, run under wine) on one test text, plus the compile / link diagnostics behaviour_of hides. Log: work/rt_<tag>.log.
import sys, os, subprocess, re
from pathlib import Path
sys.path.insert(0, '/home/user/FableDecomp/tools/decomp_pipeline'); import verify_and_land as V
A = Path('/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad/w22/agentNF363')
tag, a, tp, patt = sys.argv[1:5]
wd = A / 'work' / 'rt' / a; wd.mkdir(parents=True, exist_ok=True); e = V.env()
log = open(A / 'work' / ('rt_%s.log' % tag), 'a')
st = V.behaviour_of(V.vc71(Path(tp).read_text(encoding='utf-8')), a, patt, wd, e)
print('%s %s %s' % (a, st, tp), file=log)
if st != 'PASS':
    t = wd / (a + '.test.cpp'); o = wd / (a + '.t.obj')
    cp = V.cl(['/nologo', '/c', '/Od', '/W3', '/Fo%s' % o, str(t)], e); print((cp.stdout or '')[-3000:], file=log)
    if o.exists():
        lk = V.run_tool(V.VC / 'bin' / 'link.exe', ['/nologo', '/out:%s' % (wd / (a + '.exe')), str(o)], e); print((lk.stdout or '')[-3000:], file=log)
    ex = wd / (a + '.exe')
    if ex.exists():
        rn = subprocess.run(([V.WINE] if V.WINE else []) + [str(ex)], capture_output=True, text=True, errors='replace', env=e); print('RUN rc=%d %s' % (rn.returncode, rn.stdout[-500:]), file=log)
print('RT_DONE', file=log); log.close()
