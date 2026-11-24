# medit.py <spec.json>: {addr: [[old, new, count], ...]} -> copies HEAD to orig/ + rows/ (if absent) and applies each replacement
# exactly `count` times in rows/<addr>.cpp; refuses any line-count change
import sys, json, shutil, os
A = '/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad/w22/agentNF328'
sys.path.insert(0, A + '/tools'); import nf
spec = json.load(open(sys.argv[1]))
for a, reps in spec.items():
    src = str(nf.src_of(a)); o = A + '/orig/%s.cpp' % a; r = A + '/rows/%s.cpp' % a
    if not os.path.exists(o): shutil.copy(src, o)
    if not os.path.exists(r): shutil.copy(src, r)
    t = open(r, encoding='utf-8', errors='surrogateescape').read(); n0 = t.count('\n'); ok = True
    for old, new, cnt in reps:
        c = t.count(old)
        if c != cnt: print('FAIL', a, 'count=%d want %d for %r' % (c, cnt, old)); ok = False; break
        t = t.replace(old, new)
    if ok and t.count('\n') != n0: print('FAIL', a, 'line delta'); ok = False
    if ok: open(r, 'w', encoding='utf-8', errors='surrogateescape').write(t); print('OK', a)
