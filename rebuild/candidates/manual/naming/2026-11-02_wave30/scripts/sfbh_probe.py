# sfbh_probe.py <ov> <rows>: compile each row against the overlay header; classify the failure; try the contract fixes
# (add engine/CCountedPointer.h and/or fable_string.h before the first CSpawnedFunc* include) in <ov>; write runs/sfbh_fix.tsv
import sys, os, re, subprocess, concurrent.futures as cf
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from rows import rows, ROOT
W = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
ov = sys.argv[1]; A = [l.split()[0] for l in open(sys.argv[2]) if l.strip()]
R = rows()
HX = re.compile(r'^#include "engine/(CSpawnedFuncBase|CSpawnedFunc|CSpawnedFunc1|CActiveEntityScriptBase|CActiveEntityScript)\.h".*$', re.M)
def bld(a):
    r = subprocess.run([sys.executable, os.path.join(W, 'scripts/bld.py'), ov, a, os.path.join(W, 'pw', 'pr_' + a)], capture_output=True, text=True)
    return r.returncode == 0, r.stdout
def kinds(out):
    k = set()
    if "undefined class 'CCharString'" in out or 'CCharString' in out and 'C2079' in out: k.add('cs')
    if "undefined class 'CCountedPointer<T>'" in out: k.add('cp')
    if re.search(r"CSpawnedFuncBase\.h\(\d+\) : error C2143", out) or 'CCountedPointer' in out and ('C2143' in out or 'C2501' in out): k.add('cp')
    return k
def fix(a, ks):
    rel = 'rebuild/src/compiled/' + R[a]['Source']; p = os.path.join(W, ov, rel)
    src = p if os.path.exists(p) else os.path.join(ROOT, rel)
    t = open(src, encoding='utf-8', errors='surrogateescape', newline='').read()
    m = HX.search(t)
    if not m: return False
    ins = ''
    if 'cp' in ks and 'engine/CCountedPointer.h' not in t[:m.start()]: ins += '#include "engine/CCountedPointer.h"                     // CCountedPointer<T> for CSpawnedFuncBase::PPredicate (header contract)\n'
    if 'cs' in ks and 'fable_string.h' not in t[:m.start()]: ins += '#include "fable_string.h"                               // CCharString for CSpawnedFuncBase::Name / AssociatedRegionName (header contract)\n'
    if not ins: return False
    t = t[:m.start()] + ins + t[m.start():]
    os.makedirs(os.path.dirname(p), exist_ok=True); open(p, 'w', encoding='utf-8', errors='surrogateescape', newline='').write(t); return True
def one(a):
    log = []; ks_all = set()
    for it in range(3):
        ok, out = bld(a)
        if ok: return a, 'ok', ks_all, log
        ks = kinds(out); log.append(out.strip().split('\n')[0][-200:])
        if not ks or not fix(a, ks): return a, 'FAIL', ks_all | ks, log
        ks_all |= ks
    return a, 'FAIL', ks_all, log
res = list(cf.ThreadPoolExecutor(4).map(one, A))
with open(os.path.join(W, 'runs', 'sfbh_fix.tsv'), 'w') as f:
    for a, st, ks, log in res: f.write('%s\t%s\t%s\t%s\n' % (a, st, ','.join(sorted(ks)), ' | '.join(log)[:400]))
import collections; print(collections.Counter((st, ','.join(sorted(ks))) for a, st, ks, log in res))
