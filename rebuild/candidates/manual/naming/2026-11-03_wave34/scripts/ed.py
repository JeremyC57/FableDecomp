# ed.py <edits.py>: run exact-string replacements: EDITS = [(path, old, new), ...]; every old must occur once.
import sys, runpy
E = runpy.run_path(sys.argv[1])["EDITS"]
cache = {}
for p, a, b in E:
    s = cache.get(p) or open(p, encoding="utf-8").read()
    if s.count(a) != 1: sys.exit(f"COUNT {s.count(a)} in {p}: {a[:100]!r}")
    cache[p] = s.replace(a, b, 1)
for p, s in cache.items(): open(p, "w", encoding="utf-8").write(s); print("ok", p)
