# sub-wave 1c (hand): 0x00C19250 ??1PKCTRRedblackTree@@QAE@XZ -- the libpk red-black tree destructor. The body stores vtable
#   0x012B2F0C and logs ".\PKCTRRedblackTree.cpp(79) : Error: There are still items in this map" (string 0x012B2E98); the
#   landed sibling PKCTRRedblackTree::DeleteFixup 0x00C18C00 and ~CEventScript 0x00C2CC60 name the class so. The queue's
#   descriptive CPKListImpl (agent 634) yields to the upstream source name (libpk rule). Non-virtual (`??1..QAE`): vtable slot 0
#   is 0x00C18A40, not a deleting destructor, and every holder calls it directly.
import os, sys, re
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from he import edit, edit_all, resub, _rw, _w
ROWS = sys.argv[1:] or [l.split()[0] for l in open(os.path.join(os.path.dirname(os.path.abspath(__file__)), '..', '..', 'runs', 'pkc.rows.txt'))]
for a in ROWS:
    p, rel, t = _rw(a, 'src')
    if 'struct CMember_00c19250 {' not in t:
        print('already' if 'class PKCTRRedblackTree { public:   // libpk' in t else 'skip', a); continue
    t = t.replace('struct CMember_00c19250 {', 'class PKCTRRedblackTree { public:   // libpk red-black tree (".\\PKCTRRedblackTree.cpp", vtable 0x012B2F0C)', 1)
    t = re.sub(r'\bCMember_00c19250\b', 'PKCTRRedblackTree', t)
    _w(p, t); print('ok', a, 'src')
