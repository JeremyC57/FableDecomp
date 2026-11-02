# common.py (wave 30): per-family note line + Leaf setter on overlay copies
import os, sys
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import he
def note(a, fam, txt, leaf=None, k='src', tag=None):
    p, rel, t = he._rw(a, k)
    mark = '// Naming wave 31 (family %s%s): ' % (fam, (', ' + tag) if tag else '')
    if mark in t: return
    L = t.split('\n'); i = [j for j, x in enumerate(L) if x.startswith('// Leaf: ')]
    if leaf:
        if i: L[i[0]] = '// Leaf: ' + leaf
        else:
            L.insert(1 if L[0].startswith('//') else 0, '// Leaf: ' + leaf)
    i = [j for j, x in enumerate(L) if x.startswith('// Leaf: ')]
    L.insert((i[0] + 1) if i else 1, mark + txt)
    he._w(p, '\n'.join(L))
