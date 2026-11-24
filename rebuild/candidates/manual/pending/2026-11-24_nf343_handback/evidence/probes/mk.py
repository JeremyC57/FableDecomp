import sys
src, out, after_def, after_inc, defs, block = sys.argv[1:7]
L = open(src, encoding='utf-8').read().split('\n')
ad, ai = int(after_def), int(after_inc)
D = [l for l in defs.split(';') if l]
B = open(block).read().rstrip('\n').split('\n') if block != '-' else []
N = L[:ad] + ['#define ' + d for d in D] + L[ad:ai] + B + L[ai:]
open(out, 'w', encoding='utf-8').write('\n'.join(N))
