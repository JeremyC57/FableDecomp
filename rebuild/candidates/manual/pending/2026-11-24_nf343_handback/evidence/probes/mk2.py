import sys
# mk2.py src out after_def after_inc defs declblock tailblock : defines after line after_def, declblock after line after_inc, tailblock at EOF
src, out, ad, ai, defs, db, tb = sys.argv[1:8]
L = open(src, encoding='utf-8').read().rstrip('\n').split('\n'); ad, ai = int(ad), int(ai)
D = ['#define ' + d for d in defs.split(';') if d]
rd = lambda p: open(p).read().rstrip('\n').split('\n') if p != '-' else []
N = L[:ad] + D + L[ad:ai] + rd(db) + L[ai:] + rd(tb)
open(out, 'w', encoding='utf-8').write('\n'.join(N) + '\n')
