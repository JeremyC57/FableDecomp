# linkdiff.py <evidence/link_<tag>.txt>: HEAD v new from nf.py link output: duplicates per set, unresolved symbols only in HEAD
# (resolved by the new set), only in new (newly unresolved), and the count in both.
import sys, re
sets = {}; cur = None
for l in open(sys.argv[1]):
    if l.startswith('## '): cur = l.split()[1]; sets[cur] = {'dup': [], 'und': set()}
    elif l.startswith('duplicates'): sets[cur]['n'] = int(l.split()[1])
    elif l.strip().startswith('DUP'): sets[cur]['dup'].append(l.strip())
    elif l.startswith('UNRESOLVED'): sets[cur]['und'].add(l.split()[2])
h, n = sets['HEAD'], sets['new']
print('duplicates HEAD %d new %d' % (h['n'], n['n']))
for d in n['dup']: print('  NEWDUP', d)
print('unresolved HEAD %d new %d both %d' % (len(h['und']), len(n['und']), len(h['und'] & n['und'])))
for s in sorted(h['und'] - n['und']): print('  RESOLVED_BY_NEW', s)
for s in sorted(n['und'] - h['und']): print('  NEWLY_UNRESOLVED', s)
