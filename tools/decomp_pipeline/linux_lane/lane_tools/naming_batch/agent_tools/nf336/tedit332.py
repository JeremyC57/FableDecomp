# tedit332.py <spec.json>: zero-line-delta edits of a row's landed TEST -> rows/<addr>_test.cpp (HEAD copy kept in orig/<addr>_test.cpp)
# spec {addr: [[old, new, count], ...]}; each old must occur exactly `count` times
import sys, json, shutil, os
A = '/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad/w22/agentNF336'
sys.path.insert(0, A + '/tools'); import nf
for a, reps in json.load(open(sys.argv[1])).items():
    src = str(nf.test_of(a)); o = A + '/orig/%s_test.cpp' % a; r = A + '/rows/%s_test.cpp' % a
    if not os.path.exists(o): shutil.copy(src, o)
    t = open(src if not os.path.exists(r) else r, encoding='utf-8', errors='surrogateescape').read(); n0 = t.count('\n'); ok = True
    for old, new, cnt in reps:
        c = t.count(old)
        if c != cnt: print('FAIL', a, 'count=%d want %d for %r' % (c, cnt, old)); ok = False; break
        t = t.replace(old, new)
    if ok and t.count('\n') != n0: print('FAIL', a, 'line delta', t.count('\n') - n0); ok = False
    if ok: open(r, 'w', encoding='utf-8', errors='surrogateescape').write(t); print('OK', a)
