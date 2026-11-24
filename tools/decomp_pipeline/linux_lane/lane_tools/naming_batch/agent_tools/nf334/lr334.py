# lr334.py <spec.json>: line-anchored edits {addr: [[lineno, old, new], ...]}: in rows/<addr>.cpp (copied from HEAD with orig/<addr>.cpp
# when absent) replace `old` by `new` exactly once inside line <lineno> (1-based, HEAD numbering); refuses a miss or any line-count change.
import sys, json, shutil, os
A = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
sys.path.insert(0, A + '/tools'); import nf
for a, eds in json.load(open(sys.argv[1])).items():
    src = str(nf.src_of(a)); o = A + '/orig/%s.cpp' % a; r = A + '/rows/%s.cpp' % a
    if not os.path.exists(o): shutil.copy(src, o)
    if not os.path.exists(r): shutil.copy(src, r)
    L = open(r, encoding='utf-8', errors='surrogateescape').read().split('\n'); n0 = len(L); ok = True
    for ln, old, new in eds:
        if L[ln - 1].count(old) != 1: print('FAIL', a, ln, repr(old), '|', L[ln - 1][:160]); ok = False; break
        L[ln - 1] = L[ln - 1].replace(old, new)
        if '\n' in new: print('FAIL newline', a); ok = False; break
    if ok and len(L) == n0: open(r, 'w', encoding='utf-8', errors='surrogateescape').write('\n'.join(L)); print('OK', a)
