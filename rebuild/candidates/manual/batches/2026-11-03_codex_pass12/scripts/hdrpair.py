#!/usr/bin/env python3
# hdrcheck2-style preprocessed comparison of two versions of one header: default path and every FABLE_RETAIL_ macro
# named in either. Includes resolve against <incdir> (the row's own materialized context); missing system headers are
# stubbed empty in a private stub dir. usage: python3 -I hdrpair.py <A> <B> <incdir> <stubdir>  -> one line
import sys, subprocess, re, os
A, B, inc, stub = sys.argv[1:5]
def run(args, f):
    for _ in range(200):
        r = subprocess.run(args+[f], capture_output=True, text=True)
        m = re.search(r"fatal error: ([^:]+): No such file", r.stderr)
        if not m: return r
        p = os.path.join(stub, m.group(1)); os.makedirs(os.path.dirname(p), exist_ok=True); open(p, 'w').close()
    return r
macs = [None]+sorted(set(re.findall(r'FABLE_RETAIL_\w+', open(A, errors='replace').read()+open(B, errors='replace').read())))
res = []
for m in macs:
    args = ['cpp', '-P', '-nostdinc', '-undef', '-D_MSC_VER=1310', '-D_M_IX86=600', '-D_WIN32', '-I', stub, '-I', inc]+([f'-D{m}'] if m else [])
    ra = run(args, A); rb = run(args, B)
    norm = lambda s: [re.sub(r'fable_static_assert_\d+', 'SA', l) for l in s.splitlines() if l.strip()]
    a = norm(ra.stdout); b = norm(rb.stdout)
    err = '' if not (ra.returncode or rb.returncode) else '(cpp-err)'
    res.append(('default' if not m else m)+('=same' if a == b else '=DIFF')+f'[{len(a)}]'+err)
print(' '.join(res))
