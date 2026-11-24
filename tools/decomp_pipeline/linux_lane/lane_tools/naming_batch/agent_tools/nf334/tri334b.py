# tri334b.py: adds scope comparison to work/tri_all.tsv -> work/tri_b.tsv
# scope = the qualifier chain of a mangled member name (?meth@A@B@@ -> A@B), '' for a free function / extern "C" name
import re, os
A = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
E = {l.split('\t')[0]: l.rstrip('\n').split('\t') for l in list(open(A + '/ENTRIES.tsv'))[1:]}
def scope(s):
    if not s.startswith('?'): return ''
    m = re.match(r'\?\?[0-9](\w+)@', s)
    if m: return m.group(1)
    m = re.match(r'\?\?_?[0-9A-Z]((?:[\w$?]+@)*?)@', s) if s.startswith('??') and not s.startswith('??$') else re.match(r'\?(?:\?\$)?\w+@((?:[\w$?]+@)*?)@', s)
    return m.group(1).rstrip('@') if m else '?'
out = open(A + '/work/tri_b.tsv', 'w')
for l in open(A + '/work/tri_all.tsv'):
    q = l.rstrip('\n').split('\t'); pdb = E[q[1]][6]
    ss = scope(q[7]); ps = scope(pdb)
    out.write('\t'.join(q[:7] + ['S' if ss == ps else ('free' if ss == '' else 'D'), q[7]]) + '\n')
