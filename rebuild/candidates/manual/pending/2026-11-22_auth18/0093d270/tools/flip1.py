#!/usr/bin/env python3
"""flip1.py: landed rows that pass a by-value inline float getter (from rebuild/include/engine) as a non-first argument of a call
whose earlier (left, later-evaluated) arguments contain a call. Prints file, getter, objcall flag, the call text."""
import re, glob, os
R = "/home/user/FableDecomp/"
gs = set()
for h in glob.glob(R + "rebuild/include/engine/*.h") + glob.glob(R + "rebuild/include/*.h"):
    t = open(h, errors="replace").read()
    for m in re.finditer(r"\bfloat\s+(Get\w+|Peek\w+)\s*\(\s*(?:void)?\s*\)\s*const\s*\{\s*return\s+[\w.\->\[\]]+\s*;\s*\}", t):
        gs.add(m.group(1))
print("getters", len(gs))
pat = re.compile(r"(?:\.|->)(" + "|".join(sorted(gs)) + r")\s*\(\s*\)")
hits = 0
for f in sorted(glob.glob(R + "rebuild/src/compiled/*/*/*.cpp")):
    t = open(f, errors="replace").read()
    t = re.sub(r"//[^\n]*", "", t)
    for m in pat.finditer(t):
        # find enclosing '('
        i = m.start(); depth = 0; j = i - 1
        while j >= 0:
            c = t[j]
            if c in ")]": depth += 1
            elif c in "([":
                if depth == 0: break
                depth -= 1
            elif c in ";{}" and depth == 0: j = -1; break
            j -= 1
        if j < 0 or t[j] != "(": continue
        # end of arg list
        k = m.end(); depth = 0
        while k < len(t):
            c = t[k]
            if c in "([": depth += 1
            elif c in ")]":
                if depth == 0: break
                depth -= 1
            k += 1
        inner = t[j+1:k]
        # split top-level args
        args = []; depth = 0; cur = ""; pos = j + 1; idx = None
        for q, c in enumerate(inner):
            if c in "([": depth += 1
            if c in ")]": depth -= 1
            if c == "," and depth == 0: args.append(cur); cur = ""; continue
            cur += c
            if j + 1 + q == m.start(): idx = len(args)
        args.append(cur)
        if idx is None or idx == 0 or len(args) < 2: continue
        left = args[:idx]
        if not any("(" in a for a in left): continue
        callee = t[max(0, j-60):j].split()[-1] if t[max(0,j-60):j].split() else ""
        if re.search(r"\b(if|while|for|return|switch)$", callee): continue
        obj = t[max(0, m.start()-40):m.start()]
        objcall = obj.rstrip().endswith(")")
        hits += 1
        print(os.path.basename(f), m.group(1), "objcall" if objcall else "-", "|", re.sub(r"\s+", " ", t[j-40:k+1])[:220])
print("hits", hits)
