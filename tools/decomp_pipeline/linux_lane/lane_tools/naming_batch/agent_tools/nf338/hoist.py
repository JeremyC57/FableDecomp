# hoist.py <src> <dst>: move every `#define FABLE_RETAIL_*` line that sits after an #include up to just before the first #include
import re, sys
L = open(sys.argv[1], encoding='utf-8').read().split('\n')
first = next((n for n, l in enumerate(L) if re.match(r'\s*#\s*include', l)), None)
if first is not None:
    mv = [n for n, l in enumerate(L) if n > first and re.match(r'\s*#\s*define\s+FABLE_RETAIL_\w+', l)]
    lines = [L[n] for n in mv]
    L = [l for n, l in enumerate(L) if n not in mv]
    L[first:first] = lines
open(sys.argv[2], 'w', encoding='utf-8').write('\n'.join(L))
