#!/usr/bin/env python3
"""blocks.py <file> <op>...: in-place edits of local class blocks. ops:
  cut:<Class>            remove the class block
  sub:<Class>=<text>     replace the class block with text (\\n escapes)
  rm:<literal>           remove a literal line/text (\\n escapes)
  pre:<anchor>=<text>    insert text before the first occurrence of anchor
Fails (no write) if a block or literal is missing."""
import sys, os
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from common import find_block
p = sys.argv[1]; t = open(p).read()
for op in sys.argv[2:]:
    kind, arg = op.split(':', 1)
    if kind in ('cut', 'sub'):
        cls, _, txt = arg.partition('=')
        b = find_block(t, cls)
        if not b: sys.exit(f"no block {cls}")
        t = t[:b[0]] + txt.replace('\\n', '\n') + t[b[1]:]
    elif kind == 'rm':
        s = arg.replace('\\n', '\n')
        if s not in t: sys.exit(f"missing literal {s!r}")
        t = t.replace(s, '', 1)
    elif kind == 'pre':
        a, _, txt = arg.partition('=')
        a = a.replace('\\n', '\n')
        if a not in t: sys.exit(f"missing anchor {a!r}")
        i = t.index(a); t = t[:i] + txt.replace('\\n', '\n') + t[i:]
open(p, 'w').write(t); print("edit", p)
