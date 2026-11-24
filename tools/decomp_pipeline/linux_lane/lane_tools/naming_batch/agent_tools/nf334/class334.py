# class334.py: per ENTRIES index, the script class from work/tri_b.tsv:
#   HELD  every wrong table line's witness is off limits / locked by another agent / has no landed source
#   PORTC every free wrong line binds through a scope found in neither PDB name set (NF299 Q4 census form), the rest HELD
#   MAN   at least one free wrong line binds through real (PDB) classes: opened by hand
#   NONE  no wrong table line (tracker spelling from a Leaf only)
import os, collections
A = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
E = [l.rstrip('\n').split('\t') for l in list(open(A + '/ENTRIES.tsv'))[1:]]
L = collections.defaultdict(list)
for l in open(A + '/work/tri_b.tsv'):
    q = l.rstrip('\n').split('\t'); L[int(q[0])].append(q)
out = open(A + '/work/class.tsv', 'w')
for i, e in enumerate(E, 1):
    ls = L.get(i, [])
    free = [q for q in ls if q[3] == '-']
    if not ls: c = 'NONE'
    elif not free: c = 'HELD'
    elif all(q[6] != '-' for q in free): c = 'PORTC'
    else: c = 'MAN'
    out.write('%d\t%s\t%s\t%d\t%d\t%s\n' % (i, e[0], c, len(ls), len(free), ';'.join('%s:%s' % (q[2], q[3]) for q in ls if q[3] != '-')))
