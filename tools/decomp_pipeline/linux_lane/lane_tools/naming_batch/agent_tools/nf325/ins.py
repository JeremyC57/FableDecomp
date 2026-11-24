# ins.py <spec.json>: {addr: [[anchor_line, new_line, "after"|"before"|"replace"], ...]} line-level edits of rows/<addr>.cpp
# (or rows/<addr>_test.cpp when addr ends in _test); copies HEAD to orig/ + rows/ first; anchor must match exactly one whole line
import sys, json, shutil, os
A = '/tmp/claude-0/-home-user-FableDecomp/5221bbec-fb18-575f-8649-a32d96bb1c2d/scratchpad/w22/agentNF325'
sys.path.insert(0, A + '/tools'); import nf
for key, ops in json.load(open(sys.argv[1])).items():
    test = key.endswith('_test'); a = key[:8]
    src = str(nf.test_of(a) if test else nf.src_of(a)); o = A + '/orig/%s.cpp' % key; r = A + '/rows/%s.cpp' % key
    if not os.path.exists(o): shutil.copy(src, o)
    if not os.path.exists(r): shutil.copy(src, r)
    L = open(r, encoding='utf-8', errors='surrogateescape').read().split('\n'); ok = True
    for anchor, new, how in ops:
        idx = [i for i, l in enumerate(L) if l == anchor]
        if len(idx) != 1: print('FAIL', key, len(idx), repr(anchor)); ok = False; break
        i = idx[0]
        if how == 'after': L.insert(i + 1, new)
        elif how == 'before': L.insert(i, new)
        else: L[i] = new
    if ok: open(r, 'w', encoding='utf-8', errors='surrogateescape').write('\n'.join(L)); print('OK', key)
