#!/usr/bin/env python3
"""brk.py <in.cpp> <out.cpp> CASE[=r|b] ...: switch the `break;` that ends a case body (the last `break;` before the next
`case`/`default` label at the case's indent) to `return;` (r, default) or back (b)."""
import sys, re
src, out, specs = sys.argv[1], sys.argv[2], sys.argv[3:]
L = open(src).read().split('\n')
for sp in specs:
    name, _, kind = sp.partition('='); kind = kind or 'r'
    i = next(k for k, l in enumerate(L) if re.match(r'^    case ' + re.escape(name) + r':', l))
    j = next(k for k in range(i + 1, len(L)) if re.match(r'^    (case \w+:|default:|})', L[k]))
    # last top-level terminator of this case body (8 spaces of indent)
    t = max(k for k in range(i, j) if re.match(r'^        (break|return);', L[k]))
    L[t] = '        return;' if kind == 'r' else '        break;'
open(out, 'w').write('\n'.join(L))
