#!/usr/bin/env python3
"""mkrow336.py <addr> [leaf]: rows/<addr>.cpp = HEAD's leading comment block (orig/<addr>.cpp, up to the first non-comment line;
the `// Leaf:` line replaced by [leaf] when given) + work/bodies/<addr>.pre (per-row defines that must precede the preamble) + work/preamble.txt (with work/bodies/<addr>.drop line swaps) + work/bodies/<addr>.txt"""
import sys, re
A = '/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad/w22/agentNF336'
a = sys.argv[1]; leaf = sys.argv[2] if len(sys.argv) > 2 else None
head = []
for l in open(A + '/orig/%s.cpp' % a, encoding='utf-8').read().split('\n'):
    if l.startswith('//'): head.append(l)
    else: break
if leaf:
    head = [('// Leaf: ' + leaf) if l.startswith('// Leaf:') else l for l in head]
    if not any(l.startswith('// Leaf:') for l in head): head.insert(1, '// Leaf: ' + leaf)
pre = open(A + '/work/preamble.txt').read().rstrip('\n')
import os as _os
_dp = A + '/work/bodies/%s.drop' % a   # optional: tab-separated `preamble line<TAB>replacement line` pairs (same line count)
if _os.path.exists(_dp):
    for _l in open(_dp).read().split('\n'):
        if _l.strip():
            _o, _n = _l.split('\t', 1); assert pre.count(_o + '\n') == 1, _o; pre = pre.replace(_o + '\n', _n + '\n')
body = open(A + '/work/bodies/%s.txt' % a).read().rstrip('\n')
import os
prep = open(A + '/work/bodies/%s.pre' % a).read().rstrip('\n') + '\n' if os.path.exists(A + '/work/bodies/%s.pre' % a) else ''
open(A + '/rows/%s.cpp' % a, 'w', encoding='utf-8').write('\n'.join(head) + '\n' + prep + pre + '\n' + body + '\n')
print('rows/%s.cpp' % a)
