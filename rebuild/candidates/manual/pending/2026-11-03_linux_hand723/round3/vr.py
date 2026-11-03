# vr.py <addr> <base.cpp> <spec.py> : spec defines VARIANTS = [(tag, [(old,new),...], flags_list_or_None), ...]
# each variant applied to base, graded sequentially via g.sh, results appended to work/vr_<addr>.log
import sys, subprocess, os
addr, base, spec = sys.argv[1:4]
A = os.path.dirname(os.path.abspath(__file__))
ns = {}; exec(open(spec).read(), ns)
src0 = open(base).read()
for v in ns['VARIANTS']:
    tag, reps = v[0], v[1]; fl = v[2] if len(v) > 2 and v[2] else []
    s = src0
    ok = True
    for o, n in reps:
        if o not in s: print(tag, 'MISSING', o[:60]); ok = False; break
        s = s.replace(o, n, 1)
    if not ok: continue
    p = f"{A}/work/{addr}_{tag}.cpp"; open(p, 'w').write(s)
    r = subprocess.run(['bash', f'{A}/g.sh', addr, p, tag] + fl, capture_output=True, text=True, timeout=400)
    line = r.stdout.strip().split('\n')[-1]
    print(line, flush=True)
    open(f"{A}/work/vr_{addr}.log", 'a').write(line + '\t' + (v[3] if len(v) > 3 else '') + '\n')
