#!/usr/bin/env python3
"""wc.py in.cpp out.cpp : rewrite `for (;;) {\n <ws>if (IsActiveThreadTerminating())\n <ws>return;` (or the do-while(0) form) as
`while (!IsActiveThreadTerminating()) {`"""
import sys,re
s=open(sys.argv[1]).read()
s2=re.sub(r'for \(;;\) \{\s*\n(\s*)(?:if \(IsActiveThreadTerminating\(\)\)\s*\n\s*return;|do \{ if \(IsActiveThreadTerminating\(\)\) return; \} while \(0\);)',
          r'while (!IsActiveThreadTerminating()) {',s)
open(sys.argv[2],'w').write(s2); print(s.count('for (;;)'), s2.count('while (!IsActiveThreadTerminating()) {'), file=sys.stderr)
