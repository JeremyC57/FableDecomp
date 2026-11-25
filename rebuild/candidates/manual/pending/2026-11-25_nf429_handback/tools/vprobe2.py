# vprobe2.py (NF325): vprobe.py plus PROBE_PRE (scratch-TU-only text put before the #defines, e.g. `class CCombatAction {};` so the
# implicit copy constructor of a model holding std::vector<CCombatAction> compiles; never in a model or row).
# vprobe.py <tag> <Class>[:DEF1,DEF2...] ... : slot-order probe for the opt-in models (NF322, 2026-11-23). For each class, a scratch
# TU in work/probe/<tag>/ defines FABLE_RETAIL_<CLASS>_VIRTUAL_MODEL (+ the listed prerequisite switches), includes engine/<Class>.h
# from work/tree (FABLE_HDR_OVERLAY) and returns `new <Class>` (the implicit inline constructor stores the vptr, so VC7.1 emits the
# ??_7 COMDAT). Compiled with VC7.1 cl /c /O2 /Oy /W3; then tools/vtcmp.py compares the emitted ??_7<Class>@@6B@ slot by slot with the
# retail vftable (work/model_<Class>.tsv). Never a row: scratch only, never handed back as source. Run in the background.
import sys, os, re, subprocess
from pathlib import Path
A = Path('/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad/w22/agentNF429'); W = A / 'work'
sys.path.insert(0, '/home/user/FableDecomp/tools/decomp_pipeline')
os.environ['FABLE_HDR_OVERLAY'] = os.environ.get('PROBE_TREE', str(W / 'tree'))   # NF329: PROBE_TREE for the negative control
import verify_and_land as V
tag = sys.argv[1]; d = W / 'probe' / tag; d.mkdir(parents=True, exist_ok=True)
log = open(W / ('probe_%s.log' % tag), 'w')
for spec in sys.argv[2:]:
    X, _, defs = spec.partition(':')
    defs = [x for x in defs.split(',') if x]
    vt = open(W / ('model_%s.tsv' % X)).readline().split('\t')[1]
    src = os.environ.get('PROBE_PRE', '').replace('\\n', '\n') + '\n' + ''.join('#define %s\n' % x for x in ['FABLE_RETAIL_%s_VIRTUAL_MODEL' % X.upper()] + defs)
    obj = d / (X + '.obj')
    # form 1: `new X` (implicit default ctor); form 2 (no default ctor): `new X(x)` (the implicit inline copy ctor stores the vptr too)
    for form in ('void* NF325Probe_%s() { return new %s; }\n' % (X, X), 'void* NF325Probe_%s(const %s& x) { return new %s(x); }\n' % (X, X, X)):
        sp = d / (X + '.cpp'); sp.write_text(src + '#include "engine/%s.h"\n' % X + form)
        cp = V.cl(['/nologo', '/c', '/O2', '/Oy', '/W3', '/Fo' + str(obj), str(sp)], V.env())
        t = (cp.stdout or '') + (cp.stderr or '')
        errs = [l for l in t.splitlines() if ' error ' in l or 'fatal error' in l]
        if not (cp.returncode or errs): break
    if cp.returncode or errs or not obj.exists():
        log.write('%s COMPILE_FAIL rc=%d %s\n' % (X, cp.returncode, ' | '.join(e[-200:] for e in errs[:4]))); log.flush(); continue
    r = subprocess.run(['python3', str(A / 'tools/vtcmp.py'), str(obj), '??_7%s@@6B@' % X, vt], capture_output=True, text=True)
    log.write('== %s vftable %s\n%s%s\n' % (X, vt, r.stdout, r.stderr[-400:])); log.flush()
log.write('PROBE_DONE\n'); log.close()
