# hdr.py (wave 27): edit a shared header on the overlay copy ($HE_OV/rebuild/include/<rel>; copied from the repo on first touch)
import os, shutil, sys
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import he
def _p(rel):
    dst = os.path.join(he.W, os.environ.get('HE_OV', 'ov_hand'), 'rebuild/include', rel)
    if not os.path.exists(dst):
        os.makedirs(os.path.dirname(dst), exist_ok=True); shutil.copyfile(os.path.join('/home/user/FableDecomp/rebuild/include', rel), dst)
    return dst
def edit(rel, old, new):
    p = _p(rel); t = open(p, encoding='utf-8', newline='').read()
    if new in t: print('already', rel); return
    if t.count(old) != 1: raise SystemExit('COUNT %d in %s for %r' % (t.count(old), rel, old[:80]))
    open(p, 'w', encoding='utf-8', newline='').write(t.replace(old, new)); print('ok', rel)
