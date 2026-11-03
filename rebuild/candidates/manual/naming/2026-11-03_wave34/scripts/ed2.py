# ed2.py <edits.py>: EDITS = [(path, old, new[, n])]; n = expected occurrences (default 1, 0 = any >=1, replace all).
# Saves the pristine file to $S/wave34/orig/<path> before its first change (revert with rev.py).
import sys, runpy, os, shutil
S = os.environ["S"]; O = S + "/wave34/orig/"
E = runpy.run_path(sys.argv[1])["EDITS"]
cache = {}
for e in E:
    p, a, b = e[:3]; n = e[3] if len(e) > 3 else 1
    s = cache[p] if p in cache else open(p, encoding="utf-8", errors="surrogateescape").read()
    c = s.count(a)
    if n == -1: pass
    elif (n and c != n) or c == 0: sys.exit(f"COUNT {c} (want {n}) in {p}: {a[:120]!r}")
    cache[p] = s.replace(a, b)
for p, s in cache.items():
    if not os.path.exists(O + p): os.makedirs(os.path.dirname(O + p), exist_ok=True); shutil.copy(p, O + p)
    open(p, "w", encoding="utf-8", errors="surrogateescape").write(s); print("ok", p)
