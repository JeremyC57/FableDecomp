# hand-edit helper: he.py <row> <src|tst> <old> <new>  (exact, once) on ov_hand copies (copied from repo on first use)
import sys, os, shutil
sys.path.insert(0, os.path.join(os.path.dirname(os.path.abspath(__file__)), '..', 'tools')); import rows
W = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
R = rows.rows()
def path(a, k):
    rel = ('rebuild/src/compiled/' + R[a]['Source']) if k == 'src' else ('rebuild/tests/' + R[a]['TestSource'])
    dst = os.path.join(W, os.environ.get('HE_OV', 'ov_hand'), rel)
    if not os.path.exists(dst):
        os.makedirs(os.path.dirname(dst), exist_ok=True); shutil.copyfile(os.path.join(rows.ROOT, rel), dst)
    return dst, rel
def edit(a, k, old, new):
    p, rel = path(a, k)
    t = open(p, encoding='utf-8', errors='surrogateescape', newline='').read()
    n = t.count(old)
    if n != 1:
        if new in t: print('already', a, k); return
        raise SystemExit('COUNT %d in %s for %r' % (n, rel, old[:80]))
    open(p, 'w', encoding='utf-8', errors='surrogateescape', newline='').write(t.replace(old, new))
    print('ok', a, k)
if __name__ == '__main__':
    edit(sys.argv[1], sys.argv[2], sys.argv[3].encode().decode('unicode_escape'), sys.argv[4].encode().decode('unicode_escape'))
