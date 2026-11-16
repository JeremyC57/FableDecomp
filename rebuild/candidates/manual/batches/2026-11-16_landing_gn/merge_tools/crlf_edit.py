# usage: crlf_edit.py file pairs.py  -- pairs.py defines PAIRS=[(old,new),...] in LF form; applied CRLF-aware, each must occur once
import sys
f = sys.argv[1]
ns = {}
exec(open(sys.argv[2]).read(), ns)
raw = open(f, newline='').read()
crlf = '\r\n' in raw
t = raw.replace('\r\n', '\n')
for o, n in ns['PAIRS']:
    c = t.count(o)
    if c != 1:
        sys.exit(f"{f}: expected 1 match, got {c}: {o[:80]!r}")
    t = t.replace(o, n)
if crlf:
    t = t.replace('\n', '\r\n')
open(f, 'w', newline='').write(t)
print(f, "ok", len(ns['PAIRS']))
