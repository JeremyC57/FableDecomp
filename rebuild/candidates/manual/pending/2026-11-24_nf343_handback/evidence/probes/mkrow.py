import sys, re
# mkrow.py <src> <out> <defs> <declblock> <tailblock>: defines after the last top #define (before first #include), declblock after the last #include line, tail at EOF
src, out, defs, db, tb = sys.argv[1:6]
L = open(src, encoding='utf-8').read().rstrip('\n').split('\n')
inc = [i for i, l in enumerate(L) if l.startswith('#include')]
first_inc, last_inc = inc[0], inc[-1]
dfs = [i for i, l in enumerate(L[:first_inc]) if l.startswith('#define')]
ad = (dfs[-1] + 1) if dfs else first_inc
D = ['#define ' + d for d in defs.split(';') if d]
rd = lambda p: open(p).read().rstrip('\n').split('\n') if p != '-' else []
N = L[:ad] + D + L[ad:last_inc + 1] + rd(db) + L[last_inc + 1:] + rd(tb)
open(out, 'w', encoding='utf-8').write('\n'.join(N) + '\n')
print(out, 'defs after line', ad, 'decl after line', last_inc + 1)
