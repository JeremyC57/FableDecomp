#!/usr/bin/env python3
"""gen7.py <base.cpp> <out.cpp> <name>[,<name>...]: bind every declaration of each named local to an unused local reference
(`T& nameRef7 = name;` right after the declaration; for a for-init variable, first statement of the loop body). In a
regime-1 function (>10000 tuples) this takes the local out of c2's register candidates with no code (session 7)."""
import sys, re
src = open(sys.argv[1]).read().split('\n')
names = sys.argv[3].split(',')
out = []; n = 0
for l in src:
    out.append(l)
    for nm in names:
        m = re.match(r'^(\s+)((?:const )?[A-Za-z_][\w:<>, ]*?[ *&]*?)\s*\b%s\b(\s*=\s*[^;]*|\([^;]*\))?;\s*(//.*)?$' % re.escape(nm), l)
        if m and not l.strip().startswith(('return', 'delete')) and '(' not in m.group(2):
            ty = m.group(2).strip()
            if ty in ('else', 'return'): continue
            out.append(f'{m.group(1)}{ty}& {nm}Ref7 = {nm};'); n += 1
        m = re.match(r'^(\s+)for \(((?:const )?[A-Za-z_][\w:<>]*) %s = [^;]*;.*\{\s*$' % re.escape(nm), l)
        if m:
            out.append(f'{m.group(1)}    {m.group(2)}& {nm}Ref7 = {nm};'); n += 1
open(sys.argv[2], 'w').write('\n'.join(out))
print(n, 'bindings')
