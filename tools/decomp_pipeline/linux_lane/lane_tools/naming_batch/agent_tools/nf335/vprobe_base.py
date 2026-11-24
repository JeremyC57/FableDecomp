# vprobe_base.py (NF330, coordinator check 2026-11-24): vprobe2.py for a LANDED base model that has no _VIRTUAL_MODEL switch.
# vprobe_base.py <tag> <Class>:DEF1,DEF2,... [...]: defines ONLY the listed switches (no FABLE_RETAIL_<X>_VIRTUAL_MODEL), includes
# engine/<Class>.h from work/tree, and makes VC7.1 emit ??_7<Class>@@6B@ via the implicit inline copy ctor (`new X(x)`; the landed
# default ctor is out of line, so `new X` alone may not emit the vftable); falls back to `new X`. Then tools/vtcmp.py against the
# retail RTTI vftable in work/model_<Class>.tsv. A spec may carry a suffix `@label` (e.g. `@neg`) for a log label. Scratch only.
import sys, os, subprocess
from pathlib import Path
A = Path('/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad/w22/agentNF335'); W = A / 'work'
sys.path.insert(0, '/home/user/FableDecomp/tools/decomp_pipeline')
os.environ['FABLE_HDR_OVERLAY'] = os.environ.get('PROBE_TREE', str(W / 'tree'))
import verify_and_land as V
tag = sys.argv[1]; d = W / 'probe' / tag; d.mkdir(parents=True, exist_ok=True)
log = open(W / ('probe_%s.log' % tag), 'w')
for spec in sys.argv[2:]:
    spec, _, lab = spec.partition('@')
    X, _, defs = spec.partition(':')
    defs = [x for x in defs.split(',') if x]
    vt = open(W / ('model_%s.tsv' % X)).readline().split('\t')[1]
    src = os.environ.get('PROBE_PRE', '').replace('\\n', '\n') + '\n' + ''.join('#define %s\n' % x for x in defs)   # PROBE_PRE: scratch-TU forward declarations (as vprobe2)
    stem = X + ('_' + lab if lab else '')
    obj = d / (stem + '.obj')
    for form in ('void* NF330BaseProbe_%s(const %s& x) { return new %s(x); }\n' % (X, X, X), 'void* NF330BaseProbe_%s() { return new %s; }\n' % (X, X)):
        sp = d / (stem + '.cpp'); sp.write_text(src + '#include "engine/%s.h"\n' % X + form)
        if obj.exists(): obj.unlink()
        cp = V.cl(['/nologo', '/c', '/O2', '/Oy', '/W3', '/Fo' + str(obj), str(sp)], V.env())
        t = (cp.stdout or '') + (cp.stderr or '')
        errs = [l for l in t.splitlines() if ' error ' in l or 'fatal error' in l]
        if not (cp.returncode or errs): break
    if cp.returncode or errs or not obj.exists():
        log.write('%s %s COMPILE_FAIL rc=%d %s\n' % (X, lab, cp.returncode, ' | '.join(e[-200:] for e in errs[:4]))); log.flush(); continue
    r = subprocess.run(['python3', str(A / 'tools/vtcmp.py'), str(obj), '??_7%s@@6B@' % X, vt], capture_output=True, text=True)
    log.write('== %s [%s] pre=%r defines=%s form=%s vftable %s\n%s%s\n' % (X, lab or 'main', os.environ.get('PROBE_PRE', ''), ','.join(defs), 'copy' if 'const ' in form else 'new', vt, r.stdout, r.stderr[-400:])); log.flush()
log.write('PROBE_DONE\n'); log.close()
