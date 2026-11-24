# medit.py <spec.json>: {addr: [[old, new, count], ...], "<addr>_test": [...]} -> HEAD copied to orig/ (tests: orig/tests/) and rows/ (if
# absent), each replacement applied exactly `count` times in rows/<addr>.cpp / rows/<addr>_test.cpp; a line-count change is refused unless
# the spec key ends in "!" (then it is reported)
import sys, json, shutil, os
A = '/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad/w22/agentNF339'
sys.path.insert(0, A + '/tools'); import nf
spec = json.load(open(sys.argv[1])); os.makedirs(A + '/orig/tests', exist_ok=True); os.makedirs(A + '/rows', exist_ok=True)
for key, reps in spec.items():
    allow = key.endswith('!'); k = key.rstrip('!')
    if k.endswith('_test'):
        a = k[:8]; src = str(nf.test_of(a)); o = A + '/orig/tests/%s_test.cpp' % a; r = A + '/rows/%s_test.cpp' % a
    else:
        a = k; src = str(nf.src_of(a)); o = A + '/orig/%s.cpp' % a; r = A + '/rows/%s.cpp' % a
    if not os.path.exists(o): shutil.copy(src, o)
    t = open(r if os.path.exists(r) else src, encoding='utf-8', errors='surrogateescape').read(); n0 = t.count('\n'); ok = True
    for old, new, cnt in reps:
        c = t.count(old)
        if c != cnt: print('FAIL', k, 'count=%d want %d for %r' % (c, cnt, old)); ok = False; break
        t = t.replace(old, new)
    if ok and t.count('\n') != n0:
        if allow: print('NOTE', k, 'line delta', t.count('\n') - n0)
        else: print('FAIL', k, 'line delta'); ok = False
    if ok: open(r, 'w', encoding='utf-8', errors='surrogateescape').write(t); print('OK', k)
