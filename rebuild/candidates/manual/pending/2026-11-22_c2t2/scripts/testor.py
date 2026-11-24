#!/usr/bin/env python3
"""testor.py <listing.lst>: every `test r1, r2 ; jne ; or rA, rB` site with the last writer of each register before the test.
Roles: M = the register loaded from [.. + 8] (the bookmark mask word the or stores back), S = the other one."""
import sys, re
L = []
for l in open(sys.argv[1]):
    p = l.split(None, 2)
    if len(p) == 3 and re.match(r"^[0-9a-f]{8}$", p[0]):
        L.append((p[0], p[2].strip()))
for i, (a, t) in enumerate(L):
    m = re.match(r"test (e\w\w), (e\w\w)$", t)
    if not m or i + 2 >= len(L):
        continue
    o = re.match(r"or (e\w\w), (e\w\w)$", L[i + 2][1])
    if not (L[i + 1][1].startswith("jne") and o):
        continue
    st = L[i + 3][1] if i + 3 < len(L) else ""
    def writer(r):
        for j in range(i - 1, max(i - 40, -1), -1):
            if re.match(r"(mov|movzx|lea|and|or|xor|shl|shr|add|sub|pop) %s\b" % r, L[j][1]):
                return L[j][1]
        return "?"
    r1, r2 = m.group(1), m.group(2)
    w1, w2 = writer(r1), writer(r2)
    def role(w):
        return "M" if re.search(r"\+ 8\]", w) else "S"
    print(f"{a}  test {r1}({role(w1)}),{r2}({role(w2)})  or {o.group(1)}({role(writer(o.group(1)))}),{o.group(2)}  | {r1}<- {w1} ; {r2}<- {w2} | {st}")
